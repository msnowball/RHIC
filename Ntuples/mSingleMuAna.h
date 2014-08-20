#include <SubsysReco.h>
#include <TTree.h>
#include <map>

class TH1F;
class TH2F;
class TH3F;
class SingleMuon;

class mSingleMuAna : public SubsysReco
{ 
 public:

  //! default constructor
  mSingleMuAna();
  
  //! destructor
  virtual 
  ~mSingleMuAna();
  
  //! global initialization
  int Init(PHCompositeNode *topNode);
  
  //! Run initialization
  int InitRun(PHCompositeNode *topNode);
  
  //! event method
  int process_event(PHCompositeNode *topNode);
  
  //! global termination
  int End(PHCompositeNode *topNode);
  
 private:

  //Functions
  bool Cut(SingleMuon* muon);
  bool CutFvtx(SingleMuon* muon);
  
  void InitTree(TTree *tree);
  void InitVariables();

  TTree *tree;

  //Hist Maps
  std::map<std::string,TH1F*> hists1D_;
  std::map<std::string,TH2F*> hists2D_;

  int counter;

  //Event Variables
  int iEvent;
  int Run, Event;
  float Evt_bbcZ, Evt_Cent;
  float Evt_vtxX, Evt_vtxY, Evt_vtxZ;
  float Evt_fvtxX, Evt_fvtxY, Evt_fvtxZ;
  float Evt_fvtxX2, Evt_fvtxY2, Evt_fvtxZ2;

  
  //Muons
  int muons_nMuons;
  float muons_pT[50], muons_pX[50], muons_pY[50], muons_pZ[50], muons_p[50];
  float muons_eta[50];
  bool muons_charge[50];
  short muons_idHits[50], muons_nIdHits[50], muons_lastGap[50];
  float muons_DG0[50], muons_DDG0[50], muons_DS3[50];
  float muons_trkChi2[50], muons_idChi2[50], muons_x0[50], muons_y0[50], muons_z0[50];
  float muons_xst1[50], muons_xst2[50], muons_xst3[50], muons_yst1[50], muons_yst2[50], muons_yst3[50];
  
  float muons_idX[50], muons_idY[50], muons_st1pX[50], muons_st1pY[50], muons_st1pZ[50];
  float muons_dcaZ[50], muons_dcaR[50], muons_dcaPhi[50];
  bool muons_MUID1D[50], muons_MUID1S[50];
  float muons_cov[5][5][50], muons_cov_fvtx[5][5][50], muons_cov_fvtxmutr[5][5][50];
  float muons_pX_fvtx[50], muons_pY_fvtx[50], muons_pZ_fvtx[50];
  float muons_pX_fvtxmutr[50], muons_pY_fvtxmutr[50], muons_pZ_fvtxmutr[50];

  size_t muons_nHits_fvtx[50], muons_snHits_fvtx[50];
  float muons_dPhi_fvtx[50], muons_dTheta_fvtx[50], muons_dR_fvtx[50];
  float muons_chi2_fvtx[50], muons_x0_fvtx[50], muons_y0_fvtx[50], muons_z0_fvtx[50];
  float muons_chi2_fvtxmutr[50], muons_x0_fvtxmutr[50], muons_y0_fvtxmutr[50], muons_z0_fvtxmutr[50];
  float muons_fvtx_charge[4][50];
  unsigned int muons_nfvtx_tracklets[50];
  unsigned long muons_nfvtx_tracklets_cone[50], muons_nfvtx_clusters_cone[50];
  unsigned short muons_cluster_size1[50];
  short muons_mutoo_trk_index[50];

  float muons_rpc_DCA[50], muons_rpc_pT[50], muons_rpc_time[50], muons_rpc1_DCA[50], muons_rpc1_time[50];
  float muons_rpc1_vtxDCA[50], muons_rpc1_vtxTime[50], muons_rpc3_vtxDCA[50], muons_rpc3_vtxTime[50];
  float muons_rpc1_St1DCA[50], muons_rpc1_St1Time[50], muons_rpc3_St1DCA[50], muons_rpc3_St1Time[50];
  float muons_rpc1_St3DCA[50], muons_rpc1_St3Time[50], muons_rpc3_St3DCA[50], muons_rpc3_St3Time[50];
  float muons_rpc1_MUIDDCA[50], muons_rpc1_MUIDTime[50], muons_rpc3_MUIDDCA[50], muons_rpc3_MUIDTime[50];
  float muons_dr_fvtx_sigma[50], muons_dtheta_fvtx_sigma[50], muons_dphi_fvtx_sigma[50], muons_sdr_fvtx[50];
  float muons_sdtheta_fvtx[50], muons_sdphi_fvtx[50], muons_sdr_fvtx_sigma[50], muons_sdtheta_fvtx_sigma[50], muons_sdphi_fvtx_sigma[50];

  float muons_sdcaR[50], muons_sdcaPhi[50], muons_sdcaZ[50], muons_schi2_fvtxmutr[50];
  short muons_vtx_index[50], muons_n_matchings[50];
  unsigned short muons_hit_pattern[50], muons_shit_pattern[50];
  float muons_x_fvtx[50], muons_y_fvtx[50], muons_z_fvtx[50];
  float muons_x_mutproj_fvtx[50], muons_y_mutproj_fvtx[50], muons_z_mutproj_fvtx[50];
  float muons_cov_mutsta1[5][5][50];
  float muons_pX_fvtxproj[3][50], muons_pY_fvtxproj[3][50], muons_x_fvtxproj[3][50], muons_y_fvtxproj[3][50];
  float muons_pX_mutproj[3][50], muons_pY_mutproj[3][50], muons_x_mutproj[3][50], muons_y_mutproj[3][50];
  float muons_dR_fvtxmutr_sigma[3][50], muons_dPhi_fvtxmutr_sigma[3][50], muons_dpR_fvtxmutr_sigma[3][50];
  float muons_dpPhi_fvtxmutr_sigma[3][50], muons_spX_fvtxproj[3][50], muons_spY_fvtxproj[3][50], muons_sx_fvtxproj[3][50];
  float muons_sy_fvtxproj[3][50], muons_spX_mutproj[3][50], muons_spY_mutproj[3][50], muons_sx_mutproj[3][50], muons_sy_mutproj[3][50];
  float muons_sdR_fvtxmutr_sigma[3][50], muons_sdPhi_fvtxmutr_sigma[3][50], muons_sdpR_fvtxmutr_sigma[3][50], muons_sdpPhi_fvtxmutr_sigma[3][50];
  float muons_maxres_sigma[50], muons_smaxres_sigma[50];

};
