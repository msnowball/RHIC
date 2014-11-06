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

#include "Pythia8/Pythia.h"
#include "Pythia8/Pythia8ToHepMC.h"
#include "HepMC/GenEvent.h"

#include <TMCParticle.h>
#include <PHIODataNode.h>
#include <PHDataNode.h>
#include <PHObject.h>
#include <PHCompositeNode.h>
#include <PHNodeIterator.h>
#include <PHNodeReset.h>
#include <PHTimeStamp.h>
#include <Fun4AllReturnCodes.h>

#include <PHPythia8.h>
#include <PHPythiaHeaderV2.h>
#include <PHPythiaContainerV2.h>

#include "PHHepMCGenEvent.h"
#include "PHPy8GenTrigger.h"

#include <TClonesArray.h>
#include <TFile.h>
#include <TTree.h>
#include <TDirectory.h>
#include <TObjArray.h>
#include <TParticle.h>


using namespace std;

typedef PHIODataNode<PHObject> PHObjectNode_t;

static const Float_t CM2MM = 10.;	// cm to mm comversion

//__________________________________________________________
PHPythia8::PHPythia8(const std::string &name, FORMAT outputFormat): 
  SubsysReco(name),
  eventcount( 0 ),
  _configFile( "pythia.cfg" ),
  phpythiaheader(0),
  phpythia( 0 ),
  fSeed(-1)
{
  std::string thePath = getenv("PYTHIA8");
  if (thePath==NULL) cout << "Could not find $PYTHIA8 path!" << endl;
  thePath += "/xmldoc/";
  pythia = new Pythia8::Pythia(thePath.c_str());
  
  if (outputFormat == HEPMC) _isHepMC = true;
  else _isHepMC = false;
  pythiaToHepMC = new HepMC::Pythia8ToHepMC();
  pythiaToHepMC->set_store_proc(true);
  pythiaToHepMC->set_store_pdf(true);
  pythiaToHepMC->set_store_xsec(true);
  
  hepmcevt = new HepMC::GenEvent(HepMC::Units::GEV, HepMC::Units::MM);

  _triggersOR = true;
  _triggersAND = false;
}

//__________________________________________________________
PHPythia8::~PHPythia8()
{ if (pythia!=0) { delete pythia; } }

//__________________________________________________________
int PHPythia8::Init(PHCompositeNode *topNode)
{
  
  ReadConfig();  
  CreateNodeTree(topNode);

  // event numbering will start from 1
  eventcount = 0;
  
  if ( fSeed < 0 )
  {
    // first try getting seed from /dev/random
    ifstream devrandom;
    devrandom.open("/dev/random",ios::binary);
    devrandom.read((char*)&fSeed,sizeof(fSeed));
    devrandom.close();
    
    if ( fSeed != -1 )
    {
      cout << PHWHERE << " Got seed from /dev/random" << endl;
      fSeed = abs(fSeed)%900000000;
    }
    else
    {
      // /dev/random failed, get the random seed from the time of day, to the microsecond
      //fSeed = (Int_t)(time(NULL)/3);
      cout << PHWHERE << " Getting seed from gettimeofday()" << endl;
      timeval xtime;
      int status = gettimeofday(&xtime,NULL);
      if ( status==0 )
      {
        fSeed = ((xtime.tv_sec << 12) + (xtime.tv_usec&0xfff))%900000000;
      }
      else
      {
        cout << PHWHERE << " something wrong with gettimeofday()" << endl;
      }
    }
  }
  
  
  if ( (fSeed>=0) && (fSeed<=900000000) ) {
    pythia->readString("Random:setSeed = on");
    pythia->readString(Form("Random:seed = %lu",fSeed));
  } else {
    cout << PHWHERE << " ERROR: seed " << fSeed << " is not valid" << endl;
    exit(2); 
  }

  pythia->init();

  PrintConfig();

  TDirectory *orig_dir = gDirectory;

  if(!_isHepMC) xsecfile = new TFile("phpy_xsec.root","RECREATE");
  
  // Set up the normalization TTree
  Tp = new TTree("Tp","Pythia Normalization Tree");
  Tp->Branch("isub",&tp_isub,"isub/i");
  Tp->Branch("proc",&tp_proc,"proc/C");
  Tp->Branch("nevt",&tp_nevt,"nevt/i");
  Tp->Branch("sigma",&tp_sigma,"sigma/D");
  Tp->Branch("sigma_err",&tp_sigma_err,"sigma_err/D");
  Tp->Branch("nevt_sigma",&tp_nevt_sigma,"nevt_sigma/D");
  Tp->Branch("integlumi",&tp_integlumi,"integlumi/D");

  orig_dir->cd();

  return EVENT_OK;

}

