#ifndef PHG3toG4ParticleGun_h
#define PHG3toG4ParticleGun_h 1

#include <fstream>

#include "G4VUserPrimaryGeneratorAction.hh"
#include "globals.hh"
#include "G4ThreeVector.hh"
#include "TRandom.h"

class G4VPrimaryGenerator;
class G4Event;



class PHG3toG4ParticleGun: public G4VUserPrimaryGeneratorAction
{
 public:
  PHG3toG4ParticleGun();
  virtual ~PHG3toG4ParticleGun();
  
  virtual void GeneratePrimaries(G4Event* anEvent);
  void UseVertexFile(std::string fileName){ useVertexFile = true; vertexFileName = fileName;}
  void SetParticle(G4String thePartName,  G4double thePartEnergy=-1);
  void SetUserVertex(G4ThreeVector theVertex){ userVtx = theVertex; doUserVtx = true;}
  void SetRandEnergy(G4double minE, G4double maxE){doRandEnergy = true; minRandEnergy = minE; maxRandEnergy = maxE;}
  void SetRandVertex(G4double minX, G4double maxX, G4double minY, G4double maxY, G4double minZ, G4double maxZ)
  {
    minRandVtx_X = minX;
    minRandVtx_Y = minY;
    minRandVtx_Z = minZ;
    maxRandVtx_X = maxX;
    maxRandVtx_Y = maxY;
    maxRandVtx_Z = maxZ;
    doRandVtx = true;
  }
  void SetDirectionForward(){ doForwardMomDir = true;}
  void SetUserMomDir(G4ThreeVector momDir){userMomDirVec = momDir; userMomDir = true;}
  void PrintConfig();
  
  
  
 private:
  
  G4ThreeVector GetVertexFromFile();

  G4String particleName;
  G4double particleEnergy;
  G4ThreeVector particleMomDir;

  G4VPrimaryGenerator* fParticleGun;
  G4int fEventCounter;
  G4bool doRandEnergy;
  G4double minRandEnergy, maxRandEnergy;
  G4bool doRandVtx;
  G4double minRandVtx_X, minRandVtx_Y, minRandVtx_Z;
  G4double maxRandVtx_X, maxRandVtx_Y, maxRandVtx_Z;
  G4ThreeVector userVtx;
  G4bool doUserVtx;
  G4bool doForwardMomDir;
  G4bool userMomDir;
  G4ThreeVector userMomDirVec;
  G4bool useVertexFile;
  std::string vertexFileName;
  ifstream theVtxFile;

  TRandom *rand;
  
  
};

#endif


