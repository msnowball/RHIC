#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <stdlib.h>
#include <stdexcept>
#include <ctime>
#include <sys/time.h>
#include <algorithm>
#include <cctype>

//PHG3toG4
#include "PHG3toG4.h"
#include "PHG3toG4PrimaryGeneratorAction.h"
#include "PHG3toG4StackingAction.h"
#include "PHG3toG4EventAction.h"
#include "PHG3toG4TrackingAction.h"
#include "PHG3toG4SteppingAction.h"
#include "PHG3toG4PostDetConstruction.h"
#include "PHG3toG4RootManager.h"
#include "PHG3toG4MagneticField.h"
#include "PHG3toG4ParticleGun.h"

//HepMC
#include "HepMC/GenEvent.h"

//Fun4All
#include <PHIODataNode.h>
#include <PHDataNode.h>
#include <PHObject.h>
#include <PHCompositeNode.h>
#include <PHNodeIterator.h>
#include <PHNodeReset.h>
#include <PHTimeStamp.h>
#include <Fun4AllReturnCodes.h>
#include <getClass.h>
#include <VtxOut.h>
#include <PHPoint.h>

//PHHepMC
#include <PHHepMCGenEvent.h>
#include <PHHepMCCombiner.h>

//Root
#include <TClonesArray.h>
#include <TFile.h>
#include <TTree.h>
#include <TDirectory.h>
#include <TObjArray.h>
#include <TParticle.h>
#include <TGeoManager.h>
#include <TG4RootNavMgr.h>

//G4
#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4LorentzVector.hh"
#include "G4Event.hh"
#include "G4PrimaryParticle.hh"
#include "G4PrimaryVertex.hh"
#include "G4TransportationManager.hh"
#include "G4PhysicalConstants.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysListFactory.hh"
#include "G4VModularPhysicsList.hh"
#include "G4StepLimiterPhysics.hh"
#include "G4ios.hh"
#include "G4MagneticField.hh"
#include "G4FieldManager.hh"
#include "G4TransportationManager.hh"
#include "G4EquationOfMotion.hh"
#include "G4EqMagElectricField.hh"
#include "G4Mag_UsualEqRhs.hh"
#include "G4MagIntegratorStepper.hh"
#include "G4MagIntegratorDriver.hh"
#include "G4ChordFinder.hh"
#include "G4ClassicalRK4.hh"
#include "G4SimpleRunge.hh"
#include "G4SimpleHeum.hh"
#include "G4HelixImplicitEuler.hh"
#include "G4NystromRK4.hh"
#include "G4Timer.hh"
#include "Randomize.hh"
#include "G4VUserPrimaryGeneratorAction.hh"
#include "G4HadronicProcessStore.hh"
#include "G4LossTableManager.hh"

using namespace std;

typedef PHIODataNode<PHObject> PHObjectNode_t;
//------------------------------------------------------//