//__________________________________________________________
int PHPythia8::End(PHCompositeNode *topNode)
{
  //-* dump out closing info (cross-sections, etc)
  pythia->stat();
  

  if(verbosity > 1) cout << "PHPythia8::End - I'm here!" << endl;

  // Write out the normalization TTree, which has the x-sections, etc
  //std::vector<int> theCodes = pythia->info.codesHard(); //needs >=8186
  if(!_isHepMC)
    {
      for(int i = 0; i < 500; i++)
	{
	  if(pythia->info.nAccepted(i) < 1) continue; 
	  tp_isub = i;
	  //tp_proc = pythia->info.nameProc(i); //needs >= 8186
	  tp_nevt = pythia->info.nAccepted(i);
	  tp_sigma = pythia->info.sigmaGen(i);
	  tp_sigma_err = pythia->info.sigmaErr(i);
	  tp_nevt_sigma = tp_nevt*tp_sigma;
	  if ( tp_sigma!=0. ) tp_integlumi = 1e-9*tp_nevt/tp_sigma;		// in pb^-1
	  else                tp_integlumi = 0.;
	  Tp->Fill();
	}
      
      xsecfile->Write();
      xsecfile->Close();
    }

  cout << " |                                                                                                                 | " << endl; //match pythia printout
  cout << "                         PHPythia8::End - " << eventcount << " events passed trigger" << endl;
  cout << "                         Fraction passed: " << eventcount << "/" << pythia->info.nAccepted() <<" = " << eventcount/float(pythia->info.nAccepted()) << endl;
  cout << " *-------  End PYTHIA Trigger Statistics  -------------------------------------------------------------------------* " << endl;

  return EVENT_OK;
}

//__________________________________________________________
int PHPythia8::ReadConfig(const char *cfg_file)
{

  if( cfg_file ) _configFile = cfg_file;
  cout << "PHPythia8::ReadConfig - Reading " << _configFile << endl;
  
  ifstream infile( _configFile.c_str() ); 
  if (infile.fail ())
  {
    cout << "PHPythia8::ReadConfig - Failed to open file " << _configFile << endl;
    exit(2);
  }

  pythia->readFile(_configFile.c_str());

  return EVENT_OK;
}

//-* print pythia config info
void PHPythia8::PrintConfig() const
{
  //pythia->init.showProcesses();
  pythia->info.list();

  cout << "Using seed " << fSeed << endl;
}

