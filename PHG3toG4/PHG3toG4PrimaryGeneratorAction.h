#ifndef PHG3toG4PrimaryGeneratorAction_H__
#define PHG3toG4PrimaryGeneratorAction_H__


#include <G4VUserPrimaryGeneratorAction.hh>
//#include "TG4PrimaryGeneratorAction.h"
//#include <globals.hh>

class TG4Verbose;

namespace HepMC
{
  class GenEvent;
};

class G4Event;

namespace CLHEP{
  class Hep3Vector;
};

typedef class CLHEP::Hep3Vector G4ThreeVector;


class PHG3toG4PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
  
 public:
  
 PHG3toG4PrimaryGeneratorAction():
  verbosity(0),
    hepmcevt(0)
    {}
  
  virtual ~PHG3toG4PrimaryGeneratorAction()
    {}
  
  virtual void GeneratePrimaries(G4Event* anEvent);
  void SetHepMCEvent(HepMC::GenEvent *theEvt);

  //! Set/Get verbosity
  void Verbosity(const int val) { verbosity=val; }
  int Verbosity() const { return verbosity; }

 protected:
  
  int verbosity;

 private:

  HepMC::GenEvent *hepmcevt;
  virtual bool CheckVertexInsideWorld(const G4ThreeVector& pos) const;

  
};
#endif // PHG4PrimaryGeneratorAction_H__
