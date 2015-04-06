#include "PHG3toG4SvxSD.h"
#include "PHG3toG4SvxHit.h"
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

PHG3toG4SvxSD::PHG3toG4SvxSD(G4String name): 
  G4VSensitiveDetector(name), 
  fHitsCollection(0), 
  fHCID(-1),
  fMinEnergyDep(0)
{
    collectionName.insert("svxHitCollection");
    fCollectionSize = 0;
    //maps detId to actual layer and arm
    for(unsigned int i=0; i < 20; i++)
      {
	if(i+1 <= 4)
	  {
	    layerMap.push_back(i+1);
	    armMap.push_back(0);
	  }
	else if(i+1 > 4 && i+1 <= 12)
	  {
	    armMap.push_back(-1);
	    if(i+1 <= 6){layerMap.push_back(4);}
	    else if(i+1 <= 8){layerMap.push_back(3);}
	    else if(i+1 <= 10){layerMap.push_back(2);}
	    else if(i+1 <= 12){layerMap.push_back(1);}
	  }
	else if(i+1 > 12)
	  {
	    armMap.push_back(1);
	    if(i+1 <= 14){layerMap.push_back(1);}
	    else if(i+1 <= 16){layerMap.push_back(2);}
	    else if(i+1 <= 18){layerMap.push_back(3);}
	    else if(i+1 <= 20){layerMap.push_back(4);}
	  }
      }

}


PHG3toG4SvxSD::~PHG3toG4SvxSD()
{}


void PHG3toG4SvxSD::Initialize(G4HCofThisEvent* hce)
{

  fHitsCollection = new PHG3toG4SvxHitsCollection(SensitiveDetectorName,collectionName[0]);
  if (fHCID<0) { fHCID = G4SDManager::GetSDMpointer()->GetCollectionID(fHitsCollection); }
  hce->AddHitsCollection(fHCID,fHitsCollection);
  fCollectionSize = 0;
}