int PHPythia8::process_event(PHCompositeNode *topNode)
{


  if(verbosity > 1) cout << "PHPythia8::process_event - event: " << eventcount << endl;
  
  bool passedGen = false;
  bool passedTrigger = false;
  std::vector<bool> theTriggerResults;
  while (!passedTrigger)
    {
      while (!passedGen)
	{
	  passedGen = pythia->next();
	}
      bool andScoreKeeper = true;
      if(verbosity > 2) cout << "PHPythia8::process_event - triggersize: " << _registeredTriggers.size() << endl;
      for(unsigned int tr = 0; tr < _registeredTriggers.size(); tr++)
	{ 
	  bool trigResult = _registeredTriggers[tr]->Apply(pythia);
	  if(verbosity > 2) cout << "PHPythia8::process_event trigger: " << _registeredTriggers[tr]->GetName() << "  " << trigResult << endl;
	  if(_triggersOR && trigResult)
	    {
	      passedTrigger = true;
	      break;
	    }
	  else if(_triggersAND)
	    {
	      andScoreKeeper &= trigResult;
	    }
	  if(verbosity > 2 && !passedTrigger) cout << "PHPythia8::process_event - failed trigger: " << _registeredTriggers[tr]->GetName() <<  endl;
	}
      if(andScoreKeeper && _triggersAND) passedTrigger = true;
      passedGen = false;
    }


  if(!_isHepMC)
    {
      fillPythiaNode(pythia);      
    }
  else{
    delete hepmcevt;
    hepmcevt = new HepMC::GenEvent(HepMC::Units::GEV, HepMC::Units::MM);

    //cout << pythia->event.size() << endl;
    pythiaToHepMC->fill_next_event(*pythia, hepmcevt, eventcount);
    if(!phhepmcevt->addEvent(hepmcevt)) cout << "PHPythia8::process_event - Failed to add event to HepMC record!" << endl;
    //phhepmcevt->moveVertex(1.,1.,1.,1.);
  }
  
  if (verbosity > 2) cout << "PHPythia8::process_event - FINISHED WHOLE EVENT" << endl;
  if (eventcount < 2 && verbosity > 1) pythia->event.list();   // list full pythia generated event
  if (eventcount >= 2 && verbosity > 5) pythia->event.list();

  ++eventcount;
  return EVENT_OK;
}

int PHPythia8::CreateNodeTree(PHCompositeNode *topNode)
{

  PHCompositeNode *dstNode;
  PHNodeIterator iter(topNode);

  dstNode = dynamic_cast<PHCompositeNode*>(iter.findFirst("PHCompositeNode", "DST"));
  if (!dstNode)
    {
      cout << PHWHERE << "DST Node missing doing nothing" << endl;
      return -1;
    }

  if(_isHepMC)
    {
      //PHIODataNode<HepMC::GenEvent> *newNode = new PHIODataNode<>(phhepmcevt,"HEPMC","GenEvent");
      phhepmcevt = new PHHepMCGenEvent();
      PHObjectNode_t *newNode = new PHObjectNode_t(phhepmcevt,"PHHepMCGenEvent","PHObject");
      dstNode->addNode(newNode);
    }
  else{
        
    //-* pythia header information
    phpythiaheader = new PHPythiaHeaderV2();
    PHObjectNode_t *PHPythiaHeaderNode = new PHObjectNode_t(phpythiaheader, "PHPythiaHeader", "PHObject");
    dstNode->addNode(PHPythiaHeaderNode);
    
    //-* pythia particle information
    phpythia = new PHPythiaContainerV2(true);
    PHObjectNode_t *PHPythiaNode = new PHObjectNode_t(phpythia, "PHPythia", "PHObject");
    dstNode->addNode(PHPythiaNode);
  } 


  return 0;
}

int PHPythia8::ResetEvent(PHCompositeNode *topNode)
{
  PHNodeIterator mainIter(topNode);
  PHNodeReset reset;
  if (mainIter.cd(Name()))
    {
      mainIter.forEach(reset);
    }
  
  //if(_isHepMC)
  //  {
      //PHDataNode<HepMC::GenEvent> *HepMCNode = dynamic_cast<PHDataNode<HepMC::GenEvent> *>(mainIter.findFirst("PHDataNode","HEPMC"));
      //    HepMCNode->setData(NULL); // set pointer in Node to NULL before deleting it      
      //delete hepmcevt; //why?
      //hepmcevt = NULL; //why?
  // }

  return 0;
}



