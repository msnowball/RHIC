#ifndef PHG3toG4SvxSD_h
#define PHG3toG4SvxSD_h 1

#include <vector>
#include "G4VSensitiveDetector.hh"
#include "PHG3toG4SvxHit.h"

class G4Step;
class G4HCofThisEvent;
class G4TouchableHistory;

class PHG3toG4SvxSD : public G4VSensitiveDetector
{
 public:
  PHG3toG4SvxSD(G4String name);
  virtual ~PHG3toG4SvxSD();
  
  virtual void Initialize(G4HCofThisEvent*HCE);
  virtual G4bool ProcessHits(G4Step* aStep, G4TouchableHistory* ROhist);

  void SetMinEnergyDeposit(G4double e)
  { fMinEnergyDep = e;}
  
 private:
  PHG3toG4SvxHitsCollection* fHitsCollection;
  G4int fHCID;
  G4int fCollectionSize;
  G4double fMinEnergyDep;
  std::vector<G4int> layerMap;
  std::vector<G4int> armMap;
};


#endif
