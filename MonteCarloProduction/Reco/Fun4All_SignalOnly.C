void Fun4All_SignalOnly(
			int nEvents = 0,
			std::string pisaFile = "",
			std::string pythiaFile = "",
			std::string vertexFile = "",
			std::string outputName  = ""
			)

{

  /////////////////////////
  bool use_fvtx = true;
  bool write_dst = true;
  bool write_ndst = false;
  bool write_pdst = true;
  bool read_pythia = true;
  
  bool debug = false;
  int verbosity = 0;
  if(debug) verbosity = 5;
  /////////////////////////

  if(outputName.empty())
    {
      cout << "outputName is empty! Exiting..." << endl;
      return;
    }
  std::string outputDST = outputName+"_dst.root";
  std::string outputNDST = outputName+"_ndst.root";
  std::string outputPDST = outputName+"_pdst.root";

  int run = 0;
  //File tracking info
  if (!pisaFile.empty() && nEvents == 0)
    {
      std::string tmpString, prdf;
      std::string pFile = pisaFile+"_filetracking.txt";
      ifstream pIn;
      pIn.open(pFile.c_str());
      if(!pIn)
        {
          cout << "Could not open the filetracking file: " << pFile << endl;
          return;
        }
      pIn >> vertexFile >> pythiaFile >> tmpString;
      pIn.close();

      string vFileT = vertexFile+"_filetracking.txt";
      pIn.open(vFileT.c_str());
      if(!pIn)
	{
          cout << "Could not open the filetracking file: " << vFileT << endl;
          return;
        }
      pIn >> tmpString >> run >> nEvents >> tmpString;
      pIn.close();

      //copy background DST and change name
      cout << "Copying background DST to this directory..." << endl;
      std::string masterVertexFile = vertexFile;
      gSystem->Exec(Form("cp %s ./",masterVertexFile));
      const size_t last_slash = vertexFile.find_last_of("\/");
      if (std::string::npos != last_slash) vertexFile.erase(0, last_slash+1);
	
      std::string fileTracking = outputDST+"_filetracking.txt";
      ofstream out;
      out.open(fileTracking.c_str());
      out << masterVertexFile << "  " << run << "  " <<  nEvents << "  " << pisaFile << "  " << pythiaFile << endl;
      out.close();
    }
  else if (pisaFile.empty() || pythiaFile.empty() || vertexFile.empty())
    {
      cout << "Must provide either a pisaFile with filetracking or provide all three files as input!" << endl;
      return;
    }

  // load libraries
  gSystem->Load("libfun4all.so");
  gSystem->Load("libfun4allfuncs.so");
  gSystem->Load("libmutoo_subsysreco.so");
  gSystem->Load("libfvtx_subsysreco.so");
  gSystem->Load("libMWG_interface.so");
  gSystem->Load("libPHGeant" );
  gSystem->Load("libsimreco_base");
  gSystem->Load("libmuon_subsysreco");
  gSystem->Load("libfun4allfuncs_muons");
  gSystem->Load("libfvtx_subsysreco");
  gSystem->Load("libsvxcgl");
  gSystem->Load("libsvx");
  gSystem->Load("liblvl2");
  gSystem->Load("libPythia6.so");
  gSystem->Load("libPHPythia.so");
  gSystem->Load("libPHPythiaEventGen.so");
  gSystem->Load("/direct/phenix+scratch/snowball/SpinAnalysis/firstBuild/picoDST_object/install/lib/libpicodst_object.so");

  cerr << ">>>> libraries loaded" << endl;
  
  //TMutDatabaseCntrl::set_database_access( "use_local_global_align_file", true );
  //TMutDatabaseCntrl::set_filename( "use_local_global_align_file", "mut.globalAligConsts.dat" );
  // disable_HV determines whether wires are disabled, use_local_dead_HV_file determines
  // whether you use the db or local file:
  TMutDatabaseCntrl::set_database_access("disable_HV",true);             //should always be true
  TMutDatabaseCntrl::set_database_access("use_local_dead_HV_file",true); //db or not

  //TMutDatabaseCntrl::set_database_access("use_local_landau_parameters_file", true );
  //TMutDatabaseCntrl::set_filename("use_local_landau_parameters_file",
  //"landau_parameters.txt");
  TMutDatabaseCntrl::set_database_access("attenuated_channels",true);
  TMutDatabaseCntrl::set_database_access("dead_channels",true);


  ///////////////////////////////////////////
  // recoConsts setup
  //////////////////////////////////////////
  recoConsts *rc = recoConsts::instance();
  rc->set_IntFlag("SVXACTIVE", 1);
  rc->set_IntFlag("PRINT_MUTOO_PARAMETERS",1);
  rc->set_IntFlag("RUNNUMBER", run);
  rc->set_IntFlag("MUONFUN4SIM",1);
  rc->set_IntFlag("SIMULATIONFLAG",2);
  rc->set_IntFlag("EMBEDFLAG",0);

   // Read the FVTX dead channel map and geometry from the database:
   TFvtxGlobalParCntrl::set_bool_par("is_sim",true);
   TFvtxGlobalParCntrl::set_bool_par("deadmap_use_calibration_database", true); 
   TFvtxGlobalParCntrl::set_bool_par("geom_use_calibration_database", false); 
   //TFvtxGlobalParCntrl::set_string_par("geom_root_file_path","/phenix/u/jinhuang/work/FVTX/miliped_work/millipede/");
   TFvtxGlobalParCntrl::set_string_par("geom_root_file_path","/direct/phenix+hhj/snowball/public/pisa/");
   TFvtxGlobalParCntrl::set_string_par("geom_root_file_name", "fvtx_geometry.root");
   
   TMutDatabaseCntrl::set_database_access("disable_HV",true);
   TMutDatabaseCntrl::set_database_access("use_local_dead_wire_file",false);
   TMutDatabaseCntrl::set_database_access("use_local_dead_HV_file",false);
   TMutDatabaseCntrl::set_database_access("attenuated_channels",true);
   TMutDatabaseCntrl::set_database_access("dead_channels",true);


  // mutoo vertex source configuration
  // this allows to print which vertex is used and its value
  //TMutExtVtx::get().set_vtx_source( TMutExtVtx::MC );
  //TMutExtVtx::get().set_smear_z( false );
  TMutExtVtx::get().set_verbosity( MUTOO::SOME );

  //mMfmMT::setMapFileFlag( mMfmMT::MAP_3D_PLUS_PLUS );
  //mMfmMT::setMapFileScale(1.0);
  //MuonUtil::set_check_mapfile_scale( false );
  TMutExtVtx::get().set_priorities( "SIM", 0 );
  //TMutExtVtx::get().set_priorities( "FVTX", 0 );
  //TMutExtVtx::get().set_priorities( "SVX_PRECISE", 1 );
  //TMutExtVtx::get().set_priorities( "BBC", 2 );



  ///////////////////////////////////////////
  // Make the Server
  //////////////////////////////////////////
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(verbosity);

  ///////////////////////////////////////////
  // Subsystems
  //////////////////////////////////////////

  // run header and trigger setting
  se->registerSubsystem( new HeadSimreco() );
  //    se->registerSubsystem( new TrigSimreco() );

  // vertex simulation
  // puts the vertex from the pisa header node into vtxOut object
  VtxSimreco* vtx_reco = new VtxSimreco();
  vtx_reco->SmearZ( false );
  vtx_reco->SmearY( false );
  vtx_reco->SmearX( false );
  vtx_reco->UseXY( true );
  vtx_reco->OverwriteBBC( false );
  vtx_reco->XVertexSigma( 0.002 ); // VTX resolution of 20 microns
  vtx_reco->YVertexSigma( 0.002 );
  vtx_reco->ZVertexSigma( 0.002 );
  se->registerSubsystem( vtx_reco );

  // Counter
  se->registerSubsystem( new MuonCounter() );

  // global detectors subsystem
  //se->registerSubsystem( new HeadReco() );
  //se->registerSubsystem( new TrigReco( ));
  //se->registerSubsystem( new BbcReco() );
  //se->registerSubsystem( new ZdcReco() );
  se->registerSubsystem( new VtxReco() );

//   // local level1 subsystem
//   TrigSelect *minBias = new TrigSelect("MB");
//   minBias->AddTrigger("BBCLL1");
//   se->registerSubsystem(minBias);

  // muon prdf unpacker
  MuonUnpackPisa *muon_unpack_pisa( new MuonUnpackPisa() );
  muon_unpack_pisa->Verbosity( MUTOO::SOME );
  muon_unpack_pisa->set_flag(MuonUnpackPRDF::SKIP_ZERO_SUPPRESSION,1);
  se->registerSubsystem( muon_unpack_pisa );
  // mutoo reconstruction
  se->registerSubsystem( new MuiooReco() );
  MuonDev *muon_dev = new MuonDev();
  muon_dev->set_flag(MuonDev::SKIP_VERTEX_FIT, 1);
 //Option to force the cluster fitting to use a particular method:
  //muon_dev->set_flags(MuonDev::FORCE_LOOKUP);
  se->registerSubsystem( muon_dev );

  // SVX reconstruction
  SvxParManager *svxpar = new SvxParManager();
  svxpar->Verbosity(verbosity);
  //svxpar->set_BeamCenter(0.1653,-0.1353);
  //svxpar->set_OffsetVtxToCnt(-0.124,-0.279,0.0);
  //svxpar->set_OffsetEastToWest(0.0451,0.0119,0.0030);
  //svxpar->set_BeamCenter(0.,0.);
  //svxpar->set_OffsetVtxToCnt(0.0, 0.0, 0.0);
  //svxpar->set_OffsetEastToWest(0.0, 0.0, 0.0);
  //svxpar->set_ReadGeoParFromFile(1);
  //svxpar->set_GeometryFileName("svxPISA.par.ideal");
  se->registerSubsystem(svxpar);

  SvxSimulator *svxsim = new SvxSimulator();
  svxsim->Verbosity(verbosity);
  se->registerSubsystem(svxsim);

  /*
  SvxDecode *svxdecode = new SvxDecode();
  svxdecode->Verbosity(verbosity);
  svxdecode->includePixel(true);
  svxdecode->includeStripixel(true);
  svxdecode->setAdcOffset(24);
  svxdecode->setAdcCutoff(-24);
  se->registerSubsystem(svxdecode);
  */

  SvxApplyHotDead *svxhotdead = new SvxApplyHotDead();
  svxhotdead->Verbosity(verbosity);
  se->registerSubsystem(svxhotdead);

  SvxReco *svxrec = new SvxReco();
  svxrec->Verbosity(verbosity);
  // svxrec->Load_ThresholdFile("threshold.h");
  svxrec->set_ThisIsSimulation();
  //SvxParManager::set_UseStripThresholdDatbase(true);
  svxrec->set_UseStripThresholdDatbase(true);
  //svxrec->Load_ThresholdFile("threshold_ideal.h");
  svxrec->set_StripixelAdcSumThreshold(0);
  se->registerSubsystem(svxrec);

  SvxPriVertexSeedFinder *svxvtxseedfinder = new SvxPriVertexSeedFinder();
  svxvtxseedfinder->Verbosity(verbosity);
  se->registerSubsystem(svxvtxseedfinder);

  SvxStandAloneReco *svxstandalone = new SvxStandAloneReco();
  svxstandalone->Verbosity(verbosity);
  svxstandalone->setVertexRecoFlag(2);
  se->registerSubsystem(svxstandalone);

  SvxPrimVertexFinder *svxprimvtxfinder = new SvxPrimVertexFinder();
  svxprimvtxfinder->Verbosity(verbosity);
  se->registerSubsystem(svxprimvtxfinder);

  // fvtx prdf unpacker
  if (use_fvtx){
    FvtxUnpackPisa *fvtx_unpack = new FvtxUnpackPisa();
    //fvtx_unpack->set_do_response( true );
    //fvtx_unpack->Verbosity(verbosity);
    se->registerSubsystem( fvtx_unpack );

    FvtxReco* fvtxreco = new FvtxReco();
    //fvtxreco->set_do_mutkalfiteval(true);         // Adds VTX hits to tracking
    //fvtxreco->run_after_burner(false);         // Adds VTX hits to tracking
    //fvtxreco->set_use_svx_cluster(true);     // Uses smeared PISA hits if false
    //fvtxreco->set_do_mutr_matching(false);     // Match FVTX-MuTr tracks and refit or not
    //fvtxreco->set_finder(3);                  // Use Hough track finder
    se->registerSubsystem(fvtxreco);


    // Perform FVTX-Mutr track matching and refit track:
    se->registerSubsystem( new FvtxRecoWithMut() );


    FvtxPrimVertex* fvtxprimvtx = new FvtxPrimVertex();
    fvtxprimvtx->set_source(FvtxPrimVertex::Tracks,FvtxPrimVertex::Segments);
    se->registerSubsystem(fvtxprimvtx);


  }

  se->registerSubsystem( new MuonAnaTuples() );
  se->registerSubsystem( new MuonEval() );

  if (use_fvtx)
    {
      //FvtxMCEval* fvtxeval = new FvtxMCEval();
      FvtxEval* fvtxeval = new FvtxEval("FvtxEval","fvtx_eval_pisa.root");
      se->registerSubsystem(fvtxeval);
      FvtxMCEval* fvtxeval_mc = new FvtxMCEval("FvtxMCEval","fvtx_mc_eval_pisa.root");
      se->registerSubsystem(fvtxeval_mc);
    }



  ///////////////////////////////////////////
  // IOManagers...
  ///////////////////////////////////////////

  // dst
  if( write_dst ) 
    {
      Fun4AllDstOutputManager *dstManager  = new Fun4AllDstOutputManager("DSTOUT",outputDST.c_str());
      
      dstManager->AddNode("Sync");
      dstManager->AddNode("RunHeader");
      dstManager->AddNode("EventHeader");
      dstManager->AddNode("PreviousEvent");
      dstManager->AddNode("TrigRunLvl1");
      dstManager->AddNode("VtxOut");
      dstManager->AddNode("BbcOut");
      //dstManager->AddNode("BbcRaw");
      dstManager->AddNode("ZdcOut");
      //dstManager->AddNode("ZdcRaw");
      
      dstManager->AddNode("TMCPrimary");
      dstManager->AddNode("PHPythiaHeader");
      dstManager->AddNode("PHPythia");
      
      dstManager->AddNode("TrigLvl1");
      //dstManager->AddNode("L2Decision");
      //dstManager->AddNode("Lvl2OutArray");
      
      // Muioo nodes
      dstManager->AddNode("TMuiHitO");
      dstManager->AddNode("TMuiMCHitO");
      
      // Mutoo nodes
      dstManager->AddNode("TMutHit");
      dstManager->AddNode("TMutMCHit");
      dstManager->AddNode("TMutMCTrk");
      
      dstManager->AddNode("PHMuoTracksOO");
      
      // FVTX nodes
      dstManager->AddNode("TFvtxHit");
      dstManager->AddNode("TFvtxMCHit");
      dstManager->AddNode("TFvtxPisaHit");
      //dstManager->AddNode("TFvtxCompactTrk");
      
      // SVX nodes
      dstManager->AddNode("McSingle");
      dstManager->AddNode("SvxPisaHit");
      dstManager->AddNode("SvxGhitList");
      dstManager->AddNode("SvxRawhitList");
      dstManager->AddNode("SvxGhitRawhitList");
      //dstManager->AddNode("PHCentralTrack");
      //dstManager->AddNode("SvxCentralTrackList");
      
      // From EVA node
      //dstManager->AddNode("header");
      //dstManager->AddNode("fkin");
      //dstManager->AddNode("primary");
      //dstManager->AddNode("pythia");
      
      // PHGlobal
      dstManager->AddNode("PHGlobal");
      dstManager->AddNode("PHGlobal_MUON");
      
      dstManager->AddNode("PHPythiaHeader");
      dstManager->AddNode("PHPythia");
      
      se->registerOutputManager(dstManager);
    }

  // picoDST
  if( write_pdst )
    {
      //      se->registerSubsystem( new MpcReco() );
      //      gSystem->Load("librxnp_subsysreco.so");
      //      se->registerSubsystem( new RxnpReco() );
      //      se->registerSubsystem( new RpSumXYReco() ); // recalibrator and rp doesn't work together!
      
      // global Reco
      se->registerSubsystem( new GlobalReco() );
      se->registerSubsystem( new GlobalReco_muons() );
      
      //add the DC based global evaluation module
      //se->registerSubsystem( new McEvalSimreco() );
      
      // MWG
      gSystem->Load("libMWGOO.so");
      PHInclusiveNanoCuts *MWGcuts = new MWGInclusiveNanoCutsv2();
      se->registerSubsystem(new MWGFvtxReco(MWGcuts));
      
      gSystem->Load("libpicodst_object.so");
      mFillSingleMuonContainer* msngl = new mFillSingleMuonContainer();
      msngl->set_bbcz_cut(100.0);
      msngl->set_is_sim(true);
      se->registerSubsystem(msngl);

      mFillMCSingleMuonFvtxContainer* msngl_mc = new mFillMCSingleMuonFvtxContainer();
      se->registerSubsystem(msngl_mc);
      
      
      if ( write_ndst )
	{
	  Fun4AllOutputManager *outndst = new Fun4AllDstOutputManager("Outndst",outputNDST.c_str());
	  outndst->AddNode("Sync");
	  outndst->AddNode("TrigLvl1");
	  outndst->AddNode("VtxOut");
	  outndst->AddNode("TFvtxCompactTrk");
	  outndst->AddNode("PHGlobal");
	  outndst->AddNode("PHPythiaHeader");
	  outndst->AddNode("PHPythia");
	  outndst->AddNode("PHMuoTracksOO");
	  se->registerOutputManager(outndst);
	}


      mFvtxMuTrAssociation* mfvtxmutr = new mFvtxMuTrAssociation();
      mfvtxmutr->set_nsigmas_fvtxmutr_match(9999.9);
      mfvtxmutr->set_chi2_fvtx_cut(999.9);
      mfvtxmutr->set_min_nhits_fvtx(2);
      se->registerSubsystem(mfvtxmutr);


      
      Fun4AllOutputManager *outsmu = new Fun4AllDstOutputManager("Outsmu",outputPDST.c_str());
      outsmu->AddNode("Sync");
      outsmu->AddNode("SingleMuonContainer");
      outsmu->AddNode("VtxOut");
      outsmu->AddNode("PHGlobal");
      outsmu->AddNode("TFvtxCompactTrk");
      outsmu->AddNode("PHPythiaHeader");
      outsmu->AddNode("PHPythia");
      outsmu->AddNode("MCSingleMuonFvtxContainer");
      outsmu->AddEventSelector("mFillSingleMuonContainer");
      se->registerOutputManager(outsmu);
      

    }
  
  
  Fun4AllPisaInputManager *inMan = new Fun4AllPisaInputManager("PisaIn");
  se->registerInputManager(inMan);
  se->fileopen(inMan->Name(),pisaFile.c_str());

  if ( read_pythia )
    {
      cout << "Copying pythia file to this directory..." << endl;
      std::string masterPythiaFile = pythiaFile;
      gSystem->Exec(Form("cp %s ./",masterPythiaFile));
      const size_t last_slashp = pythiaFile.find_last_of("\/");
      if (std::string::npos != last_slashp) pythiaFile.erase(0, last_slashp+1);

      Fun4AllDstInputManager *ipythia = new Fun4AllNoSyncDstInputManager("DSTin2","DST");
      se->registerInputManager(ipythia);
      se->fileopen(ipythia->Name(),pythiaFile.c_str());
    }

  se->run(nEvents);
  se->End();

  cout << "Completed reconstruction." << endl;
}
