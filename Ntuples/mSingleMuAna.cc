#include <Fun4AllReturnCodes.h>
#include <PHCompositeNode.h>
#include <getClass.h>
#include <MWGConsts.h>
#include <Tools.h>
#include <Fun4AllServer.h>
#include <Fun4AllHistoManager.h>
#include <RunNumberRanges.h>
#include <TH1F.h>
#include <TrigRunLvl1.h>
#include <TH3.h>
#include <TH2.h>
#include <TTree.h>
#include <TRandom.h>
#include <SyncObject.h>
#include <RunHeader.h>
#include <EventHeader.h>
#include <TMath.h>
#include <SingleMuon.h>
#include <DiMuonContainer.h>
#include <SingleMuonContainer.h>
#include <TFvtxCompactTrkMap.h>
#include <DiMuon.h>

#include "mSingleMuAna.h"


using namespace std;

mSingleMuAna::mSingleMuAna():
  SubsysReco("mSingleMuAna")
{
  Fun4AllServer* se = Fun4AllServer::instance();
  
  Fun4AllHistoManager *hm = se->getHistoManager("picodstobject");
  if (!hm)
    {
      hm = new Fun4AllHistoManager("picodstobject");
      se->registerHistoManager(hm);
    }
  
  tree = new TTree("passedEvents","passedEvents");
  InitTree(tree);
  hm->registerHisto(tree);

  InitVariables();

  hists1D_["nEvents"] = new TH1F("nEvents","nEvents",2,0,2);
  hm->registerHisto(hists1D_["nEvents"]);
  iEvent = 0;
  
}


//! destructor
mSingleMuAna::~mSingleMuAna()
{

}



//______________________________________________________
int mSingleMuAna::Init(PHCompositeNode *top_node)
{  
  return 0;
}

//______________________________________________________
int mSingleMuAna::InitRun(PHCompositeNode *top_node)
{
  MUTOO::PRINT( cout, "mSingleMuAna::InitRun" );

  return 0;
}

