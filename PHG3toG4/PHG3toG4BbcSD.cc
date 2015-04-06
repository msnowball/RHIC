#include "PHG3toG4BbcSD.h"
#include "PHG3toG4BbcHit.h"
#include "PHG3toG4KinHit.h"
#include "PHG3toG4RootManager.h"
#include "PHG3toG4UserTrackInfo.h"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"


PHG3toG4BbcSD::PHG3toG4BbcSD(G4String name): 
  G4VSensitiveDetector(name), 
  fHitsCollection(0), 
  fHCID(-1),
  fBbcCrystalsSize(132), //66 on each end
  fMinEnergyDep(0.)
{
    collectionName.insert("bbcHitCollection");
    fCollectionSize = 0;
}


PHG3toG4BbcSD::~PHG3toG4BbcSD()
{}


void PHG3toG4BbcSD::Initialize(G4HCofThisEvent* hce)
{
  fHitsCollection = new PHG3toG4BbcHitsCollection(SensitiveDetectorName,collectionName[0]);
  if (fHCID<0) { fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); }
  hce->AddHitsCollection(fHCID,fHitsCollection);
  fCollectionSize = 0;
  /*
  // fill calorimeter hits with zero energy deposition
  for (G4int iBbc=0; iBbc < fBbcCrystalsSize; iBbc++)
    {
      PHG3toG4BbcHit* hit = new PHG3toG4BbcHit();
      fHitsCollection->insert(hit);
    }
  */
}


G4bool PHG3toG4BbcSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    
    G4double edep = step->GetTotalEnergyDeposit();
    //if (edep==0.) return true;
    if (edep/MeV < fMinEnergyDep ) return true;
   

    G4TouchableHistory* touchable = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
    G4TouchableHistory* postTouchable = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
    G4VPhysicalVolume* motherPhysical = touchable->GetVolume(1); // mother
    //G4int copyNo = motherPhysical->GetCopyNo();//Need to find layer ID
    G4String mvn = motherPhysical->GetName();

    G4ThreeVector worldPosIn = step->GetPreStepPoint()->GetPosition();
    G4ThreeVector localPosIn = touchable->GetHistory()->GetTopTransform().TransformPoint(worldPosIn);
    G4ThreeVector worldPosOut = step->GetPostStepPoint()->GetPosition();
    G4ThreeVector localPosOut = postTouchable->GetHistory()->GetTopTransform().TransformPoint(worldPosOut);
    G4ThreeVector momIn = step->GetPreStepPoint()->GetMomentum();
    G4ThreeVector momOut = step->GetPostStepPoint()->GetMomentum();
    G4String vn = touchable->GetVolume(0)->GetName();

    // There are 2 copies of the main holder volumes for the crystals
    G4int detIdMain = touchable->GetVolume(2)->GetCopyNo(); //BBCM --> See http://p25ext.lanl.gov/~hubert/phenix/mutr/g4/index.html
    G4int detId = touchable->GetVolume(1)->GetCopyNo()+(detIdMain*1000); 
    //if(detIdMain == 2) detId += 65; //66-1 (66 on each side) (-1 for vector)
    G4int trkId = step->GetTrack()->GetTrackID();
    G4double pId = step->GetTrack()->GetDefinition()->GetPDGEncoding();
    G4double armId;
    if(worldPosIn.z() > 0) armId = 1;
    else armId = -1;

    PHG3toG4BbcHit* hit = new PHG3toG4BbcHit();
    hit->SetWorldPosIn(worldPosIn/cm);
    hit->SetWorldPosOut(worldPosOut/cm);
    hit->SetLocalPosIn(localPosIn/cm);
    hit->SetLocalPosOut(localPosOut/cm);
    hit->SetMomentumIn(momIn/GeV);
    hit->SetMomentumOut(momOut/GeV);
    hit->SetTimeIn(step->GetPreStepPoint()->GetGlobalTime());
    hit->SetTimeOut(step->GetPostStepPoint()->GetGlobalTime());
    hit->SetdE(edep/GeV);
    hit->SetArmID(armId);
    hit->SetDetID(detId);
    hit->SetTrackID(trkId);
    hit->SetParticleID(pId);
    hit->SetVolumeName(vn);
    hit->SetMotherVolumeName(mvn);
    hit->SetStepLength(step->GetStepLength()/cm);
 
    PHG3toG4BbcHit* compareHit = NULL;
    if(fCollectionSize > 0) compareHit = (*fHitsCollection)[fCollectionSize-1];
    if(hit->isTheSameHit(compareHit) && fCollectionSize > 0)
      {
        *(*fHitsCollection)[fCollectionSize-1] += *hit;
      }
    else
      {
        fCollectionSize++;
        G4ThreeVector avg((worldPosOut.x()+worldPosIn.x())/2, (worldPosOut.y()+worldPosIn.y())/2, (worldPosOut.z()+worldPosIn.z())/2);
        hit->SetWorldPosAvg(avg/cm);

        fHitsCollection->insert(hit);

	PHG3toG4UserTrackInfo *theTrackInfo = (PHG3toG4UserTrackInfo*)(step->GetTrack()->GetUserInformation());

	PHG3toG4KinHit *khit = new PHG3toG4KinHit();
        khit->SetVertexPos(step->GetTrack()->GetVertexPosition()/cm);
	khit->SetMomentum(step->GetTrack()->GetMomentum()/GeV);
        khit->SetTrackID(step->GetTrack()->GetTrackID());
        khit->SetParticleID(pId);
	if(!step->GetTrack()->GetParentID())
          {
            khit->SetParentPartID(0);
            khit->SetParentTrackID(0);
          }
        else{
          khit->SetParentPartID(theTrackInfo->GetOriginalParticleID());
          khit->SetParentTrackID(theTrackInfo->GetOriginalTrackID());
	}
	khit->SetWorldPos(step->GetTrack()->GetPosition()/cm);
	PHG3toG4RootManager::GetInstance()->AddKinHit(khit);

      }

    
    return true;
}

