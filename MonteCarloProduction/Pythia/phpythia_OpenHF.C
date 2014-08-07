//#include <libgen.h>

void phpythia_OpenHF(
  const int nevents = 1000, 
  const char *outputname = "output/pythia_ccbar.root",
  const char *oscar_outputname = "oscar/oscar.txt"
  )
{
  //gSystem->Load("libfun4allfuncs.so");	// framework only
  gSystem->Load("libfun4all.so");	// framework + reco modules
  gSystem->Load("libPHPythiaEventGen.so");
  gSystem->Load("libPHPythia.so");
  //gSystem->Load("/direct/phenix+u/workarea/slash/pythia/libPythia6PDF.so");
  gSystem->Load("libPHPyTrigger.so");		// For PHPyTrigger derived classes
  gSystem->Load("libPHPyParticleSelect.so");	// For PHPyParticleSelect derived classes
  gSystem->Load("libsimreco.so");	// framework + reco modules

  recoConsts *rc = recoConsts::instance();
  rc->set_IntFlag("RUNNUMBER",0);

  /////////////////////////////////////////////////////////////////
  //  Server...
  Fun4AllServer *se = Fun4AllServer::instance();

  /////////////////////////////////////////////////////////////////
  //  Reconstruction Modules...
  
  SubsysReco *sync = new SyncSimreco();
  se->registerSubsystem(sync);

  PHPythia *phpythia = new PHPythia();
  
  // Set your own seed, otherwise, seeds from /dev/random
  //phpythia->SetSeed(1999);			
  phpythia->SetConfigFile("phpythia_OpenHF.cfg");

  se->registerSubsystem(phpythia);

  //** You can force the generated particles to use a vertex read from a file,
  //** in place of the default (z=0) value
  //** this is needed for instance when you want to have matching vertices between 
  //** different types of simulated files, prior to sending that to PISA
  // se->registerSubsystem( new PHPyVertexShift( "PHPyVertexShift", "./events.txt") );
  
  PHPyCharmMuonTrigger *trigger = new PHPyCharmMuonTrigger();
  trigger->set_arm_selection(3);
  trigger->set_muon_pt_min(0.0);
  trigger->set_use_internal_vertex_shift(false);

  //** You can use dedicated triggers, derived from the PHPyTrigger base class
  se->registerSubsystem(trigger);
  //se->registerSubsystem( new PHPyBeautyMuonTrigger() );

  //** You can select only particular particles to write out
  //PHPyParticleSelect *pselect = new PHPyParticleSelect();
  //se->registerSubsystem( pselect );

  //** A dummy (null) input is needed for the Fun4All framework
  Fun4AllDummyInputManager *in1 = new Fun4AllDummyInputManager("DSTin1", "DST");
  se->registerInputManager(in1);

  // DST output manager
  TString OUTPUT = outputname;
  Fun4AllDstOutputManager *dst_output_mgr  = new Fun4AllDstOutputManager("PHPYTHIA",OUTPUT.Data());
  dst_output_mgr->AddNode("Sync");
  dst_output_mgr->AddNode("PHPythiaHeader");
  dst_output_mgr->AddNode("PHPythia");
  se->registerOutputManager(dst_output_mgr);

  // OSCAR output manager
  // with following output manager, one can write the PHPythia output in an oscar formated output text file
  // PHPyOscarOutputManager *oscar_manager  = new PHPyOscarOutputManager( "OSCAR", oscar_outputname );
  // se->registerOutputManager(oscar_manager);
  
  // run over all events
  se->run(nevents);  
  se->End();
}

