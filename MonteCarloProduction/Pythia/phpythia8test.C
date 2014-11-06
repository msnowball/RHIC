void phpythia8test(
		    int nevents = 100, 
		    std::string vertexFile = "",
		    std::string outputname = "pythia8test.root"
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


  
  ////////////////////////////
  float ptMin = 1.0;
  int armSelect = 3;
  ////////////////////////////

  int run = 0;
  if (!vertexFile.empty() && nevents == 0)
    {
      std::string tmpString, prdf;
    
      std::string vtxFile = vertexFile+"_filetracking.txt";
      ifstream vtxIn;
      vtxIn.open(vtxFile.c_str());
      if(!vtxIn)
	{
	  cout << "Could not open the filetracking file: " << vtxFile << endl;
	  return;
	}
      vtxIn >> tmpString >> run >> nevents >> prdf;
      vtxIn.close();
      
      std::string fileTracking = outputname+"_filetracking.txt";
      ofstream out;
      out.open(fileTracking.c_str());
      out << vertexFile << "  " << run << "  " <<  nevents << "  " << prdf << endl;
      out.close();
    }

  recoConsts *rc = recoConsts::instance();
  if(run > 0) rc->set_IntFlag("RUNNUMBER",run);

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);
  
  SubsysReco *sync = new SyncSimreco();
  se->registerSubsystem(sync);

  PHPy8ParticleTrigger *pTrig = new PHPy8ParticleTrigger();
  //pTrig->Verbosity(6);
  pTrig->AddParticles("13,-13");
  pTrig->AddParents("521,511,531,541,513,523,533,543");
  pTrig->SetPtLow(1.0);
  pTrig->SetAbsEtaHighLow(2.4,1.2);
  pTrig->PrintConfig();

  //Pythia 
  PHPythia8 *phpythia = new PHPythia8("Pythia8",PHPythia8::HEPMC);
  //PHPythia8 *phpythia = new PHPythia8("Pythia8");
  // Set your own seed, otherwise, seeds from /dev/random
  TRandom3 *r = new TRandom3(0);
  int rand = r->Uniform(1,1000000);
  //phpythia->Verbosity(0);
  phpythia->SetSeed(rand);			
  phpythia->SetConfigFile("phpythia8test.cfg");
  phpythia->registerTrigger(pTrig);
  se->registerSubsystem(phpythia);

  //** You can force the generated particles to use a vertex read from a file,
  //** in place of the default (z=0) value
  //** this is needed for instance when you want to have matching vertices between 
  //** different types of simulated files, prior to sending that to PISA
  // se->registerSubsystem( new PHPyVertexShift( "PHPyVertexShift", "./events.txt") );

  
  //** You can select only particular particles to write out
  //PHPyParticleSelect *pselect = new PHPyParticleSelect();
  //se->registerSubsystem( pselect );

  //** A dummy (null) input is needed for the Fun4All framework
  Fun4AllDummyInputManager *inManager1 = new Fun4AllDummyInputManager("DSTin1", "DST");
  se->registerInputManager(inManager1);

  // DST output manager
  Fun4AllDstOutputManager *dst_output_mgr  = new Fun4AllDstOutputManager("PHPYTHIA",outputname.c_str());
  dst_output_mgr->AddNode("Sync");
  dst_output_mgr->AddNode("EventHeader");
  dst_output_mgr->AddNode("PHPythiaHeader");
  dst_output_mgr->AddNode("PHPythia");
  //dst_output_mgr->AddNode("PHHepMCGenEvent");
  dst_output_mgr->Print("ALL");
  se->registerOutputManager(dst_output_mgr);

  // OSCAR output manager
  // with following output manager, one can write the PHPythia output in an oscar formated output text file
  // PHPyOscarOutputManager *oscar_manager  = new PHPyOscarOutputManager( "OSCAR", outputname.c_str() );
  // se->registerOutputManager(oscar_manager);

  cout << "NEVENTS: " << nevents << endl;
  
  // run over all events
  se->run(nevents,true);  
  se->End();
}

