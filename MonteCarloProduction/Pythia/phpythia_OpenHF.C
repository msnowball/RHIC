
void phpythia_OpenHF(
		     int nevents = 0, 
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

  //Pythia 
  PHPythia *phpythia = new PHPythia();
  // Set your own seed, otherwise, seeds from /dev/random
  TRandom3 *r = new TRandom3(0);
  int rand = r->Uniform(1,1000000);
  phpythia->SetSeed(rand);			
  phpythia->SetConfigFile("phpythia_OpenHF.cfg");
  se->registerSubsystem(phpythia);

  //** You can force the generated particles to use a vertex read from a file,
  //** in place of the default (z=0) value
  //** this is needed for instance when you want to have matching vertices between 
  //** different types of simulated files, prior to sending that to PISA
  // se->registerSubsystem( new PHPyVertexShift( "PHPyVertexShift", "./events.txt") );

  PHPyCharmMuonTrigger *trigger = new PHPyCharmMuonTrigger();
  //PHPyBeautyMuonTrigger *trigger = new PHPyBeautyMuonTrigger();
  trigger->set_arm_selection(armSelect);
  trigger->set_use_internal_vertex_shift(false);
  //trigger->set_normalization_filename( normalization_out );
  //trigger->vertex_shift_module()->SetVtxFileVersion( 1 ); //version1 = 0, version2 = 1 
  //trigger->vertex_shift_module()->SetVtxFile(vertexFile);
  //trigger->vertex_shift_module()->SetVerbosity(3);
  //trigger->set_accepted_parents(parents, 6);
  //trigger->set_parent_mode(PHPyCharmMuonTrigger::Ancestor);
  trigger->set_pt_min(ptMin);
  se->registerSubsystem(trigger);
  
  //** You can select only particular particles to write out
  //PHPyParticleSelect *pselect = new PHPyParticleSelect();
  //se->registerSubsystem( pselect );

  //** A dummy (null) input is needed for the Fun4All framework
  Fun4AllDummyInputManager *inManager1 = new Fun4AllDummyInputManager("DSTin1", "DST");
  se->registerInputManager(inManager1);

  // DST output manager
  Fun4AllDstOutputManager *dst_output_mgr  = new Fun4AllDstOutputManager("PHPYTHIA",outputname.c_str());
  //dst_output_mgr->AddNode("Sync");
  dst_output_mgr->AddNode("EventHeader");
  dst_output_mgr->AddNode("PHPythiaHeader");
  dst_output_mgr->AddNode("PHPythia");
  se->registerOutputManager(dst_output_mgr);

  // OSCAR output manager
  // with following output manager, one can write the PHPythia output in an oscar formated output text file
  // PHPyOscarOutputManager *oscar_manager  = new PHPyOscarOutputManager( "OSCAR", outputname.c_str() );
  // se->registerOutputManager(oscar_manager);
  
  // run over all events
  se->run(nevents,true);  
  se->End();
}

