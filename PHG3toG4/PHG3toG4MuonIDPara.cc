#include "PHG3toG4MuonIDPara.h"
#include <vector>


PHG3toG4MuonIDPara::PHG3toG4MuonIDPara()
{

  fNr_muon_arms = 2;
  fNr_muid_planes = 3;
  fNdet_per_pl = 10; // messed up in tree
  fMu_floor_flg = 1; // messed up in tree
  fMuid_zmin = 695.70001;
  fMuid_zmax = 902.75;
  fMuid_ang = 37;
  fBeam_height = 523.20001;
  fMu_top_height = 1066.8;
  fMu_floor_height = 8;
  fMu_strp_thick = 0.05;
  fMu_gas_thick = 10;
  fMu_plas_thick = 0.15;
  fMu_yoke_thick.push_back(15);
  fMu_yoke_thick.push_back(10);
  fMu_donut_thick1.push_back(10);
  fMu_donut_thick1.push_back(5);
  fMu_donut_thick2.push_back(0);
  fMu_donut_thick2.push_back(10);
  fRmax1_donut1.push_back(116.4);
  fRmax1_donut1.push_back(150);
  fRmax1_donut2.push_back(0);
  fRmax1_donut2.push_back(88.403999);
  fRmax2_donut1.push_back(116.4);
  fRmax2_donut1.push_back(160);
  fRmax2_donut2.push_back(0);
  fRmax2_donut2.push_back(108.40399);
  fMu_floor_thick = 4;
  
  fMu_abs_thick.push_back(0);
  fMu_abs_thick.push_back(5);
  fMu_abs_thick.push_back(5);
  fMu_abs_thick.push_back(10);
  fMu_abs_thick.push_back(10);
  fMu_abs_thick.push_back(10);
  fMu_abs_thick.push_back(0);
  fMu_abs_thick.push_back(2);
  fMu_abs_thick.push_back(2.5);
  fMu_abs_thick.push_back(2.5);

  fMu_hi_abs.push_back(2.5);
  fMu_hi_abs.push_back(41);
  fMu_hi_abs.push_back(41);
  fMu_hi_abs.push_back(41);
  fMu_hi_abs.push_back(41);
  fMu_hi_abs.push_back(41);
  fMu_hi_abs.push_back(41);
  fMu_hi_abs.push_back(447);
  fMu_hi_abs.push_back(447);
  fMu_hi_abs.push_back(447);
  fMu_hi_abs.push_back(447);

  fMu_hi_med.push_back(1.138e9);//crap?
  fMu_hi_med.push_back(1.138e9);
  fMu_hi_med.push_back(1.145e9);
  fMu_hi_med.push_back(1.138e9);
  fMu_hi_med.push_back(1.138e9);
  fMu_hi_med.push_back(1.086e9);
  fMu_hi_med.push_back(1.084e9);
  fMu_hi_med.push_back(1.086e9);
  fMu_hi_med.push_back(1.109e9);
  fMu_hi_med.push_back(1.084e9);

  fMu_lo_med.push_back(1.082e9);
  fMu_lo_med.push_back(1.084e9);
  fMu_lo_med.push_back(1.077e9);
  fMu_lo_med.push_back(1.086e9);
  fMu_lo_med.push_back(1.073e9);
  fMu_lo_med.push_back(1.086e9);
  fMu_lo_med.push_back(1.082e9);
  fMu_lo_med.push_back(1.086e9);
  fMu_lo_med.push_back(1.093e9);
  fMu_lo_med.push_back(1.093e9);
  fMu_lo_med.push_back(1.093e9);

  fNmed_mu_ps = 1.093e9;
  fNmed_mu_gas = 1.093e9;
  fNmed_mu_cs = 1.093e9;
  fNmed_mu_sh = 1.142e9;
  fNmed_mu_sd = -1.00e9;
  fNmed_mudn = 1.112e9;
  fNmed_muhl = 0;
  fNmed_mufl = 0;
  fNmed_yoke = 1.045e9;
  fMu_hi_color = 0;
  fMu_lo_color = 1.082e9;
  fColor_muid = 1.138e9;
  fColor_hole = -1e9;
  fColor_dont = 1.147e9;
  fColor_floor = 1.084e9;
  fColor_strd = 1.073e9;
  fColor_yoke = 1.073e9;
  fRykmin1.push_back(2);
  fRykmin1.push_back(683);
  fRykmin2.push_back(0);
  fRykmin2.push_back(930);
  fRmin_donut.push_back(0);
  fRmin_donut.push_back(0);
  fZyoke.push_back(0);
  fZyoke.push_back(6.349999);
  
  fZgap_yoke_abs = 6.349999;
  fZgap_labs_ldet = 0;
  fMuid_delx = 0;
  fStr_xstag = 0;
  fMuhl_shld_flag = 0;
  fColor_muhl_shld = 1.118e9;
  fNmed_muhl_shld = 1.118e9;
  fZ_muhl_shld.push_back(0);
  fZ_muhl_shld.push_back(0);
  fThick_muhl_shld = 0;
  fMuhl_config_flag = 0;
  fMuabs_config_flag = 196620;
  fNpl_muhl = 0;
  fZ_muhl.push_back(0);
  fZ_muhl.push_back(5.932e-39);
  fZ_muhl.push_back(1.401e-45);
  fZ_muhl.push_back(13.140945);
  fZ_muhl.push_back(0);
  fZ_muhl.push_back(0);
  fRmin_muhl.push_back(4);
  fRmin_muhl.push_back(19);
  fRmin_muhl.push_back(3);
  fRmin_muhl.push_back(3);
  fRmin_muhl.push_back(2);
  fRmin_muhl.push_back(3);
  fRmax_muhl.push_back(3);
  fRmax_muhl.push_back(2);
  fRmax_muhl.push_back(70);
  fRmax_muhl.push_back(452);
  fRmax_muhl.push_back(1);
  fRmax_muhl.push_back(20.684);


}


