/************************************************************/
/*     Class: PHG3toG4RootManager                           */
/*                                                          */
/*     Desc: Handles the filling and writing of             */
/*           the PISAEvent tree                             */
/*           Also takes care of the KinHits (track info)    */
/*                                                          */
/*   Author: Matt Snowball (snowball.at.rcf.rhic.bnl.gov)   */
/*                                                          */
/************************************************************/

#ifndef __PHG3toG4RootManager_h__
#define __PHG3toG4RootManager_h__

#include <fstream>
#include <string>
#include <SubsysReco.h>
#include <Rtypes.h>
#include <string>
#include <vector>
#include <set>
#include <map>
#include "PHG3toG4KinHit.h"
#include "G4Track.hh"


class PISAEvent;
class TFile;
class TTree;


class PHG3toG4RootManager
{
public:
  

  PHG3toG4RootManager(std::string fileName = "PISAEvent.root", std::string treeName = "T");

  virtual ~PHG3toG4RootManager();
  static PHG3toG4RootManager *GetInstance();
  

  int Init();
  int SetEventHeader(std::vector<float> Data);
  int EndEvent();
  int End();

  void AddKinHit(PHG3toG4KinHit *hit);
  PHG3toG4KinHit GetKinHit(int i);
  int KinHitSize();
  void FinishKinHits();
  bool IsKinHitRecorded(int ID);

  void Verbosity(int v)
  {_verbosity = v;}

  void CompressionLevel(int c)
  {_theCompressionLevel = c;}

  PISAEvent *pisaevent;

  void AddStackTrack(const G4Track* theTrack)
  {
    int id = theTrack->GetTrackID();
    fAllTracksInStack[id] = theTrack;
  }

 private:
  static PHG3toG4RootManager *fgInstance; // Self pointer                                                                                                                                                                                                                      
  int _verbosity;
  std::string _theFileName;
  std::string _theTreeName;
  int _theCompressionLevel;
  
  TFile *hfile; 
  TTree *tree;
  long nbytes;

  PHG3toG4KinHitsCollection* fKinHitsCollection;
  G4int fCollectionSize;
  std::map<int, PHG3toG4KinHit*> fKinHitMap;
  std::vector<int> fKinHitsWithParent;
  std::map<int,const G4Track*> fAllTracksInStack;

};

#endif	

