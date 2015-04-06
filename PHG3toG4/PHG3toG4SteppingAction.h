/************************************************************/
/*     Class: PHG3toG4SteppingAction                        */
/*                                                          */
/*     Desc: Runs before and after every step               */
/*           Kills tracks with e < theTrackEnergyCut        */
/*                                                          */
/*   Author: Matt Snowball (snowball.at.rcf.rhic.bnl.gov)   */
/*                                                          */
/************************************************************/

#ifndef PHG3toG4SteppingAction_h
#define PHG3toG4SteppingAction_h 1

#include "G4UserSteppingAction.hh"
#include "G4Track.hh"
#include <vector>

class DetectorConstruction;
class EventAction;


class PHG3toG4SteppingAction : public G4UserSteppingAction
{
 public:
  PHG3toG4SteppingAction(const G4double trackEnergyCut = 1.0);
  virtual ~PHG3toG4SteppingAction();

  void Reset();
  void UserSteppingAction(const G4Step*);
  void Verbosity(G4int v)
  {
    verbosity = v;
  }

 private:
  G4double theTrackEnergyCut;//in MeV
  DetectorConstruction* detector;
  EventAction*          eventaction;
  G4Track* theTrack;

  std::vector<int> _zeroStepLengthTracks;
  std::vector<int> _zeroStepLengthTracksCount;
  std::vector<int> _zeroStepLengthTracksMoveCount;

  G4int verbosity;

};


#endif
