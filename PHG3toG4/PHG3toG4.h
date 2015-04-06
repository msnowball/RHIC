/************************************************************/
/*     Class: PHG3toG4                                      */
/*                                                          */
/*     Desc: SubsysReco that controls the whole sequence    */
/*           Intakes PHHepMC DST node, converts to G4Event  */
/*           and processes it through G4                    */
/*                                                          */
/*   Author: Matt Snowball (snowball.at.rcf.rhic.bnl.gov)   */
/*                                                          */
/************************************************************/

#ifndef __PHG3TOG4_H__
#define __PHG3TOG4_H__

#include <fstream>
#include <string>
#include <SubsysReco.h>
#include <Rtypes.h>
#include "G4ios.hh"
#include "G4ThreeVector.hh"
#include "TVector3.h"

namespace HepMC
{
  class GenEvent;
};

//Root
class TGeoManager;
class TG4RootNavMgr;

//G4
class G4Event;
class G4RunManager;
class G4UImanager;
class G4PhysListFactory;
class G4VUserPhysicsList;
class G4VModularPhysicsList;
class G4Mag_UsualEqRhs;
class G4FieldManager;
class G4EqMagElectricField;
class G4MagIntegratorStepper;
class G4MagInt_Driver;
class G4ChordFinder;
class G4Timer;

//PHG3toG4
class PHG3toG4PrimaryGeneratorAction;
class PHG3toG4PostDetConstruction;
class PHG3toG4StackingAction;
class PHG3toG4EventAction;
class PHG3toG4TrackingAction;
class PHG3toG4SteppingAction;
class PHG3toG4RootManager;
class PHG3toG4MagneticField;
class PHG3toG4ParticleGun;

//PHHepMC
class PHHepMCCombiner;
class PHHepMCGenEvent;

class PHG3toG4: public SubsysReco
{
public:
  

  PHG3toG4(const std::string &name = "PHG3toG4", const std::string fieldFileName = "Sim3D++.root", const std::string geoFileName = "");
  virtual ~PHG3toG4();

  //! Methods Derived from SubsysReco
  int Init(PHCompositeNode *topNode);
  //int InitRun(PHCompositeNode *topNode);
  //! event method
  int process_event(PHCompositeNode *topNode);
  //! event reset
  int ResetEvent(PHCompositeNode *topNode);
  //! end of job
  int End(PHCompositeNode *topNode);

  //! PHG3toG4 methods
  void SetPhysicsList(std::string list);

  void CheckOverlaps(bool check = true)
  {_checkOverlaps = check;}

  void SetTrackEnergyCut(double energyCut)
  {_theTrackEnergyCut = energyCut;}

  void SetBField(bool b)
  {_useBField = b;}
  void SetBFieldMinStep(double m)
  { _theFieldMinStep = m;}//mm

  void SetMaxToF(std::string detector, double tof);
  void SetMaxStep(std::string detector, double step);
  void SetMinKinEnergy(std::string detector, double e);
  void SetMinEnergyDeposit(std::string detector, double e);

  //Particle Gun
  void UseParticleGun(std::string partName, double energy = 10.)
  { 
    _partGunPartName = partName; 
    _partGunEnergy = energy;
    _usePartGun = true;
  }
  void ParticleGunMomentumVec(TVector3 partGunVec)
  {
    _usePartGunVec = true;
    _partGunVector.setX(partGunVec.X());
    _partGunVector.setY(partGunVec.Y());
    _partGunVector.setZ(partGunVec.Z());
  }
  void UseParticleGunRandomEnergy(double minE, double maxE){ _usePartGunRandE = true; _partGunMinE = minE; _partGunMaxE = maxE;}
  void UseParticleGunRandomVtx(double minX, double maxX, double minY, double maxY, double minZ, double maxZ)
  {
    _usePartGunRandVtx = true;
    _partGunMinVtxX = minX; _partGunMaxVtxX = maxX;
    _partGunMinVtxY = minY; _partGunMaxVtxY = maxY;
    _partGunMinVtxZ = minZ; _partGunMaxVtxZ = maxZ;
  }
  void UseParticleGunVertex( TVector3 partGunVtx )
  {
    _usePartGunUserVertex = true;
    _partGunVertex.setX(partGunVtx.X());
    _partGunVertex.setX(partGunVtx.X());
    _partGunVertex.setX(partGunVtx.X());
  }
  void UseParticleGunForwardOnly(){ _usePartGunForward = true; }
  void UseParticleGunVertexFile(std::string vertexFileName){ _partGunVertexFile = vertexFileName; _usePartGunVertexFile = true;}
  void UseParticleGunVertexDST(std::string vertexName = "FVTX"){_usePartGunVertexDST = true; _partGunVertexName = vertexName;}

