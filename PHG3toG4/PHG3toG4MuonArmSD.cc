#include "PHG3toG4MuonArmSD.h"
#include "PHG3toG4MuonArmHit.h"
#include "PHG3toG4RootManager.h"
#include "PHG3toG4KinHit.h"
#include "PHG3toG4UserTrackInfo.h"

#include "G4HCofThisEvent.hh"
#include "G4TouchableHistory.hh"
#include "G4Track.hh"
#include "G4Step.hh"
#include "G4SDManager.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"


PHG3toG4MuonArmSD::PHG3toG4MuonArmSD(G4String name): 
  G4VSensitiveDetector(name), 
  fHitsCollection(0), 
  fHCID(-1),
  fMinEnergyDep(0)
{
    collectionName.insert("muonArmHitCollection");
    fCollectionSize = 0;
}


PHG3toG4MuonArmSD::~PHG3toG4MuonArmSD()
{}


void PHG3toG4MuonArmSD::Initialize(G4HCofThisEvent* hce)
{
  fHitsCollection = new PHG3toG4MuonArmHitsCollection(SensitiveDetectorName,collectionName[0]);
  if (fHCID<0) { fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); }
  hce->AddHitsCollection(fHCID,fHitsCollection);
  fCollectionSize = 0;
}


G4bool PHG3toG4MuonArmSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    G4double pId = step->GetTrack()->GetDefinition()->GetPDGEncoding();
    G4double charge = step->GetTrack()->GetDefinition()->GetPDGCharge();
    if (charge==0. && abs(pId) > 0) return true;
    
    G4double edep = step->GetTotalEnergyDeposit();
    //if (edep==0.) return true;
    if (edep/MeV < fMinEnergyDep) return true;

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
    G4String vn = step->GetTrack()->GetVolume()->GetName();

    G4String detIdString = vn.substr(2);
    G4int station = atoi(vn.substr(2,1).c_str());//third character
    G4int layer = atoi(vn.substr(3,1).c_str());//fourth character
    G4int detId = atoi(detIdString); //third plus fourth character
    G4int trkId = step->GetTrack()->GetTrackID();
    G4double armId;
    if(station < 4) armId = 1;
    else armId = -1;

    PHG3toG4MuonArmHit* hit = new PHG3toG4MuonArmHit();
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
    hit->SetLayerID(layer);
    hit->SetStationID(station);
    hit->SetDetID(detId);
    hit->SetTrackID(trkId);
    hit->SetParticleID(pId);
    hit->SetVolumeName(vn);
    hit->SetMotherVolumeName(mvn);

    PHG3toG4MuonArmHit* compareHit = NULL;
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

	//Save Track info
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
