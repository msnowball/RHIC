void particleGun(
		 int nevents = 10000
		 )
{

  //gSystem->Load("libjprof.so");
  //prof *Pr = new prof;

  gROOT->LoadMacro("g4libs.C");
  g4libs();

  gSystem->Load("libfun4all.so");	// framework + reco modules
  gSystem->Load("libfvtx_subsysreco.so");
  gSystem->Load("libsimreco.so");
  gSystem->Load("libPHHepMC.so");
  gSystem->Load("libPHG3toG4.so");

  
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);
  
  SubsysReco *sync = new SyncSimreco();
  se->registerSubsystem(sync);


  TRandom3 *r = new TRandom3(0);
  //int rand = r->Uniform(-1,1);
  //double cosTheta = 2*rand - 1.;

  PHG3toG4 *myG3toG4 = new PHG3toG4("PHG3toG4","Sim3D++.root","geometry_5.root");
  myG3toG4->UseParticleGun("mu-",10/*GeV*/);
  myG3toG4->UseParticleGunRandomEnergy(1,15);
  myG3toG4->UseParticleGunForwardOnly();
  myG3toG4->UseParticleGunRandomVtx(-2,2,-2,2,-30,30);

  //myG3toG4->ParticleGunMomentumVec(TVector3(0.3,0.3,1));
  //myG3toG4->UseParticleGun("geantino",10/*GeV*/);
  //myG3toG4->CheckOverlaps(true);
  //myG3toG4->SetMaxToF("ALL",400);//nsec
  //myG3toG4->SetMaxStep("SVX",0.1);//mm
  //myG3toG4->SetMaxStep("MUT",1.0);//mm
  //myG3toG4->SetMaxStep("MUI",0.5);//mm
  //myG3toG4->SetTrackEnergyCut(15.0);//MeV
  //myG3toG4->SetMinKinEnergy("BBC",20);//MeV
  //myG3toG4->SetMinKinEnergy("MUT",0.1);//MeV
  
  //myG3toG4->SetMinEnergyDeposit("BBC",0);//MeV
  //myG3toG4->SetMinEnergyDeposit("MUI",0);//MeV
  //myG3toG4->SetMinEnergyDeposit("MUT",0);//MeV
  //myG3toG4->SetMinEnergyDeposit("SVX",0);//MeV
  
  //myG3toG4->SetBField(false);
  //myG3toG4->Verbosity(2);
  //myG3toG4->SetPhysicsList("QGSP_BERT");
  se->registerSubsystem(myG3toG4);

  // DST output manager
  //Fun4AllDstInputManager *dst_input_mgr  = new Fun4AllDstInputManager("HepMC","DST");
  //se->registerInputManager(dst_input_mgr);
  //se->fileopen( dst_input_mgr->Name(), inputname.c_str() );

  // run over all events
  //se->skip(5);
  se->run(nevents);  
  se->End();

}