  void AddNode(std::string n)
  {
    std::cout << "PHG3toG4::AddNode - Added PHHepMCGenEvent node: " << n << std::endl;
    _nodeNames.push_back(n);
  }
  PHHepMCGenEvent* getPHHepMCGenEvent(PHCompositeNode *topNode, std::string node_name);

  void ReportEvery(int e)
  {_reportEvery = e;}
  
 protected:
  
  void SetBField();


 private:

  int _eventCounter;
  int _reportEvery;
  bool _checkOverlaps;
  bool _userLimits;
  bool _useBField;
  bool _usePartGun, _usePartGunVec, _usePartGunRandE, _usePartGunRandVtx, _usePartGunUserVertex, _usePartGunForward, _usePartGunVertexFile, _usePartGunVertexDST;
  double _theTrackEnergyCut; //kill threshold for tracks in MeV
  double _theMaxTof, _theMaxTof_SVX, _theMaxTof_MUI, _theMaxTof_MUT, _theMaxTof_BBC, _theMaxTof_MUPC;//max ToF for G4 LVs in nsec
  double _theMaxStep, _theMaxStep_SVX, _theMaxStep_MUI, _theMaxStep_MUT, _theMaxStep_BBC, _theMaxStep_MUPC;//max step lengths for G4 LVs in mm
  double _theMinKinEnergy, _theMinKinEnergy_SVX, _theMinKinEnergy_MUI, _theMinKinEnergy_MUT, _theMinKinEnergy_BBC, _theMinKinEnergy_MUPC;//min energy dep in MeV
  double _theMinEnergyDep_SVX, _theMinEnergyDep_MUI, _theMinEnergyDep_MUT, _theMinEnergyDep_BBC, _theMinEnergyDep_MUPC;//min energy dep for hit step in MeV
  double _theFieldMinStep; //min step for field calculation in mm
  double _partGunEnergy, _partGunMinE, _partGunMaxE;
  double _partGunMinVtxX, _partGunMaxVtxX, _partGunMinVtxY, _partGunMaxVtxY, _partGunMinVtxZ, _partGunMaxVtxZ;
  

  std::string _theFileName;
  std::string _thePhysList;
  std::string _magFieldFileName;
  std::string _partGunPartName;
  std::string _partGunVertexFile;
  std::string _partGunVertexName;

  std::vector<std::string> _nodeNames;

  //Root
  TGeoManager* geoManager;
  TG4RootNavMgr* g4rootNav;

  //HepMC
  PHHepMCCombiner *_hepmcCombiner;
  HepMC::GenEvent *_theHepMCEvt;

  //G4
  G4RunManager* runManager;
  G4PhysListFactory* physListFactory;
  //G4VUserPhysicsList* physicsList;
  G4VModularPhysicsList* physicsList;
  G4UImanager* UImanager;
  G4FieldManager *theFieldManager;
  G4MagIntegratorStepper *theFieldStepper;
  G4MagInt_Driver *theIntgrDriver;
  G4ChordFinder *theChordFinder;
  G4Mag_UsualEqRhs* theFieldEquation;
  G4Timer *timer;
  double _totalTime, _longestEventTime, _shortestEventTime;
  G4int _longestEvent, _shortestEvent;
  G4ThreeVector _partGunVector, _partGunVertex;
  

  //PHG3toG4
  PHG3toG4PrimaryGeneratorAction *thePrimGA;
  PHG3toG4ParticleGun *thePrimPG;
  PHG3toG4PostDetConstruction *theDet;
  PHG3toG4StackingAction *theStackA;
  PHG3toG4EventAction *theEA;
  PHG3toG4TrackingAction *theTA;
  PHG3toG4SteppingAction *theSA;
  PHG3toG4RootManager *theRootMgr;
  PHG3toG4MagneticField *theBField;

};

#endif	

