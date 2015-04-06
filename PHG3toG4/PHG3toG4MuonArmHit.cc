#include "PHG3toG4MuonArmHit.h"

#include "G4VVisManager.hh"
#include "G4VisAttributes.hh"
#include "G4Circle.hh"
#include "G4Colour.hh"
#include "G4AttDefStore.hh"
#include "G4AttDef.hh"
#include "G4AttValue.hh"
#include "G4UIcommand.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

G4ThreadLocal G4Allocator<PHG3toG4MuonArmHit>* PHG3toG4MuonArmHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PHG3toG4MuonArmHit::PHG3toG4MuonArmHit():
  G4VHit(), 
  fParticleID(0),
  fTrackID(-1),
  fLayerID(-1), 
  fStationID(-99),
  fDetID(-99),
  fArmID(-99),
  fVolumeName(""),
  fMotherVolumeName(""),
  fTimeIn(0.),
  fTimeOut(0.),
  fdE(0.),
  fIsFirstHit(false),
  fIsLastHit(false),
  fLocalPosIn(0), 
  fLocalPosOut(0), 
  fWorldPosIn(0),
  fWorldPosOut(0),
  fWorldPosAvg(0),
  fMomentumIn(0),
  fMomentumOut(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PHG3toG4MuonArmHit::~PHG3toG4MuonArmHit()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PHG3toG4MuonArmHit::PHG3toG4MuonArmHit(const PHG3toG4MuonArmHit &right):
  G4VHit() 
{
  fParticleID = right.fParticleID;
  fTrackID = right.fTrackID;
  fLayerID = right.fLayerID;
  fStationID = right.fStationID;
  fDetID = right.fDetID;
  fArmID = right.fArmID;
  fVolumeName = right.fVolumeName;
  fMotherVolumeName = right.fMotherVolumeName;
  fTimeIn = right.fTimeIn;
  fTimeOut = right.fTimeOut;
  fdE = right.fdE;
  fIsFirstHit = right.fIsFirstHit;
  fIsLastHit = right.fIsLastHit;
  fWorldPosIn = right.fWorldPosIn;
  fWorldPosOut = right.fWorldPosOut;
  fWorldPosAvg = right.fWorldPosAvg;
  fLocalPosIn = right.fLocalPosIn;
  fLocalPosOut = right.fLocalPosOut;
  fMomentumIn = right.fMomentumIn;
  fMomentumOut = right.fMomentumOut;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const PHG3toG4MuonArmHit& PHG3toG4MuonArmHit::operator=(const PHG3toG4MuonArmHit &right)
{
  fParticleID = right.fParticleID;
  fTrackID = right.fTrackID;
  fLayerID = right.fLayerID;
  fStationID = right.fStationID;
  fDetID = right.fDetID;
  fArmID = right.fArmID;
  fVolumeName = right.fVolumeName;
  fMotherVolumeName = right.fMotherVolumeName;
  fTimeIn = right.fTimeIn;
  fTimeOut = right.fTimeOut;
  fdE = right.fdE;
  fIsFirstHit = right.fIsFirstHit;
  fIsLastHit = right.fIsLastHit;
  fWorldPosIn = right.fWorldPosIn;
  fWorldPosOut = right.fWorldPosOut;
  fWorldPosAvg = right.fWorldPosAvg;
  fLocalPosIn = right.fLocalPosIn;
  fLocalPosOut = right.fLocalPosOut;
  fMomentumIn = right.fMomentumIn;
  fMomentumOut = right.fMomentumOut;
  return *this;
}

const PHG3toG4MuonArmHit& PHG3toG4MuonArmHit::operator+=(const PHG3toG4MuonArmHit &right)
{

  fdE = fdE + right.fdE;
  fWorldPosOut = right.fWorldPosOut;
  fWorldPosAvg.setX((fWorldPosIn.x()+right.fWorldPosOut.x())/2);
  fWorldPosAvg.setY((fWorldPosIn.y()+right.fWorldPosOut.y())/2);
  fWorldPosAvg.setZ((fWorldPosIn.z()+right.fWorldPosOut.z())/2);
  fLocalPosOut = right.fLocalPosOut;
  fMomentumOut = right.fMomentumOut;
  fTimeOut = right.fTimeOut;
  
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int PHG3toG4MuonArmHit::operator==(const PHG3toG4MuonArmHit &right) const
{
  return (  fParticleID == right.fParticleID &&  
	    fTrackID == right.fTrackID &&
	    fLayerID == right.fLayerID &&
	    fStationID == right.fStationID &&
	    fDetID == right.fDetID &&
	    fArmID == right.fArmID &&
	    fVolumeName == right.fVolumeName &&
	    fMotherVolumeName == right.fMotherVolumeName &&
	    fTimeIn == right.fTimeIn &&
	    fTimeOut == right.fTimeOut &&
	    fdE == right.fdE &&
	    fIsFirstHit == right.fIsFirstHit &&
	    fIsLastHit == right.fIsLastHit &&
	    fWorldPosIn == right.fWorldPosIn &&
	    fWorldPosOut == right.fWorldPosOut &&
	    fWorldPosAvg == right.fWorldPosAvg &&
	    fLocalPosIn == right.fLocalPosIn &&
	    fLocalPosOut == right.fLocalPosOut &&
	    fMomentumIn == right.fMomentumIn &&
	    fMomentumOut == right.fMomentumOut
	    );

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PHG3toG4MuonArmHit::Draw()
{
    G4VVisManager* pVVisManager = G4VVisManager::GetConcreteInstance();
    if(pVVisManager)
    {
        G4Circle circle(fWorldPosIn);
        circle.SetScreenSize(2);
        circle.SetFillStyle(G4Circle::filled);
        G4Colour colour(1.,1.,0.);
        G4VisAttributes attribs(colour);
        circle.SetVisAttributes(attribs);
        pVVisManager->Draw(circle);
    }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const std::map<G4String,G4AttDef>* PHG3toG4MuonArmHit::GetAttDefs() const
{
    G4bool isNew;
    std::map<G4String,G4AttDef>* store
      = G4AttDefStore::GetInstance("PHG3toG4MuonArmHit",isNew);

    if (isNew) {
        (*store)["HitType"] 
          = G4AttDef("HitType","Hit Type","Physics","","G4String");

        (*store)["ParticleID"] 
          = G4AttDef("ParticleID","Particle ID","Physics","","G4int");

        (*store)["TrackID"] 
          = G4AttDef("TrackID","Track ID","Physics","","G4int");
        
        (*store)["StationID"] 
          = G4AttDef("StationID","Station ID","Physics","","G4int");

        (*store)["LayerID"] 
          = G4AttDef("LayerID","Layer ID","Physics","","G4int");

        (*store)["DetectorID"] 
          = G4AttDef("DetectorID","Detector ID","Physics","","G4int");

        (*store)["ArmID"] 
          = G4AttDef("ArmID","Arm ID","Physics","","G4int");

        (*store)["VolumeName"] 
          = G4AttDef("VolumeName","Volume Name","Physics","","G4String");

        (*store)["MotherVolumeName"] 
          = G4AttDef("MotherVolumeName","Mother Volume Name","Physics","","G4String");
        
        (*store)["TimeIn"] 
          = G4AttDef("TimeIn","TimeIn","Physics","G4BestUnit","G4double");

        (*store)["TimeOut"] 
          = G4AttDef("TimeOut","TimeOut","Physics","G4BestUnit","G4double");

        (*store)["dE"] 
          = G4AttDef("dE","dE","Physics","GeV","G4double");

        (*store)["IsFirstHit"] 
          = G4AttDef("IsFirstHit","IsFirstHit","Physics","","G4bool");

        (*store)["IsLastHit"] 
          = G4AttDef("IsLastHit","IsLastHit","Physics","","G4bool");
        
        (*store)["LocalPosIn"] 
          = G4AttDef("LocalPosIn", "Local Position In", "Physics","cm","G4ThreeVector");

        (*store)["LocalPosOut"] 
          = G4AttDef("LocalPosOut", "Local Position Out", "Physics","cm","G4ThreeVector");

        (*store)["WorldPosIn"] 
          = G4AttDef("WorldPosIn", "Global Position In", "Physics","cm","G4ThreeVector");

        (*store)["WorldPosOut"] 
          = G4AttDef("WorldPosOut", "Global Position Out", "Physics","cm","G4ThreeVector");

        (*store)["WorldPosAvg"] 
          = G4AttDef("WorldPosAvg", "Global Position (In+Out)/2", "Physics","cm","G4ThreeVector");

        (*store)["MomentumIn"] 
          = G4AttDef("MomentumIn", "MomentumIn", "Physics","GeV","G4ThreeVector");

        (*store)["MomentumOut"] 
          = G4AttDef("MomentumOut", "MomentumOut", "Physics","GeV","G4ThreeVector");
    }
    return store;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<G4AttValue>* PHG3toG4MuonArmHit::CreateAttValues() const
{
    std::vector<G4AttValue>* values = new std::vector<G4AttValue>;
    
    values
      ->push_back(G4AttValue("HitType","MuonArmHit",""));
    values
      ->push_back(G4AttValue("ParticleID",G4UIcommand::ConvertToString(fParticleID),""));
    values
      ->push_back(G4AttValue("TrackID",G4UIcommand::ConvertToString(fTrackID),""));
    values
      ->push_back(G4AttValue("SationID",G4UIcommand::ConvertToString(fStationID),""));
    values
      ->push_back(G4AttValue("LayerID",G4UIcommand::ConvertToString(fLayerID),""));
    values
      ->push_back(G4AttValue("DetectorID",G4UIcommand::ConvertToString(fDetID),""));
    values
      ->push_back(G4AttValue("ArmID",G4UIcommand::ConvertToString(fArmID),""));
    values
      ->push_back(G4AttValue("VolumeName",fVolumeName,""));
    values
      ->push_back(G4AttValue("MotherVolumeName",fMotherVolumeName,""));
    values
      ->push_back(G4AttValue("TimeIn",G4BestUnit(fTimeIn,"Time"),""));
    values
      ->push_back(G4AttValue("TimeOut",G4BestUnit(fTimeOut,"Time"),""));
    values
      ->push_back(G4AttValue("dE",G4UIcommand::ConvertToString(fdE/GeV),""));
    values
      ->push_back(G4AttValue("IsFirstHit",G4UIcommand::ConvertToString(fIsFirstHit),""));
    values
      ->push_back(G4AttValue("IsLastHit",G4UIcommand::ConvertToString(fIsLastHit),""));
    values
      ->push_back(G4AttValue("LocalPosIn",G4UIcommand::ConvertToString(fLocalPosIn/cm),""));
    values
      ->push_back(G4AttValue("LocalPosOut",G4UIcommand::ConvertToString(fLocalPosOut/cm),""));
    values
      ->push_back(G4AttValue("WorldPosIn",G4UIcommand::ConvertToString(fWorldPosIn/cm),""));
    values
      ->push_back(G4AttValue("WorldPosOut",G4UIcommand::ConvertToString(fWorldPosOut/cm),""));
    values
      ->push_back(G4AttValue("WorldPosAvg",G4UIcommand::ConvertToString(fWorldPosAvg/cm),""));
    values
      ->push_back(G4AttValue("MomentumIn",G4UIcommand::ConvertToString(fMomentumIn/GeV),""));
    values
      ->push_back(G4AttValue("MomentumOut",G4UIcommand::ConvertToString(fMomentumOut/GeV),""));
    
    return values;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PHG3toG4MuonArmHit::Print()
{
  G4cout << " DetID: " << fDetID << " ArmID " << fArmID << " StationID: " << fStationID  << " LayerID: " << fLayerID << " TrackID: " << fTrackID << " ParticleID: " << fParticleID << G4endl  
	 << "        MVN: " << fMotherVolumeName << "  VN: " << fVolumeName << "   FirstHit: " << fIsFirstHit << "  LastHit: " << fIsLastHit << G4endl
	 << "        TimeIn " << fTimeIn/ns << "ns  TimeOut " << fTimeOut << "ns  dE "<< fdE/GeV  << G4endl 
	 << "        local in ("<<fLocalPosIn.x()<<","<<fLocalPosIn.y()<<") -- global in ("<<fWorldPosIn.x()<<","<<fWorldPosIn.y()<<","<<fWorldPosIn.z()<<")" << G4endl 
	 << "        local out ("<<fLocalPosOut.x()<<","<<fLocalPosOut.y()<<") -- global out ("<<fWorldPosOut.x()<<","<<fWorldPosOut.y()<<","<<fWorldPosOut.z()<<")" << G4endl
	 << "        global avg ("<<fWorldPosAvg.x()<<","<<fWorldPosAvg.y()<<","<<fWorldPosAvg.z()<<")" << G4endl
	 << "        Pin ("<<fMomentumIn.x()/GeV<<","<<fMomentumIn.y()/GeV<<","<<fMomentumIn.z()/GeV<<")" 
	 << "  Pout ("<<fMomentumOut.x()/GeV<<","<<fMomentumOut.y()/GeV<<","<<fMomentumOut.z()/GeV<<")" << G4endl
	 << "----------------------------------------------------------------------------" << G4endl;
}


G4bool PHG3toG4MuonArmHit::isTheSameHit(PHG3toG4MuonArmHit *right)
{

  if(right == NULL) return false;

  //G4cout << fParticleID << "  " <<  right->GetParticleID() << G4endl
  //<< fTrackID << "  " << right->GetTrackID() << G4endl
  //<< fLayerID << "  " << right->GetLayerID() << G4endl
  //<< fStationID << "  " << right->GetStationID() << G4endl
  //<< fDetID << "  " << right->GetDetID() << G4endl
  //<< fArmID << "  " << right->GetArmID() << G4endl 
  //<< fVolumeName << "  " << right->GetVolumeName() << G4endl
  //<< fMotherVolumeName << "  " << right->GetMotherVolumeName() << G4endl
  //<< "------------------------------------------------" << G4endl;

  return (  fParticleID == right->GetParticleID() &&  
	    fTrackID == right->GetTrackID() &&
	    fLayerID == right->GetLayerID() &&
	    fStationID == right->GetStationID() &&
	    fDetID == right->GetDetID() &&
	    fArmID == right->GetArmID() &&
	    fVolumeName == right->GetVolumeName() &&
	    fMotherVolumeName == right->GetMotherVolumeName() 
	    );
}


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void* PHG3toG4MuonArmHit::operator new(size_t)
{
    if (!PHG3toG4MuonArmHitAllocator)
        PHG3toG4MuonArmHitAllocator = new G4Allocator<PHG3toG4MuonArmHit>;
    return (void*)PHG3toG4MuonArmHitAllocator->MallocSingle();
}

void PHG3toG4MuonArmHit::operator delete(void* aHit)
{
    PHG3toG4MuonArmHitAllocator->FreeSingle((PHG3toG4MuonArmHit*) aHit);
}
