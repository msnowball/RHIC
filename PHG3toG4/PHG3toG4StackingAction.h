#ifndef PHG3toG4StackingAction_h
#define PHG3toG4StackingAction_h 1

#include "G4UserStackingAction.hh"
#include "globals.hh"
#include "PHG3toG4RootManager.h"

/// Stacking action class : manage the newly generated particles
///
/// One wishes do not track secondary neutrino.Therefore one kills it 
/// immediately, before created particles will  put in a stack.


class PHG3toG4StackingAction : public G4UserStackingAction
{
  public:
    PHG3toG4StackingAction();
    virtual ~PHG3toG4StackingAction();

    virtual G4ClassificationOfNewTrack ClassifyNewTrack(const G4Track*);        

    void SetMinTrackEnergy(G4double e);

 private:
    G4double _theMinTrackEnergy;
    PHG3toG4RootManager *_theRootMgr;

};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