PHG3toG4::PHG3toG4(const std::string &name, const std::string fieldFileName, const std::string geoFileName): 
  SubsysReco(name),
  _eventCounter(0),
  _reportEvery(1000),
  timer(0),
  _partGunVector(0),
  _partGunVertex(0)
{
  if(verbosity > 1) cout << "PHG3toG4::PHG3toG4 - in constructor" << endl;
  if(geoFileName.empty())throw runtime_error("PHG3toG4::PHG3toG4 - geometry filename is empty!!!!");
  if(fieldFileName.empty())throw runtime_error("PHG3toG4::PHG3toG4 - B field filename is empty!!!!");

  _theFileName = geoFileName;
  _magFieldFileName = fieldFileName;

  /***********Initialize some master settings************/
  _hepmcCombiner = NULL;
  _theHepMCEvt = NULL;
  _useBField = true;
  _checkOverlaps = false;
  _userLimits = true;

  //Particle Gun
  _usePartGun = false;
  _usePartGunVec = false;
  _usePartGunRandE = false;
  _usePartGunRandVtx = false;
  _usePartGunUserVertex = false;
  _usePartGunForward = false;
  _usePartGunVertexFile = false;
  _usePartGunVertexDST = false;
  _partGunPartName = "mu-";
  _partGunVertexFile = "";
  _partGunEnergy = -1;//GeV
  _partGunMinE = -1;
  _partGunMaxE = -1;
  _partGunMinVtxX = -99;
  _partGunMaxVtxX = -99;
  _partGunMinVtxY = -99;
  _partGunMaxVtxY = -99;
  _partGunMinVtxZ = -99;
  _partGunMaxVtxZ = -99;
 
  //Phys settings
  _thePhysList = "QGSP_BERT";//default in sPHENIX

  //Det settings
  _theMaxTof = 400;//nsec
  _theMaxTof_SVX = 400;
  _theMaxTof_MUI = 400;
  _theMaxTof_MUT = 400;
  _theMaxTof_BBC = 400;
  _theMaxTof_MUPC = 400;
  
  _theMaxStep = -1;//mm
  _theMaxStep_SVX = 0.05;
  _theMaxStep_MUI = 0.5;
  _theMaxStep_MUT = 1.0;
  _theMaxStep_BBC = -1;
  _theMaxStep_MUPC = -1;

  _theMinKinEnergy = -1;//MeV
  _theMinKinEnergy_SVX = -1;
  _theMinKinEnergy_MUI = -1;
  _theMinKinEnergy_MUT = 0.1;
  _theMinKinEnergy_BBC = 20.0;
  _theMinKinEnergy_MUPC = -1;

  _theMinEnergyDep_SVX = 0.001;//MeV
  _theMinEnergyDep_MUI = 0.00035;
  _theMinEnergyDep_MUT = 0.000001;
  _theMinEnergyDep_BBC = 0.1;
  _theMinEnergyDep_MUPC = 0.001;

  // Thresholds for production and steps
  _theTrackEnergyCut = 20.0;//MeV
  _theFieldMinStep = 0.005;//mm
  /******************************************************/


  ///////////////////////////////////////////////////////////////
  geoManager = TGeoManager::Import(_theFileName.c_str());
  g4rootNav = TG4RootNavMgr::GetInstance(geoManager);
  runManager = new G4RunManager();
  if(verbosity > 1) cout << "PHG3toG4::PHG3toG4 - done" << endl;

  //Event timing
  _totalTime = 0;
  _longestEventTime = 0;
  _shortestEventTime = 99999;
  _longestEvent = -1;
  _shortestEvent = -1;

  
}

//__________________________________________________________
PHG3toG4::~PHG3toG4()
{ 
}

