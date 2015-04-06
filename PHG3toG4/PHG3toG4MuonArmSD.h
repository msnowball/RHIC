#ifndef PHG3toG4MuonArmSD_h
#define PHG3toG4MuonArmSD_h 1

#include "G4VSensitiveDetector.hh"
#include "PHG3toG4MuonArmHit.h"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class PHG3toG4MuonArmSD : public G4VSensitiveDetector
{
 public:
  PHG3toG4MuonArmSD(G4String name);
  virtual ~PHG3toG4MuonArmSD();
  
  virtual void Initialize(G4HCofThisEvent*HCE);
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);

  void SetMinEnergyDeposit(G4double e)
  { fMinEnergyDep = e;}
  
 private:
  PHG3toG4MuonArmHitsCollection* fHitsCollection;
  G4int fHCID;
  G4int fCollectionSize;
  G4double fMinEnergyDep;
};


#endif
