#include "PHPy8ParticleScheme.h"
#include "Pythia8/Pythia.h"
#include <iterator>

using namespace std;

//__________________________________________________________
PHPy8ParticleScheme::PHPy8ParticleScheme(const std::string &name):
_name(name)
{}

//__________________________________________________________
PHPy8ParticleScheme::~PHPy8ParticleScheme()
{}


int PHPy8ParticleScheme::GetPdgId(std::string particle);


void PHPy8ParticleScheme::loadParticles()
{

  for(



}
