#include "PHG3toG4MuonRPCSD.h"
#include "PHG3toG4MuonRPCHit.h"
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

PHG3toG4MuonRPCSD::PHG3toG4MuonRPCSD(G4String name): 
  G4VSensitiveDetector(name), 
  fHitsCollection(0), 
  fHCID(-1),
  fMinEnergyDep(0)
{
    collectionName.insert("muonRPCHitCollection");
    fCollectionSize = 0;
    
    //map copy number to plane ID for hit
    planeMap.push_back(-1);//start from i=1
    planeMap.push_back(1);//copy #1 --> plane 1N
    planeMap.push_back(2);
    planeMap.push_back(3);
    planeMap.push_back(4);
    planeMap.push_back(5);
    planeMap.push_back(-1);//Descoped 6th plane, but still in G3
    planeMap.push_back(1);//copy #7 --> plane 1S
    planeMap.push_back(2);
    planeMap.push_back(3);
    planeMap.push_back(4);
    planeMap.push_back(5);
    planeMap.push_back(-1);

}


PHG3toG4MuonRPCSD::~PHG3toG4MuonRPCSD()
{}


void PHG3toG4MuonRPCSD::Initialize(G4HCofThisEvent* hce)
{
  fHitsCollection = new PHG3toG4MuonRPCHitsCollection(SensitiveDetectorName,collectionName[0]);
  if (fHCID<0) { fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); }
  hce->AddHitsCollection(fHCID,fHitsCollection);
  fCollectionSize = 0;
}


G4bool PHG3toG4MuonRPCSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    G4double pId = step->GetTrack()->GetDefinition()->GetPDGEncoding();
    G4double charge = step->GetTrack()->GetDefinition()->GetPDGCharge();
    if (charge==0. && abs(pId) > 0) return true;
    
    G4double edep = step->GetTotalEnergyDeposit();
    //if (edep==0.) return true;
    if (edep/MeV < fMinEnergyDep) return true;

    G4TouchableHistory* touchable = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
    //Ignore hits in 6th plane that was descoped
    if(planeMap[(int)touchable->GetVolume(2)->GetCopyNo()] < 0) return true;
 

    G4TouchableHistory* postTouchable = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
    G4VPhysicalVolume* motherPhysical = touchable->GetVolume(1); // mother
    //G4int copyNo = motherPhysical->GetCopyNo();//Need to find layerID
    G4String mvn = motherPhysical->GetName();

    G4ThreeVector worldPosIn = step->GetPreStepPoint()->GetPosition();
    G4ThreeVector localPosIn = touchable->GetHistory()->GetTopTransform().TransformPoint(worldPosIn);
    G4ThreeVector worldPosOut = step->GetPostStepPoint()->GetPosition();
    G4ThreeVector localPosOut = postTouchable->GetHistory()->GetTopTransform().TransformPoint(worldPosOut);
    G4ThreeVector momIn = step->GetPreStepPoint()->GetMomentum();
    G4ThreeVector momOut = step->GetPostStepPoint()->GetMomentum();
    G4String vn = step->GetTrack()->GetVolume()->GetName();

    G4int trkId = step->GetTrack()->GetTrackID();


    G4double armId;
    if(worldPosIn.z() > 0) armId = 1;
    else armId = -1;

    PHG3toG4MuonRPCHit* hit = new PHG3toG4MuonRPCHit();
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
    hit->SetTrackID(trkId);
    hit->SetParticleID(pId);
    hit->SetVolumeName(vn);
    hit->SetMotherVolumeName(mvn);

    int detID = -1;
    if(vn == "UG1N") detID = 1;
    if(vn == "UG3N") detID = 3;
    if(vn == "UG1S") detID = 4;
    if(vn == "UG3S") detID = 6;
    hit->SetDetID(detID);
    hit->SetStepLength(step->GetStepLength()/cm);

    PHG3toG4MuonRPCHit* compareHit = NULL;
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

