#ifndef PHG3toG4MuonIDSD_h
#define PHG3toG4MuonIDSD_h 1

#include "G4VSensitiveDetector.hh"

#include "PHG3toG4MuonIDHit.h"
#include <vector>

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class PHG3toG4MuonIDSD : public G4VSensitiveDetector
{
 public:
  PHG3toG4MuonIDSD(G4String name);
  virtual ~PHG3toG4MuonIDSD();
  
  virtual void Initialize(G4HCofThisEvent*HCE);
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);
  
  void SetMinEnergyDeposit(G4double e)
  { fMinEnergyDep = e;}

 private:
  PHG3toG4MuonIDHitsCollection* fHitsCollection;
  G4int fHCID;
  G4int fCollectionSize;
  std::vector<G4int> planeMap;
  G4double fMinEnergyDep;

};


#endif
