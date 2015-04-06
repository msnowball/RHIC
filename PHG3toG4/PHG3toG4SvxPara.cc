#include "PHG3toG4SvxPara.h"
#include <vector>
#include <iostream>

using namespace std;

PHG3toG4SvxPara::PHG3toG4SvxPara()
{

  fInitialized = 1;
  fSili_br_nLayers = 4;
  fSili_sideLayers = 16;
  fnhh = 21;
  fnbrv = 5;
  fnecv = 9;
    
  fSili_cg_rmn = 2.2;
  fSili_cg_thck = 0.5;
  fSili_cg_inthck = 0.2;
  fSili_cg_tempc = 0;
  fSili_cg_npcon = 6;
  fSili_cg_z.push_back(-40.0);
  fSili_cg_z.push_back(-26.5);

  fSili_cg_rmx.push_back(53);
  fSili_cg_rmx.push_back(53);

  fSili_cg_xdisp = -26.5;
  fSili_cg_ydisp = 22.147819;
  fSili_cg_zdisp = 26.5;

  fSili_br_snhalfx.push_back(22.147819);
  fSili_br_snhalfx.push_back(2.6300001);
  fSili_br_snhalfx.push_back(13.6);
  fSili_br_snhalfx.push_back(0);

  fSili_br_snhalfy.push_back(26.5);
  fSili_br_snhalfy.push_back(0);
  fSili_br_snhalfy.push_back(1);
  fSili_br_snhalfy.push_back(8);

  fSili_br_snhalfz.push_back(53);
  fSili_br_snhalfz.push_back(26.7);
  fSili_br_snhalfz.push_back(0);
  fSili_br_snhalfz.push_back(1.745);

  fSili_br_x0add.push_back(40);
  fSili_br_x0add.push_back(1);
  fSili_br_x0add.push_back(10);
  fSili_br_x0add.push_back(0.03125);

  fSili_br_snzgap.push_back(53);
  fSili_br_snzgap.push_back(0);
  fSili_br_snzgap.push_back(1.745);
  fSili_br_snzgap.push_back(3.1877);

  fSili_br_tilt.push_back(0);
  fSili_br_tilt.push_back(5);
  fSili_br_tilt.push_back(0.03125);
  fSili_br_tilt.push_back(0);

  fSili_br_nsn.push_back(0);
  fSili_br_nsn.push_back(0);
  fSili_br_nsn.push_back(3);
  fSili_br_nsn.push_back(0);

  fSili_br_r.push_back(0);
  fSili_br_r.push_back(0.00999999);
  fSili_br_r.push_back(0);
  fSili_br_r.push_back(0);

  fSili_br_z.push_back(0.6959999);
  fSili_br_z.push_back(2.8359999);
  fSili_br_z.push_back(0);
  fSili_br_z.push_back(6);

  fSili_br_dphi.push_back(0.0099999);
  fSili_br_dphi.push_back(0.0189999);
  fSili_br_dphi.push_back(0);
  fSili_br_dphi.push_back(16.687);

  fSili_br_nsec.push_back(2);
  fSili_br_nsec.push_back(0);
  fSili_br_nsec.push_back(5);
  fSili_br_nsec.push_back(0);

  std::vector<float> tmpPhic1;
  tmpPhic1.push_back(0.0189999);
  tmpPhic1.push_back(0);
  tmpPhic1.push_back(11.765);
  tmpPhic1.push_back(0);
  fSili_br_phic.push_back(tmpPhic1);
  std::vector<float> tmpPhic2;
  tmpPhic2.push_back(13);
  tmpPhic2.push_back(0);
  tmpPhic2.push_back(17);
  tmpPhic2.push_back(0);
  fSili_br_phic.push_back(tmpPhic2);

  std::vector<int> tmpLad1;
  tmpLad1.push_back(0);
  tmpLad1.push_back(0);
  tmpLad1.push_back(0);
  tmpLad1.push_back(0);
  fSili_br_nlad.push_back(tmpLad1);
  std::vector<int> tmpLad2;
  tmpLad2.push_back(4);
  tmpLad2.push_back(0);
  tmpLad2.push_back(1.071e9);
  tmpLad2.push_back(0);
  fSili_br_nlad.push_back(tmpLad2);

  fSili_phi1_side.push_back(0);
  fSili_phi1_side.push_back(0);
  fSili_phi1_side.push_back(0);
  fSili_phi1_side.push_back(0);
  fSili_phi1_side.push_back(0);
  fSili_phi1_side.push_back(0);
  fSili_phi1_side.push_back(0);
  fSili_phi1_side.push_back(0);
  fSili_phi1_side.push_back(2.755e-40);
  fSili_phi1_side.push_back(3.643e-44);
  fSili_phi1_side.push_back(0);
  fSili_phi1_side.push_back(5.5);
  fSili_phi1_side.push_back(360);
  fSili_phi1_side.push_back(5.5);
  fSili_phi1_side.push_back(5.584e-39);
  fSili_phi1_side.push_back(1.794e-10);
  fSili_phi1_side.push_back(0);
  fSili_phi1_side.push_back(0);
  fSili_phi1_side.push_back(0);
  fSili_phi1_side.push_back(0);

  fSili_dph_side.push_back(0);
  fSili_dph_side.push_back(0);
  fSili_dph_side.push_back(0);
  fSili_dph_side.push_back(0);
  fSili_dph_side.push_back(0);
  fSili_dph_side.push_back(0);
  fSili_dph_side.push_back(0);
  fSili_dph_side.push_back(0);
  fSili_dph_side.push_back(0);
  fSili_dph_side.push_back(3.643e-44);
  fSili_dph_side.push_back(0);
  fSili_dph_side.push_back(15);
  fSili_dph_side.push_back(6);
  fSili_dph_side.push_back(12.5);
  fSili_dph_side.push_back(2.802e-44);
  fSili_dph_side.push_back(1.401e-45);
  fSili_dph_side.push_back(0);
  fSili_dph_side.push_back(0);
  fSili_dph_side.push_back(0);
  fSili_dph_side.push_back(0);

  fSili_z1_side.push_back(0);
  fSili_z1_side.push_back(0);
  fSili_z1_side.push_back(0);
  fSili_z1_side.push_back(0);
  fSili_z1_side.push_back(0);
  fSili_z1_side.push_back(0);
  fSili_z1_side.push_back(0);
  fSili_z1_side.push_back(0);
  fSili_z1_side.push_back(0);
  fSili_z1_side.push_back(3.643e-44);
  fSili_z1_side.push_back(360);
  fSili_z1_side.push_back(0.5);
  fSili_z1_side.push_back(2);
  fSili_z1_side.push_back(144.35);
  fSili_z1_side.push_back(1.401e-45);
  fSili_z1_side.push_back(6.22e-39);
  fSili_z1_side.push_back(0);
  fSili_z1_side.push_back(0);
  fSili_z1_side.push_back(0);
  fSili_z1_side.push_back(0);

  fSili_rmin1_side.push_back(0);
  fSili_rmin1_side.push_back(0);
  fSili_rmin1_side.push_back(0);
  fSili_rmin1_side.push_back(0);
  fSili_rmin1_side.push_back(0);
  fSili_rmin1_side.push_back(0);
  fSili_rmin1_side.push_back(0);
  fSili_rmin1_side.push_back(0);
  fSili_rmin1_side.push_back(5.628e-39);
  fSili_rmin1_side.push_back(3.643e-44);
  fSili_rmin1_side.push_back(6);
  fSili_rmin1_side.push_back(0.2);
  fSili_rmin1_side.push_back(-1.5);
  fSili_rmin1_side.push_back(-144.35);
  fSili_rmin1_side.push_back(1.401e-45);
  fSili_rmin1_side.push_back(2.802e-39);
  fSili_rmin1_side.push_back(0);
  fSili_rmin1_side.push_back(0);
  fSili_rmin1_side.push_back(0);
  fSili_rmin1_side.push_back(0);

  fSili_rmax1_side.push_back(0);
  fSili_rmax1_side.push_back(0);
  fSili_rmax1_side.push_back(0);
  fSili_rmax1_side.push_back(0);
  fSili_rmax1_side.push_back(0);
  fSili_rmax1_side.push_back(0);
  fSili_rmax1_side.push_back(0);
  fSili_rmax1_side.push_back(0);
  fSili_rmax1_side.push_back(1.401e-45);
  fSili_rmax1_side.push_back(3.643e-44);
  fSili_rmax1_side.push_back(2);
  fSili_rmax1_side.push_back(5.5);
  fSili_rmax1_side.push_back(0);
  fSili_rmax1_side.push_back(0);
  fSili_rmax1_side.push_back(1.401e-45);
  fSili_rmax1_side.push_back(5.604e-39);
  fSili_rmax1_side.push_back(0);
  fSili_rmax1_side.push_back(0);
  fSili_rmax1_side.push_back(0);
  fSili_rmax1_side.push_back(0);

  fSili_z2_side.push_back(0);
  fSili_z2_side.push_back(0);
  fSili_z2_side.push_back(0);
  fSili_z2_side.push_back(0);
  fSili_z2_side.push_back(0);
  fSili_z2_side.push_back(0);
  fSili_z2_side.push_back(0);
  fSili_z2_side.push_back(0);
  fSili_z2_side.push_back(13.140945);
  fSili_z2_side.push_back(4.484e-44);
  fSili_z2_side.push_back(-0.5);
  fSili_z2_side.push_back(15);
  fSili_z2_side.push_back(1.26999999);
  fSili_z2_side.push_back(2.346e-38);
  fSili_z2_side.push_back(5.605e-44);
  fSili_z2_side.push_back(5.590e-39);
  fSili_z2_side.push_back(0);
  fSili_z2_side.push_back(0);
  fSili_z2_side.push_back(0);
  fSili_z2_side.push_back(0);

  fSili_rmin2_side.push_back(0);
  fSili_rmin2_side.push_back(0);
  fSili_rmin2_side.push_back(0);
  fSili_rmin2_side.push_back(0);
  fSili_rmin2_side.push_back(0);
  fSili_rmin2_side.push_back(0);
  fSili_rmin2_side.push_back(0);
  fSili_rmin2_side.push_back(0);
  fSili_rmin2_side.push_back(0);
  fSili_rmin2_side.push_back(2.830e-43);
  fSili_rmin2_side.push_back(0.2);
  fSili_rmin2_side.push_back(0.5);
  fSili_rmin2_side.push_back(1.5);
  fSili_rmin2_side.push_back(2.346e-38);
  fSili_rmin2_side.push_back(2.802e-42);
  fSili_rmin2_side.push_back(0);
  fSili_rmin2_side.push_back(0);
  fSili_rmin2_side.push_back(0);
  fSili_rmin2_side.push_back(0);
  fSili_rmin2_side.push_back(0);

  fSili_rmax2_side.push_back(0);
  fSili_rmax2_side.push_back(0);
  fSili_rmax2_side.push_back(0);
  fSili_rmax2_side.push_back(0);
  fSili_rmax2_side.push_back(0);
  fSili_rmax2_side.push_back(0);
  fSili_rmax2_side.push_back(0);
  fSili_rmax2_side.push_back(0);
  fSili_rmax2_side.push_back(0);
  fSili_rmax2_side.push_back(2.816e-43);
  fSili_rmax2_side.push_back(1.3999999);
  fSili_rmax2_side.push_back(1.09);
  fSili_rmax2_side.push_back(0);
  fSili_rmax2_side.push_back(2.346e-38);
  fSili_rmax2_side.push_back(4.20e-45);
  fSili_rmax2_side.push_back(0);
  fSili_rmax2_side.push_back(0);
  fSili_rmax2_side.push_back(0);
  fSili_rmax2_side.push_back(0);
  fSili_rmax2_side.push_back(0);

  fSili_npdv_side.push_back(0);
  fSili_npdv_side.push_back(0);
  fSili_npdv_side.push_back(0);
  fSili_npdv_side.push_back(0);
  fSili_npdv_side.push_back(0);
  fSili_npdv_side.push_back(0);
  fSili_npdv_side.push_back(0);
  fSili_npdv_side.push_back(0);
  fSili_npdv_side.push_back(6.866e-44);
  fSili_npdv_side.push_back(3.643e-44);
  fSili_npdv_side.push_back(0.5);
  fSili_npdv_side.push_back(1.2899999);
  fSili_npdv_side.push_back(1.2699999);
  fSili_npdv_side.push_back(2.346e-38);
  fSili_npdv_side.push_back(2.662e-44);
  fSili_npdv_side.push_back(0);
  fSili_npdv_side.push_back(0);
  fSili_npdv_side.push_back(0);
  fSili_npdv_side.push_back(0);
  fSili_npdv_side.push_back(0);

  fSili_nz_side.push_back(0);
  fSili_nz_side.push_back(0);
  fSili_nz_side.push_back(0);
  fSili_nz_side.push_back(0);
  fSili_nz_side.push_back(0);
  fSili_nz_side.push_back(0);
  fSili_nz_side.push_back(0);
  fSili_nz_side.push_back(0);
  fSili_nz_side.push_back(8.407e-45);
  fSili_nz_side.push_back(5.5);
  fSili_nz_side.push_back(0.2);
  fSili_nz_side.push_back(2.2);
  fSili_nz_side.push_back(0.3);
  fSili_nz_side.push_back(2.346e-38);
  fSili_nz_side.push_back(0);
  fSili_nz_side.push_back(0);
  fSili_nz_side.push_back(0);
  fSili_nz_side.push_back(0);
  fSili_nz_side.push_back(0);
  fSili_nz_side.push_back(0);

  fSili_zCenter_side.push_back(0);
  fSili_zCenter_side.push_back(0);
  fSili_zCenter_side.push_back(0);
  fSili_zCenter_side.push_back(0);
  fSili_zCenter_side.push_back(0);
  fSili_zCenter_side.push_back(0);
  fSili_zCenter_side.push_back(0);
  fSili_zCenter_side.push_back(0);
  fSili_zCenter_side.push_back(1.401e-45);
  fSili_zCenter_side.push_back(14.5);
  fSili_zCenter_side.push_back(1.3999999);
  fSili_zCenter_side.push_back(0);
  fSili_zCenter_side.push_back(5);
  fSili_zCenter_side.push_back(2.346e-38);
  fSili_zCenter_side.push_back(3.629e+12);
  fSili_zCenter_side.push_back(0);
  fSili_zCenter_side.push_back(0);
  fSili_zCenter_side.push_back(0);
  fSili_zCenter_side.push_back(0);
  fSili_zCenter_side.push_back(0);

}


