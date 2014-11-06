void phpythia8hepmcreader(
			  int nevents = 0, 
			  std::string inputname = "pythia8test.root"
			  )
{
  //gSystem->Load("libfun4allfuncs.so");	// framework only
  //gSystem->Load("/direct/phenix+hhj/snowball/local/pythia6/libPythia6PDF.so");
  //gSystem->Load("/direct/phenix+hhj/snowball/local/pythia6/libLHAPDF.so");
  gSystem->Load("libfun4all.so");	// framework + reco modules
  gSystem->Load("/direct/phenix+scratch/snowball/SpinAnalysis/firstBuild/Pythia/event_gen/src/HepMC/install/lib/libPHHepMC.so");
  gSystem->Load("/direct/phenix+scratch/snowball/SpinAnalysis/firstBuild/Pythia/event_gen/src/Test/install/lib/libPHPythia.so");
  gSystem->Load("/direct/phenix+scratch/snowball/SpinAnalysis/firstBuild/Pythia/event_gen/src/Test/install/lib/libPHPythiaEventGen.so");
  gSystem->Load("libLHAPDF");
  //gSystem->Load("libPHPythiaEventGen.so");
  //gSystem->Load("libPHPythia.so");
  gSystem->Load("libPHPyTrigger.so");		// For PHPyTrigger derived classes
  gSystem->Load("libPHPyParticleSelect.so");	// For PHPyParticleSelect derived classes
  gSystem->Load("libsimreco.so");	// framework + reco modules
  gSystem->Load("libfvtx_subsysreco.so");


  
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(10);
  
  SubsysReco *sync = new SyncSimreco();
  se->registerSubsystem(sync);

  PHHepMCHelper *helper = new PHHepMCHelper("PHHepMCHelper");
  helper->SetVerbosity(10);
  helper->WriteHREventsToText("theHepMCEvents.txt");
  helper->WriteEventsToText("theHepMCEvents.txt");
  se->registerSubsystem(helper);

  // DST output manager
  Fun4AllDstInputManager *dst_input_mgr  = new Fun4AllDstInputManager("HepMC","DST");
  se->registerInputManager(dst_input_mgr);
  se->fileopen( dst_input_mgr->Name(), inputname.c_str() );

  // run over all events
  se->run(nevents);  
  se->End();
}