//______________________________________________________
int mSingleMuAna::process_event(PHCompositeNode *top_node)
{

  //Analysis Loop
  InitVariables();
  iEvent++;

  //Providence Info
  RunHeader* runh = findNode::getClass<RunHeader>(top_node,"RunHeader");
  if (runh) Run = runh->get_RunNumber();
  
  EventHeader* eventh = findNode::getClass<EventHeader>(top_node,"EventHeader");
  if (eventh) Event = eventh->get_EvtSequence();


  SingleMuonContainer* muonContainer = findNode::getClass<SingleMuonContainer>(top_node,"SingleMuonContainer");
  if (!muonContainer) muonContainer = findNode::getClass<SingleMuonContainer>(top_node,"SingleMuonContainer_filtered");
  if (!muonContainer)
    {
      cout << "mSingleMuAna:: SingleMuonContainer not in Node Tree" << endl;
      return ABORTRUN;
    }


  Evt_bbcZ = muonContainer->get_Evt_bbcZ();
  Evt_vtxX = muonContainer->get_Evt_vtxX();
  Evt_vtxY = muonContainer->get_Evt_vtxY();
  Evt_vtxZ = muonContainer->get_Evt_vtxZ();
  Evt_fvtxX = muonContainer->get_Evt_fvtxX();
  Evt_fvtxY = muonContainer->get_Evt_fvtxY();
  Evt_fvtxZ = muonContainer->get_Evt_fvtxZ();
  Evt_fvtxX2 = muonContainer->get_Evt_fvtxX2();
  Evt_fvtxY2 = muonContainer->get_Evt_fvtxY2();
  Evt_fvtxZ2 = muonContainer->get_Evt_fvtxZ2();
  Evt_Cent = muonContainer->get_Evt_Cent();




  counter = 0;
  muons_nMuons = muonContainer->get_nSingleMuons();
  //Loop over single muons
  for(unsigned int nMu = 0; nMu < muonContainer->get_nSingleMuons(); nMu++)
    {
      const SingleMuon *mu = muonContainer->get_SingleMuon(nMu);
      muons_pT[counter] = mu->get_pT();
      muons_pX[counter] = mu->get_px();
      muons_pY[counter] = mu->get_py();
      muons_pZ[counter] = mu->get_pz();
      muons_p[counter] = mu->get_p();
      muons_eta[counter] = mu->get_rapidity();
      muons_charge[counter] = mu->get_charge();

      
      muons_idHits[counter] = mu->get_idhits();
      muons_nIdHits[counter] = mu->get_nidhits();
      muons_lastGap[counter] = mu->get_lastgap();
      muons_DG0[counter] = mu->get_DG0();
      muons_DDG0[counter] = mu->get_DDG0();
      muons_DS3[counter] = mu->get_DS3();
      muons_trkChi2[counter] = mu->get_trchi2();
      muons_idChi2[counter] = mu->get_idchi2();
      muons_x0[counter] = mu->get_x0();
      muons_y0[counter] = mu->get_y0();
      muons_z0[counter] = mu->get_z0();
      muons_xst1[counter] = mu->get_xst1();
      muons_xst2[counter] = mu->get_xst2();
      muons_xst3[counter] = mu->get_xst3();
      muons_yst1[counter] = mu->get_yst1();
      muons_yst2[counter] = mu->get_yst2();
      muons_yst3[counter] = mu->get_yst3();
      

      muons_idX[counter] = mu->get_idx();
      muons_idY[counter] = mu->get_idy();
      muons_st1pX[counter] = mu->get_st1px();
      muons_st1pY[counter] = mu->get_st1py();
      muons_st1pZ[counter] = mu->get_st1pz();
      muons_dcaZ[counter] = mu->get_dca_z();
      muons_dcaR[counter] = mu->get_dca_r();
      muons_dcaPhi[counter] = mu->get_dca_phi();
      muons_MUID1D[counter] = mu->get_MUID1D();
      muons_MUID1S[counter] = mu->get_MUID1S();
      for(int i = 0; i < 5; i++)
	{
	  for(int j = 0; j < 5; j++)
	    {
	      muons_cov[i][j][counter] = mu->get_cov(i,j);
	      muons_cov_fvtx[i][j][counter] = mu->get_cov_fvtx(i,j);
	      muons_cov_fvtxmutr[i][j][counter] = mu->get_cov_fvtxmutr(i,j);
	    }
	}
      muons_pX_fvtx[counter] = mu->get_px_fvtx();
      muons_pY_fvtx[counter] = mu->get_py_fvtx();
      muons_pZ_fvtx[counter] = mu->get_pz_fvtx();
      muons_pX_fvtxmutr[counter] = mu->get_px_fvtxmutr();
      muons_pY_fvtxmutr[counter] = mu->get_py_fvtxmutr();
      muons_pZ_fvtxmutr[counter] = mu->get_pz_fvtxmutr();
      muons_nHits_fvtx[counter] = mu->get_nhits_fvtx();
      muons_snHits_fvtx[counter] = mu->get_snhits_fvtx();
      muons_dPhi_fvtx[counter] = mu->get_dphi_fvtx();
      muons_dTheta_fvtx[counter] = mu->get_dtheta_fvtx();
      muons_dR_fvtx[counter] = mu->get_dr_fvtx();
      muons_chi2_fvtx[counter] = mu->get_chi2_fvtx();
      muons_x0_fvtx[counter] = mu->get_x0_fvtx();
      muons_y0_fvtx[counter] = mu->get_y0_fvtx();
      muons_z0_fvtx[counter] = mu->get_z0_fvtx();
      muons_chi2_fvtxmutr[counter] = mu->get_chi2_fvtxmutr();
      muons_x0_fvtxmutr[counter] = mu->get_x0_fvtxmutr();
      muons_y0_fvtxmutr[counter] = mu->get_y0_fvtxmutr();
      muons_z0_fvtxmutr[counter] = mu->get_z0_fvtxmutr();
      for(int i = 0; i < 4; i++)
	{
	  muons_fvtx_charge[i][counter] = mu->get_fvtx_charge(i);
	}
      muons_nfvtx_tracklets[counter] = mu->get_nfvtx_tracklets();
      muons_nfvtx_tracklets_cone[counter] = mu->get_nfvtx_tracklets_cone();
      muons_nfvtx_clusters_cone[counter] = mu->get_nfvtx_clusters_cone();
      muons_cluster_size1[counter] = mu->get_clusters_size1();
      muons_mutoo_trk_index[counter] = mu->get_mutoo_trk_index();
      muons_rpc_DCA[counter] = mu->get_RpcDCA();
      muons_rpc_pT[counter] = mu->get_RpcpT();
      muons_rpc_time[counter] = mu->get_Rpctime();
      muons_rpc1_DCA[counter] = mu->get_Rpc1DCA();
      muons_rpc1_time[counter] = mu->get_Rpc1time();
      muons_rpc1_vtxDCA[counter] = mu->get_Rpc1VtxDCA();
      muons_rpc1_vtxTime[counter] = mu->get_Rpc1VtxTime();
      muons_rpc3_vtxDCA[counter] = mu->get_Rpc3VtxDCA();
      muons_rpc3_vtxTime[counter] = mu->get_Rpc3VtxTime();
      muons_rpc1_St1DCA[counter] = mu->get_Rpc1St1DCA();
      muons_rpc1_St1Time[counter] = mu->get_Rpc1St1Time();
      muons_rpc3_St1DCA[counter] = mu->get_Rpc3St1DCA();
      muons_rpc3_St1Time[counter] = mu->get_Rpc3St1Time();
      muons_rpc1_St3DCA[counter] = mu->get_Rpc1St3DCA();
      muons_rpc1_St3Time[counter] = mu->get_Rpc1St3Time();
      muons_rpc3_St3DCA[counter] = mu->get_Rpc3St3DCA();
      muons_rpc3_St3Time[counter] = mu->get_Rpc3St3Time();
      muons_rpc1_MUIDDCA[counter] = mu->get_Rpc1MuIDDCA();
      muons_rpc1_MUIDTime[counter] = mu->get_Rpc1MuIDTime();
      muons_rpc3_MUIDDCA[counter] = mu->get_Rpc3MuIDDCA();
      muons_rpc3_MUIDTime[counter] = mu->get_Rpc3MuIDTime();
      muons_dr_fvtx_sigma[counter] = mu->get_dr_fvtx_sigma();
      muons_dtheta_fvtx_sigma[counter] = mu->get_dtheta_fvtx_sigma();
      muons_dphi_fvtx_sigma[counter] = mu->get_dphi_fvtx_sigma();
      muons_sdr_fvtx[counter] = mu->get_sdr_fvtx();
      muons_sdtheta_fvtx[counter] = mu->get_sdtheta_fvtx();
      muons_sdphi_fvtx[counter] = mu->get_sdphi_fvtx();
      muons_sdr_fvtx_sigma[counter] = mu->get_sdr_fvtx_sigma();
      muons_sdtheta_fvtx_sigma[counter] = mu->get_sdtheta_fvtx_sigma();
      muons_sdphi_fvtx_sigma[counter] = mu->get_sdphi_fvtx_sigma();
      

      muons_sdcaR[counter] = mu->get_sdca_r();
      muons_sdcaPhi[counter] = mu->get_sdca_phi();
      muons_sdcaZ[counter] = mu->get_sdca_z();
      muons_schi2_fvtxmutr[counter] = mu->get_schi2_fvtxmutr();
      muons_vtx_index[counter] = mu->get_vtx_index();
      muons_n_matchings[counter] = mu->get_n_matchings();
      muons_hit_pattern[counter] = mu->get_hit_pattern();
      muons_shit_pattern[counter] = mu->get_shit_pattern();
      muons_x_fvtx[counter] = mu->get_x_fvtx();
      muons_y_fvtx[counter] = mu->get_y_fvtx();
      muons_z_fvtx[counter] = mu->get_z_fvtx();
      muons_x_mutproj_fvtx[counter] = mu->get_x_mutproj_fvtx();
      muons_y_mutproj_fvtx[counter] = mu->get_y_mutproj_fvtx();
      muons_z_mutproj_fvtx[counter] = mu->get_z_mutproj_fvtx();
      for(int i = 0; i < 5; i++)
	{
	  for(int j = 0; j < 5; j++)
	    {
	      muons_cov_mutsta1[i][j][counter] = mu->get_cov_mutsta1(i,j);
	    }
	}
      for(int i = 0; i < 3; i++)
	{
      
	  muons_pX_fvtxproj[i][counter] = mu->get_px_fvtxproj(i);
	  muons_pY_fvtxproj[i][counter] = mu->get_py_fvtxproj(i);
	  muons_x_fvtxproj[i][counter] = mu->get_x_fvtxproj(i);
	  muons_y_fvtxproj[i][counter] = mu->get_y_fvtxproj(i);
	  muons_pX_mutproj[i][counter] = mu->get_px_mutproj(i);
	  muons_pY_mutproj[i][counter] = mu->get_py_mutproj(i);
	  muons_x_mutproj[i][counter] = mu->get_x_mutproj(i);
	  muons_y_mutproj[i][counter] = mu->get_y_mutproj(i);
	  muons_dR_fvtxmutr_sigma[i][counter] = mu->get_dr_fvtxmutr_sigma(i);
	  muons_dPhi_fvtxmutr_sigma[i][counter] = mu->get_dphi_fvtxmutr_sigma(i);
	  muons_dpR_fvtxmutr_sigma[i][counter] = mu->get_dpr_fvtxmutr_sigma(i);
	  muons_dpPhi_fvtxmutr_sigma[i][counter] = mu->get_dpphi_fvtxmutr_sigma(i);
	  muons_spX_fvtxproj[i][counter] = mu->get_spx_fvtxproj(i);
	  muons_spY_fvtxproj[i][counter] = mu->get_spy_fvtxproj(i);
	  muons_sx_fvtxproj[i][counter] = mu->get_sx_fvtxproj(i);
	  muons_sy_fvtxproj[i][counter] = mu->get_sy_fvtxproj(i);
	  muons_spX_mutproj[i][counter] = mu->get_spx_mutproj(i);
	  muons_spY_mutproj[i][counter] = mu->get_spy_mutproj(i);
	  muons_sx_mutproj[i][counter] = mu->get_sx_mutproj(i);
	  muons_sy_mutproj[i][counter] = mu->get_sy_mutproj(i);
	  muons_sdR_fvtxmutr_sigma[i][counter] = mu->get_sdr_fvtxmutr_sigma(i);
	  muons_sdPhi_fvtxmutr_sigma[i][counter] = mu->get_sdphi_fvtxmutr_sigma(i);
	  muons_sdpR_fvtxmutr_sigma[i][counter] = mu->get_sdpr_fvtxmutr_sigma(i);
	  muons_sdpPhi_fvtxmutr_sigma[i][counter] = mu->get_sdpphi_fvtxmutr_sigma(i);

	}
      muons_maxres_sigma[counter] = mu->get_maxres_sigma();
      muons_smaxres_sigma[counter] = mu->get_smaxres_sigma();
            
      counter++;
    }


  tree->Fill();

  return 0;

  //return DISCARDEVENT;
}