//__________________________________________________________
int PHG3toG4::Init(PHCompositeNode *topNode)
{
  if(verbosity > 1) cout << "PHG3toG4::Init - begin init"<< endl;
  
  //PHHepMC Multiple Nodes
  if(_nodeNames.size() > 0) _hepmcCombiner = new PHHepMCCombiner();

  theDet = PHG3toG4PostDetConstruction::GetInstance();

  //Set user limits on tof, steps, and energy
  if(_theMaxTof_SVX>0) theDet->SetMaxTof("SVX",_theMaxTof_SVX);
  if(_theMaxTof_MUI>0) theDet->SetMaxTof("MUI",_theMaxTof_MUI);
  if(_theMaxTof_MUT>0) theDet->SetMaxTof("MUT",_theMaxTof_MUT);
  if(_theMaxTof_BBC>0) theDet->SetMaxTof("BBC",_theMaxTof_BBC);
  if(_theMaxTof_MUPC>0) theDet->SetMaxTof("MUPC",_theMaxTof_MUPC);
  
  if(_theMaxStep_SVX>0) theDet->SetMaxStep("SVX",_theMaxStep_SVX);
  if(_theMaxStep_MUI>0) theDet->SetMaxStep("MUI",_theMaxStep_MUI);
  if(_theMaxStep_MUT>0) theDet->SetMaxStep("MUT",_theMaxStep_MUT);
  if(_theMaxStep_BBC>0) theDet->SetMaxStep("BBC",_theMaxStep_BBC);
  if(_theMaxStep_MUPC>0) theDet->SetMaxStep("MUPC",_theMaxStep_MUPC);
  
  if(_theMinKinEnergy_SVX>=0) theDet->SetMinKinEnergy("SVX",_theMinKinEnergy_SVX);
  if(_theMinKinEnergy_MUI>=0) theDet->SetMinKinEnergy("MUI",_theMinKinEnergy_MUI);
  if(_theMinKinEnergy_MUT>=0) theDet->SetMinKinEnergy("MUT",_theMinKinEnergy_MUT);
  if(_theMinKinEnergy_BBC>=0) theDet->SetMinKinEnergy("BBC",_theMinKinEnergy_BBC);
  if(_theMinKinEnergy_MUPC>=0) theDet->SetMinKinEnergy("MUPC",_theMinKinEnergy_MUPC);
  
  if(_theMinEnergyDep_SVX>=0) theDet->SetMinEnergyDep("SVX",_theMinEnergyDep_SVX);
  if(_theMinEnergyDep_MUI>=0) theDet->SetMinEnergyDep("MUI",_theMinEnergyDep_MUI);
  if(_theMinEnergyDep_MUT>=0) theDet->SetMinEnergyDep("MUT",_theMinEnergyDep_MUT);
  if(_theMinEnergyDep_BBC>=0) theDet->SetMinEnergyDep("BBC",_theMinEnergyDep_BBC);
  if(_theMinEnergyDep_MUPC>=0) theDet->SetMinEnergyDep("MUPC",_theMinEnergyDep_MUPC);


  //Initalize geometry and connect it to G4
  g4rootNav->Initialize(theDet);
  g4rootNav->ConnectToG4();
  if(verbosity > 0) g4rootNav->PrintG4State();

  //UI manager for string inputs
  UImanager = G4UImanager::GetUIpointer();
  //UImanager->ApplyCommand("/tracking/verbose 1");

  if(verbosity > 20) geoManager->SetVerboseLevel(10);
  if(_checkOverlaps)
    {
      geoManager->CheckOverlaps(0.0001);
      geoManager->PrintOverlaps();
    }
 
  if(_useBField) SetBField();

  ////////////////////////////////////////////////
  timer = new G4Timer();

  physListFactory = new G4PhysListFactory();
  if(verbosity < 1) physListFactory->SetVerbose(0);
  physicsList = physListFactory->GetReferencePhysList(_thePhysList.c_str());
  if(_userLimits) physicsList->RegisterPhysics(new G4StepLimiterPhysics());
  runManager->SetUserInitialization(physicsList);


  if(_usePartGun)
    {
      thePrimPG = new PHG3toG4ParticleGun();
      if(_usePartGunVertexFile) thePrimPG->UseVertexFile(_partGunVertexFile);
      runManager->SetUserAction(thePrimPG);
      if(_usePartGunVec) thePrimPG->SetUserMomDir(_partGunVector);
      if(_usePartGunRandE) thePrimPG->SetRandEnergy(_partGunMinE,_partGunMaxE);
      if(_usePartGunRandVtx) thePrimPG->SetRandVertex(_partGunMinVtxX,_partGunMaxVtxX,_partGunMinVtxY,_partGunMaxVtxY,_partGunMinVtxZ,_partGunMaxVtxZ);
      if(_usePartGunUserVertex) thePrimPG->SetUserVertex(_partGunVertex);
      if(_usePartGunForward) thePrimPG->SetDirectionForward();
    }
  else{
    thePrimGA = new PHG3toG4PrimaryGeneratorAction();
    thePrimGA->Verbosity(verbosity);
    runManager->SetUserAction(thePrimGA);
  }

  if(verbosity < 1)
    {
      G4HadronicProcessStore::Instance()->SetVerbose(0);
      G4LossTableManager::Instance()->SetVerbose(0);
    }
  //Must be initialized before StackingAction
  theRootMgr = PHG3toG4RootManager::GetInstance();
  theRootMgr->Init();

  theStackA = new PHG3toG4StackingAction();
  theStackA->SetMinTrackEnergy(_theTrackEnergyCut);
  runManager->SetUserAction(theStackA);


  theEA = new PHG3toG4EventAction();
  theEA->Verbosity(verbosity);
  runManager->SetUserAction(theEA);


  theSA = new PHG3toG4SteppingAction(_theTrackEnergyCut);
  theSA->Verbosity(verbosity);
  runManager->SetUserAction(theSA);


  theTA = new PHG3toG4TrackingAction();
  runManager->SetUserAction(theTA);

  runManager->Initialize();

  if(verbosity > 1) cout << "PHG3toG4::Init - end init"<< endl;

  return EVENT_OK;
}

