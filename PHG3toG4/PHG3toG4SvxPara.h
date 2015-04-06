#ifndef PHG3toG4SvxPara_h
#define PHG3toG4SvxPara_h 1

#include <vector>

class PHG3toG4SvxPara 
{

private:

    int fInitialized;
    int fSili_br_nLayers;
    int fSili_sideLayers;
    int fnhh;
    int fnbrv;
    int fnecv;
    
    float fSili_cg_rmn;
    float fSili_cg_thck;
    float fSili_cg_inthck;
    float fSili_cg_tempc;
    int fSili_cg_npcon;
    std::vector<float> fSili_cg_z;
    std::vector<float> fSili_cg_rmx;
    float fSili_cg_xdisp;
    float fSili_cg_ydisp;
    float fSili_cg_zdisp;
    std::vector<float> fSili_br_snhalfx;
    std::vector<float> fSili_br_snhalfy;
    std::vector<float> fSili_br_snhalfz;
    std::vector<float> fSili_br_x0add;
    std::vector<float> fSili_br_snzgap;
    std::vector<float> fSili_br_tilt;
    std::vector<int> fSili_br_nsn;
    std::vector<float> fSili_br_r;
    std::vector<float> fSili_br_z;
    std::vector<float> fSili_br_dphi;
    std::vector<int> fSili_br_nsec;
    std::vector< std::vector<float> > fSili_br_phic;
    std::vector< std::vector<int> > fSili_br_nlad;
    std::vector<float> fSili_phi1_side;
    std::vector<float> fSili_dph_side;
    std::vector<float> fSili_z1_side;
    std::vector<float> fSili_rmin1_side;
    std::vector<float> fSili_rmax1_side;
    std::vector<float> fSili_z2_side;
    std::vector<float> fSili_rmin2_side;
    std::vector<float> fSili_rmax2_side;
    std::vector<float> fSili_npdv_side;
    std::vector<float> fSili_nz_side;
    std::vector<float> fSili_zCenter_side;


public:
    PHG3toG4SvxPara();
    virtual ~PHG3toG4SvxPara();

    int initialized()
    { return fInitialized;}
    
    int sili_br_nlayers()
    { return fSili_br_nLayers;}
    
    int sili_sidelayers()
    { return fSili_sideLayers;}
    
    int nhh()
    { return fnhh;}
    
    int nbrv()
    { return fnbrv;}

    int necv()
    { return fnecv;}

    float sili_cg_rmn()
    { return fSili_cg_rmn;}
    
    float sili_cg_thck()
    { return fSili_cg_thck;}

    float sili_cg_inthck()
    { return fSili_cg_inthck;}

    float sili_cg_tempc()
    { return fSili_cg_tempc;}

    int sili_cg_npcon()
    { return fSili_cg_npcon;}

    float sili_cg_z(int i)
    { return fSili_cg_z[i-1];}

    float sili_cg_rmx(int i)
    { return fSili_cg_rmx[i-1];}

    float sili_cg_xdisp()
    { return fSili_cg_xdisp;}

    float sili_cg_ydisp()
    { return fSili_cg_ydisp;}

    float sili_cg_zdisp()
    { return fSili_cg_zdisp;}
    
    float sili_br_snhalfx(int i)
    { return fSili_br_snhalfx[i-1];}

    float sili_br_snhalfy(int i)
    { return fSili_br_snhalfy[i-1];}

    float sili_br_snhalfz(int i)
    { return fSili_br_snhalfz[i-1];}

    float sili_br_x0add(int i)
    { return fSili_br_x0add[i-1];}

    float sili_br_snzgap(int i)
    { return fSili_br_snzgap[i-1];}

    float sili_br_tilt(int i)
    { return fSili_br_tilt[i-1];}

    int sili_br_nsn(int i)
    { return fSili_br_nsn[i-1];}

    float sili_br_r(int i)
    { return fSili_br_r[i-1];}

    float sili_br_z(int i)
    { return fSili_br_z[i-1];}

    float sili_br_dphi(int i)
    { return fSili_br_dphi[i-1];}
    
    int sili_br_nsec(int i)
    { return fSili_br_nsec[i-1];}

    float sili_br_phic(int i, int j)
    { return fSili_br_phic[i-1][j-1];}

    int sili_br_nlad(int i, int j)
    { return fSili_br_nlad[i-1][j-1];}

    float sili_phi1_side(int i)
    { return fSili_phi1_side[i-1];}

    float sili_dph_side(int i)
    { return fSili_dph_side[i-1];}

    float sili_z1_side(int i)
    { return fSili_z1_side[i-1];}

    float sili_rmin1_side(int i)
    { return fSili_rmin1_side[i-1];}

    float sili_rmax1_side(int i)
    { return fSili_rmax1_side[i-1];}

    float sili_z2_side(int i)
    { return fSili_z2_side[i-1];}

    float sili_rmin2_side(int i)
    { return fSili_rmin2_side[i-1];}

    float sili_rmax2_side(int i)
    { return fSili_rmax2_side[i-1];}

    float sili_npdv_side(int i)
    { return fSili_npdv_side[i-1];}

    float sili_nz_side(int i)
    { return fSili_nz_side[i-1];}

    float sili_zcenter_side(int i)
    { return fSili_zCenter_side[i-1];}

    void InitArrays(int *iData, float *fData);



    

};


#endif
