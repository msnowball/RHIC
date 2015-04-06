#ifndef PHG3toG4MuonArmPara_h
#define PHG3toG4MuonArmPara_h 1

#include <vector>

class PHG3toG4MuonArmPara 
{

private:


  int mumtrflg;
  int mum_arms;
  int mum_stations;
  int mum_channels;
  int mum_color;
  int ArmMedium;
  int HoneyMedium;
  int FEEFlag;
  int StationMedium;
  std::vector<int> PlanesPerStation;//[6]
  std::vector<float> StationOneFrame;//[8]
  std::vector<float> StationOnePanel;//[8]
  std::vector<float> StationOneAnode;//[4]
  std::vector<float> StationOneRead;//[8]
  std::vector<float> StationOneRib;//[7]
  std::vector<float> StationOneGas;//[8]
  std::vector<float> StationOneMount;//[7]
  std::vector<float> StationOneFee;//[10]
  std::vector<float> StationOneAngles;//[48]
  std::vector<float> StationOneOffsets;//[24]
  std::vector<float> StationTwoFFrame;//[14]
  std::vector<float> StationTwoBFrame;//[4]
  std::vector<float> StationTwoFBar;//[4]
  std::vector<float> StationTwoBBar;//[4]
  std::vector<float> StationTwoAnode;//[2]
  float StationTwoFoilThickness;
  float StationTwoAlFoilThickness;
  std::vector<float> StationTwoFRib;//[8]
  std::vector<float> StationTwoBRib;//[8]
  std::vector<float> StationTwoFGas;//[12]
  std::vector<float> StationTwoBGas;//[2]
  std::vector<float> StationTwoAngles;//[96]
  std::vector<float> StationTwoOffsets;//[48]
  std::vector<float> StationThreeFrame;//[18]
  std::vector<float> StationThreePanel;//[14]
  std::vector<float> StationThreeAnode;//[10]
  std::vector<float> StationThreeRib;//[8]
  std::vector<float> StationThreeGas;//[12]
  std::vector<float> StationThreeAngles;//[96]
  std::vector<float> StationThreeOffsets;//[48]
  std::vector<int>   FrameMedium;//[6]
  std::vector<float> StationNominalZpos;//[8]
  int   SpokeMedium;

  int mu_arm_medium;
  std::vector<float> mt_chm_thick1;//[5]
  std::vector<float> mt_chm_thick2;//[5]
  std::vector<int> mt_frame_medium1;//[5]
  std::vector<int> mt_frame_medium2;//[5]
  std::vector<float> mt_frame_side_thick1;//[5]
  std::vector<float> mt_frame_side_thick2;//[5]
  std::vector<float> mt_frame_end_thick1;//[5]
  std::vector<float> mt_frame_end_thick2;//[5]
  std::vector<int> mt_planes_per_station1;//[5]
  std::vector<int> mt_planes_per_station2;//[5]
  std::vector<int> mt_station_medium1;//[5]
  std::vector<int> mt_station_medium2;//[5]
  std::vector<float> mt_plane_thickness1;//[5]
  std::vector<float> mt_plane_thickness2;//[5]
  std::vector<float> mt_plane_spacing11;//[10]
  std::vector<float> mt_plane_spacing12;//[10]
  std::vector<float> mt_plane_spacing13;//[10]
  std::vector<float> mt_plane_spacing14;//[10]
  std::vector<float> mt_plane_spacing15;//[10]
  std::vector<float> mt_plane_spacing21;//[10]
  std::vector<float> mt_plane_spacing22;//[10]
  std::vector<float> mt_plane_spacing23;//[10]
  std::vector<float> mt_plane_spacing24;//[10]
  std::vector<float> mt_plane_spacing25;//[10]
  std::vector<float> mt_station_z1;//[5]
  std::vector<float> mt_station_z2;//[5]
  std::vector<float> mt_station_inner_radius1;//[5]
  std::vector<float> mt_station_inner_radius2;//[5]
  std::vector<float> mt_station_outer_radius1;//[5]
  std::vector<float> mt_station_outer_radius2;//[5]




public:
    PHG3toG4MuonArmPara();
    virtual ~PHG3toG4MuonArmPara();


    void InitArrays(int *iData, float *fData);
    

};


#endif
