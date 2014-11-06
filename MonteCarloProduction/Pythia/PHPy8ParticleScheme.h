#ifndef __PHPY8PARTICLESCHEME_H__
#define __PHPY8PARTICLESCHEME_H__

#include <iostream>
#include <string>
#include <vector>
#include <utility>

class PHPy8ParticleScheme{

 public:
  //! constructor
  PHPy8ParticleScheme(const std::string &name = "PHPy8ParticleScheme");
  virtual ~PHPy8ParticleScheme();

  virtual std::string GetName()
  { return _name;}
  
  int _verbosity;
  void Verbosity(int v)
  { _verbosity = v; }

  int GetPdgId(std::string particle);


 private:

  std::string _name;

  void loadParticles();

  std::vector< std::pair<std::string,int> > _particleScheme;
  


};

#endif	