//__________________________________________________________
int PHG3toG4::End(PHCompositeNode *topNode)
{

  theRootMgr->End();
  G4cout << "************************* PHG3toG4::End *************************" << G4endl
	 << "    Total Number of Events:  " << _eventCounter << G4endl
	 << "    Total Job Time:          " << _totalTime << "s" << G4endl 
	 << "    Average Event Time:      " << _totalTime/_eventCounter << "s" << G4endl
	 << "    Longest Event Time:      " << _longestEventTime << "s  (Event: " << _longestEvent << ")" << G4endl 
	 << "    Shortest Event Time:     " << _shortestEventTime << "s  (Event: " << _shortestEvent << ")" << G4endl
	 << "*****************************************************************" << G4endl;
  
  return EVENT_OK;
}

int PHG3toG4::process_event(PHCompositeNode *topNode)
{
  //Event timer/counter
  timer->Start();
  _eventCounter++;
  if((_eventCounter == 1 || _eventCounter%_reportEvery == 0) && verbosity > -1) G4cout << ">>>> Event: " << _eventCounter << G4endl;

  if(verbosity > 2) cout << "PHG3toG4::process_event - new event" << endl;

  //Particle Generation
  if(_usePartGun)
    {
      if(_usePartGunVertexDST)
	{
	  VtxOut* vtx = findNode::getClass<VtxOut>(topNode, "VtxOut" );
	  if (!vtx)
	    {
	      G4ExceptionDescription msg;
	      msg << "Error - VtxOut Node not present! Check your input DST!" << G4endl;
	      G4Exception("PHG3toG4::process_event()", "MyCode0009", FatalException, msg);
	    }
	  double vtx_x = vtx->get_Vertex(_partGunVertexName.c_str()).getX();
	  double vtx_y = vtx->get_Vertex(_partGunVertexName.c_str()).getY();
	  double vtx_z = vtx->get_Vertex(_partGunVertexName.c_str()).getZ();
	  G4ThreeVector thePartGunVert(vtx_x,vtx_y,vtx_z);
	  thePrimPG->SetUserVertex(thePartGunVert);
  	}
      thePrimPG->SetParticle(_partGunPartName,_partGunEnergy);
    }
  else{
    //Get Single or Multiple HepMC Event Nodes
    if(_nodeNames.size() == 0)
      {
	//Single PHHepMCGenEvent Node
	PHHepMCGenEvent* hepmcEvent = findNode::getClass<PHHepMCGenEvent>(topNode,"PHHepMCGenEvent");
	if (!hepmcEvent && !_usePartGun)
	  {
	    cout << "PHG3toG4::process_event - unable to get PHHepMCGenEvent, is Node missing?" << endl;
	    return ABORTRUN;
	  }

	//Fill HepMC event from DST
	if(verbosity > 2) cout << "PHG3toG4::process_event - grabbing HepMC::GenEvent" << endl;
	_theHepMCEvt = hepmcEvent->getEvent();
	if(verbosity > 9) _theHepMCEvt->print();

      }
    else{
      
      //Multiple PHHepMCGenEvent Nodes Combined
      _theHepMCEvt = new HepMC::GenEvent(HepMC::Units::GEV,HepMC::Units::MM);
      for(unsigned int i = 0; i < _nodeNames.size(); i++)
	{
	  PHHepMCGenEvent* hepmcEvent = getPHHepMCGenEvent(topNode,_nodeNames[i]);
	  HepMC::GenEvent* tmpEvent = hepmcEvent->getEvent();
	  _hepmcCombiner->combineEvents(tmpEvent,_theHepMCEvt);
	}
      //_theHepMCEvt->print();

    }//if(_nodeNames.size() == 0) else


    //Fill G4Event from HepMC event
    if(verbosity > 2) cout << "PHG3toG4::process_event - filling G4Event" << endl;
    thePrimGA->SetHepMCEvent(_theHepMCEvt);

  }//if(_usePartGun) else


  //Run event through G4
  if(verbosity > 2) cout << "PHG3toG4::process_event - BeamOn" << endl;
  runManager->BeamOn(1);
  if(verbosity > 2) cout << "PHG3toG4::process_event - done" << endl;


  //Timing
  timer->Stop();
  _totalTime += timer->GetRealElapsed();
  if(timer->GetRealElapsed() > _longestEventTime){ _longestEventTime = timer->GetRealElapsed(); _longestEvent = _eventCounter;}
  if(timer->GetRealElapsed() < _shortestEventTime){ _shortestEventTime = timer->GetRealElapsed(); _shortestEvent = _eventCounter;}


  return EVENT_OK;
}