void PHPythia8::fillPythiaNode(Pythia8::Pythia *pythia)
{

  Int_t numpart = pythia->event.size() - 1;
  TClonesArray *fParticles = new TClonesArray("TParticle",1000);
  fParticles->Clear();
  TClonesArray &a = *((TClonesArray*)fParticles);
  
  if(verbosity > 2) cout << "PHPythia8::process_event - numparticles: " << numpart << endl;
  for (Int_t i = 1; i <= numpart; i++) 
    {
      /* This would require changing all of the container
	 new(a[i]) TParticle(pythia->event[i].id(),
	 pythia->event[i].isFinal(),
	 pythia->event[i].mother1()  - 1,
	 pythia->event[i].mother2()  - 1,
	 pythia->event[i].daughter1() - 1,
	 pythia->event[i].daughter2() - 1,
	 pythia->event[i].px(),       // [GeV/c]
	 pythia->event[i].py(),       // [GeV/c] 
	 pythia->event[i].pz(),       // [GeV/c]
	 pythia->event[i].e(),        // [GeV]
	 pythia->event[i].xProd(),    // [mm] 
	 pythia->event[i].yProd(),    // [mm]
	 pythia->event[i].zProd(),    // [mm] 
	 pythia->event[i].tProd());   // [mm/c]
      */
      new(a[i-1]) TMCParticle(pythia->event[i].status(),
			      pythia->event[i].id(),
			      pythia->event[i].mother1(), //TMC is old and has no space for mother2...
			      pythia->event[i].daughter1(),
			      pythia->event[i].daughter2(),
			      pythia->event[i].px(),       // [GeV/c]
			      pythia->event[i].py(),       // [GeV/c] 
			      pythia->event[i].pz(),       // [GeV/c]
			      pythia->event[i].e(),        // [GeV]
			      pythia->event[i].m(),
			      pythia->event[i].xProd(),    // [mm]
			      pythia->event[i].yProd(),    // [mm]
			      pythia->event[i].zProd(),    // [mm] 
			      pythia->event[i].tProd(),
			      pythia->event[i].tau());   // [mm/c]
      
      if (verbosity > 8) cout << "particle: " << i << "  " << pythia->event[i].id() << endl;
      
    }
  
  if (verbosity > 3) cout << "PHPythia8::process_event - finished filling TClonesArray" << endl;
  
  Int_t numParticles = a.GetLast() + 1;	
  int nstable = 0;
  for (Int_t ipart=0; ipart<numParticles; ipart++)
    { 
      // get the particle information
      TMCParticle *particle = (TMCParticle*)a.At(ipart);	
      phpythia->addParticle(*particle);
      if ( phpythia->isStable(ipart) ) ++nstable;
    }
  
  if (verbosity > 3) cout << "PHPythia8::process_event - nstable: " << nstable << endl;
  
  phpythiaheader->SetEvt(eventcount);		// Event number
  phpythiaheader->SetNpart(numpart);	// Number of particle entries in entire history
  phpythiaheader->SetProcessid(pythia->info.code());	// Process ID
  phpythiaheader->SetParId_1(pythia->info.id1());	// KF codes for partons participating in hard scattering
  phpythiaheader->SetParId_2(pythia->info.id2());	// KF codes for partons participating in hard scattering
  phpythiaheader->SetX1(pythia->info.x1());		// Bjorken x1,x2
  phpythiaheader->SetX2(pythia->info.x2());
  phpythiaheader->SetSvar(pythia->info.sHat());	// partonic s,t,u
  phpythiaheader->SetTvar(pythia->info.tHat());
  phpythiaheader->SetUvar(pythia->info.uHat());
  phpythiaheader->SetQsqr(pythia->info.Q2Ren());	// Q squared
  phpythiaheader->SetPt(pythia->info.pTHat());		// transverse momentum
  
  // primary vertex information. Assume position is at zero by default
  phpythiaheader->SetPrimaryVertexX(0);
  phpythiaheader->SetPrimaryVertexY(0);
  phpythiaheader->SetPrimaryVertexZ(0);
  
  // rapidity of parton parton com frame
  phpythiaheader->SetYcom(pythia->info.y());	
   
  
}


void PHPythia8::registerTrigger(PHPy8GenTrigger *theTrigger)
{
  if(verbosity > 1) cout << "PHPythia8::registerTrigger - trigger " << theTrigger->GetName() << " registered" << endl;
  _registeredTriggers.push_back(theTrigger);
}
