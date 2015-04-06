#include "PHG3toG4TrackingAction.h"
#include "PHG3toG4UserTrackInfo.h"

#include "G4VProcess.hh"
#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4TrackingManager.hh"
#include "G4Track.hh"
#include "G4TrackVector.hh"
#include "G4ios.hh"

PHG3toG4TrackingAction::PHG3toG4TrackingAction():
  G4UserTrackingAction()
{}

PHG3toG4TrackingAction::~PHG3toG4TrackingAction()
{}


// This happens when a track is created
void PHG3toG4TrackingAction::PreUserTrackingAction(const G4Track* theTrack)
{
  if(theTrack->GetParentID()==0 && theTrack->GetUserInformation()==0)
    {
      PHG3toG4UserTrackInfo* anInfo = new PHG3toG4UserTrackInfo(theTrack);
      G4Track* aTrack = (G4Track*)theTrack;
      aTrack->SetUserInformation(anInfo);
    }

}

// This happens right before a track is destroyed
void PHG3toG4TrackingAction::PostUserTrackingAction(const G4Track* theTrack)
{
  G4TrackVector* secondaries = fpTrackingManager->GimmeSecondaries();
  if(secondaries)
    {
      PHG3toG4UserTrackInfo* info = (PHG3toG4UserTrackInfo*)(theTrack->GetUserInformation());
      size_t nSeco = secondaries->size();
      if(nSeco>0)
	{
	  for(size_t i=0;i<nSeco;i++)
	    { 
	      PHG3toG4UserTrackInfo* infoNew = new PHG3toG4UserTrackInfo(info);
	      (*secondaries)[i]->SetUserInformation(infoNew);
	    }
	}
    }

}
