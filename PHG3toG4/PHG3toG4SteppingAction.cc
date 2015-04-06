#include "PHG3toG4SteppingAction.h"

#include "G4Step.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4ios.hh"
#include "Randomize.hh"

using namespace std;

PHG3toG4SteppingAction::PHG3toG4SteppingAction(const G4double trackEnergyCut /*MeV*/)
{
  theTrackEnergyCut = trackEnergyCut;
  G4cout << "PHG3toG4SteppingAction::PHG3toG4SteppingAction - Min Track Energy Cut Set To: " << theTrackEnergyCut <<"MeV" << G4endl;

}

PHG3toG4SteppingAction::~PHG3toG4SteppingAction()
{}

void PHG3toG4SteppingAction::Reset()
{ 
  if(_zeroStepLengthTracks.size() > 0) _zeroStepLengthTracks.clear();
  if(_zeroStepLengthTracksCount.size() > 0) _zeroStepLengthTracksCount.clear();
  if(_zeroStepLengthTracksMoveCount.size() > 0) _zeroStepLengthTracksMoveCount.clear();
}


void PHG3toG4SteppingAction::UserSteppingAction(const G4Step* aStep)
{

  if(verbosity > 9)
    {
      G4cout << "SteppingAction - Track: " << aStep->GetTrack()->GetTrackID() 
	     << "  PDG: " << aStep->GetTrack()->GetDefinition()->GetPDGEncoding()
	     << "  Status: " << aStep->GetTrack()->GetTrackStatus() 
	     << "  Energy: " << aStep->GetTrack()->GetTotalEnergy()/MeV << "MeV" 
	     << "  Vol:  " << aStep->GetTrack()->GetVolume()->GetName()
	     << "  Pos: " << aStep->GetPreStepPoint()->GetPosition() 
	     << G4endl;
    }

  theTrack = aStep->GetTrack();
  if (theTrack->GetTotalEnergy()/MeV < theTrackEnergyCut/*MeV*/)
    {
      if(verbosity > 9) G4cout << theTrack->GetTrackID() << " killed with " << theTrack->GetTotalEnergy()/MeV << G4endl;
      theTrack->SetTrackStatus(fStopAndKill);
      return;
    }

  //Kill tracks with more than 1 zero step length
  if (aStep->GetTrack()->GetStepLength() == 0.)
    {
      bool trackFound = false;
      int kFound = -1;
      for(unsigned int i = 0; i < _zeroStepLengthTracks.size(); i++)
	{
	  if (aStep->GetTrack()->GetTrackID() == _zeroStepLengthTracks[i] &&  _zeroStepLengthTracksCount[i] > 10 && _zeroStepLengthTracksMoveCount[i] < 10)
	    {
	      G4ThreeVector thePos = theTrack->GetPosition();
	      G4ThreeVector theNewPos;
	      G4double randSign = 1;
	      if (G4UniformRand() > 0.5) randSign = -1;//Random +/- for X movement
	      G4double moveX = randSign*G4UniformRand()*0.001*mm;//G4UniformRand = rand(0,1)
	      G4double moveY = 0;//only move Y if 5 tries of X and Z dont work
	      G4double moveZ = G4UniformRand()*0.01*mm;
	      if (thePos.z() < 0) moveZ *= -1;
	      if (_zeroStepLengthTracksMoveCount[i] > 5)
		{
		  moveY = G4UniformRand()*0.01*mm;//G4UniformRand = rand(0,1)                                                                                                                                                                                                  
		}
	      theNewPos.set(thePos.x()+moveX,thePos.y()+moveY,thePos.z()+moveZ);

	      _zeroStepLengthTracksMoveCount[i] = _zeroStepLengthTracksMoveCount[i] + 1;
	      G4ExceptionDescription msg;
	      msg << "WARNING: Moved a track that had > 10 zero length steps!" << G4endl
		  << "         TrackID:  " << aStep->GetTrack()->GetTrackID() << G4endl
		  << "         Energy:   " << aStep->GetTrack()->GetTotalEnergy()/MeV << "MeV" << G4endl
		  << "         Position: " << aStep->GetPreStepPoint()->GetPosition() << " ---> " << theNewPos << G4endl
		  << "         Volume:   " << aStep->GetTrack()->GetVolume()->GetName() << "_" << aStep->GetTrack()->GetVolume()->GetCopyNo() << G4endl
		  << "       Move Count: " << _zeroStepLengthTracksMoveCount[i] << G4endl;
	      G4Exception("PHG3toG4SteppingAction::UserSteppingAction", "MyCode0004", JustWarning, msg);
	      theTrack->SetPosition(theNewPos);
	      theTrack->SetTrackStatus(fAlive);
	      _zeroStepLengthTracksCount[i] = _zeroStepLengthTracksCount[i] - 2;//So that we dont move it again later if not necessary
	      return;
	    }
	  else if (aStep->GetTrack()->GetTrackID() == _zeroStepLengthTracks[i] &&  _zeroStepLengthTracksCount[i] > 50)
	    {
	      G4ExceptionDescription msg;
	      msg << "WARNING: Killed a track that had > 50 zero length steps!" << G4endl
		  << "         TrackID:  " << aStep->GetTrack()->GetTrackID() << G4endl
		  << "         Energy:   " << aStep->GetTrack()->GetTotalEnergy()/MeV << "MeV" << G4endl
		  << "         Position: " << aStep->GetPreStepPoint()->GetPosition() << G4endl
		  << "         Volume:   " << aStep->GetTrack()->GetVolume()->GetName() << "_" << aStep->GetTrack()->GetVolume()->GetCopyNo() << G4endl
		  << "       Move Count: " << _zeroStepLengthTracksMoveCount[i] << G4endl;
	      G4Exception("PHG3toG4SteppingAction::UserSteppingAction", "MyCode0005", JustWarning, msg);
	      theTrack->SetTrackStatus(fStopAndKill);
	      return;
	    }
	  else if(aStep->GetTrack()->GetTrackID() == _zeroStepLengthTracks[i])
	    {
	      trackFound = true;
	      kFound = i;
	      break;
	    }
	}
      
      if(trackFound)_zeroStepLengthTracksCount[kFound] = _zeroStepLengthTracksCount[kFound] + 1;
      else{ _zeroStepLengthTracks.push_back(aStep->GetTrack()->GetTrackID()); _zeroStepLengthTracksCount.push_back(1); _zeroStepLengthTracksMoveCount.push_back(0);}
    }

  
}
