/************************************************************/
/*     Class: PHG3toG4EventAction                           */
/*                                                          */
/*     Desc: Runs before and after every event              */
/*           Does the heavy lifting in converting to        */
/*           PISAEvent format and getting hit collections   */
/*                                                          */
/*   Author: Matt Snowball (snowball.at.rcf.rhic.bnl.gov)   */
/*                                                          */
/************************************************************/

#ifndef PHG3toG4EventAction_h
#define PHG3toG4EventAction_h 1

#include "PHG3toG4SvxHit.h"
#include "PHG3toG4MuonArmHit.h"
#include "PHG3toG4MuonIDHit.h"
#include "PHG3toG4MuonRPCHit.h"
#include "PHG3toG4BbcHit.h"

#include "G4UserEventAction.hh"

#include "G4THitsMap.hh"
#include "globals.hh"
#include <vector>

class G4SDManager;

class PHG3toG4RootManager;
class PHG3toG4SvxPara;
class PHG3toG4BbcPara;
class PHG3toG4MuonIDPara;
class PHG3toG4MuonArmPara;


class PHG3toG4EventAction : public G4UserEventAction
{
public:
  PHG3toG4EventAction();
  virtual ~PHG3toG4EventAction();

  virtual void  BeginOfEventAction(const G4Event* event);
  virtual void    EndOfEventAction(const G4Event* event);

  void Verbosity(G4int v)
  { _verbosity = v;}
    
private:
  G4int _verbosity;

  int theDate;
  
  G4THitsMap<G4double>* GetHitsCollection(G4int hcID,
                                          const G4Event* event) const;
  G4double GetSum(G4THitsMap<G4double>* hitsMap) const;

  G4SDManager *SDman;
  G4int svxHCID, mutHCID, muiHCID, mupcHCID, bbcHCID;
  PHG3toG4SvxHitsCollection* SvxHC;
  PHG3toG4MuonArmHitsCollection* MutHC;
  PHG3toG4MuonIDHitsCollection* MuiHC;
  PHG3toG4MuonRPCHitsCollection* MupcHC;
  PHG3toG4BbcHitsCollection* BbcHC;

  // data members                  
  G4int eventCounter;
  std::vector<float> Data;
  void encodePisaEvt_Svx(PHG3toG4SvxHitsCollection* hits);
  void encodePisaEvt_Bbc(PHG3toG4BbcHitsCollection* hits);
  void encodePisaEvt_Mui(PHG3toG4MuonIDHitsCollection* hits);
  void encodePisaEvt_Mut(PHG3toG4MuonArmHitsCollection* hits);
  void encodePisaEvt_Mupc(PHG3toG4MuonRPCHitsCollection* hits);
  void encodePisaEvt_Kin();

  PHG3toG4RootManager *_theRootMgr;

  //Para matching
  PHG3toG4SvxPara *svxPara;
  int iData_svx[500];
  float fData_svx[500];

  PHG3toG4BbcPara *bbcPara;
  int iData_bbc[500];
  float fData_bbc[500];

  PHG3toG4MuonIDPara *muiPara;
  int iData_mui[500];
  float fData_mui[500];

  PHG3toG4MuonArmPara *mutPara;
  int iData_mut[1000];
  float fData_mut[1000];
  
 
  
};
                     

#endif

    
