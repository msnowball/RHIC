#ifndef __PHPYTHIA8_H__
#define __PHPYTHIA8_H__

#include <fstream>
#include <string>
#include <SubsysReco.h>
#include <Rtypes.h>
#include "Pythia8/Pythia.h"
//#include "PHHepMCEvent.h"

class PHCompositeNode;
class PHPythiaHeader;
class PHPythiaContainer;
class PHHepMCGenEvent;
class TTree;
class TFile;

class PHPy8GenTrigger;

namespace HepMC
{
  class GenEvent;
  class Pythia8ToHepMC;
};

namespace Pythia8
{
  class Pythia;
};

class PHPythia8: public SubsysReco
{
public:
  
  enum FORMAT{ PHPYTHIA, HEPMC };

  //! constructor
  PHPythia8(const std::string &name = "PHPythia8", FORMAT outputFormat = PHPYTHIA);
  
  //! destructor
  virtual ~PHPythia8();

  //! configuration file
  void SetConfigFile( const char* cfg_file )
  { if( cfg_file ) _configFile = cfg_file; }
  
  //! Read Config File
  /*! if argument is 0 current _configFile is used. It is overwritten otherwise */
  int ReadConfig(const char *cfg_file = 0);

  //! Print Config File
  void PrintConfig() const;

  //! Set Seed of random number generator
  void SetSeed(const int s) { fSeed = s; }

  //! Methods Derived from SubsysReco
  int Init(PHCompositeNode *topNode);
  
  //int InitRun(PHCompositeNode *topNode);
  
  //! event method
  int process_event(PHCompositeNode *topNode);
  
  //! event reset
  int ResetEvent(PHCompositeNode *topNode);
  
  //! end of job
  int End(PHCompositeNode *topNode);

  void registerTrigger(PHPy8GenTrigger *theTrigger);
  void setTriggerOR()
  { _triggersOR = true; } // default true
  void setTriggerAND()
  { _triggersAND = true; }

protected:
  
  //! node tree
  int CreateNodeTree(PHCompositeNode *topNode);
  
  //! event
  int eventcount;
  
  //! configuration file. Default is "phpythia.cfg
  std::string _configFile;
  
  //! pythia interface
  Pythia8::Pythia *pythia;
  
  //! pythia header output node
  PHPythiaHeader *phpythiaheader;
  
  //! pythia container output node
  PHPythiaContainer *phpythia;

  //! seed to random number generator
  long int fSeed;		
  
  //! Tree for normalization
  TFile *xsecfile;
  TTree *Tp;
  UInt_t   tp_isub;
  Char_t  *tp_proc;
  UInt_t   tp_nevt;
  Double_t tp_sigma;
  Double_t tp_sigma_err;
  Double_t tp_nevt_sigma;
  Double_t tp_integlumi;


 private:
  //HepMC
  HepMC::GenEvent *hepmcevt;
  HepMC::Pythia8ToHepMC *pythiaToHepMC;
  bool _isHepMC;
  PHHepMCGenEvent *phhepmcevt;

  void fillPythiaNode(Pythia8::Pythia *pythia);
  
  std::vector<PHPy8GenTrigger*> _registeredTriggers;

  bool _triggersOR;
  bool _triggersAND;

};

#endif	/* __PHPYTHIA8_H__ */