int PHG3toG4::ResetEvent(PHCompositeNode *topNode)
{
  if(_nodeNames.size() > 0 && _theHepMCEvt) delete _theHepMCEvt;
  theSA->Reset();
  return 0;
}



void PHG3toG4::SetBField()
{
  //B Field from map
  theBField = new PHG3toG4MagneticField(_magFieldFileName);

  G4double theFieldMinStep = _theFieldMinStep*mm ; // minimal step of 10 microns
  theFieldManager = G4TransportationManager::GetTransportationManager()->GetFieldManager();
  //G4int nvar = 8;//Only need 8 if field varies with time
  theFieldEquation = new G4Mag_UsualEqRhs(theBField);
  //theFieldEquation = new G4EqMagElectricField(theBField); 

  //  4th order robust (not recommended for maps)
  //theFieldStepper = new G4ClassicalRK4( theFieldEquation, nvar );
  //  3rd  order, a good alternative to ClassicalRK
  //theFieldStepper = new G4SimpleHeum( theFieldEquation);         
  //  2nd  order, for less smooth fields
  //theFieldStepper = new G4SimpleRunge( theFieldEquation ); 
  // 4/5th order for very smooth fields 
  //theFieldStepper = new G4CashKarpRKF45( theFieldEquation );     
  // 2nd order
  //theFieldStepper = new G4HelixImplicitEuler( theFieldEquation );
  // 1st order (not recommended)
  //theFieldStepper = new G4HelixExplicitEuler( theFieldEquation );
  // 2nd order
  //theFieldStepper = new G4HelixSimpleRunge( theFieldEquation );
  // Simpler RK (recommended for maps)
  theFieldStepper = new G4NystromRK4( theFieldEquation );

  theFieldManager->SetDetectorField(theBField);
  theIntgrDriver = new G4MagInt_Driver(theFieldMinStep,theFieldStepper,theFieldStepper->GetNumberOfVariables());
  theChordFinder = new G4ChordFinder(theIntgrDriver);
  theFieldManager->SetChordFinder( theChordFinder );

  //double point[4] = {0,0,0,0};
  //theBField->GetFieldValue(&point[0],&magfield_at_000[0]);
  //for (int i=0; i<4;i++)
  // {
  //   magfield_at_000[i] = magfield_at_000[i]/tesla;
  // }

}





/********************* User Settings ************************/

void PHG3toG4::SetPhysicsList(std::string list)
{
  if (list == "FTFP_BERT") _thePhysList = list;
  else if (list == "QGSP_BERT") _thePhysList = list;
  else if (list == "QGSP_BIC") _thePhysList = list;
  else if (list == "QGSP_BIC_HP"){_thePhysList = list; setenv("AllowForHeavyElements","1",1);}
  else if (list == "LHEP") _thePhysList = list;
  else if (list == "FTFP_BERT_HP") _thePhysList = list;
  else if (list == "QGSP_BERT_HP"){_thePhysList = list; setenv("AllowForHeavyElements","1",1);}
  else
    {
      G4cout << "Physics List " << list << " not implemented" << G4endl
	     << "Defaulting to FTFP_BERT" << G4endl;
      return;
    }
  G4cout << ">>> Physics List set to " << list << G4endl;
}


void PHG3toG4::SetMaxToF(std::string detector, double tof)
{
  if(detector.empty() || detector == "ALL") _theMaxTof = tof;
  else if(detector == "SVX") _theMaxTof_SVX = tof;
  else if(detector == "MUI") _theMaxTof_MUI = tof;
  else if(detector == "MUT") _theMaxTof_MUT = tof;
  else if(detector == "BBC") _theMaxTof_BBC = tof;
  else if(detector == "MUPC") _theMaxTof_MUPC = tof;
  else{ G4cout << "PHG3toG4::SetMaxToF - Unknown detector id: " << detector << G4endl; return; }
  
  //If master is set, set all detectors
  if(detector.empty() || detector == "ALL")
    {
      _theMaxTof_SVX = _theMaxTof;
      _theMaxTof_MUI = _theMaxTof;
      _theMaxTof_MUT = _theMaxTof;
      _theMaxTof_BBC = _theMaxTof;
      _theMaxTof_MUPC = _theMaxTof;
    }
  
  if(detector.empty() || detector == "ALL") detector = "ALL DETECTORS";
  G4cout << "PHG3toG4::SetMaxToF - Set " << detector << " max tof to " << tof << "s" << G4endl;
  
  _userLimits = true;
}