PHG3toG4SvxPara::~PHG3toG4SvxPara()
{}


void PHG3toG4SvxPara::InitArrays(int *iData, float *fData)
{

  iData[0] = sili_br_nlayers();
  iData[1] = sili_sidelayers();
  iData[2] = nhh();
  iData[3] = nbrv();
  iData[4] = necv();

  fData[0] = sili_cg_rmn();
  fData[1] = sili_cg_thck();
  fData[2] = sili_cg_inthck();
  fData[3] = sili_cg_tempc();
  fData[4] = sili_cg_npcon();
  fData[5] = sili_cg_z(1);
  fData[6] = sili_cg_rmx(1);
  fData[7] = sili_cg_z(2);
  fData[8] = sili_cg_rmx(2);
  fData[9] = sili_cg_xdisp();
  fData[10] = sili_cg_ydisp();
  fData[11] = sili_cg_zdisp();

  //cout << fSili_br_phic[0][0] << "  " << fSili_br_phic[0][1] << "  " << fSili_br_phic[0][2] << "  " << fSili_br_phic[0][3] << endl;
  //cout << fSili_br_phic[1][0] << "  " << fSili_br_phic[1][1] << "  " << fSili_br_phic[1][2] << "  " << fSili_br_phic[1][3] << endl;

  for(int i = 0; i < sili_br_nlayers(); i++)
    {
      fData[12+i*15] = sili_br_snhalfx(i+1);
      fData[13+i*15] = sili_br_snhalfy(i+1);
      fData[14+i*15] = sili_br_snhalfz(i+1);
      fData[15+i*15] = sili_br_x0add(i+1);
      fData[16+i*15] = sili_br_snzgap(i+1);
      fData[17+i*15] = sili_br_tilt(i+1);
      fData[18+i*15] = sili_br_nsn(i+1);
      fData[19+i*15] = sili_br_r(i+1);
      fData[20+i*15] = sili_br_z(i+1);
      fData[21+i*15] = sili_br_dphi(i+1);
      fData[22+i*15] = sili_br_nsec(i+1);

      for(int j = 0; j < sili_br_nsec(i+1); j++)
	{
	  //cout << i << "  " << j << "   " << 23+i*15+j*2 << "   " << sili_br_phic(i+1,j+1) << endl;
	  //fData[23+i*15+j*2] = sili_br_phic(i+1,j+1);
	  //fData[24+i*15+j*2] = sili_br_nlad(i+1,j+1);
	}

    }

  for(int i = 0; i < sili_sidelayers(); i++)
    {
      fData[72+i*11] = sili_phi1_side(i+1);
      fData[73+i*11] = sili_dph_side(i+1);
      fData[74+i*11] = sili_z1_side(i+1);
      fData[75+i*11] = sili_rmin1_side(i+1);
      fData[76+i*11] = sili_rmax1_side(i+1);
      fData[77+i*11] = sili_z2_side(i+1);
      fData[78+i*11] = sili_rmin2_side(i+1);
      fData[79+i*11] = sili_rmax2_side(i+1);
      fData[80+i*11] = sili_npdv_side(i+1);
      fData[81+i*11] = sili_nz_side(i+1);
      fData[82+i*11] = sili_zcenter_side(i+1);
    }


}
