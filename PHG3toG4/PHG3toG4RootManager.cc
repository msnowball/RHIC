#include "PHG3toG4RootManager.h"
#include "PHG3toG4UserTrackInfo.h"

#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdlib.h>
#include <ctime>
#include <sys/time.h>
#include <algorithm>
#include <cctype>

//Root
#include <TClonesArray.h>
#include <TFile.h>
#include <TTree.h>
#include <TDirectory.h>
#include <TObjArray.h>
#include <TParticle.h>
#include <TGeoManager.h>

//Encode to pisa
#include "PISAEvent.h"


using namespace std;

PHG3toG4RootManager *PHG3toG4RootManager::fgInstance = 0;


//__________________________________________________________
PHG3toG4RootManager::PHG3toG4RootManager(std::string fileName,std::string treeName)
{
  _verbosity = 0;
  _theFileName = fileName;
  _theTreeName = treeName;
  _theCompressionLevel = 2;
  hfile = NULL;

  nbytes = 0;

  fCollectionSize = 0;
  
}

//__________________________________________________________
PHG3toG4RootManager::~PHG3toG4RootManager()
{ 
  if(hfile) delete hfile;
  if(fKinHitsCollection) delete fKinHitsCollection;
}

PHG3toG4RootManager *PHG3toG4RootManager::GetInstance()
{
  // Returns self pointer.
  if (fgInstance) return fgInstance;
  fgInstance = new PHG3toG4RootManager();
  return fgInstance;
}


//__________________________________________________________
int PHG3toG4RootManager::Init()
{

  // startflag = 0  for subevent output of ROOT clone objects
  // startflag = 1  for full event output of ROOT clone objects <---
  fKinHitsCollection = new PHG3toG4KinHitsCollection("KinHitDummyDet","kinHitsCollection");

  if( hfile && hfile->IsOpen() )
    {
      cout << "PHG3toG4RootManager::Init - TFile output is already opened." << endl;
      return -1;
    }

  if(_verbosity > 0) cout << "PHG3toG4RootManager::Init - pisa output filename: " << _theFileName << endl;

  // Set up ROOT File
  hfile = new TFile(_theFileName.c_str(),"RECREATE","PISA ROOT file");

  // compresses floats in TClones arrays
  hfile->SetCompressionLevel(_theCompressionLevel);

  // Create a PISAevent instance
  pisaevent = new PISAEvent();
  pisaevent->SetStartFlag(1);

  // Set up Tree structure
  int split = 1;
  int branchStyle = 0;

  tree = new TTree("T","PISA Hits in a ROOT tree");
  tree->SetMaxTreeSize(10000000000LL); // set max size to ~10 GB 
  tree->SetAutoSave(30000000);
  int bufsize = 64000;
  if (split)  bufsize /= 4;
  TTree::SetBranchStyle(branchStyle);
  tree->Branch("pisaevent", "PISAEvent", &pisaevent, bufsize,split);

  
  return 0;

}

int PHG3toG4RootManager::SetEventHeader(std::vector<float> Data) // so ugly...
{
  
  int i[100];
  float f[100];


  for(unsigned int k = 0; k < Data.size(); k++)
    {
      i[k] = int(Data[k]);
      f[k] = Data[k];

    }
 
  pisaevent->SetHeader(i, f);



  return 0;
}


//__________________________________________________________
int PHG3toG4RootManager::EndEvent()
{

  pisaevent->SetEndFlag(2);
  nbytes += tree->Fill();
  //pisaevent->Clear();

  delete pisaevent;
  pisaevent = new PISAEvent();
  pisaevent->SetStartFlag(1);

  delete fKinHitsCollection;
  fKinHitsCollection = new PHG3toG4KinHitsCollection("KinHitDummyDet","kinHitsCollection");
  fCollectionSize = 0;
  fKinHitMap.clear();
  fKinHitsWithParent.clear();
  fAllTracksInStack.clear();

  return 0;

}

int PHG3toG4RootManager::End()
{
 
  if(_verbosity > 0) cout << "PHG3toG4RootManager::End - nbytes: " << nbytes << endl;
  if( hfile && hfile->IsOpen() )
    {
      cout << "PHG3toG4RootManager::End - closing PISA TFile" << endl;
      hfile->Write();
      hfile->Close();
      delete hfile;
      hfile = 0;
    }
  else{ return -1; }
  
  return 0;

}


void PHG3toG4RootManager::AddKinHit(PHG3toG4KinHit *hit)
{
  int trackID = hit->GetTrackID();
  if(!IsKinHitRecorded(trackID))
    {
      fKinHitsCollection->insert(hit);
      fCollectionSize++;
      fKinHitMap[trackID] = hit;
      if(hit->GetParentTrackID() > 0) fKinHitsWithParent.push_back(trackID);
    }
}


PHG3toG4KinHit PHG3toG4RootManager::GetKinHit(int i)
{
  return *(*fKinHitsCollection)[i];
}


int PHG3toG4RootManager::KinHitSize()
{
  return fCollectionSize;
}


void PHG3toG4RootManager::FinishKinHits()
{
  /*
  PHG3toG4UserTrackInfo *theTrackInfo = NULL;
  for(unsigned int i = 0; i < fKinHitsWithParent.size(); i++)
    {
      std::map<int, PHG3toG4KinHit*>::iterator fKinHitMap_iterator = fKinHitMap.find(fKinHitsWithParent[i]);
      if(fKinHitMap_iterator == fKinHitMap.end()) continue;
      std::map<int, const G4Track*>::iterator fAllTracksInStack_iterator = fAllTracksInStack.find(fKinHitsWithParent[i]);
      if(fAllTracksInStack_iterator == fAllTracksInStack.end()) continue;

      theTrackInfo = (PHG3toG4UserTrackInfo*)(fAllTracksInStack_iterator->second->GetUserInformation());
      int thePDG = theTrackInfo->GetOriginalParticle()->GetPDGEncoding();
      int theTrackID = theTrackInfo->GetOriginalTrackID();
      fKinHitMap_iterator->second->SetParentPartID( thePDG );
      fKinHitMap_iterator->second->SetParentTrackID( theTrackID );
      G4cout << i << "  " << fKinHitsWithParent[i] << "  " << fAllTracksInStack_iterator->second->GetTrackID() << "  " << theTrackID << "  " << thePDG << G4endl;
    }
  */
}


bool PHG3toG4RootManager::IsKinHitRecorded(int ID)
{
  std::map<int, PHG3toG4KinHit*>::iterator fKinHitMap_iterator = fKinHitMap.find(ID);
  if(fKinHitMap_iterator != fKinHitMap.end()) return true;
  return false;
}
