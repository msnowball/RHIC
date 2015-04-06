#include "PHG3toG4StackingAction.h"

#include "G4ios.hh"
#include "G4Track.hh"
#include "G4NeutrinoE.hh"
#include "G4Electron.hh"
#include "G4Gamma.hh"
#include "G4SystemOfUnits.hh"

PHG3toG4StackingAction::PHG3toG4StackingAction():
  G4UserStackingAction(),
  _theMinTrackEnergy(0)
{
  _theRootMgr = PHG3toG4RootManager::GetInstance();
}

PHG3toG4StackingAction::~PHG3toG4StackingAction()
{ }

void PHG3toG4StackingAction::SetMinTrackEnergy(G4double e)
{_theMinTrackEnergy = e;}

G4ClassificationOfNewTrack PHG3toG4StackingAction::ClassifyNewTrack(const G4Track* track)
{
  _theRootMgr->AddStackTrack(track);

  if (track->GetParentID() == 0) return fUrgent;
  else if (track->GetDefinition() == G4NeutrinoE::NeutrinoE()){ return fKill;}
  else if (track->GetDefinition() == G4Electron::ElectronDefinition() && track->GetKineticEnergy()/MeV < _theMinTrackEnergy){ return fKill;}
  else if (track->GetDefinition() == G4Gamma::GammaDefinition() && track->GetKineticEnergy()/MeV < _theMinTrackEnergy){ return fKill;}
  else return fWaiting;
  
}

