#ifndef PHG3toG4MuonIDPara_h
#define PHG3toG4MuonIDPara_h 1

#include <vector>

class PHG3toG4MuonIDPara 
{

private:
  
  int fNr_muon_arms;
  int fNr_muid_planes;
  int fNdet_per_pl;
  int fMu_floor_flg;
  float fMuid_zmin;
  float fMuid_zmax;
  float fMuid_ang;
  float fBeam_height;
  float fMu_top_height;
  float fMu_floor_height;
  float fMu_strp_thick;
  float fMu_gas_thick;
  float fMu_plas_thick;
  std::vector<float> fMu_yoke_thick;
  std::vector<float> fMu_donut_thick1, fMu_donut_thick2;
  std::vector<float> fRmax1_donut1, fRmax1_donut2;
  std::vector<float> fRmax2_donut1, fRmax2_donut2;
  float fMu_floor_thick;
  
  std::vector<float> fMu_abs_thick;
  std::vector<float> fMu_hi_abs, fMu_hi_med, fMu_lo_med;
  
  int fNmed_mu_ps;
  int fNmed_mu_gas;
  int fNmed_mu_cs;
  int fNmed_mu_sh;
  int fNmed_mu_sd;
  int fNmed_mudn;
  int fNmed_muhl;
  int fNmed_mufl;
  int fNmed_yoke;
  int fMu_hi_color;
  int fMu_lo_color;
  int fColor_muid;
  int fColor_hole;
  int fColor_dont;
  int fColor_floor;
  int fColor_strd;
  int fColor_yoke;
  std::vector<float> fRykmin1, fRykmin2;
  std::vector<float> fRmin_donut;
  std::vector<float> fZyoke;
  
  float fZgap_yoke_abs;
  float fZgap_labs_ldet;
  float fMuid_delx;
  float fStr_xstag;
  int fMuhl_shld_flag;
  int fColor_muhl_shld;
  int fNmed_muhl_shld;
  std::vector<float> fZ_muhl_shld;
  float fThick_muhl_shld;
  int fMuhl_config_flag;
  int fMuabs_config_flag;
  int fNpl_muhl;
  std::vector<float> fZ_muhl, fRmin_muhl, fRmax_muhl;


public:
    PHG3toG4MuonIDPara();
    virtual ~PHG3toG4MuonIDPara();


    void InitArrays(int *iData, float *fData);
    

};


#endif
