#include "PHG3toG4UserTrackInfo.h"
#include "G4ios.hh"

G4Allocator<PHG3toG4UserTrackInfo> aTrackInformationAllocator;

PHG3toG4UserTrackInfo::PHG3toG4UserTrackInfo()
{
  originalTrackID = 0;
  particleDefinition = 0;
  originalParticleID = 0;
  originalPosition = G4ThreeVector(0.,0.,0.);
  originalMomentum = G4ThreeVector(0.,0.,0.);
  originalEnergy = 0.;
  originalTime = 0.;
}

PHG3toG4UserTrackInfo::PHG3toG4UserTrackInfo(const G4Track* aTrack)
{
  originalTrackID = aTrack->GetTrackID();
  particleDefinition = aTrack->GetDefinition();
  originalParticleID = aTrack->GetDefinition()->GetPDGEncoding();
  originalPosition = aTrack->GetPosition();
  originalMomentum = aTrack->GetMomentum();
  originalEnergy = aTrack->GetTotalEnergy();
  originalTime = aTrack->GetGlobalTime();
}

PHG3toG4UserTrackInfo::PHG3toG4UserTrackInfo(const PHG3toG4UserTrackInfo* aTrackInfo)
{
  originalTrackID = aTrackInfo->originalTrackID;
  particleDefinition = aTrackInfo->particleDefinition;
  originalParticleID = aTrackInfo->originalParticleID;
  originalPosition = aTrackInfo->originalPosition;
  originalMomentum = aTrackInfo->originalMomentum;
  originalEnergy = aTrackInfo->originalEnergy;
  originalTime = aTrackInfo->originalTime;
}

PHG3toG4UserTrackInfo::~PHG3toG4UserTrackInfo()
{}

void PHG3toG4UserTrackInfo::Print() const
{
    G4cout << "Original track ID " << originalTrackID 
	   << " at " << originalPosition << G4endl;
}

void* PHG3toG4UserTrackInfo::operator new(size_t)
{ void* aTrackInfo;
  aTrackInfo = (void*)aTrackInformationAllocator.MallocSingle();
  return aTrackInfo;
}

void PHG3toG4UserTrackInfo::operator delete(void *aTrackInfo)
{ aTrackInformationAllocator.FreeSingle((PHG3toG4UserTrackInfo*)aTrackInfo);}