void PHG3toG4::SetMaxStep(std::string detector, double step)
{
  if(detector.empty() || detector == "ALL") _theMaxStep = step;
  else if(detector == "SVX") _theMaxStep_SVX = step;
  else if(detector == "MUI") _theMaxStep_MUI = step;
  else if(detector == "MUT") _theMaxStep_MUT = step;
  else if(detector == "BBC") _theMaxStep_BBC = step;
  else if(detector == "MUPC") _theMaxStep_MUPC = step;
  else{ G4cout << "PHG3toG4::SetMaxStep - Unknown detector id: " << detector << G4endl; return; }
  
  //If master is set, set all detectors
  if(detector.empty() || detector == "ALL")
    {
      _theMaxStep_SVX = _theMaxStep;
      _theMaxStep_MUI = _theMaxStep;
      _theMaxStep_MUT = _theMaxStep;
      _theMaxStep_BBC = _theMaxStep;
      _theMaxStep_MUPC = _theMaxStep;
    }
  
  if(detector.empty() || detector == "ALL") detector = "ALL DETECTORS";
  G4cout << "PHG3toG4::SetMaxStep - Set " << detector << " max step length to " << step << "mm" << G4endl;
  
  _userLimits = true;
}


void PHG3toG4::SetMinKinEnergy(std::string detector, double e)
{
  if(detector.empty() || detector == "ALL") _theMinKinEnergy = e;
  else if(detector == "SVX") _theMinKinEnergy_SVX = e;
  else if(detector == "MUI") _theMinKinEnergy_MUI = e;
  else if(detector == "MUT") _theMinKinEnergy_MUT = e;
  else if(detector == "BBC") _theMinKinEnergy_BBC = e;
  else if(detector == "MUPC") _theMinKinEnergy_MUPC = e;
  else{ G4cout << "PHG3toG4::SetMinKinEnergy - Unknown detector id: " << detector << G4endl; return; }
  
  //If master is set, set all detectors
  if(detector.empty() || detector == "ALL")
    {
      _theMinKinEnergy_SVX = _theMinKinEnergy;
      _theMinKinEnergy_MUI = _theMinKinEnergy;
      _theMinKinEnergy_MUT = _theMinKinEnergy;
      _theMinKinEnergy_BBC = _theMinKinEnergy;
      _theMinKinEnergy_MUPC = _theMinKinEnergy;
    }
  
  if(detector.empty() || detector == "ALL") detector = "ALL DETECTORS";
  G4cout << "PHG3toG4::SetMinKinEnergy - Set " << detector << " min energy deposit to " << e << "MeV" << G4endl;
  
  _userLimits = true;
}

void PHG3toG4::SetMinEnergyDeposit(std::string detector, double e)
{
  if(detector == "SVX") _theMinEnergyDep_SVX = e;
  else if(detector == "MUI") _theMinEnergyDep_MUI = e;
  else if(detector == "MUT") _theMinEnergyDep_MUT = e;
  else if(detector == "BBC") _theMinEnergyDep_BBC = e;
  else if(detector == "MUPC") _theMinEnergyDep_MUPC = e;
  else{ G4cout << "PHG3toG4::SetMinEnergyDeposit - Unknown detector id: " << detector << G4endl; return; }
  
  G4cout << "PHG3toG4::SetMinKinEnergy - Set " << detector << " min energy deposit to " << e << "MeV" << G4endl;
  _userLimits = true;
}


PHHepMCGenEvent* PHG3toG4::getPHHepMCGenEvent(PHCompositeNode *topNode, std::string node_name)
{
  PHHepMCGenEvent* hepmcEvent = findNode::getClass<PHHepMCGenEvent>(topNode,node_name.c_str());
  if (!hepmcEvent && !_usePartGun)
    {
      G4ExceptionDescription msg;
      msg << "PHG3toG4::getPHHepMCGenEvent - unable to get PHHepMCGenEvent node named " << node_name << ". Is Node missing?" << G4endl;
      G4Exception("PHG3toG4::getPHHepMCGenEvent()", "MyCode0010", FatalException, msg);
    }
  
  return hepmcEvent;
}
