#include <SubsysReco.h>
#include <TTree.h>
#include <map>
#include <Fun4AllHistoManager.h>
#include <Fun4AllServer.h>

class TH1F;
class TH2F;
class TH3F;
class SingleMuon;

class mSingleMuAna : public SubsysReco
{ 
 public:

  //! default constructor
  mSingleMuAna(const TString fileName);
  //mSingleMuAna(const char* output);
  
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
  TFile *treeFile;
  TString outFileName, fileCreateMode;
  int fileCompressMode;
  const std::string OutputFileName;
  Fun4AllHistoManager *HistoManager;
  //Fun4AllServer* se;

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
  float muons_pT[10], muons_pX[10], muons_pY[10], muons_pZ[10], muons_p[10];
  float muons_eta[10];
  bool muons_charge[10];
  short muons_idHits[10], muons_nIdHits[10], muons_lastGap[10];
  float muons_DG0[10], muons_DDG0[10], muons_DS3[10];
  float muons_trkChi2[10], muons_idChi2[10], muons_x0[10], muons_y0[10], muons_z0[10];
  float muons_xst1[10], muons_xst2[10], muons_xst3[10], muons_yst1[10], muons_yst2[10], muons_yst3[10];
  
  float muons_idX[10], muons_idY[10], muons_st1pX[10], muons_st1pY[10], muons_st1pZ[10];
  float muons_dcaZ[10], muons_dcaR[10], muons_dcaPhi[10];
  bool muons_MUID1D[10], muons_MUID1S[10];
  float muons_cov[5][5][10], muons_cov_fvtx[5][5][10], muons_cov_fvtxmutr[5][5][10];
  float muons_pX_fvtx[10], muons_pY_fvtx[10], muons_pZ_fvtx[10];
  float muons_pX_fvtxmutr[10], muons_pY_fvtxmutr[10], muons_pZ_fvtxmutr[10];

  size_t muons_nHits_fvtx[10], muons_snHits_fvtx[10];
  float muons_dPhi_fvtx[10], muons_dTheta_fvtx[10], muons_dR_fvtx[10];
  float muons_chi2_fvtx[10], muons_x0_fvtx[10], muons_y0_fvtx[10], muons_z0_fvtx[10];
  float muons_chi2_fvtxmutr[10], muons_x0_fvtxmutr[10], muons_y0_fvtxmutr[10], muons_z0_fvtxmutr[10];
  float muons_fvtx_charge[4][10];
  unsigned int muons_nfvtx_tracklets[10];
  unsigned long muons_nfvtx_tracklets_cone[10], muons_nfvtx_clusters_cone[10];
  unsigned short muons_cluster_size1[10];
  short muons_mutoo_trk_index[10];

  float muons_rpc_DCA[10], muons_rpc_pT[10], muons_rpc_time[10], muons_rpc1_DCA[10], muons_rpc1_time[10];
  float muons_rpc1_vtxDCA[10], muons_rpc1_vtxTime[10], muons_rpc3_vtxDCA[10], muons_rpc3_vtxTime[10];
  float muons_rpc1_St1DCA[10], muons_rpc1_St1Time[10], muons_rpc3_St1DCA[10], muons_rpc3_St1Time[10];
  float muons_rpc1_St3DCA[10], muons_rpc1_St3Time[10], muons_rpc3_St3DCA[10], muons_rpc3_St3Time[10];
  float muons_rpc1_MUIDDCA[10], muons_rpc1_MUIDTime[10], muons_rpc3_MUIDDCA[10], muons_rpc3_MUIDTime[10];
  float muons_dr_fvtx_sigma[10], muons_dtheta_fvtx_sigma[10], muons_dphi_fvtx_sigma[10], muons_sdr_fvtx[10];
  float muons_sdtheta_fvtx[10], muons_sdphi_fvtx[10], muons_sdr_fvtx_sigma[10], muons_sdtheta_fvtx_sigma[10], muons_sdphi_fvtx_sigma[10];

  float muons_sdcaR[10], muons_sdcaPhi[10], muons_sdcaZ[10], muons_schi2_fvtxmutr[10];
  short muons_vtx_index[10], muons_n_matchings[10];
  unsigned short muons_hit_pattern[10], muons_shit_pattern[10];
  float muons_x_fvtx[10], muons_y_fvtx[10], muons_z_fvtx[10];
  float muons_x_mutproj_fvtx[10], muons_y_mutproj_fvtx[10], muons_z_mutproj_fvtx[10];
  float muons_cov_mutsta1[5][5][10];
  float muons_pX_fvtxproj[3][10], muons_pY_fvtxproj[3][10], muons_x_fvtxproj[3][10], muons_y_fvtxproj[3][10];
  float muons_pX_mutproj[3][10], muons_pY_mutproj[3][10], muons_x_mutproj[3][10], muons_y_mutproj[3][10];
  float muons_dR_fvtxmutr_sigma[3][10], muons_dPhi_fvtxmutr_sigma[3][10], muons_dpR_fvtxmutr_sigma[3][10];
  float muons_dpPhi_fvtxmutr_sigma[3][10], muons_spX_fvtxproj[3][10], muons_spY_fvtxproj[3][10], muons_sx_fvtxproj[3][10];
  float muons_sy_fvtxproj[3][10], muons_spX_mutproj[3][10], muons_spY_mutproj[3][10], muons_sx_mutproj[3][10], muons_sy_mutproj[3][10];
  float muons_sdR_fvtxmutr_sigma[3][10], muons_sdPhi_fvtxmutr_sigma[3][10], muons_sdpR_fvtxmutr_sigma[3][10], muons_sdpPhi_fvtxmutr_sigma[3][10];
  float muons_maxres_sigma[10], muons_smaxres_sigma[10];

};
