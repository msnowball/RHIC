#include <PHHepMCGenEvent.h>
#include "HepMC/GenEvent.h"

#include <TBuffer.h>
#include <TClass.h>

#include <RVersion.h> // root version 

#include <cassert>
#include <cstdlib>
#include <iomanip>
#include <sstream>
#include <vector>

ClassImp(PHHepMCGenEvent)

using namespace std;

PHHepMCGenEvent::PHHepMCGenEvent(int theEnergy,int theDistance)
{
  _isVtxShiftApplied = false;
  _theEnergyUnits = theEnergy;
  _theDistanceUnits = theDistance;
  if(_theEnergyUnits > -99 && _theDistanceUnits > -99)
    {
      _theEvt = new HepMC::GenEvent(_theEnergyUnits, _theDistanceUnits);
    }
  else {
    _theEvt = new HepMC::GenEvent(HepMC::Units::GEV, HepMC::Units::MM);
  }

}

PHHepMCGenEvent::~PHHepMCGenEvent()
{
  if(_theEvt) delete _theEvt;
}


HepMC::GenEvent* PHHepMCGenEvent::getEvent()
{
  return _theEvt;
}

bool PHHepMCGenEvent::addEvent(HepMC::GenEvent *evt)
{

  _theEvt = evt;
  if(!_theEvt) return false;
  return true;
}

bool PHHepMCGenEvent::addEvent(HepMC::GenEvent &evt)
{
  HepMC::GenEvent tmp(evt);
  _theEvt->swap(tmp);
  if(!_theEvt) return false;
  return true;
}


void PHHepMCGenEvent::moveVertex(const float x, const float y, const float z, const float t = 0)
{

  static const float CM2MM = 10.;

  if(!_isVtxShiftApplied)
    {
      for ( HepMC::GenEvent::vertex_iterator vt = _theEvt->vertices_begin();
	    vt != _theEvt->vertices_end(); ++vt )
	{
	  double xShift = (*vt)->position().x() + x*CM2MM;
	  double yShift = (*vt)->position().y() + y*CM2MM;
	  double zShift = (*vt)->position().z() + z*CM2MM;
	  double tShift = (*vt)->position().t() + t;
	  //std::cout << " vertex (x,y,z)= " << x <<" " << y << " " << z << std::endl;
	  (*vt)->set_position( HepMC::FourVector(xShift,yShift,zShift,tShift) ) ;      
	}
      
      _isVtxShiftApplied = true;
    }
  else{ cout << "PHHepMCGenEvent::moveVertex - vertex has already been shifted for this event!" << endl;}

}

int PHHepMCGenEvent::size(void) const
{ 
  return _theEvt->particles_size();
}

int PHHepMCGenEvent::vertexSize(void) const
{ 
  return _theEvt->vertices_size();
}

void PHHepMCGenEvent::Reset()
{
  _isVtxShiftApplied = false;
  //delete _theEvt;
}

//_____________________________________________________________________________
void PHHepMCGenEvent::identify(std::ostream& os ) const
{
  os << "identify yourself: PHHepMCGenEvent Object" << endl;
  os << "No of Particles: " << size() << endl;
  return;
}

void PHHepMCGenEvent::print(std::ostream& out) const
{
  identify(out);
}