PHG3toG4MuonIDPara::~PHG3toG4MuonIDPara()
{}


void PHG3toG4MuonIDPara::InitArrays(int *iData, float *fData)
{

  int p = 0;
  int i = 0;

  iData[p++] = fNr_muid_planes;
  iData[p++] = fNdet_per_pl;
  iData[p++] = fMu_floor_flg;
  fData[p++] = fMuid_zmin;
  fData[p++] = fMuid_zmax;
  fData[p++] = fMuid_ang;
  fData[p++] = fBeam_height;
  fData[p++] = fMu_top_height;
  fData[p++] = fMu_floor_height;
  fData[p++] = fMu_strp_thick;
  fData[p++] = fMu_gas_thick;
  fData[p++] = fMu_plas_thick;
  for (i = 0; i < fNr_muon_arms; i++) {
    fData[p++] = fMu_yoke_thick[i];
  }
  for (i = 0; i < fNr_muon_arms; i++) {
    fData[p++] = fMu_donut_thick1[i];
    fData[p++] = fMu_donut_thick2[i];
  }
  for (i = 0; i < fNr_muon_arms; i++) {
    fData[p++] = fRmax1_donut1[i];
    fData[p++] = fRmax1_donut2[i];
    fData[p++] = fRmax2_donut1[i];
    fData[p++] = fRmax2_donut2[i];
  }
  fData[p++] = fMu_floor_thick;
  for (i = 0; i < 11; i++) {
    fData[p++] = fMu_abs_thick[i];
  }
  for (i = 0; i < 11; i++) {
    fData[p++] = fMu_hi_abs[i];
  }
  for (i = 0; i < 11; i++) {
    iData[p++] = fMu_hi_med[i];
  }
  for (i = 0; i < 11; i++) {
    iData[p++] = fMu_lo_med[i];
  }
  iData[p++] = fNmed_mu_ps;
  iData[p++] = fNmed_mu_gas;
  iData[p++] = fNmed_mu_cs;
  iData[p++] = fNmed_mu_sh;
  iData[p++] = fNmed_mu_sd;
  iData[p++] = fNmed_mudn;
  iData[p++] = fNmed_muhl;
  iData[p++] = fNmed_mufl;
  iData[p++] = fNmed_yoke;
  iData[p++] = fMu_hi_color;
  iData[p++] = fMu_lo_color;
  iData[p++] = fColor_muid;
  iData[p++] = fColor_hole;
  iData[p++] = fColor_dont;
  iData[p++] = fColor_floor;
  iData[p++] = fColor_strd;
  iData[p++] = fColor_yoke;
  for (i = 0; i < fNr_muon_arms; i++) {
    fData[p++] = fRykmin1[i];
  }
  for (i = 0; i < fNr_muon_arms; i++) {
    fData[p++] = fRykmin2[i];
  }
  for (i = 0; i < fNr_muon_arms; i++) {
    fData[p++] = fRmin_donut[i];
  }
  for (i = 0; i < fNr_muon_arms; i++) {
    fData[p++] = fZyoke[i];
  }
  fData[p++] = fZgap_yoke_abs;
  fData[p++] = fZgap_labs_ldet;
  fData[p++] = fMuid_delx;
  fData[p++] = fStr_xstag;
  iData[p++] = fMuhl_shld_flag;
  iData[p++] = fColor_muhl_shld;
  iData[p++] = fNmed_muhl_shld;
  fData[p++] = fZ_muhl_shld[0];
  fData[p++] = fZ_muhl_shld[1];
  fData[p++] = fThick_muhl_shld;
  iData[p++] = fMuhl_config_flag;
  iData[p++] = fMuabs_config_flag;
  iData[p++] = fNpl_muhl;
  for (i = 0; i < 6; i++) {
    fData[p++] = fZ_muhl[i];
  }
  for (i = 0; i < 6; i++) {
    fData[p++] = fRmin_muhl[i];
  }
  for (i = 0; i < 6; i++) {
    fData[p++] = fRmin_muhl[i];
  }
  for (i = 0; i < 6; i++) {
    fData[p++] = fRmax_muhl[i];
  }


}
