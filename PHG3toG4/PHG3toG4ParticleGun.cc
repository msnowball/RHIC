#include <stdlib.h>
#include <time.h>
#include <fstream>
#include <sstream>
#include <string>

#include "PHG3toG4ParticleGun.h"

#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"    
#include "G4Geantino.hh"
#include "G4PhysicalConstants.hh"
#include "Randomize.hh"


PHG3toG4ParticleGun::PHG3toG4ParticleGun():
  G4VUserPrimaryGeneratorAction(),
  fParticleGun(0),
  fEventCounter(0),
  doRandEnergy(false),
  minRandEnergy(-99),
  maxRandEnergy(-99),
  doRandVtx(false),
  minRandVtx_X(-99), minRandVtx_Y(-99), minRandVtx_Z(-99),
  maxRandVtx_X(-99), maxRandVtx_Y(-99), maxRandVtx_Z(-99),
  userVtx(0),
  doUserVtx(false),
  doForwardMomDir(false),
  userMomDir(false),
  useVertexFile(false)
{

  rand = new TRandom(time(NULL));
  vertexFileName = "";
}

PHG3toG4ParticleGun::~PHG3toG4ParticleGun()
{
  if(fParticleGun) delete fParticleGun;
}

void PHG3toG4ParticleGun::SetParticle(G4String thePartName, G4double thePartEnergy)
{

  particleName = thePartName;

 
  if(fParticleGun) delete fParticleGun;
  G4int n_particle = 1;
  G4ParticleGun* particleGun = new G4ParticleGun(n_particle);
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle(particleName);
  particleGun->SetParticleDefinition(particle);
  particleEnergy = thePartEnergy;

  //Momentum Direction
  if(userMomDir)
    {
      particleMomDir = userMomDirVec;
    }
  else if(doForwardMomDir)
    {
      G4double eta = 0;
      while(abs(eta) < 1.0)
	{
	  //Make forward only
	  G4double cosTheta = 2*G4UniformRand() - 1.;
	  G4double phi = twopi*G4UniformRand();
	  G4double sinTheta = sqrt(1. - cosTheta*cosTheta);
	  G4double ux = sinTheta*cos(phi);
	  G4double uy = sinTheta*sin(phi);
	  G4double uz = cosTheta;
	  particleMomDir = G4ThreeVector(ux,uy,uz);
	  eta = particleMomDir.eta();
	  //G4cout << "PHG3toG4ParticleGun::SetParticle - eta = " << eta << G4endl;
	}
    }
  else 
    {
      //Random over full solid angle
      G4double cosTheta = 2*G4UniformRand() - 1.;
      G4double phi = twopi*G4UniformRand();
      G4double sinTheta = sqrt(1. - cosTheta*cosTheta);
      G4double ux = sinTheta*cos(phi);
      G4double uy = sinTheta*sin(phi);
      G4double uz = cosTheta;
      G4ThreeVector threeV(ux,uy,uz);
      particleMomDir = threeV;
    }
  particleGun->SetParticleMomentumDirection(particleMomDir);

  //Energy
  if(doRandEnergy)
    {
      particleEnergy = rand->Uniform(minRandEnergy,maxRandEnergy);
    }
  else if (particleEnergy <= 0)
    {
      G4ExceptionDescription msg;
      msg << "Error - Particle Gun has no energy input!" << G4endl;
      G4Exception("PHG3toG4ParticleGun::SetParticle()", "MyCode0006", FatalException, msg);
    }      
  particleGun->SetParticleEnergy(particleEnergy*GeV);

  //Vertex
  G4double theVtxX = 0;
  G4double theVtxY = 0;
  G4double theVtxZ = 0;
  if(useVertexFile)
    {
      G4ThreeVector vec = GetVertexFromFile();
      theVtxX = vec.x();
      theVtxY = vec.y();
      theVtxZ = vec.z();
    }
  else if(doRandVtx)
    {
      theVtxX = rand->Uniform(minRandVtx_X,maxRandVtx_X);
      theVtxY = rand->Uniform(minRandVtx_Y,maxRandVtx_Y);
      theVtxZ = rand->Uniform(minRandVtx_Z,maxRandVtx_Z);
    }
  else if(doUserVtx)
    {
      theVtxX = userVtx.x();
      theVtxY = userVtx.y();
      theVtxZ = userVtx.z();
    }
  particleGun->SetParticlePosition(G4ThreeVector(theVtxX*cm,theVtxY*cm,theVtxZ*cm));

  if(fEventCounter < 1) PrintConfig();
  fParticleGun = particleGun;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo...... 
void PHG3toG4ParticleGun::GeneratePrimaries(G4Event* anEvent)
{
  fEventCounter++;
  fParticleGun->GeneratePrimaryVertex(anEvent); 
}


void PHG3toG4ParticleGun::PrintConfig()
{
  G4cout << " " << G4endl;
  G4cout << ">>>> PHG3toG4ParticleGun::PrintConfig()" << G4endl
	 << "Particle Type: " << particleName << G4endl;
  if(doRandEnergy) G4cout << "Energy: Random (" << minRandEnergy << "-" << maxRandEnergy << ") GeV" << G4endl;
  else G4cout << "Energy: " << particleEnergy << " GeV" << G4endl;

  if(useVertexFile) G4cout << "Vertex: from file " << vertexFileName << G4endl;
  else if(doUserVtx) G4cout << "Vertex: " << userVtx << G4endl;
  else if(doRandVtx) G4cout << "Vertex: Random (" << minRandVtx_X << "-" << maxRandVtx_X 
			    << "," << minRandVtx_Y << "-" << maxRandVtx_Y 
			    << "," << minRandVtx_Z << "-" << maxRandVtx_Z 
			    << ")" << G4endl;
  else G4cout << "Vertex: (0,0,0)" << G4endl;
  
  if(userMomDir) G4cout << "Momentum Dir: " << userMomDirVec << G4endl;
  else if(doForwardMomDir) G4cout << "Momentum Dir: Random in forward directions" << G4endl;
  else G4cout << "Momentum Dir: Random over full solid angle" << G4endl;
  
  G4cout << "Random Seed: " << rand->GetSeed() << G4endl; 

  G4cout << " " << G4endl;
  
}

G4ThreeVector PHG3toG4ParticleGun::GetVertexFromFile()
{

  if(fEventCounter < 1)
    {
      theVtxFile.open(vertexFileName.c_str());
      if(!theVtxFile.good()) 
	{
	  G4ExceptionDescription msg;
	  msg << "Could not find vertex file: " << vertexFileName << G4endl;
	  G4Exception("PHG3toG4ParticleGun::GetVertexFromFile", "MyCode0006", FatalException, msg);
	}
    }


  double x = 0;
  double y = 0;
  double z = 0;

  if(theVtxFile.eof()) 
    {
      if(theVtxFile.is_open()) theVtxFile.close();
      G4ExceptionDescription msg;
      msg << "End of vertex file, using (0,0,0)! " << G4endl;
      G4Exception("PHG3toG4ParticleGun::GetVertexFromFile", "MyCode0007", JustWarning, msg);
      return G4ThreeVector(x,y,z);
    }
  else{
    std::string theLine;
    while(getline(theVtxFile, theLine))
      {
	if(theLine.find("#") == 0) continue;
	std::vector<double> theInfo;
	double number = 0;
	for(std::istringstream numbers_iss(theLine); numbers_iss >> number; )
	  {
	    theInfo.push_back(number);
	  }
	if(theInfo.size() < 4)
	  {
	    G4ExceptionDescription msg;
	    msg << "Wrong vertex file format, using (0,0,0)! " << G4endl;
	    G4Exception("PHG3toG4ParticleGun::GetVertexFromFile", "MyCode0008", JustWarning, msg);
	    return G4ThreeVector(x,y,z);
	  }
	else 
	  {
	    x = theInfo[1];
	    y = theInfo[2];
	    z = theInfo[3];
	    return G4ThreeVector(x,y,z);
	    break;
	  }
      }//while(getline) 
  }
  
  return G4ThreeVector(x,y,z);

}
