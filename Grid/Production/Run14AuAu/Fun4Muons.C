#include <string>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;


void Fun4Muons( int nEvents=100, int skip = 0, const char* inputfile="./EVENTDATA_P00-0000408999-0000.PRDFF")
{
  // Tell root to not to start signal handling but crash
  for (int i = 0; i < kMAXSIGNALS; i++)
    {
      gSystem->IgnoreSignal((ESignals)i);
    }

  gSystem->Exec("/bin/env");

  std::cout << "Processing file: " << inputfile << std::endl;

  char ifile[strlen(inputfile)+1]; // + 1 for the \0 which marks the end of string
  strcpy(ifile,inputfile);
  strtok(ifile,"-");
  int runnumber = atoi(strtok(0,"-"));
  int segnumber = atoi(strtok(strtok(0,"-"),"."));
  //cout << "runnumber: " <<   runnumber << " segment " << segnumber << endl;

  // Loading libraries
  gSystem->Load("libfun4all");
  gSystem->Load("libmutoo_subsysreco" );
  gSystem->Load("libfun4allfuncs_muons");
  gSystem->Load("liblvl2");
  gSystem->Load("libfvtx_subsysreco.so");
  gSystem->Load("librecal.so");
  gSystem->Load( "librpc_subsysreco" );
  gSystem->Load("libmutrg");
  gSystem->Load("libMWGOO");
  gSystem->Load("librecal");
  gSystem->Load("libSvxDstQA.so");

  //IO manager
  gROOT->ProcessLine(".L OutputManager.C");
  gROOT->ProcessLine(".L rawdatacheck.C");
  gROOT->ProcessLine(".L QA.C");
  gROOT->ProcessLine(".L TrigSelect.C");


  SetCvsTag();

  Fun4AllServer* se= Fun4AllServer::instance();
  se->Verbosity(0);

  // get pointer to raw data checker to pass to reco modules
  RawDataCheck *raw = rawdatacheck();

  // RecoConsts setup
  recoConsts *rc = recoConsts::instance();
  rc->set_IntFlag("PRINT_MUTOO_PARAMETERS",0);

  //************
  rc->set_IntFlag( "MUTOO_ERRORSTATS", 0 ); // to stop TMutErrorStats
  // use # of strips with hard-coded number
  MutrgPar::IS_READ_NSTRIP_DB = true;
  rc->set_IntFlag( "RpcGeomType", 3 );//NEEDED for run 12+ RPC geometry...
  rc->set_IntFlag("SVXACTIVE", true);
  rc->Print();

  TMutExtVtx::get().set_verbosity( MUTOO::NONE );

  // Print database readouts
  // May have to use local dead channel file .. don't know yet?
  TMutDatabaseCntrl::print();

  //muon timeserver instance
  PHTimeServer* timeserv = PHTimeServer::get();

 //Define and configure all subsystems
  HeadReco *head = new HeadReco();
  head->SetRawDataCheck(raw); // add the rawdatacheck pointer so a list of 
                              // bad packets is added to the EventHeader
  se->registerSubsystem(head);

  SubsysReco *sync = new SyncReco();
  se->registerSubsystem(sync);

  SubsysReco *trig    = new TrigReco();
  se->registerSubsystem(trig);

  SubsysReco *peve    = new PreviousEventReco();
  se->registerSubsystem(peve);

  BbcReco *bbc        = new BbcReco();
  bbc->setBbcVtxError( 0.5 );
  se->registerSubsystem(bbc);

  SubsysReco *zdc     = new ZdcReco();
  se->registerSubsystem(zdc);

  SubsysReco *t0      = new T0Reco();
  se->registerSubsystem(t0);

  SubsysReco *vtx     = new VtxReco();
  se->registerSubsystem(vtx);

  ////////////////////////////////// 
  // Accounting
  ////////////////////////////////// 
  SubsysReco *trigacc = new TriggerAccounting();
  se->registerSubsystem(trigacc);

  ///////////////////////////////////////////
  /// Trigger dicing now in loaded TrigSelect.C macro
  ///////////////////////////////////////////
  TrigSelect();

  SubsysReco *outacc = new OutputAccounting();
  se->registerSubsystem(outacc);

  SubsysReco *unpack = new MuonUnpackPRDF();
  se->registerSubsystem(unpack);

  // remove events with Z vertex > 40cm where tracks reach MuTr with no absorber
  MuonTrigFilter * filter_zvertex = new MuonTrigFilter("ZVERTEX",
  						       MuonTrigFilter::MUTHITS_ZVERTEX, MuonTrigFilter::ABORT_EVENT);
  filter_zvertex->set_z_vertex_window(-40, 40);
  se->registerSubsystem(filter_zvertex);

  SvxParManager *svxpar = new SvxParManager();
  se->registerSubsystem(svxpar);

  SvxDecode *svxdecode = new SvxDecode();
  svxdecode->includePixel(true);
  svxdecode->includeStripixel(true);
  svxdecode->setAdcOffset(24);
  svxdecode->setAdcCutoff(-24);
  se->registerSubsystem(svxdecode);
 
  se->registerSubsystem( new SvxApplyHotDead());
  
  SubsysReco *svxrec                       = new SvxReco();
  se->registerSubsystem(svxrec);

  SvxPriVertexSeedFinder *svxvtxseedfinder = new SvxPriVertexSeedFinder();
  se->registerSubsystem(svxvtxseedfinder);

  SvxStandAloneReco *svxstandalone         = new SvxStandAloneReco();
  svxstandalone->setVertexRecoFlag(2);
  //  svxstandalone->setPPFlag(true);
  se->registerSubsystem(svxstandalone);

  SvxPrimVertexFinder *svxprimvtxfinder    = new SvxPrimVertexFinder();
  se->registerSubsystem(svxprimvtxfinder);
    
  FvtxUnpackPRDF *Fvtx_unpack = new FvtxUnpackPRDF();
  se->registerSubsystem(Fvtx_unpack);

  //FvtxReco* fvtxreco = new FvtxReco();
  //se->registerSubsystem(fvtxreco);

  //FvtxPrimVertex* fvtxprimvtx = new FvtxPrimVertex();
  //se->registerSubsystem(fvtxprimvtx);
  
  MuiooReco *muioo  = new MuiooReco();
  muioo->set_max_occupancy_per_arm(1000);
  se->registerSubsystem(muioo);

  SubsysReco *mutoo  = new MuonDev();
  se->registerSubsystem(mutoo);

  // mutrg unpacker & reconstruction -- from Yoshi 
  MutrgReco* mutrgreco = new MutrgReco();
  mutrgreco->DoClustering(true,4);
  se->registerSubsystem(mutrgreco);

  TFvtxGlobalParCntrl::set_bool_par("quick_muon_reco", true);
  
  FvtxReco* fvtxreco = new FvtxReco();
  se->registerSubsystem(fvtxreco);

  FvtxPrimVertex* fvtxprimvtx = new FvtxPrimVertex();
  se->registerSubsystem(fvtxprimvtx);

  SubsysReco* fvtxmut = new FvtxRecoWithMut();
  se->registerSubsystem( fvtxmut );
  
  SubsysReco *global = new GlobalReco();
  se->registerSubsystem(global);

  SubsysReco *global_muons = new GlobalReco_muons();
  se->registerSubsystem(global_muons);

  SubsysReco *mwg = new MWGFvtxReco(new MWGInclusiveNanoCutsv2()); //muon nDST module
  se->registerSubsystem(mwg);

  MuidEffic *muid_effic = new MuidEffic();          // muid efficiency module
  muid_effic->set_filename(muid_eff_IOManager(runnumber,segnumber));
  se->registerSubsystem(muid_effic);

  rc->set_IntFlag("MPC_RECO_MODE",0x3);
  SubsysReco *mpc     = new MpcReco();
  se->registerSubsystem(mpc);

  SubsysReco *svxrp   = new SvxRpSumXYReco();
  se->registerSubsystem(svxrp);

  QAInit();

  SubsysReco* qafvtx = new QAFvtx();
  se->registerSubsystem(qafvtx);

  TMutDatabaseCntrl::set_database_access("internal_alignment_corrections", false);
  TMutDatabaseCntrl::set_database_access("global_alignment_corrections", false);

  // now use local file to process internal alignment for both MuTr and MuID
  MuonDisalign * muon_misalign(new MuonDisalign());
  muon_misalign->add_corrections(
      "./mu_arm_alignment_corrections.Input.txt");
//  muon_misalign->set_flag(MuonDisalign::PRINT_MUTR, true);
//  muon_misalign->set_flag(MuonDisalign::PRINT_MUID, true);
//  muon_misalign->set_flag(MuonDisalign::PRINT_MUTR_GEOM, true);
  se->registerSubsystem(muon_misalign);

  //  // Print database readouts
  //  TMutDatabaseCntrl::print();

  //  SubsysReco *svxproductionqa  = new SvxQAForProduction();
  //  se->registerSubsystem(svxproductionqa);

  //  SubsysReco *svxalignmentqa  = new SvxAlignment_QA();
  //  se->registerSubsystem(svxalignmentqa);
 
  // nDST output
  //////////////////////////////////
  // trgsel is a global vector from TrigSelect.C which
  // contains the names of the trigger selectors
  // which are used to determine the filenames
  char dstname[100];
  for (int i = 0; i < trgsel.size(); i++)
    {
      sprintf(dstname, "MWG_%s", trgsel[i].c_str());
      MWG_IOManager(runnumber, segnumber, dstname, trgsel[i].c_str());

      sprintf(dstname, "MuonDST_%s", trgsel[i].c_str());
      MuonDST_IOManager(runnumber, segnumber, dstname, trgsel[i].c_str());
    }

  gSystem->Exec("ps -o sid,ppid,pid,user,comm,vsize,rssize,time");

  DeathToMemoryHogs * dt = new DeathToMemoryHogs();
  dt->event_frequency(100); // check every hundred events
  dt->SaveHisto();  // save data in histogram
  se->registerSubsystem(dt);

  Fun4AllInputManager *in = new Fun4AllPrdfInputManager("PRDFin");
  in->fileopen(inputfile);
  se->registerInputManager(in);

  se->skip(skip*nEvents);
  se->run(nEvents);

  se->End();
  PrintTrigSelect();

  QA_IOManager(runnumber, segnumber);
  //  SVXQA_IOManager(runnumber, segnumber);

  int evts = se->PrdfEvents();
  std::cout << "Total Events:  " << evts << std::endl;

  char sql[1000];
  //sprintf(sql,"echo \"update %s set nevents=%d,time='now' where runnumber=%d and sequence=%d and prodtag = \'%s\';\" | isql phnxreco_odbc",gSystem->Getenv("PRODDBTABLE"),evts,runnumber,segnumber,gSystem->Getenv("PRODTAG"));
  sprintf(sql,"\"update %s set nevents=%d,time='now' where runnumber=%d and sequence=%d and prodtag = \'%s\';\" | isql phnxreco_odbc",gSystem->Getenv("PRODDBTABLE"),evts,runnumber,segnumber,gSystem->Getenv("PRODTAG"));
  std::cout << "Update DB statement: " << sql << std::endl;
  //gSystem->Exec(sql);
  ofstream out1;
  out1.open(Form("%s_%d.DBUPDATE",inputfile,skip));
  out1 << sql << endl;
  out1.close();

  delete se;
  //delete all singletons to make valgrind happy :)
  delete timeserv;
  TMutParameterDB &mutdb = TMutParameterDB::get();
  delete &mutdb;

  gSystem->Exec("ps -o sid,ppid,pid,user,comm,vsize,rssize,time");

  cout<<"Fun4All successfully completed "<<endl;
  gSystem->Exit(0);

}
