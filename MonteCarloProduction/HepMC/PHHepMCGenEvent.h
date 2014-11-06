#ifndef __PHHEPMCGENEVENT__
#define __PHHEPMCGENEVENT__

#include "phool.h"
#include "PHObject.h"

#include "HepMC/GenEvent.h"
#include "HepMC/GenVertex.h"
#include "HepMC/GenParticle.h"


namespace HepMC
{
  class GenEvent;
};


class PHHepMCGenEvent : public PHObject
{
 public:
  
  PHHepMCGenEvent(int theEnergy = -99,int theDistance = -99);
  virtual ~PHHepMCGenEvent();
  
  virtual HepMC::GenEvent *getEvent();

  bool addEvent(HepMC::GenEvent *evt);
  bool addEvent(HepMC::GenEvent &evt);

  virtual void moveVertex(const float x, const float y, const float z, const float t);

  // the number of entries in the array of particles
  virtual int size(void) const ;
  virtual int vertexSize(void) const ;

  virtual int isValid() const
  { PHOOL_VIRTUAL_WARNING; return 0; }

  virtual void Reset();

  virtual void identify(std::ostream& os=std::cout) const;

  virtual void print(std::ostream& os=std::cout) const;

protected:
  
  HepMC::GenEvent *_theEvt;
  bool _isVtxShiftApplied;
  int _theEnergyUnits;
  int _theDistanceUnits;

private:

  ClassDef(PHHepMCGenEvent,1)
    
};

#endif	// __PHHEPMCEVENT__
