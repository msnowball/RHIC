
void phpythia_vertexShifter(
			    std::string pythiaFile = "vertex.txt",
			    std::string vertexFile = "vertex.txt",
			    std::string outputname = "output/pythia_ccbar.root"
  )
{

  //gSystem->Load("libfun4allfuncs.so");	// framework only
  gSystem->Load("/direct/phenix+hhj/snowball/local/pythia6/libPythia6PDF.so");
  gSystem->Load("/direct/phenix+hhj/snowball/local/pythia6/libLHAPDF.so");
  gSystem->Load("libfun4all.so");	// framework + reco modules
  gSystem->Load("libPHPythiaEventGen.so");
  gSystem->Load("libPHPythia.so");
  gSystem->Load("libPHPyTrigger.so");		// For PHPyTrigger derived classes
  gSystem->Load("libPHPyParticleSelect.so");	// For PHPyParticleSelect derived classes
  gSystem->Load("libsimreco.so");	// framework + reco modules
  gSystem->Load("libfvtx_subsysreco.so");

  //////////////////////
  bool debug = false;
  int verbosity = 0;
  if (debug) verbosity = 5;
  //////////////////////

  recoConsts *rc = recoConsts::instance();

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(verbosity);
  
  SubsysReco *sync = new SyncSimreco();
  se->registerSubsystem(sync);

  Fun4AllNoSyncDstInputManager *pythiaMgr = new Fun4AllNoSyncDstInputManager("PythiaIn","DST");
  se->registerInputManager(pythiaMgr);
  se->fileopen(pythiaMgr->Name(),pythiaFile.c_str());

  Fun4AllNoSyncDstInputManager *inputMgr = new Fun4AllNoSyncDstInputManager("DSTVertexIn","DST");
  se->registerInputManager(inputMgr);
  se->fileopen(inputMgr->Name(),vertexFile.c_str());

  PHPyVertexShift *shifter = new PHPyVertexShift("VertexShifter","");
  shifter->SetVerbosity(verbosity);
  se->registerSubsystem(shifter);
  
  if(debug)
    {
      FvtxPrinter *thePrinter = new FvtxPrinter("FVTXPRINTER");
      se->registerSubsystem(thePrinter);
    }

  // DST output manager
  Fun4AllDstOutputManager *dst_output_mgr  = new Fun4AllDstOutputManager("PHPYTHIA",outputname.c_str());
  //dst_output_mgr->AddNode("Sync");
  dst_output_mgr->AddNode("EventHeader");
  dst_output_mgr->AddNode("PHPythiaHeader");
  dst_output_mgr->AddNode("PHPythia");
  se->registerOutputManager(dst_output_mgr);

  // run over all events
  se->run(0);  
  se->End();
}