G4bool PHG3toG4SvxSD::ProcessHits(G4Step* step, G4TouchableHistory*)
{
    G4double pId = step->GetTrack()->GetDefinition()->GetPDGEncoding();
    //G4double charge = step->GetTrack()->GetDefinition()->GetPDGCharge();
    //if (charge==0. && abs(pId) > 0) return true;
    
    G4double edep = step->GetTotalEnergyDeposit();
    //if (edep/MeV < fMinEnergyDep) return true;


    G4TouchableHistory* touchable = (G4TouchableHistory*)(step->GetPreStepPoint()->GetTouchable());
    G4TouchableHistory* postTouchable = (G4TouchableHistory*)(step->GetPostStepPoint()->GetTouchable());

    G4ThreeVector worldPosIn = step->GetPreStepPoint()->GetPosition();
    G4ThreeVector localPosIn = touchable->GetHistory()->GetTopTransform().TransformPoint(worldPosIn);
    G4ThreeVector worldPosOut = step->GetPostStepPoint()->GetPosition();
    G4ThreeVector localPosOut = postTouchable->GetHistory()->GetTopTransform().TransformPoint(worldPosOut);
    G4ThreeVector momIn = step->GetPreStepPoint()->GetMomentum();
    G4ThreeVector momOut = step->GetPostStepPoint()->GetMomentum();
    G4String vn = step->GetTrack()->GetVolume()->GetName();
    G4int trkId = step->GetTrack()->GetTrackID();


    G4String mvn;
    std::vector<G4int> hitV;
    if(step->GetPreStepPoint()->GetTouchable()->GetVolume(0)->GetName() == "SISI")
      {
	mvn = step->GetPreStepPoint()->GetTouchable()->GetVolume(3)->GetName();
	//hit volume map
	//hitV.push_back(step->GetPreStepPoint()->GetTouchable()->GetVolume(8)->GetCopyNo());
	hitV.push_back(step->GetPreStepPoint()->GetTouchable()->GetVolume(7)->GetCopyNo());
	hitV.push_back(step->GetPreStepPoint()->GetTouchable()->GetVolume(6)->GetCopyNo());
	hitV.push_back(step->GetPreStepPoint()->GetTouchable()->GetVolume(5)->GetCopyNo());
	hitV.push_back(step->GetPreStepPoint()->GetTouchable()->GetVolume(4)->GetCopyNo());
	hitV.push_back(1);
	hitV.push_back(step->GetPreStepPoint()->GetTouchable()->GetVolume(3)->GetCopyNo());
	hitV.push_back(step->GetPreStepPoint()->GetTouchable()->GetVolume(2)->GetCopyNo());
	hitV.push_back(step->GetPreStepPoint()->GetTouchable()->GetVolume(1)->GetCopyNo());
	hitV.push_back(step->GetPreStepPoint()->GetTouchable()->GetVolume(0)->GetCopyNo());
      }
    else
      {
	mvn = step->GetPreStepPoint()->GetTouchable()->GetVolume(1)->GetName();
	G4String testVol = step->GetPreStepPoint()->GetTouchable()->GetVolume(3)->GetName();
	if (testVol == "SVXE" || testVol == "SVXW")
	  {
	    //NEW (03.24.15) setup for VTX with E/W holder volumes
	    hitV.push_back(1);
	    hitV.push_back(1);
	    hitV.push_back(1);
	    hitV.push_back(step->GetPreStepPoint()->GetTouchable()->GetVolume(2)->GetCopyNo());//Ladder - 3
	    hitV.push_back(step->GetPreStepPoint()->GetTouchable()->GetVolume(0)->GetCopyNo());//Sensor - 4
	  }
	else{
	  //OLD pisa setup for VTX
	  //hit volume map
	  hitV.push_back(step->GetPreStepPoint()->GetTouchable()->GetVolume(4)->GetCopyNo());
	  hitV.push_back(step->GetPreStepPoint()->GetTouchable()->GetVolume(3)->GetCopyNo());
	  hitV.push_back(step->GetPreStepPoint()->GetTouchable()->GetVolume(2)->GetCopyNo());
	  hitV.push_back(step->GetPreStepPoint()->GetTouchable()->GetVolume(1)->GetCopyNo());
	  hitV.push_back(step->GetPreStepPoint()->GetTouchable()->GetVolume(0)->GetCopyNo());
	}
      }
    G4int detId = atoi(mvn.substr(2,2).c_str());

    /*
    //translation full->half disks
    if(hitV.size() >= 7 && vn == "SISI")
      {
	//move to next half disk
	if(hitV[6] > 24)
	  {
	    hitV[6] = hitV[6] - 24;
	    detId += 1;
	  }
      }
    */

    PHG3toG4SvxHit* hit = new PHG3toG4SvxHit();
    hit->SetWorldPosIn(worldPosIn/cm);
    hit->SetWorldPosOut(worldPosOut/cm);
    hit->SetLocalPosIn(localPosIn/cm);
    hit->SetLocalPosOut(localPosOut/cm);
    hit->SetMomentumIn(momIn/GeV);
    hit->SetMomentumOut(momOut/GeV);
    hit->SetTimeIn(step->GetPreStepPoint()->GetGlobalTime());
    hit->SetTimeOut(step->GetPostStepPoint()->GetGlobalTime());
    hit->SetdE(edep/GeV);
    hit->SetArmID(armMap[detId-1]);
    hit->SetLayerID(layerMap[detId-1]);
    hit->SetDetID(detId);
    hit->SetTrackID(trkId);
    hit->SetParticleID(pId);
    hit->SetVolumeName(vn);
    hit->SetMotherVolumeName(mvn);
    for(unsigned int i = 0; i < hitV.size(); i++) hit->SetHitVolume(i,hitV[i]);


    PHG3toG4SvxHit* compareHit = NULL;
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