//______________________________________________________
int mSingleMuAna::End(PHCompositeNode *top_node)
{
  hists1D_["nEvents"]->SetBinContent(1,iEvent);
  
  MUTOO::PRINT( cout, "mSingleMuAna::End" );

  return 0 ;
}

bool mSingleMuAna::Cut(SingleMuon* muon)
{
  return true;
}

bool mSingleMuAna::CutFvtx(SingleMuon* muon)
{
  return true;  
}


void mSingleMuAna::InitTree(TTree *tree)
{
  //Event
  tree->Branch("Run",&Run);
  tree->Branch("Event",&Event);
  tree->Branch("Evt_bbcZ",&Evt_bbcZ);
  tree->Branch("Evt_vtxX",&Evt_vtxX);
  tree->Branch("Evt_vtxY",&Evt_vtxY);
  tree->Branch("Evt_vtxZ",&Evt_vtxZ);
  tree->Branch("Evt_fvtxX",&Evt_fvtxX);
  tree->Branch("Evt_fvtxY",&Evt_fvtxY);
  tree->Branch("Evt_fvtxZ",&Evt_fvtxZ);
  tree->Branch("Evt_fvtxX2",&Evt_fvtxX2);
  tree->Branch("Evt_fvtxY2",&Evt_fvtxY2);
  tree->Branch("Evt_fvtxZ2",&Evt_fvtxZ2);
  tree->Branch("Evt_Cent",&Evt_Cent);
  
  //Muons
  tree->Branch("muons_nMuons", &muons_nMuons, "muons_nMuons/I");
  tree->Branch("muons_pT", &muons_pT, "muons_pT[muons_nMuons]/F");
  tree->Branch("muons_pX", &muons_pX, "muons_pX[muons_nMuons]/F");
  tree->Branch("muons_pY", &muons_pY, "muons_pY[muons_nMuons]/F");
  tree->Branch("muons_pZ", &muons_pZ, "muons_pZ[muons_nMuons]/F");
  tree->Branch("muons_p", &muons_p, "muons_p[muons_nMuons]/F");
  tree->Branch("muons_eta", &muons_eta, "muons_eta[muons_nMuons]/F");
  tree->Branch("muons_charge", &muons_charge, "muons_charge[muons_nMuons]/O");

  tree->Branch("muons_idHits", &muons_idHits, "muons_idHits[muons_nMuons]/s");
  tree->Branch("muons_nIdHits", &muons_nIdHits, "muons_nIdHits[muons_nMuons]/s");
  tree->Branch("muons_lastGap", &muons_lastGap, "muons_lastGap[muons_nMuons]/s");
  tree->Branch("muons_DG0", &muons_DG0, "muons_DG0[muons_nMuons]/F");
  tree->Branch("muons_DDG0", &muons_DDG0, "muons_DDG0[muons_nMuons]/F");
  tree->Branch("muons_DS3", &muons_DS3, "muons_DS3[muons_nMuons]/F");
  tree->Branch("muons_trkChi2", &muons_trkChi2, "muons_trkChi2[muons_nMuons]/F");
  tree->Branch("muons_idChi2", &muons_idChi2, "muons_idChi2[muons_nMuons]/F");
  tree->Branch("muons_x0", &muons_x0, "muons_x0[muons_nMuons]/F");
  tree->Branch("muons_y0", &muons_y0, "muons_y0[muons_nMuons]/F");
  tree->Branch("muons_z0", &muons_z0, "muons_z0[muons_nMuons]/F");
  tree->Branch("muons_xst1", &muons_xst1, "muons_xst1[muons_nMuons]/F");
  tree->Branch("muons_xst2", &muons_xst2, "muons_xst2[muons_nMuons]/F");
  tree->Branch("muons_xst3", &muons_xst3, "muons_xst3[muons_nMuons]/F");
  tree->Branch("muons_yst1", &muons_yst1, "muons_yst1[muons_nMuons]/F");
  tree->Branch("muons_yst2", &muons_yst2, "muons_yst2[muons_nMuons]/F");
  tree->Branch("muons_yst3", &muons_yst3, "muons_yst3[muons_nMuons]/F");
  tree->Branch("muons_idX", &muons_idX, "muons_idX[muons_nMuons]/F");
  tree->Branch("muons_idY", &muons_idY, "muons_idY[muons_nMuons]/F");
  tree->Branch("muons_st1pX", &muons_st1pX, "muons_st1pX[muons_nMuons]/F");
  tree->Branch("muons_st1pY", &muons_st1pY, "muons_st1pY[muons_nMuons]/F");
  tree->Branch("muons_st1pZ", &muons_st1pZ, "muons_st1pZ[muons_nMuons]/F");
  tree->Branch("muons_dcaZ", &muons_dcaZ, "muons_dcaZ[muons_nMuons]/F");
  tree->Branch("muons_dcaR", &muons_dcaR, "muons_dcaR[muons_nMuons]/F");
  tree->Branch("muons_dcaPhi", &muons_dcaPhi, "muons_dcaPhi[muons_nMuons]/F");
  tree->Branch("muons_MUID1D", &muons_MUID1D, "muons_MUID1D[muons_nMuons]/O");
  tree->Branch("muons_MUID1S", &muons_MUID1S, "muons_MUID1S[muons_nMuons]/O");
  tree->Branch("muons_cov", &muons_cov, "muons_cov[5][5][muons_nMuons]/F");
  tree->Branch("muons_cov_fvtx", &muons_cov_fvtx, "muons_cov_fvtx[5][5][muons_nMuons]/F");
  tree->Branch("muons_cov_fvtxmutr", &muons_cov_fvtxmutr, "muons_cov_fvtxmutr[5][5][muons_nMuons]/F");
  tree->Branch("muons_pX_fvtx", &muons_pX_fvtx, "muons_pX_fvtx[muons_nMuons]/F");
  tree->Branch("muons_pY_fvtx", &muons_pY_fvtx, "muons_pY_fvtx[muons_nMuons]/F");
  tree->Branch("muons_pZ_fvtx", &muons_pZ_fvtx, "muons_pZ_fvtx[muons_nMuons]/F");
  tree->Branch("muons_pX_fvtxmutr", &muons_pX_fvtxmutr, "muons_pX_fvtxmutr[muons_nMuons]/F");
  tree->Branch("muons_pY_fvtxmutr", &muons_pY_fvtxmutr, "muons_pY_fvtxmutr[muons_nMuons]/F");
  tree->Branch("muons_pZ_fvtxmutr", &muons_pZ_fvtxmutr, "muons_pZ_fvtxmutr[muons_nMuons]/F");
  tree->Branch("muons_dPhi_fvtx", &muons_dPhi_fvtx, "muons_dPhi_fvtx[muons_nMuons]/F");
  tree->Branch("muons_dTheta_fvtx", &muons_dTheta_fvtx, "muons_dTheta_fvtx[muons_nMuons]/F");
  tree->Branch("muons_dR_fvtx", &muons_dR_fvtx, "muons_dR_fvtx[muons_nMuons]/F");
  tree->Branch("muons_chi2_fvtx", &muons_chi2_fvtx, "muons_chi2_fvtx[muons_nMuons]/F");
  tree->Branch("muons_x0_fvtx", &muons_x0_fvtx, "muons_x0_fvtx[muons_nMuons]/F");
  tree->Branch("muons_y0_fvtx", &muons_y0_fvtx, "muons_y0_fvtx[muons_nMuons]/F");
  tree->Branch("muons_z0_fvtx", &muons_z0_fvtx, "muons_z0_fvtx[muons_nMuons]/F");
  tree->Branch("muons_chi2_fvtxmutr", &muons_chi2_fvtxmutr, "muons_chi2_fvtxmutr[muons_nMuons]/F");
  tree->Branch("muons_x0_fvtxmutr", &muons_x0_fvtxmutr, "muons_x0_fvtxmutr[muons_nMuons]/F");
  tree->Branch("muons_y0_fvtxmutr", &muons_y0_fvtxmutr, "muons_y0_fvtxmutr[muons_nMuons]/F");
  tree->Branch("muons_z0_fvtxmutr", &muons_z0_fvtxmutr, "muons_z0_fvtxmutr[muons_nMuons]/F");
  tree->Branch("muons_fvtx_charge", &muons_fvtx_charge, "muons_fvtx_charge[muons_nMuons]/F");
  tree->Branch("muons_nHits_fvtx", &muons_nHits_fvtx, "muons_nHits_fvtx[muons_nMuons]/F");
  tree->Branch("muons_snHits_fvtx", &muons_snHits_fvtx, "muons_snHits_fvtx[muons_nMuons]/F");
  tree->Branch("muons_nfvtx_tracklets", &muons_nfvtx_tracklets, "muons_nfvtx_tracklets[muons_nMuons]/i");
  tree->Branch("muons_nfvtx_tracklets_cone", &muons_nfvtx_tracklets_cone, "muons_nfvtx_tracklets_cone[muons_nMuons]/l");
  tree->Branch("muons_nfvtx_clusters_cone", &muons_nfvtx_clusters_cone, "muons_nfvtx_clusters_cone[muons_nMuons]/l");
  tree->Branch("muons_cluster_size1", &muons_cluster_size1, "muons_cluster_size1[muons_nMuons]/s");
  tree->Branch("muons_mutoo_trk_index", &muons_mutoo_trk_index, "muons_mutoo_trk_index[muons_nMuons]/S");
  tree->Branch("muons_rpc_DCA", &muons_rpc_DCA, "muons_rpc_DCA[muons_nMuons]/F");
  tree->Branch("muons_rpc_pT", &muons_rpc_pT, "muons_rpc_pT[muons_nMuons]/F");
  tree->Branch("muons_rpc_time", &muons_rpc_time, "muons_rpc_time[muons_nMuons]/F");
  tree->Branch("muons_rpc1_DCA", &muons_rpc1_DCA, "muons_rpc1_DCA[muons_nMuons]/F");
  tree->Branch("muons_rpc1_time", &muons_rpc1_time, "muons_rpc1_time[muons_nMuons]/F");
  tree->Branch("muons_rpc1_vtxDCA", &muons_rpc1_vtxDCA, "muons_rpc1_vtxDCA[muons_nMuons]/F");
  tree->Branch("muons_rpc1_vtxTime", &muons_rpc1_vtxTime, "muons_rpc1_vtxTime[muons_nMuons]/F");
  tree->Branch("muons_rpc3_vtxDCA", &muons_rpc3_vtxDCA, "muons_rpc3_vtxDCA[muons_nMuons]/F");
  tree->Branch("muons_rpc3_vtxTime", &muons_rpc3_vtxTime, "muons_rpc3_vtxTime[muons_nMuons]/F");
  tree->Branch("muons_rpc1_St1DCA", &muons_rpc1_St1DCA, "muons_rpc1_St1DCA[muons_nMuons]/F");
  tree->Branch("muons_rpc1_St1Time", &muons_rpc1_St1Time, "muons_rpc1_St1Time[muons_nMuons]/F");
  tree->Branch("muons_rpc3_St1DCA", &muons_rpc3_St1DCA, "muons_rpc3_St1DCA[muons_nMuons]/F");
  tree->Branch("muons_rpc3_St1Time", &muons_rpc3_St1Time, "muons_rpc3_St1Time[muons_nMuons]/F");
  tree->Branch("muons_rpc1_St3DCA", &muons_rpc1_St3DCA, "muons_rpc1_St3DCA[muons_nMuons]/F");
  tree->Branch("muons_rpc1_St3Time", &muons_rpc1_St3Time, "muons_rpc1_St3Time[muons_nMuons]/F");
  tree->Branch("muons_rpc3_St3DCA", &muons_rpc3_St3DCA, "muons_rpc3_St3DCA[muons_nMuons]/F");
  tree->Branch("muons_rpc3_St3Time", &muons_rpc3_St3Time, "muons_rpc3_St3Time[muons_nMuons]/F");
  tree->Branch("muons_rpc1_MUIDDCA", &muons_rpc1_MUIDDCA, "muons_rpc1_MUIDDCA[muons_nMuons]/F");
  tree->Branch("muons_rpc1_MUIDTime", &muons_rpc1_MUIDTime, "muons_rpc1_MUIDTime[muons_nMuons]/F");
  tree->Branch("muons_rpc3_MUIDDCA", &muons_rpc3_MUIDDCA, "muons_rpc3_MUIDDCA[muons_nMuons]/F");
  tree->Branch("muons_rpc3_MUIDTime", &muons_rpc3_MUIDTime, "muons_rpc3_MUIDTime[muons_nMuons]/F");
  tree->Branch("muons_dr_fvtx_sigma", &muons_dr_fvtx_sigma, "muons_dr_fvtx_sigma[muons_nMuons]/F");
  tree->Branch("muons_dtheta_fvtx_sigma", &muons_dtheta_fvtx_sigma, "muons_dtheta_fvtx_sigma[muons_nMuons]/F");
  tree->Branch("muons_dphi_fvtx_sigma", &muons_dphi_fvtx_sigma, "muons_dphi_fvtx_sigma[muons_nMuons]/F");
  tree->Branch("muons_sdr_fvtx", &muons_sdr_fvtx, "muons_sdr_fvtx[muons_nMuons]/F");
  tree->Branch("muons_sdtheta_fvtx", &muons_sdtheta_fvtx, "muons_sdtheta_fvtx[muons_nMuons]/F");
  tree->Branch("muons_sdphi_fvtx", &muons_sdphi_fvtx, "muons_sdphi_fvtx[muons_nMuons]/F");
  tree->Branch("muons_sdr_fvtx_sigma", &muons_sdr_fvtx_sigma, "muons_sdr_fvtx_sigma[muons_nMuons]/F");
  tree->Branch("muons_sdtheta_fvtx_sigma", &muons_sdtheta_fvtx_sigma, "muons_sdtheta_fvtx_sigma[muons_nMuons]/F");
  tree->Branch("muons_sdphi_fvtx_sigma", &muons_sdphi_fvtx_sigma, "muons_sdphi_fvtx_sigma[muons_nMuons]/F");
  tree->Branch("muons_sdcaR", &muons_sdcaR, "muons_sdcaR[muons_nMuons]/F");
  tree->Branch("muons_sdcaPhi", &muons_sdcaPhi, "muons_sdcaPhi[muons_nMuons]/F");
  tree->Branch("muons_sdcaZ", &muons_sdcaZ, "muons_sdcaZ[muons_nMuons]/F");
  tree->Branch("muons_schi2_fvtxmutr", &muons_schi2_fvtxmutr, "muons_schi2_fvtxmutr[muons_nMuons]/F");
  tree->Branch("muons_vtx_index", &muons_vtx_index, "muons_vtx_index[muons_nMuons]/S");
  tree->Branch("muons_n_matchings", &muons_n_matchings, "muons_n_matchings[muons_nMuons]/S");
  tree->Branch("muons_hit_pattern", &muons_hit_pattern, "muons_hit_pattern[muons_nMuons]/s");
  tree->Branch("muons_shit_pattern", &muons_shit_pattern, "muons_shit_pattern[muons_nMuons]/s");
  tree->Branch("muons_x_fvtx", &muons_x_fvtx, "muons_x_fvtx[muons_nMuons]/F");
  tree->Branch("muons_y_fvtx", &muons_y_fvtx, "muons_y_fvtx[muons_nMuons]/F");
  tree->Branch("muons_z_fvtx", &muons_z_fvtx, "muons_z_fvtx[muons_nMuons]/F");
  tree->Branch("muons_x_mutproj_fvtx", &muons_x_mutproj_fvtx, "muons_x_mutproj_fvtx[muons_nMuons]/F");
  tree->Branch("muons_y_mutproj_fvtx", &muons_y_mutproj_fvtx, "muons_y_mutproj_fvtx[muons_nMuons]/F");
  tree->Branch("muons_z_mutproj_fvtx", &muons_z_mutproj_fvtx, "muons_z_mutproj_fvtx[muons_nMuons]/F");
  tree->Branch("muons_cov_mutsta1", &muons_cov_mutsta1, "muons_cov_mutsta1[5][5][muons_nMuons]/F");
  tree->Branch("muons_pX_fvtxproj", &muons_pX_fvtxproj, "muons_pX_fvtxproj[3][muons_nMuons]/F");
  tree->Branch("muons_pY_fvtxproj", &muons_pY_fvtxproj, "muons_pY_fvtxproj[3][muons_nMuons]/F");
  tree->Branch("muons_x_fvtxproj", &muons_x_fvtxproj, "muons_x_fvtxproj[3][muons_nMuons]/F");
  tree->Branch("muons_y_fvtxproj", &muons_y_fvtxproj, "muons_y_fvtxproj[3][muons_nMuons]/F");
  tree->Branch("muons_pX_mutproj", &muons_pX_mutproj, "muons_pX_mutproj[3][muons_nMuons]/F");
  tree->Branch("muons_pY_mutproj", &muons_pY_mutproj, "muons_pY_mutproj[3][muons_nMuons]/F");
  tree->Branch("muons_x_mutproj", &muons_x_mutproj, "muons_x_mutproj[3][muons_nMuons]/F");
  tree->Branch("muons_y_mutproj", &muons_y_mutproj, "muons_y_mutproj[3][muons_nMuons]/F");
  tree->Branch("muons_dR_fvtxmutr_sigma", &muons_dR_fvtxmutr_sigma, "muons_dR_fvtxmutr_sigma[3][muons_nMuons]/F");
  tree->Branch("muons_dPhi_fvtxmutr_sigma", &muons_dPhi_fvtxmutr_sigma, "muons_dPhi_fvtxmutr_sigma[3][muons_nMuons]/F");
  tree->Branch("muons_dpR_fvtxmutr_sigma", &muons_dpR_fvtxmutr_sigma, "muons_dpR_fvtxmutr_sigma[3][muons_nMuons]/F");
  tree->Branch("muons_dpPhi_fvtxmutr_sigma", &muons_dpPhi_fvtxmutr_sigma, "muons_dpPhi_fvtxmutr_sigma[3][muons_nMuons]/F");
  tree->Branch("muons_spX_fvtxproj", &muons_spX_fvtxproj, "muons_spX_fvtxproj[3][muons_nMuons]/F");
  tree->Branch("muons_spY_fvtxproj", &muons_spY_fvtxproj, "muons_spY_fvtxproj[3][muons_nMuons]/F");
  tree->Branch("muons_sx_fvtxproj", &muons_sx_fvtxproj, "muons_sx_fvtxproj[3][muons_nMuons]/F");
  tree->Branch("muons_sy_fvtxproj", &muons_sy_fvtxproj, "muons_sy_fvtxproj[3][muons_nMuons]/F");
  tree->Branch("muons_spX_mutproj", &muons_spX_mutproj, "muons_spX_mutproj[3][muons_nMuons]/F");
  tree->Branch("muons_spY_mutproj", &muons_spY_mutproj, "muons_spY_mutproj[3][muons_nMuons]/F");
  tree->Branch("muons_sx_mutproj", &muons_sx_mutproj, "muons_sx_mutproj[3][muons_nMuons]/F");
  tree->Branch("muons_sy_mutproj", &muons_sy_mutproj, "muons_sy_mutproj[3][muons_nMuons]/F");
  tree->Branch("muons_sdR_fvtxmutr_sigma", &muons_sdR_fvtxmutr_sigma, "muons_sdR_fvtxmutr_sigma[3][muons_nMuons]/F");
  tree->Branch("muons_sdPhi_fvtxmutr_sigma", &muons_sdPhi_fvtxmutr_sigma, "muons_sdPhi_fvtxmutr_sigma[3][muons_nMuons]/F");
  tree->Branch("muons_sdpR_fvtxmutr_sigma", &muons_sdpR_fvtxmutr_sigma, "muons_sdpR_fvtxmutr_sigma[3][muons_nMuons]/F");
  tree->Branch("muons_sdpPhi_fvtxmutr_sigma", &muons_sdpPhi_fvtxmutr_sigma, "muons_sdpPhi_fvtxmutr_sigma[3][muons_nMuons]/F");
  tree->Branch("muons_maxres_sigma", &muons_maxres_sigma, "muons_maxres_sigma[muons_nMuons]/F");
  tree->Branch("muons_smaxres_sigma", &muons_smaxres_sigma, "muons_smaxres_sigma[muons_nMuons]/F");
  //tree->Branch("", &, "[muons_nMuons]/F");
  //tree->Branch("", &, "[muons_nMuons]/F");
  //tree->Branch("", &, "[muons_nMuons]/F");
  
  
}

void mSingleMuAna::InitVariables()
{
  counter = 0;


  Run = -1;
  Event = -1;
  Evt_bbcZ = -999;
  Evt_Cent = -999;
  Evt_vtxX = -999;
  Evt_vtxY = -999;
  Evt_vtxZ = -999;
  Evt_fvtxX = -999;
  Evt_fvtxY = -999;
  Evt_fvtxZ = -999;
  Evt_fvtxX2 = -999;
  Evt_fvtxY2 = -999;
  Evt_fvtxZ2 = -999;

  muons_nMuons = 0;

}
