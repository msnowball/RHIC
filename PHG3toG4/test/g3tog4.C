void g3tog4(
	    int nevents = 1000, 
	    std::string inputname = "JPsiPy8_1.root"
	    )
{

  //gSystem->Load("libjprof.so");
  //prof *Pr = new prof;

  gROOT->LoadMacro("g4libs.C");
  g4libs();

  gSystem->Load("libfun4all.so");	// framework + reco modules
  gSystem->Load("libfvtx_subsysreco.so");
  gSystem->Load("libsimreco.so");
  gSystem->Load("/direct/phenix+scratch/snowball/SpinAnalysis/firstBuild/Pythia/event_gen/src/HepMC/install/lib/libPHHepMC.so");
  gSystem->Load("/direct/phenix+scratch/snowball/SpinAnalysis/firstBuild/simulation/PHG3toG4/install/lib/libPHG3toG4.so");

  
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(10);
  
  SubsysReco *sync = new SyncSimreco();
  se->registerSubsystem(sync);

  PHG3toG4 *myG3toG4 = new PHG3toG4("PHG3toG4","Sim3D++.root","geom_rpc.root");
  //myG3toG4->CheckOverlaps(true);
  //myG3toG4->SetMaxToF("ALL",400);//nsec
  //myG3toG4->SetMaxStep("SVX",0.1);//mm
  //myG3toG4->SetMaxStep("MUT",1.0);//mm
  //myG3toG4->SetMaxStep("MUI",0.5);//mm
  //myG3toG4->SetTrackEnergyCut(15.0);//MeV
  //myG3toG4->SetMinKinEnergy("BBC",20);//MeV
  //myG3toG4->SetMinKinEnergy("MUT",0.1);//MeV
  //myG3toG4->SetMinEnergyDeposit("BBC",.1);//MeV
  //myG3toG4->SetMinEnergyDeposit("MUI",.002);//MeV
  //myG3toG4->SetBField(false);
  //myG3toG4->Verbosity(10);
  //myG3toG4->SetPhysicsList("QGSP_BERT");
  se->registerSubsystem(myG3toG4);

  // DST output manager
  Fun4AllDstInputManager *dst_input_mgr  = new Fun4AllDstInputManager("HepMC","DST");
  se->registerInputManager(dst_input_mgr);
  se->fileopen( dst_input_mgr->Name(), inputname.c_str() );

  // run over all events
  //se->skip(5);
  se->run(nevents);  
  se->End();

}

