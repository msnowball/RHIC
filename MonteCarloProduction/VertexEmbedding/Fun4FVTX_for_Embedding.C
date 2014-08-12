void Fun4FVTX_for_Embedding(
int nEvents = 0,
int nEventsToSkip = 0,
int runnumber = 372412,
char *input_file = "PRDFF",
char *dst_file = "dst_out.root",
char *event_file = "event_file.txt"
)

{
  // load libraries
  gSystem->Load("libfun4all");
  gSystem->Load("libmutoo_subsysreco" );
  gSystem->Load("libfun4allfuncs_muons");
  gSystem->Load("liblvl2");
  gSystem->Load("libfvtx_subsysreco.so");
  gSystem->Load("librecal.so");

  Fun4AllServer *se = Fun4AllServer::instance();
  se->Verbosity(0);

  se->registerSubsystem( new SyncReco() );

  ///////////////////////////////////////////
  // recoConsts setup
  //////////////////////////////////////////
  recoConsts *rc = recoConsts::instance();
  //  rc->set_IntFlag("RUNNUMBER", runnumber);
  rc->set_IntFlag("SVXACTIVE", 1);

  //  TMutExtVtx::get().set_verbosity( MUTOO::SOME );

  se->registerSubsystem( new HeadReco() );
  se->registerSubsystem( new TrigReco() );
  se->registerSubsystem( new PreviousEventReco() );

  //TrigSelect *minbias = new TrigSelect("MB");
  //minbias->AddTrigger("BBCLL1(>1 tubes)");
  //minbias->AddTrigger("BBCLL1(>1 tubes) CopyA");
  //minbias->AddTrigger("BBCLL1(>1 tubes) CopyB");
  //minbias->AddTrigger("BBCLL1(>1 tubes) CopyC");
  //minbias->AddTrigger("BBCLL1(>1 tubes) narrowvtx");
  //minbias->AddTrigger("BBCLL1(>0 tubes) narrowvtx");
  //minbias->AddTrigger("BBCLL1(>1 tubes) narrowvtx CopyA");
  //minbias->AddTrigger("BBCLL1(>1 tubes) narrowvtx CopyB");
  //minbias->AddTrigger("BBCLL1(>1 tubes) narrowvtx CopyC");
  //minbias->AddTrigger("BBCLL1(>1 tubes) novertex");
  //se->registerSubsystem( minbias );

  TrigSelect *signalTrig = new TrigSelect("Sig");
  signalTrig->AddTrigger("MUIDLL1_N1D");
  signalTrig->AddTrigger("MUIDLL1_S1D");
  //signalTrig->AddTrigger("BBCLL1(noVtx)");
  se->registerSubsystem( signalTrig );


  //set BBC-vtx resolution
  BbcReco *bbc_reco = new BbcReco();
  bbc_reco->setBbcVtxError(0.5);
  se->registerSubsystem( bbc_reco ); 

  se->registerSubsystem( new ZdcReco() );
  se->registerSubsystem( new T0Reco() );
  se->registerSubsystem( new VtxReco() );

  MuonCounter *counter = new MuonCounter();
  counter->set_event_dump(1000);
  se->registerSubsystem(  counter );

 /////////////// SVX reconstruction ///////////////////
  SvxParManager *svxpar = new SvxParManager();
  svxpar->Verbosity(0);
  /*
  svxpar->set_BeamCenter(0.,0.);
  svxpar->set_OffsetVtxToCnt(0.0, 0.0, 0.0);
  svxpar->set_OffsetEastToWest(0.0, 0.0, 0.0);
  svxpar->set_ReadGeoParFromFile(false);
  svxpar->set_GeometryFileName("svxPISA.par.ideal");
  */
  se->registerSubsystem(svxpar);

  SvxDecode *svxdecode = new SvxDecode();
  svxdecode->Verbosity(0);
  svxdecode->includePixel(true);
  svxdecode->includeStripixel(true);
  svxdecode->setAdcOffset(24);
  svxdecode->setAdcCutoff(-24);
  se->registerSubsystem(svxdecode);

  SvxApplyHotDead *svxhotdead = new SvxApplyHotDead();
  svxhotdead->Verbosity(0);
  se->registerSubsystem(svxhotdead);

  SvxReco *svxrec = new SvxReco();
  svxrec->Verbosity(0);
  // svxrec->Load_ThresholdFile("threshold.h");
  svxrec->set_UseStripThresholdDatbase(true);
  svxrec->set_StripixelAdcSumThreshold(0);
  se->registerSubsystem(svxrec);

  SvxPriVertexSeedFinder *svxvtxseedfinder = new SvxPriVertexSeedFinder();
  svxvtxseedfinder->Verbosity(0);
  se->registerSubsystem(svxvtxseedfinder);

  SvxPriVertexSeedFinder *vtxseedw = new SvxPriVertexSeedFinder("SVXPRIVERTEXFINDERW",1);
  se->registerSubsystem(vtxseedw);

  SvxPriVertexSeedFinder *vtxseede = new SvxPriVertexSeedFinder("SVXPRIVERTEXFINDERE",2);
  se->registerSubsystem(vtxseede);

  SvxStandAloneReco *svxstandalone = new SvxStandAloneReco();
  svxstandalone->Verbosity(0);
  svxstandalone->setVertexRecoFlag(2);
  svxstandalone->setWindowScale(10);
  se->registerSubsystem(svxstandalone);

  SvxPrimVertexFinder *svxprimvtxfinder = new SvxPrimVertexFinder();
  svxprimvtxfinder->Verbosity(0);
  se->registerSubsystem(svxprimvtxfinder);
  /////////////////////////////////

  se->registerSubsystem( new MuonUnpackPRDF() );

  se->registerSubsystem( new FvtxUnpackPRDF() );

  FvtxReco* fvtxreco = new FvtxReco();
  fvtxreco->set_do_mutr_matching(false); // matching will be performed after primary vertex determination
  se->registerSubsystem( fvtxreco );

  FvtxPrimVertex* fvtxprimvtx = new FvtxPrimVertex();
  fvtxprimvtx->set_fvtx_Rres(1.0); // changed from 0.5 by Mike McCumber request
  fvtxprimvtx->set_source(FvtxPrimVertex::Tracks);
  //  fvtxprimvtx->set_clustering(FvtxPrimVertex::Cesars); // change needed in order to obtain multiple vertex working
  fvtxprimvtx->set_fitter_active(true);
  se->registerSubsystem( fvtxprimvtx );

  // mutoo reconstruction
  se->registerSubsystem( new MuiooReco() );
  se->registerSubsystem( new MuonDev() );
  se->registerSubsystem( new FvtxRecoWithMut() );

  // rpc unpacker
  //  se->registerSubsystem( new RpcUnpackPRDF() );
  //  se->registerSubsystem( new RpcHodoUnpackPRDF() );
  // rpc reconstruction
  // se->registerSubsystem( new RpcReco() );

  se->registerSubsystem( new GlobalReco() );
  se->registerSubsystem( new GlobalReco_muons() );
  //  se->registerSubsystem( new RpcMuoReco() );

  // recalibrator (needed for centrality)
  gSystem->Load("librecal");
  MasterRecalibratorManager * recal = new MasterRecalibratorManager();
  se->registerSubsystem(recal);

  /// This is the module that produces the event file information with vertex and centrality for each event
  MuonAnaTuples* ana_tuple = new MuonAnaTuples( "MUONANATUPLES");
  se->registerSubsystem( ana_tuple );
  ana_tuple->set_event_filename( event_file );
  ana_tuple->set_species_pp(true); // use it only for p+p embedding
  ana_tuple->set_vertex_name("FVTX");   // or SVX_PRECISE or BBC
  ana_tuple->set_flags( MuonAnaTuples::VERTEX | MuonAnaTuples::CENTRALITY );// | MuonAnaTuples::REACTION_PLANE);

  Fun4AllDstOutputManager *dstManager  = new Fun4AllDstOutputManager("DSTOUT",  dst_file);
  dstManager->AddNode("Sync");
  dstManager->AddNode("RunHeader");
  dstManager->AddNode("EventHeader");
  //dstManager->AddNode("PreviousEvent");
  dstManager->AddNode("VtxOut");
  dstManager->AddNode("BbcOut");
  dstManager->AddNode("ZdcOut");
  //dstManager->AddNode("BbcRaw");
  //dstManager->AddNode("ZdcRaw");
  dstManager->AddNode("TrigLvl1");
  //dstManager->AddNode("TrigRunLvl1");
  dstManager->AddNode("PHGlobal");
  //dstManager->AddNode("PHGlobal_MUON");

  // Fvtx nodes
  dstManager->AddNode("TFvtxHit");
  dstManager->AddNode("TFvtxTrk");

  // SVX nodes
  //dstManager->AddNode("SvxRawhitList");
  //dstManager->AddNode("SvxCluster");
  //dstManager->AddNode("PHCentralTrack");
      
  // Muioo nodes
  dstManager->AddNode("TMuiHitO");
  //dstManager->AddNode("TMuiRoadO");
  //dstManager->AddNode("TMuiClusterO");
  //dstManager->AddNode("TMui1DRoadO");
      
  // Mutoo nodes
  dstManager->AddNode("TMutHit");
  dstManager->AddNode("TMutMuiRoad");
  dstManager->AddNode("TMutClus");
  dstManager->AddNode("TMutCoord");
  dstManager->AddNode("TMutGapCoord");
  //dstManager->AddNode("TMutStub");
  //dstManager->AddNode("TMutTrk");
  //dstManager->AddNode("TMutVtx");
  se->registerOutputManager(dstManager);

  Fun4AllInputManager *in = new Fun4AllPrdfInputManager("PRDFin");
  in->fileopen(input_file);
  //  in->AddListFile(input_file);
  se->registerInputManager(in);
  se->skip(nEventsToSkip);
  se->run(nEvents);

  se->End();

 int evts = se->PrdfEvents();                                                                                                           
 std::cout << "Total Events:  " << evts << std::endl;                                                                                   
 se->End();

 MuonUtil::dump_process_time();
 signalTrig->Print("Summary");



  cout << "Completed reconstruction." << endl;
}
