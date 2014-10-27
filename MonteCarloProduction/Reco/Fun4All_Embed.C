void Fun4All_Embed(
		   int nEvents = 0,
		   std::string signalFile = "",
		   std::string backgroundFile = "",
		   std::string outputName = ""
		   )
{

  if(outputName.empty())
    {
      cout << "outputName is empty! Exiting..." << endl;
      return;
    }

  std::string outputDST = outputName+"_dst.root";
  std::string outputNDST = outputName+"_ndst.root";
  std::string outputPDST = outputName+"_pdst.root";
  std::string outputPRDF = outputName+".PRDFF";

  int run = 0;
  if (backgroundFile.empty() && nEvents == 0 && !signalFile.empty())
    {
      std::string tmpString, pythiaFile;
      std::string sigFile = signalFile+"_filetracking.txt";
      ifstream sigIn;
      sigIn.open(sigFile.c_str());
      if(!sigIn)
        {
          cout << "Could not open the filetracking file: " << sigFile << endl;
          return;
        }
      sigIn >> backgroundFile >> run >> nEvents >> tmpString >> pythiaFile;
      sigIn.close();

      std::string fileTracking = outputPDST+"_filetracking.txt";
      ofstream out;
      out.open(fileTracking.c_str());
      out << backgroundFile << "  " << run << "  " <<  nEvents << "  " << signalFile << "  " << pythiaFile << endl;
      out.close();

      //copy background DST and change name
      cout << "Checking if background DST exists locally..." << endl;
      std::string masterVertexFile = backgroundFile;
      const size_t last_slash = backgroundFile.find_last_of("\/");
      if (std::string::npos != last_slash) backgroundFile.erase(0, last_slash+1);
      ifstream file(backgroundFile.c_str(), std::ios::binary );
      if( !file )
	{
	  cout << "Background DST does not exist locally, copying..." << endl;
	  gSystem->Exec(Form("cp %s ./",masterVertexFile));
	}

      /*
      //copy pythia file and change name
      cout << "Checking if pythia file exists locally..." << endl;
      std::string masterPythiaFile = pythiaFile;
      const size_t last_slashp = pythiaFile.find_last_of("\/");
      if (std::string::npos != last_slashp) pythiaFile.erase(0, last_slashp+1);
      ifstream filep(pythiaFile.c_str(), std::ios::binary );
      if( !filep )
	{
	  cout << "Pythia file does not exist locally, copying..." << endl;
	  gSystem->Exec(Form("cp %s ./",masterPythiaFile));
	}
      */
    }
  else {
    cout << "Please pass a signal file!" << endl;
    return;
  }

  cout << ">>>> Signal File: " << signalFile << endl
       << ">>>> Background File: " << backgroundFile << endl
       << ">>>> Events to Run: " << nEvents << endl
       << ">>>> Run Number: " << run << endl;


  ////////////////////////
  bool is_pp = true;
  bool is_sim = false; 
  bool use_svx = true;
  bool use_FVTX = true;
  bool FVTX_fastsim = false;   // If true, project muon hits to FVTX and create hits
  bool FVTX_mcreco = false;    // Take MC hits, optionally smear, make tracks and fit
  bool FVTX_realreco = true;   // Take FVTX clusters, make coords and tracks and fit
  bool use_RPC = false;
  bool write_dst = false;
  bool write_ndst = false;
  bool write_pdst = true;
  bool write_merged_prdf = false;

  bool do_evaluation = true;
  bool fill_mutr_mc_info = true;
  bool use_perfect_detector = false;

  bool debug = false;
  int verbosity = 0;
  if(debug) verbosity = 5;
  ////////////////////////

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
  gSystem->Load("libPythia6.so");
  gSystem->Load("libPHPythia.so");
  gSystem->Load("libPHPythiaEventGen.so");
  gSystem->Load("/direct/phenix+scratch/snowball/SpinAnalysis/firstBuild/picoDST_object/install/lib/libpicodst_object.so");

  ///////////////////////////////////////////
  // recoConsts setup
  //////////////////////////////////////////
  recoConsts *rc = recoConsts::instance();
  rc->set_IntFlag("SVXACTIVE", 1);
  rc->set_IntFlag("RUNNUMBER", run);

  // Muon ext vtx verbosity
  // this allows to print which vertex is used and its value
  unsigned int use_vertex;
  if (is_sim){
    use_vertex = MuonUnpackSim::SIGNAL; // "SIGNAL", "BACKGROUND" or "RECO"
  }
  else{
    use_vertex = MuonUnpackSim::RECO; // "SIGNAL", "BACKGROUND" or "RECO"
    //use_vertex = MuonUnpackSim::SIGNAL; // "SIGNAL", "BACKGROUND" or "RECO"
  }

  TMutExtVtx::get().set_verbosity( MUTOO::ALOT );
  if (use_vertex == MuonUnpackSim::SIGNAL){
    TMutExtVtx::get().set_priorities( "SIM", 0 );
  }
  else{
    TMutExtVtx::get().set_priorities( "FVTX", 0 );
    TMutExtVtx::get().set_priorities( "SVX_PRECISE", 1 );
    TMutExtVtx::get().set_priorities( "BBC", 2 );
  }

  // Read the FVTX dead channel map and geometry from the database:
  // changes track finding parameters and vertex weight used in fvtx-mutr match
  TFvtxGlobalParCntrl::set_bool_par("is_pp",is_pp);   
  TFvtxGlobalParCntrl::set_bool_par("use_svx",use_svx);   

  // Determine what DB parameters to read:
  TFvtxGlobalParCntrl::set_bool_par("is_sim",is_sim);
  TFvtxGlobalParCntrl::set_bool_par("deadmap_use_calibration_database", true);
  TFvtxGlobalParCntrl::set_bool_par("geom_use_calibration_database", false);
  //TFvtxGlobalParCntrl::set_string_par("geom_root_file_path","/phenix/u/jinhuang/work/FVTX/miliped_work/millipede/");
  //TFvtxGlobalParCntrl::set_string_par("geom_root_file_name", "fvtx_geometry.root");
  TFvtxGlobalParCntrl::set_string_par("geom_root_file_path","/direct/phenix+hhj/snowball/public/pisa/");
  TFvtxGlobalParCntrl::set_string_par("geom_root_file_name", "fvtx_geometry.root");
  //}
  //else{
  // TFvtxGlobalParCntrl::set_string_par("geom_root_file_path",
  //     "/phenix/u/workarea/brooks/simulation/pisa2000/wrk/");
  // TFvtxGlobalParCntrl::set_string_par("geom_root_file_name", "fvtxgeom_v7.root");
  //}

  //TFvtxGlobalParCntrl::set_bool_par("deadmap_auto_load", false); // don't auto load again
  //TFvtxDeadMap map;
  //map.dbGetAll(run_number, /*bool is_sim*/false); // fetch the map from database
  //map.apply_dead_maps();  // apply dead map immediately

  // database control initialization
  if( use_perfect_detector )
  {

    // tell muid tube efficiency must be set to 1
    rc->set_DoubleFlag("MUIOO_TUBE_EFF",1.0);
    // make perfect mutr detector
    TMutDatabaseCntrl::set_database_access("disable_HV",false);
    TMutDatabaseCntrl::set_database_access("attenuated_channels",false);
    TMutDatabaseCntrl::set_database_access("dead_channels",false);

  }
  else {
    /*
      to have muid inneficiencies turned on you need to have local copies of
      tube_eff_north_default.txt
      tube_eff_south_default.txt
    */

    /*
      to have mutr HV mask turned on, you need to have the following set to true
      and a local copy of mut.disabledAnodes.dat
    */
    TMutDatabaseCntrl::set_database_access("disable_HV",true);             //should always be true
    TMutDatabaseCntrl::set_database_access("use_local_dead_HV_file",true); // db access or not

    // this tells that the mutr dead wires should be read from the database
    TMutDatabaseCntrl::set_database_access("use_local_dead_wire_file",false);

    // this enable use of attenuateed/dead channels from database
    TMutDatabaseCntrl::set_database_access("attenuated_channels",true);
    TMutDatabaseCntrl::set_database_access("dead_channels",true);

  }

  TMutDatabaseCntrl::set_verbosity( TMutDatabaseCntrl::GLOBAL_ALIGN, TMutDatabaseCntrl::NONE );
  TMutDatabaseCntrl::set_verbosity( TMutDatabaseCntrl::INTERNAL_ALIGN, TMutDatabaseCntrl::NONE );
  TMutDatabaseCntrl::print();

  //mMfmMT::setMapFileFlag(4);
  //mMfmMT::setMapFileScale(1.0);
  //MuonUtil::set_check_mapfile_scale( false );
  //TMutChargeCorrection::set_do_correction( false );

  ///////////////////////////////////////////
  // Make the Server
  //////////////////////////////////////////
  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(verbosity);

  ///////////////////////////////////////////
  // Subsystems
  //////////////////////////////////////////

  // counter
  se->registerSubsystem( new MuonCounter() );

  // this part should no be necessary unless you run on an 'old' simulated DST
  // (for which VtxSimreco did not run in Fun4Muons_PISA
  bool use_vtx_sim_reco = false;
  if( use_vtx_sim_reco )
  {
    gSystem->Load("libsimreco_base");
    VtxSimreco* vtx_reco = new VtxSimreco();
    vtx_reco->SmearZ( false );
    vtx_reco->SmearY( false );
    vtx_reco->SmearX( false );
    vtx_reco->UseXY( true );
    vtx_reco->OverwriteBBC( false );
    //vtx_reco->OverwriteBBC( false );
    vtx_reco->XVertexSigma( 0.002 );
    vtx_reco->YVertexSigma( 0.002 );
    vtx_reco->ZVertexSigma( 0.002 );
    se->registerSubsystem( vtx_reco );
  }

  // this runs the SVX reconstruction (stand-alone)
  if( use_svx )
  {
    // VTX Reconstruction software:
    gSystem->Load("libsimreco");
    gSystem->Load("libembedreco");
    gSystem->Load("libKalFit");

    SvxParManager *svxpar = new SvxParManager();
    svxpar->Verbosity(verbosity);
    //svxpar->set_BeamCenter(0.,0.);
    //svxpar->set_OffsetVtxToCnt(0.0, 0.0, 0.0);
    //svxpar->set_OffsetEastToWest(0.0, 0.0, 0.0);
    //svxpar->set_BeamCenter(0.1653,-0.1353);
    //svxpar->set_OffsetVtxToCnt(-0.124,-0.279,0.0);
    //svxpar->set_OffsetEastToWest(0.0451,0.0119,0.0030);
    //svxpar->set_ReadGeoParFromFile(1);
    //svxpar->set_GeometryFileName("svxPISA.par.ideal");
    se->registerSubsystem(svxpar);

    
    if (is_sim)
      { 
	SvxSimulator *svxsim = new SvxSimulator();
	svxsim->Verbosity(verbosity);
	se->registerSubsystem(svxsim);
      }
    
    if ( !backgroundFile.empty() )
      {
        SvxEmbedSimhit *svxembed = new SvxEmbedSimhit();
    	svxembed->set_StripixelNoise(0.0); // no noise
    	svxembed->Verbosity(verbosity);
    	se->registerSubsystem(svxembed);
      }

    /*
    if ( backgroundfile )
      {
        SvxMergeRawHits* svxmerge = new SvxMergeRawHits();
        //svxmerge->set_mergefilename("/phenix/subsys/vtx/singles/svxhits/svxhits_347141_MB.dat");
        svxmerge->set_mergefilename(backgroundfile);
        svxmerge->Verbosity(verbosity);
        se->registerSubsystem(svxmerge);
      }
    */

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
    //svxrec->set_ThisIsSimulation();
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

  }

  // RPC unpacking/reconstruction
  if( use_RPC )
  {
    gSystem->Load("librpc_subsysreco");
    if( !backgroundFile.empty() ) se->registerSubsystem( new RpcUnpackSim( "RPCUNPACKSIM", RpcUnpackSim::MC_SIGNAL_REAL_BG ) );
    //if( backgroundFile ) se->registerSubsystem( new RpcUnpackSim( "RPCUNPACKSIM", RpcUnpackSim::MC_SIGNAL_MC_BG ) );
    else  se->registerSubsystem( new RpcUnpackSim( "RPCUNPACKSIM", RpcUnpackSim::MC_SIGNAL_NO_BG ) );
    se->registerSubsystem( new RpcReco() );
  }

  if( use_FVTX )
  {
    if( FVTX_fastsim )
    {
      se->registerSubsystem( new FvtxFastSim() );
    } 
    else if( FVTX_mcreco ) 
      {
	
	FvtxUnpackSim* fvtx_unpack = new FvtxUnpackSim();
	if( !backgroundFile.empty() ) fvtx_unpack->SetMode( FvtxUnpackSim::MC_SIGNAL_MC_BG );
	else fvtx_unpack->SetMode( FvtxUnpackSim::MC_SIGNAL_NO_BG );
	se->registerSubsystem( fvtx_unpack );
	
	se->registerSubsystem( new FvtxRecoMC() );
	
      } 
    else if( FVTX_realreco ) 
      {
	
	FvtxUnpackSim* fvtx_unpack = new FvtxUnpackSim();
	// Treat even MC backgrounds as "real" backgrounds because there isn't a utility
	// to merge MC information. In this case, only the "signal" MC information will
	// show up in the evaluation ntuples:
	//if( backgroundFile ) fvtx_unpack->SetMode( FvtxUnpackSim::MC_SIGNAL_MC_BG );
	if( !backgroundFile.empty() ) fvtx_unpack->SetMode( FvtxUnpackSim::MC_SIGNAL_REAL_BG );
	else fvtx_unpack->SetMode( FvtxUnpackSim::MC_SIGNAL_NO_BG );
	se->registerSubsystem( fvtx_unpack );
	
	FvtxReco *fvtx_reco = new FvtxReco();
	//fvtx_reco->run_after_burner(false);            // Adds VTX hits to tracking
	//fvtx_reco->set_use_svx_cluster(true);          // Uses smeared PISA hits if false
	fvtx_reco->set_do_mutr_matching(false);         // Match FVTX-MuTr tracks and refit or not
	fvtx_reco->set_finder(3);                       // Use Hough track finder
	
	se->registerSubsystem( fvtx_reco );
	
	//TMutNode<mFvtxKalFitPar>::find_node(se->topNode(),"mFvtxKalFitPar")->set_do_evaluation(true);
	TMutNode<mFvtxKalFitPar>::find_node(se->topNode(),"mFvtxKalFitPar")->set_vtx_phi_error(0.005);
	TMutNode<mFvtxKalFitPar>::find_node(se->topNode(),"mFvtxKalFitPar")->set_vtx_r_error(0.015);
	TMutNode<mFvtxKalFitPar>::find_node(se->topNode(),"mFvtxKalFitPar")->set_vtx_z_error(0.012);
	//TMutNode<mMutKalFitWithSiliRealPar>::find_node(se->topNode(),"mMutKalFitWithSiliRealPar")->set_do_evaluation(true);
	TMutNode<mMutKalFitWithSiliRealPar>::find_node(se->topNode(),"mMutKalFitWithSiliRealPar")->set_vtx_phi_error(0.005);
	TMutNode<mMutKalFitWithSiliRealPar>::find_node(se->topNode(),"mMutKalFitWithSiliRealPar")->set_vtx_r_error(0.015);
	TMutNode<mMutKalFitWithSiliRealPar>::find_node(se->topNode(),"mMutKalFitWithSiliRealPar")->set_vtx_z_error(0.012);
	//TMutNode<mFvtxKalFitPar>::find_node(se->topNode(),"mFvtxKalFitPar")->set_vtx_phi_error(1.005);
	//TMutNode<mFvtxKalFitPar>::find_node(se->topNode(),"mFvtxKalFitPar")->set_vtx_r_error(1.015);
	//TMutNode<mFvtxKalFitPar>::find_node(se->topNode(),"mFvtxKalFitPar")->set_vtx_z_error(1.012);
	
	// Set the vertex sigma that is use in FVTX-MuTr track matching:
	if (is_pp)
	  {
	    TMutNode<mMutKalFitWithSiliRealPar>::find_node(se->topNode(),"mMutKalFitWithSiliRealPar")->set_vtx_sigma_z(25.0);
	    TMutNode<mMutKalFitWithSiliRealPar>::find_node(se->topNode(),"mMutKalFitWithSiliRealPar")->set_vtx_sigma_xy(10.0);
	  }
	else
	  {
	    TMutNode<mMutKalFitWithSiliRealPar>::find_node(se->topNode(),"mMutKalFitWithSiliRealPar")->set_vtx_sigma_z(1.0);
	    TMutNode<mMutKalFitWithSiliRealPar>::find_node(se->topNode(),"mMutKalFitWithSiliRealPar")->set_vtx_sigma_xy(0.5);
	  }
	
	
	// Mutr/Muid unpacking
	MuonUnpackSim* muon_unpack = new MuonUnpackSim();
	// Treat even MC backgrounds as "real" backgrounds because there isn't a utility
	// to merge MC information. In this case, only the "signal" MC information will
	// show up in the evaluation ntuples:
	if( !backgroundFile.empty() ) muon_unpack->SetMode( MuonUnpackSim::MC_SIGNAL_REAL_BG );
	//if( backgroundFile ) muon_unpack->SetMode( MuonUnpackSim::MC_SIGNAL_MC_BG );
	else muon_unpack->SetMode( MuonUnpackSim::MC_SIGNAL_NO_BG );
	muon_unpack->SetVtxToUse( use_vertex );
	//muon_unpack->set_flag( MuonUnpackSim::NO_CHARGE_SMEAR, true );
	se->registerSubsystem( muon_unpack );
	

	// primary vertex from FVTX
	if ( use_vertex == MuonUnpackSim::RECO )
	  {
	    FvtxPrimVertex* fvtxprimvtx = new FvtxPrimVertex();
	    fvtxprimvtx->set_fvtx_Rres(1.0);  // changed to 1cm, recomended by Mike
	    //fvtxprimvtx->set_source(FvtxPrimVertex::Tracks);
	    //fvtxprimvtx->set_fitter_active(true);
	    //fvtxprimvtx->set_clustering(FvtxPrimVertex::AllInOne); // AllInOne for single vertex, Cesars for multiple verteces
	    //fvtxprimvtx->set_bbcz_window(100.0); // FVTX-BBC vertex matching window
	    fvtxprimvtx->set_source(FvtxPrimVertex::Tracks,FvtxPrimVertex::Segments);
	    // Not clear this has an effect for simulated events:
	    //fvtxprimvtx->set_mutr_use_fvtx( false );  // Do not use FVTX as primary vertex
	    //fvtxprimvtx->set_mutr_use_fvtx( true );  // Do not use FVTX as primary vertex
	    se->registerSubsystem(fvtxprimvtx);
	  }
	
      }
    
    // Mutr/Muid reconstruction
    // Note:  MuonDev must be called before FvtxRecoWithMut
    
    MuiooReco* muioo_reco = new MuiooReco();
    // Larger cut values needed if you are simulating CuAu data sets:
    //muioo_reco->set_asymm_cut_par(100);
    //muioo_reco->set_max_occupancy_per_arm(400);
    se->registerSubsystem( muioo_reco );

    MuonDev* muon_dev = new MuonDev();
    muon_dev->set_flag(MuonDev::SKIP_VERTEX_FIT, 1);
    se->registerSubsystem( muon_dev );

    // Perform FVTX-Mutr track matching and refit track:
    FvtxRecoWithMut *fvtx_reco_withmut = new FvtxRecoWithMut();
    se->registerSubsystem( fvtx_reco_withmut );

  }

  // Fill FVTX evaluation ntuples:
  if( do_evaluation && use_FVTX) 
    {
      //se->registerSubsystem( new MuonEval() );
      FvtxEval* fvtxeval = new FvtxEval("FvtxEval","fvtx_eval_embed_pisa.root");
      se->registerSubsystem(fvtxeval);
      FvtxMCEval* fvtxeval_mc = new FvtxMCEval("FvtxMCEval","fvtx_mc_eval_embed_pisa.root");
      se->registerSubsystem(fvtxeval_mc);
      se->registerSubsystem( new MuonEval("MuonEval","mutoo_eval.root") );
    }
  
  /*
    global Reco must be run after the reconstruction
    but before the evalutations for all nodes/ntuples to be filled
    properly
  */
  se->registerSubsystem( new GlobalReco() );
  se->registerSubsystem( new GlobalReco_muons() );

  // prdf packer
  if( write_merged_prdf ) se->registerSubsystem( new MuonPackPRDF() );

  ///////////////////////////////////////////
  // Input manager
  ///////////////////////////////////////////
  rc->set_CharFlag("EMBED_MC_TOPNODE","SIGNAL");
  rc->set_CharFlag("EMBED_REAL_TOPNODE","BACKGROUND");
  Fun4AllInputManager *signal = new Fun4AllNoSyncDstInputManager( "SIGNAL_IM", "DST", "SIGNAL" );
  se->registerInputManager( signal );
  se->fileopen( signal->Name(), signalFile.c_str() );

  // try load simulated background, if requested
  if(!backgroundFile.empty() )
  {

    Fun4AllInputManager *background = new Fun4AllNoSyncDstInputManager( "BACKGROUND_IM", "DST", "BACKGROUND" );
    se->registerInputManager(background);
    se->fileopen( background->Name(), backgroundFile.c_str() );

  }

  if(debug)
    {
      FvtxPrinter *thePrinter = new FvtxPrinter("FVTXPRINTER");
      se->registerSubsystem(thePrinter);
      
      FvtxPrinter *thePrinter2 = new FvtxPrinter("FVTXPRINTER_SIGNAL");
      thePrinter2->SetNodeName("SIGNAL");
      se->registerSubsystem(thePrinter2);
      
      FvtxPrinter *thePrinter3 = new FvtxPrinter("FVTXPRINTER_BACKGROUND");
      thePrinter3->SetNodeName("BACKGROUND");
      if(!backgroundFile.empty())se->registerSubsystem(thePrinter3);
    }

  ///////////////////////////////////////////
  // Output manager
  ///////////////////////////////////////////

  // dst output manager
  if( write_dst )
  {
    Fun4AllDstOutputManager *dstManager  = new Fun4AllDstOutputManager("DSTOUT",outputDST.c_str());
    // Header and vertex nodes
    dstManager->AddNode("Sync");
    dstManager->AddNode("RunHeader");
    dstManager->AddNode("EventHeader");
    dstManager->AddNode("VtxOut");
    dstManager->AddNode("BbcOut");
    dstManager->AddNode("BbcRaw");
    dstManager->AddNode("ZdcOut");
    dstManager->AddNode("ZdcRaw");
    dstManager->AddNode("TrigLvl1");
    dstManager->AddNode("TMuiPseudoLL1");
    // muioo nodes
    dstManager->AddNode("TMCPrimary");
    dstManager->AddNode("TMuiMCHitO");
    dstManager->AddNode("TMuiHitO");
    dstManager->AddNode("TMuiClusterO");
    dstManager->AddNode("TMui1DRoadO");
    dstManager->AddNode("TMuiRoadO");
    dstManager->AddNode("TMuiPseudoBLTO");
    dstManager->AddNode("TMuiPseudoLL1");
    // mutoo nodes
    dstManager->AddNode("TMutMCHit");
    dstManager->AddNode("TMutMCTrk");
    dstManager->AddNode("TMutHit");
    dstManager->AddNode("TMutClus");
    dstManager->AddNode("TMutCoord");
    dstManager->AddNode("TMutGapCoord");
    dstManager->AddNode("TMutStub");
    dstManager->AddNode("TMutTrk");
    dstManager->AddNode("TMutVtx");
    // rpc nodes
    if( use_RPC )
      {
	dstManager->AddNode("TRpcMCHit");
	dstManager->AddNode("TRpcHit");
	dstManager->AddNode("TRpcClus");
	dstManager->AddNode("TRpcCoord");
	dstManager->AddNode("TRpcTrk");
      }
    // Fvtx nodes
    if( use_FVTX )
      {
	dstManager->AddNode( "TFvtxMCHit" );
	dstManager->AddNode( "TFvtxMCTrk" );
      }
    // From EVA node
    dstManager->AddNode("header");
    dstManager->AddNode("fkin");
    dstManager->AddNode("primary");
    dstManager->AddNode("pythia");

    se->registerOutputManager(dstManager);

  }

  // ndst output manager
  if( write_ndst )
  {
    gSystem->Load("libMWGOO");

    Fun4AllDstOutputManager *ndstManager = new Fun4AllDstOutputManager( "MWGOUT",outputNDST.c_str());
    ndstManager->AddNode("RunHeader");
    ndstManager->AddNode("EventHeader");
    ndstManager->AddNode("header");
    ndstManager->AddNode("BbcOut");
    ndstManager->AddNode("PHGlobal");
    ndstManager->AddNode("PHGlobal_MUON");
    ndstManager->AddNode("PHMuoTracksOO");
    ndstManager->AddNode("TrigLvl1");
    ndstManager->AddNode("TMuiPseudoBLTO");
    ndstManager->AddNode("TMuiPseudoLL1");
    ndstManager->AddNode("VtxOut");
    ndstManager->AddNode("RpSumXYObject");
    ndstManager->AddNode("ReactionPlaneObject");
    se->registerOutputManager(ndstManager);
  }

  // picodst output manager
  if( write_pdst )
  {
    if(!write_ndst) gSystem->Load("libMWGOO");
    gSystem->Load("libpicodst_object.so");

    // MWG
    PHInclusiveNanoCuts *MWGcuts = new MWGInclusiveNanoCutsv2();
    se->registerSubsystem(new MWGFvtxReco(MWGcuts));

    mFillSingleMuonContainer* msngl = new mFillSingleMuonContainer();
    msngl->set_bbcz_cut(100.0);
    //msngl->set_pz_cut(1.0);
    //msngl->set_lastgap_cut(2);
    //msngl->set_DG0_cut(30);
    //msngl->set_DDG0_cut(20);
    //msngl->set_chi2_cut(20);
    //msngl->set_nidhits_cut(5);//1);
    msngl->set_is_sim(is_sim);
    //msngl->set_nsigmas_fvtxmutr_match(999);
    //msngl->set_make_reassociation(true);
    se->registerSubsystem(msngl);
    
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
    outsmu->AddNode("PHPythiaHeader");
    outsmu->AddNode("PHPythia");
    outsmu->AddNode("TFvtxCompactTrk");
    //outsmu->AddNode("TMutTrk");
    outsmu->AddEventSelector("mFillSingleMuonContainer");
    //if (fill_mutr_mc_info) outsmu->AddNode("MCSingleMuonContainer"); // Mutr MC information container
    if (fill_mutr_mc_info) outsmu->AddNode("MCSingleMuonFvtxContainer"); // Mutr MC information container
    se->registerOutputManager(outsmu);
    
    if(fill_mutr_mc_info)
      {
	//********************* Mutr MC information Module ******************//
	//mFillMCSingleMuonContainer* msngl_mc = new mFillMCSingleMuonContainer();
	mFillMCSingleMuonFvtxContainer* msngl_mc = new mFillMCSingleMuonFvtxContainer();
	se->registerSubsystem(msngl_mc);
      }
    
  }

  // prdf output
  if(write_merged_prdf)
  {
    Fun4AllPrdfOutputManager *prdf_io  = new Fun4AllPrdfOutputManager("PRDFOUT",outputPRDF.c_str());
    prdf_io->InitPrdfNode( se->topNode() );
    se->registerOutputManager(prdf_io);
  }


  MuonUtil::dump_process_time();
  se->run(nEvents);
  se->End();
  MuonUtil::dump_process_time();
  cout << "Completed reconstruction." << endl;

}
