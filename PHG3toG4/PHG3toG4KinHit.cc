#include "PHG3toG4KinHit.h"

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

G4ThreadLocal G4Allocator<PHG3toG4KinHit>* PHG3toG4KinHitAllocator;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PHG3toG4KinHit::PHG3toG4KinHit():
  G4VHit(), 
  fParticleID(0),
  fTrackID(-1),
  fParentPartID(0),
  fParentTrackID(-1),
  fProcessType(-1),
  fVertexXYZ(-999), 
  fWorldPos(-999),
  fMomentum(0)
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PHG3toG4KinHit::~PHG3toG4KinHit()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

PHG3toG4KinHit::PHG3toG4KinHit(const PHG3toG4KinHit &right):
  G4VHit() 
{
  fParticleID = right.fParticleID;
  fTrackID = right.fTrackID;
  fParentPartID = right.fParentPartID;
  fParentTrackID = right.fParentTrackID;
  fProcessType = right.fProcessType;
  fVertexXYZ = right.fVertexXYZ;
  fWorldPos = right.fWorldPos;
  fMomentum = right.fMomentum;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const PHG3toG4KinHit& PHG3toG4KinHit::operator=(const PHG3toG4KinHit &right)
{
  fParticleID = right.fParticleID;
  fTrackID = right.fTrackID;
  fParentPartID = right.fParentPartID;
  fParentTrackID = right.fParentTrackID;
  fProcessType = right.fProcessType;
  fVertexXYZ = right.fVertexXYZ;
  fWorldPos = right.fWorldPos;
  fMomentum = right.fMomentum;
  return *this;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int PHG3toG4KinHit::operator==(const PHG3toG4KinHit &right) const
{
  return (  fParticleID == right.fParticleID &&  
	    fTrackID == right.fTrackID &&
	    fParentPartID == right.fParentPartID &&
	    fParentTrackID == right.fParentTrackID &&
	    //fProcessType == right.fProcessType &&
	    fVertexXYZ == right.fVertexXYZ 
	    //fWorldPos == right.fWorldPos &&
	    //fMomentum == right.fMomentum
	    );

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PHG3toG4KinHit::Draw()
{
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

const std::map<G4String,G4AttDef>* PHG3toG4KinHit::GetAttDefs() const
{
    G4bool isNew;
    std::map<G4String,G4AttDef>* store
      = G4AttDefStore::GetInstance("PHG3toG4KinHit",isNew);

    if (isNew) {
        (*store)["HitType"] 
          = G4AttDef("HitType","Hit Type","Physics","","G4String");

        (*store)["ParticleID"] 
          = G4AttDef("ParticleID","Particle ID","Physics","","G4int");

        (*store)["TrackID"] 
          = G4AttDef("TrackID","Track ID","Physics","","G4int");

        (*store)["ParentPartID"] 
          = G4AttDef("ParentPartID","Parent Particle ID","Physics","","G4int");

        (*store)["ParentTrackID"] 
          = G4AttDef("ParentTrackID","Parent Track ID","Physics","","G4int");

        (*store)["ProcessType"] 
          = G4AttDef("ProcessType","Process Type","Physics","","G4int");
        
        (*store)["Vertex"] 
          = G4AttDef("Vertex", "Vertex XYZ", "Physics","cm","G4ThreeVector");

        (*store)["WorldPos"] 
          = G4AttDef("WorldPos", "World Position", "Physics","cm","G4ThreeVector");

        (*store)["Momentum"] 
          = G4AttDef("Momentum", "Momentum", "Physics","GeV","G4ThreeVector");
    }
    return store;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

std::vector<G4AttValue>* PHG3toG4KinHit::CreateAttValues() const
{
    std::vector<G4AttValue>* values = new std::vector<G4AttValue>;
    
    values
      ->push_back(G4AttValue("HitType","KinHit",""));
    values
      ->push_back(G4AttValue("ParticleID",G4UIcommand::ConvertToString(fParticleID),""));
    values
      ->push_back(G4AttValue("TrackID",G4UIcommand::ConvertToString(fTrackID),""));
    values
      ->push_back(G4AttValue("ParentPartID",G4UIcommand::ConvertToString(fParentPartID),""));
    values
      ->push_back(G4AttValue("ParentTrackID",G4UIcommand::ConvertToString(fParentTrackID),""));
    values
      ->push_back(G4AttValue("ProcessType",G4UIcommand::ConvertToString(fProcessType),""));
    values
      ->push_back(G4AttValue("Vertex",G4UIcommand::ConvertToString(fVertexXYZ/cm),""));
    values
      ->push_back(G4AttValue("WorldPos",G4UIcommand::ConvertToString(fWorldPos/cm),""));
    values
      ->push_back(G4AttValue("Momentum",G4UIcommand::ConvertToString(fMomentum/GeV),""));
    
    return values;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void PHG3toG4KinHit::Print()
{

  G4cout << " Track ID: " << fTrackID << "  Particle ID: " << fParticleID << "  Vertex: " << fVertexXYZ << "  P: " << fMomentum << G4endl;

}



//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void* PHG3toG4KinHit::operator new(size_t)
{
    if (!PHG3toG4KinHitAllocator)
        PHG3toG4KinHitAllocator = new G4Allocator<PHG3toG4KinHit>;
    return (void*)PHG3toG4KinHitAllocator->MallocSingle();
}

void PHG3toG4KinHit::operator delete(void* aHit)
{
    PHG3toG4KinHitAllocator->FreeSingle((PHG3toG4KinHit*) aHit);
}
