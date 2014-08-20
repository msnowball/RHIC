// $Id: Fun4Muons_RecoDST_sim.C,v 1.31 2006/06/06 11:42:38 hpereira Exp $
/*! 
	mutoo dst analysis loop for mutoo slowsim simulated dst, possibly embeded
	into either simulated or real data dst
*/
void Fun4SingleMuAna(const char *inputFile   = "367727_0.root",		
		     const char *outputFile  = "ana.root"
		     )
{

  // load libraries
  gSystem->Load("libfun4all.so");
  gSystem->Load("libfun4allfuncs.so");
  gSystem->Load("libmutoo_subsysreco.so");
  gSystem->Load("libfvtx_subsysreco.so");
  gSystem->Load("libMWG_interface.so");	
  gSystem->Load("libmuon_subsysreco");
  gSystem->Load("libfun4allfuncs_muons");
  gSystem->Load("libfvtx_subsysreco");
  gSystem->Load("/direct/phenix+scratch/snowball/SpinAnalysis/firstBuild/picoDST_object/install/lib/libpicodst_object.so");
  cout << ">>> Libraries loaded" << endl;
  

  ///////////////////////////////////////////
  // Make the Server
  //////////////////////////////////////////
  Fun4AllServer *se = Fun4AllServer::instance(); 
  se->Verbosity(1);

  mSingleMuAna *muonAna = new mSingleMuAna();
  se->registerSubsystem( muonAna );

  ///////////////////////////////////////////
  // Input manager
  ///////////////////////////////////////////
  Fun4AllInputManager *input = new Fun4AllNoSyncDstInputManager( "INPUT", "INPUT" );
  se->registerInputManager( input );
  se->fileopen( input->Name(), inputFile );	
  
  ///////////////////////////////////////////
  // Output manager
  ///////////////////////////////////////////
  se->run(0);
  se->End();


  
  Fun4AllHistoManager *hm = se->getHistoManager("picodstobject");
  if( hm ) hm->setOutfileName( outputFile );
  else cout << " >>> Fun4All - unable to find histoManager" << endl;
  hm->dumpHistos();
  

  
}


