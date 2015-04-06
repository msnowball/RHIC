/************************************************************/
/*     Class: PHG3toG4TrackingAction                        */
/*                                                          */
/*     Desc: Runs at the creation and death of tracks       */
/*           Currently unused (12.03.2014)                  */
/*                                                          */
/*   Author: Matt Snowball (snowball.at.rcf.rhic.bnl.gov)   */
/*                                                          */
/************************************************************/

#ifndef PHG3toG4TrackingAction_h
#define PHG3toG4TrackingAction_h 1

#include "G4UserTrackingAction.hh"
#include "G4Track.hh"

class G4String;

class PHG3toG4TrackingAction : public G4UserTrackingAction
{

 public:
  PHG3toG4TrackingAction();
  virtual ~PHG3toG4TrackingAction();
   
  virtual void PreUserTrackingAction(const G4Track*);
  virtual void PostUserTrackingAction(const G4Track*);

  void Initialize();

 private:

};

#endif
