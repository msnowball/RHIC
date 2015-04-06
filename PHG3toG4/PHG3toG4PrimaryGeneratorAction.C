#include <cstdlib>
#include <iostream>
#include <map>
#include <math.h>

#include "PHG3toG4PrimaryGeneratorAction.h"

#include "TG4PrimaryGeneratorAction.h"
#include "TG4ParticlesManager.h"
#include "TG4TrackManager.h"
#include "TG4StateManager.h"
#include "TG4UserIon.h"
#include "TG4G3Units.h"
#include "TG4Globals.h"

#include <G4Event.hh>
#include <G4ParticleTable.hh>
#include <G4PrimaryParticle.hh>
#include <G4PrimaryVertex.hh>
#include <G4SystemOfUnits.hh>
#include <G4RunManager.hh>
#include <G4LorentzVector.hh>
#include <G4TransportationManager.hh>
#include <G4PhysicalConstants.hh>
#include <G4SystemOfUnits.hh>
#include <G4Types.hh>
#include <G4ios.hh>
//#include <G4ThreeVector.hh>

#include <TVirtualMC.h>
#include <TVirtualMCApplication.h>

#include "HepMC/GenEvent.h"


using namespace std;

void PHG3toG4PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  //TG4StateManager::Instance()->SetNewState(kInEvent);
  //TVirtualMCApplication::Instance()->BeginEvent();
  
  // Generate primaries and fill the VMC stack
  //TVirtualMCApplication::Instance()->GeneratePrimaries();
  
  if (verbosity > 0) cout << "PHG3toG4PrimaryGeneratorAction::GeneratePrimaries  " << hepmcevt->particles_size() << endl;

  for(HepMC::GenEvent::vertex_const_iterator vitr= hepmcevt->vertices_begin(); vitr != hepmcevt->vertices_end(); ++vitr ) 
    { 

      //real vertex?
      G4bool qvtx = false;
      for (HepMC::GenVertex::particle_iterator pitr= (*vitr)->particles_begin(HepMC::children);
	   pitr != (*vitr)->particles_end(HepMC::children); ++pitr) 
	{
	  if (!(*pitr)->end_vertex() && (*pitr)->status()==1) 
	    {
	      qvtx=true;
	      break;
	    }
	}
      if (!qvtx) continue;

      // check world boundary
      HepMC::FourVector pos= (*vitr)-> position();
      G4LorentzVector xvtx(pos.x(), pos.y(), pos.z(), pos.t());
      if (! CheckVertexInsideWorld(xvtx.vect())) continue;
      
      // create G4PrimaryVertex and associated G4PrimaryParticles
      if( std::isnan(xvtx.x()) || std::isnan(xvtx.y()) || std::isnan(xvtx.z()) )
	{
	  G4ExceptionDescription msg;
	  msg << "Error - Vertex has NAN position!" << G4endl;
	  G4Exception("PHG3toG4ParticleGun::SetParticle()", "MyCode0015", FatalException, msg);
	}
      G4PrimaryVertex* g4vtx = new G4PrimaryVertex(xvtx.x()*mm, xvtx.y()*mm, xvtx.z()*mm, xvtx.t()*mm/c_light);
     
      for (HepMC::GenVertex::particle_iterator vpitr = (*vitr)->particles_begin(HepMC::children);
	   vpitr != (*vitr)->particles_end(HepMC::children); ++vpitr) 
	{
	  
	  if( (*vpitr)->status() != 1 ) continue;


	  G4int pdgcode= (*vpitr)-> pdg_id();
	  pos = (*vpitr)->momentum();
	  G4LorentzVector p(pos.px(), pos.py(), pos.pz(), pos.e());
	  G4PrimaryParticle* g4prim = new G4PrimaryParticle(pdgcode, p.x()*GeV, p.y()*GeV, p.z()*GeV);
	  g4vtx->SetPrimary(g4prim);
	  
	  //G4cout << g4prim->GetPDGcode() << "  " << g4prim->GetPz()/GeV << "   " << p.z() << G4endl;
	  
	}
      //G4cout << g4vtx->GetPrimary()->GetPDGcode() << "  " << g4vtx->GetZ0()/mm << "   " << xvtx.z() << G4endl;
      anEvent->AddPrimaryVertex(g4vtx);
    }

}

void PHG3toG4PrimaryGeneratorAction::SetHepMCEvent(HepMC::GenEvent *theEvt)
{
  hepmcevt = theEvt;
}


bool PHG3toG4PrimaryGeneratorAction::CheckVertexInsideWorld (const G4ThreeVector& pos) const
{
  G4Navigator* navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  G4VPhysicalVolume* world = navigator->GetWorldVolume();
  G4VSolid* solid = world->GetLogicalVolume()->GetSolid();
  EInside qinside = solid->Inside(pos*mm);

  if( qinside != kInside) return false;
  else return true;
}

