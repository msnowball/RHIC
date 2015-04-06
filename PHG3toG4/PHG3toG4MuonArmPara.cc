#include "PHG3toG4MuonArmPara.h"
#include <vector>


PHG3toG4MuonArmPara::PHG3toG4MuonArmPara()
{

  mumtrflg = 5;
  mum_arms = 2;
  mum_stations = 3;
  mum_channels = 100;
  mum_color = 4;
  ArmMedium = 19;
  HoneyMedium = 452;
  FEEFlag = 1;
  StationMedium = 70;
  PlanesPerStation.push_back(3);
  PlanesPerStation.push_back(3);
  PlanesPerStation.push_back(2);
  PlanesPerStation.push_back(3);
  PlanesPerStation.push_back(3);
  PlanesPerStation.push_back(2);

  StationOneFrame.push_back(20.684);
  StationOneFrame.push_back(147.317);
  StationOneFrame.push_back(17.017);
  StationOneFrame.push_back(131.88499);
  StationOneFrame.push_back(0.6349999);
  StationOneFrame.push_back(6.5040001);
  StationOneFrame.push_back(3.8099999);
  StationOneFrame.push_back(2.5369999);

  StationOnePanel.push_back(20.379999);
  StationOnePanel.push_back(142.636);
  StationOnePanel.push_back(19.990999);
  StationOnePanel.push_back(139.58999);
  StationOnePanel.push_back(1.4019999);
  StationOnePanel.push_back(1.1679999);
  StationOnePanel.push_back(0.0049999);
  StationOnePanel.push_back(3.536);

  StationOneAnode.push_back(18.406);
  StationOneAnode.push_back(134.55099);
  StationOneAnode.push_back(0.6439999);
  StationOneAnode.push_back(2.483);

  StationOneRead.push_back(2.983);
  StationOneRead.push_back(110.429);
  StationOneRead.push_back(0.107);
  StationOneRead.push_back(81.425003);
  StationOneRead.push_back(2.0339999);
  StationOneRead.push_back(142.63499);
  StationOneRead.push_back(144.92799);
  StationOneRead.push_back(3.535);

  StationOneRib.push_back(0.6349999);
  StationOneRib.push_back(107.79799);
  StationOneRib.push_back(79.43);
  StationOneRib.push_back(32.740001);
  StationOneRib.push_back(1.4989999);
  StationOneRib.push_back(107.79799);
  StationOneRib.push_back(60.452999);

  StationOneGas.push_back(15.579);
  StationOneGas.push_back(117.577);
  StationOneGas.push_back(15.354);
  StationOneGas.push_back(6.019);
  StationOneGas.push_back(29.069999);
  StationOneGas.push_back(131.07099);
  StationOneGas.push_back(2.397);
  StationOneGas.push_back(1.3389999);

  StationOneMount.push_back(1.269999);
  StationOneMount.push_back(2.539999);
  StationOneMount.push_back(23.5);
  StationOneMount.push_back(152.3);
  StationOneMount.push_back(25.500999);
  StationOneMount.push_back(135.328);
  StationOneMount.push_back(5.079999);

  StationOneFee.push_back(1);
  StationOneFee.push_back(22.5);
  StationOneFee.push_back(162.5);
  StationOneFee.push_back(32.5);
  StationOneFee.push_back(87.3450001);
  StationOneFee.push_back(7.9380002);
  StationOneFee.push_back(77.751998);
  StationOneFee.push_back(22.299999);
  StationOneFee.push_back(26.847999);
  StationOneFee.push_back(13.401);

  StationOneAngles.push_back(90);
  StationOneAngles.push_back(22.5);
  StationOneAngles.push_back(90);
  StationOneAngles.push_back(112.5);
  StationOneAngles.push_back(0);
  StationOneAngles.push_back(0);
  StationOneAngles.push_back(90);
  StationOneAngles.push_back(112.5);
  StationOneAngles.push_back(90);
  StationOneAngles.push_back(202.5);

  StationOneAngles.push_back(0);
  StationOneAngles.push_back(0);
  StationOneAngles.push_back(90);
  StationOneAngles.push_back(202.5);
  StationOneAngles.push_back(90);
  StationOneAngles.push_back(292.5);
  StationOneAngles.push_back(0);
  StationOneAngles.push_back(0);
  StationOneAngles.push_back(90);
  StationOneAngles.push_back(292.5);

  StationOneAngles.push_back(90);
  StationOneAngles.push_back(22.5);
  StationOneAngles.push_back(0);
  StationOneAngles.push_back(0);
  StationOneAngles.push_back(90);
  StationOneAngles.push_back(22.5);
  StationOneAngles.push_back(90);
  StationOneAngles.push_back(112.5);
  StationOneAngles.push_back(0);
  StationOneAngles.push_back(0);

  StationOneAngles.push_back(90);
  StationOneAngles.push_back(112.5);
  StationOneAngles.push_back(90);
  StationOneAngles.push_back(202.5);
  StationOneAngles.push_back(0);
  StationOneAngles.push_back(0);
  StationOneAngles.push_back(90);
  StationOneAngles.push_back(202.5);
  StationOneAngles.push_back(90);
  StationOneAngles.push_back(292.5);

  StationOneAngles.push_back(0);
  StationOneAngles.push_back(0);
  StationOneAngles.push_back(90);
  StationOneAngles.push_back(292.5);
  StationOneAngles.push_back(90);
  StationOneAngles.push_back(22.5);
  StationOneAngles.push_back(0);
  StationOneAngles.push_back(0);

  StationOneOffsets.push_back(0);
  StationOneOffsets.push_back(0);
  StationOneOffsets.push_back(0);
  StationOneOffsets.push_back(0);
  StationOneOffsets.push_back(0);
  StationOneOffsets.push_back(0);
  StationOneOffsets.push_back(0);
  StationOneOffsets.push_back(0);
  StationOneOffsets.push_back(0);
  StationOneOffsets.push_back(0);
  StationOneOffsets.push_back(0);
  StationOneOffsets.push_back(0);

  StationOneOffsets.push_back(0);
  StationOneOffsets.push_back(0);
  StationOneOffsets.push_back(0.006);
  StationOneOffsets.push_back(0);
  StationOneOffsets.push_back(0);
  StationOneOffsets.push_back(-0.014);
  StationOneOffsets.push_back(0);
  StationOneOffsets.push_back(0);
  StationOneOffsets.push_back(0.0509999);
  StationOneOffsets.push_back(0);
  StationOneOffsets.push_back(0);
  StationOneOffsets.push_back(0);

  StationTwoFFrame.push_back(66.5);
  StationTwoFFrame.push_back(265.949);
  StationTwoFFrame.push_back(52.9);
  StationTwoFFrame.push_back(239.205);
  StationTwoFFrame.push_back(2.841);
  StationTwoFFrame.push_back(-9.173999);
  StationTwoFFrame.push_back(-3.799999);
  StationTwoFFrame.push_back(68);
  StationTwoFFrame.push_back(227.37899);
  StationTwoFFrame.push_back(53.95);
  StationTwoFFrame.push_back(199.899);
  StationTwoFFrame.push_back(2.841);
  StationTwoFFrame.push_back(-9.173999);
  StationTwoFFrame.push_back(-3.799999);

  StationTwoBFrame.push_back(275.37899);
  StationTwoBFrame.push_back(248.02099);
  StationTwoBFrame.push_back(236.473);
  StationTwoBFrame.push_back(208.99299);

  StationTwoFBar.push_back(152.19);
  StationTwoFBar.push_back(154.09199);
  StationTwoFBar.push_back(138.959);
  StationTwoFBar.push_back(141.181);

  StationTwoBBar.push_back(162.164);
  StationTwoBBar.push_back(164.069);
  StationTwoBBar.push_back(143.959);
  StationTwoBBar.push_back(146.181);

  StationTwoAnode.push_back(0.1587499);
  StationTwoAnode.push_back(0.1587499);

  StationTwoFoilThickness = 0.0025;
  StationTwoAlFoilThickness = 0.005;
  
  StationTwoFRib.push_back(0.5);
  StationTwoFRib.push_back(188.119);
  StationTwoFRib.push_back(135.43499);
  StationTwoFRib.push_back(56.09899);
  StationTwoFRib.push_back(0.5);
  StationTwoFRib.push_back(147.654);
  StationTwoFRib.push_back(118.051);
  StationTwoFRib.push_back(48.897998);

  StationTwoBRib.push_back(0.5);
  StationTwoBRib.push_back(196.513);
  StationTwoBRib.push_back(139.43299);
  StationTwoBRib.push_back(57.755001);
  StationTwoBRib.push_back(0.5);
  StationTwoBRib.push_back(156.748);
  StationTwoBRib.push_back(122.25099);
  StationTwoBRib.push_back(50.638);

  StationTwoFGas.push_back(53.521999);
  StationTwoFGas.push_back(228.91);
  StationTwoFGas.push_back(9.8269996);
  StationTwoFGas.push_back(3.799999);
  StationTwoFGas.push_back(9.6359996);
  StationTwoFGas.push_back(4.262);
  StationTwoFGas.push_back(54.992);
  StationTwoFGas.push_back(193);
  StationTwoFGas.push_back(9.8269996);
  StationTwoFGas.push_back(3.799999);
  StationTwoFGas.push_back(9.6359996);
  StationTwoFGas.push_back(4.262);

  StationTwoBGas.push_back(239.022);
  StationTwoBGas.push_back(201.99899);

  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(337.5);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(67.5);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(22.5);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(112.5);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(0);

  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(67.5);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(157.5);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(112.5);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(202.5);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(0);

  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(157.5);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(247.5);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(202.5);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(292.5);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(0);

  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(247.5);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(337.5);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(292.5);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(22.5);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(0);

  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(337.5);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(67.5);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(22.5);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(112.5);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(0);

  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(67.5);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(157.5);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(112.5);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(202.5);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(0);

  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(157.5);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(247.5);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(202.5);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(292.5);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(0);

  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(247.5);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(337.5);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(292.5);
  StationTwoAngles.push_back(90);
  StationTwoAngles.push_back(22.5);
  StationTwoAngles.push_back(0);
  StationTwoAngles.push_back(0);

  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);

  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);

  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0.024);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(-0.07);

  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0.062);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(-0.095);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0.055);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(0);
  StationTwoOffsets.push_back(-0.113);

  StationThreeFrame.push_back(98.472);
  StationThreeFrame.push_back(427.47198);
  StationThreeFrame.push_back(94.501998);
  StationThreeFrame.push_back(418.42199);
  StationThreeFrame.push_back(0.6349999);
  StationThreeFrame.push_back(6.1329999);
  StationThreeFrame.push_back(2.5399999);
  StationThreeFrame.push_back(6.0349998);
  StationThreeFrame.push_back(2.5);

  StationThreeFrame.push_back(88.093002);
  StationThreeFrame.push_back(321.09298);
  StationThreeFrame.push_back(84.383003);
  StationThreeFrame.push_back(311.18301);
  StationThreeFrame.push_back(0.6349999);
  StationThreeFrame.push_back(6.1329999);
  StationThreeFrame.push_back(2.5399999);
  StationThreeFrame.push_back(6.0349998);
  StationThreeFrame.push_back(2.5);

  StationThreePanel.push_back(96.48699);
  StationThreePanel.push_back(422.94699);
  StationThreePanel.push_back(2.1649999);
  StationThreePanel.push_back(1.968);
  StationThreePanel.push_back(0.005);
  StationThreePanel.push_back(3.0669999);
  StationThreePanel.push_back(1.2699999);
  StationThreePanel.push_back(86.108001);
  StationThreePanel.push_back(316.56799);
  StationThreePanel.push_back(2.1649999);
  StationThreePanel.push_back(1.968);
  StationThreePanel.push_back(0.005);
  StationThreePanel.push_back(3.0669999);
  StationThreePanel.push_back(1.2699999);

  StationThreeAnode.push_back(89.483001);
  StationThreeAnode.push_back(412.28299);
  StationThreeAnode.push_back(0.6349999);
  StationThreeAnode.push_back(11.105999);
  StationThreeAnode.push_back(4.5999999);
  StationThreeAnode.push_back(75.847999);
  StationThreeAnode.push_back(302.64801);
  StationThreeAnode.push_back(0.6349999);
  StationThreeAnode.push_back(11.105999);
  StationThreeAnode.push_back(4.5999999);

  StationThreeRib.push_back(2.5399999);
  StationThreeRib.push_back(327.255);
  StationThreeRib.push_back(235.828);
  StationThreeRib.push_back(97.682998);
  StationThreeRib.push_back(2.539999);
  StationThreeRib.push_back(228.35499);
  StationThreeRib.push_back(180.36);
  StationThreeRib.push_back(74.708999);

  StationThreeGas.push_back(87.422996);
  StationThreeGas.push_back(405.42999);
  StationThreeGas.push_back(3.318);
  StationThreeGas.push_back(0);
  StationThreeGas.push_back(2.348);
  StationThreeGas.push_back(2.348);
  StationThreeGas.push_back(76.547996);
  StationThreeGas.push_back(298.04901);
  StationThreeGas.push_back(3.318);
  StationThreeGas.push_back(0);
  StationThreeGas.push_back(2.348);
  StationThreeGas.push_back(2.348);

  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(337.5);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(67.5);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(22.5);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(112.5);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(0);

  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(67.5);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(157.5);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(112.5);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(202.5);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(0);

  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(157.5);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(247.5);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(202.5);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(292.5);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(0);

  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(247.5);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(337.5);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(292.5);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(22.5);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(0);

  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(337.5);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(67.5);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(22.5);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(112.5);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(0);

  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(67.5);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(157.5);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(112.5);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(202.5);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(0);

  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(157.5);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(247.5);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(202.5);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(292.5);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(0);

  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(247.5);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(337.5);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(292.5);
  StationThreeAngles.push_back(90);
  StationThreeAngles.push_back(22.5);
  StationThreeAngles.push_back(0);
  StationThreeAngles.push_back(0);
  

  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0.841);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0.725);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0.344);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0.061);

  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0.04);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0.406);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0.752);

  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0.02);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(-0.005);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(-0.011);

  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(-0.034);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(-0.029);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0.02);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0);
  StationThreeOffsets.push_back(0.03);

  FrameMedium.push_back(802);
  FrameMedium.push_back(26);
  FrameMedium.push_back(802);
  FrameMedium.push_back(802);
  FrameMedium.push_back(26);
  FrameMedium.push_back(802);
  
  StationNominalZpos.push_back(186.65);
  StationNominalZpos.push_back(352.24);
  StationNominalZpos.push_back(364.92);
  StationNominalZpos.push_back(613.84);
  StationNominalZpos.push_back(186.561);
  StationNominalZpos.push_back(299.28);
  StationNominalZpos.push_back(311.596);
  StationNominalZpos.push_back(463.133);

  SpokeMedium = 802;
  mu_arm_medium = 0;

  for(int i = 0; i < 5; i++)
    {
      mt_chm_thick1.push_back(0);
      mt_chm_thick2.push_back(0);
      mt_frame_medium1.push_back(0);
      mt_frame_medium2.push_back(0);
      mt_frame_side_thick1.push_back(0);
      mt_frame_side_thick2.push_back(0);
      mt_planes_per_station1.push_back(0);
      mt_frame_end_thick1.push_back(0);
      mt_frame_end_thick2.push_back(0);
      mt_planes_per_station1.push_back(0);
      mt_planes_per_station2.push_back(0);
      mt_station_medium1.push_back(0);
      mt_station_medium2.push_back(0);
      mt_plane_thickness1.push_back(0);
      mt_plane_thickness2.push_back(0);
      mt_station_z1.push_back(0);
      mt_station_z2.push_back(0);
      mt_station_inner_radius1.push_back(0);
      mt_station_inner_radius2.push_back(0);
      mt_station_outer_radius1.push_back(0);
      mt_station_outer_radius2.push_back(0);
    }

  for(int i = 0; i < 10; i++)
    {
      mt_plane_spacing11.push_back(0);
      mt_plane_spacing12.push_back(0);
      mt_plane_spacing13.push_back(0);
      mt_plane_spacing14.push_back(0);
      mt_plane_spacing15.push_back(0);
      mt_plane_spacing21.push_back(0);
      mt_plane_spacing22.push_back(0);
      mt_plane_spacing23.push_back(0);
      mt_plane_spacing24.push_back(0);
      mt_plane_spacing25.push_back(0);
    }


}


PHG3toG4MuonArmPara::~PHG3toG4MuonArmPara()
{}


void PHG3toG4MuonArmPara::InitArrays(int *iData, float *fData)
{

  int i = 0;
  int j = 0;
  int k = 0;
  int p = 0;

  //  float junk = fData[p++];                                                                                                                                                                                                                                                    
  p++;
  fData[p++] = mumtrflg;
  fData[p++] = mum_arms;
  fData[p++] = mum_stations;
  fData[p++] = mum_channels;
  fData[p++] = mum_color;
  fData[p++] = ArmMedium;

  for (j = 0; j < 2; j++) {
    for (i = 0; i < 3; i++) {
      fData[p++] = PlanesPerStation[j*3+i];
    }
  }

  fData[p++] = StationMedium;
  fData[p++] = HoneyMedium;
  fData[p++] = FEEFlag;

  for (i = 0; i < 8; i++) {
    fData[p++] = StationOneFrame[i];
  }
  for (i = 0; i < 8; i++) {  
    fData[p++] = StationOnePanel[i];
  }
  for (i = 0; i < 4; i++) {
    fData[p++] = StationOneAnode[i];
  }
  for (i = 0; i < 8; i++) {
    fData[p++] = StationOneRead[i];
  }
  for (i = 0; i < 7; i++) {
    fData[p++] = StationOneRib[i];
  }
  for (i = 0; i < 8; i++) {
    fData[p++] = StationOneGas[i];
  }
  for (i = 0; i < 7; i++) {
    fData[p++] = StationOneMount[i];
  }
  for (i = 0; i < 10; i++) {
    fData[p++] = StationOneFee[i];
  }
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 4; i++) {
      for (k=0; k<6; k++){
	fData[p++] = StationOneAngles[j*4*6 + i*6 + k];
      }
    }
  }
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 4; i++) {
      for (k=0; k<3; k++){
	fData[p++] = StationOneOffsets[j*4*3 + i*3 + k];
      }
    }
  }
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 7; i++) {
      fData[p++] = StationTwoFFrame[j*7 + i];
    }
  }
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 2; i++) {
      fData[p++] = StationTwoBFrame[2*j + i];
    }
  }
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 2; i++) {
      fData[p++] = StationTwoFBar[2*j + i];
    }
  }
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 2; i++) {
      fData[p++] = StationTwoBBar[2*j + i];
    }
  }
  for (j = 0; j < 2; j++) {
    fData[p++] = StationTwoAnode[j];
  }
  fData[p++] = StationTwoFoilThickness;
  fData[p++] = StationTwoAlFoilThickness;
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 4; i++) {
      fData[p++] = StationTwoFRib[4*j + i];
    }
  }
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 4; i++) {
      fData[p++] = StationTwoBRib[4*j + i];
    }
  }
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 6; i++) {
      fData[p++] = StationTwoFGas[6*j + i];
    }
  }
  for (j = 0; j < 2; j++) {
    fData[p++] = StationTwoBGas[j];
  }
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 8; i++) {
      for (k=0; k<6; k++){
	fData[p++] = StationTwoAngles[j*8*6 + i*6 + k];
      }
    }
  }
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 8; i++) {
      for (k=0; k<3; k++){
	fData[p++] = StationTwoOffsets[j*8*3 + i*3 + k];
      }
    }
  }
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 9; i++) {
      fData[p++] = StationThreeFrame[j*9 + i];
    }
  }
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 7; i++) {
      fData[p++] =  StationThreePanel[j*7 + i];
    }
  }
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 5; i++) {
      fData[p++] = StationThreeAnode[j*5 + i];
    }
  }
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 4; i++) {
      fData[p++] = StationThreeRib[j*4 + i];
    }
  }
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 6; i++) {
      fData[p++] = StationThreeGas[j*6 + i];
    }
  }
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 8; i++) {
      for (k=0; k<6; k++){
	fData[p++] = StationThreeAngles[j*8*6 + i*6 + k];
      }
    }
  }
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 8; i++) {
      for (k=0; k<3; k++){
	fData[p++] = StationThreeOffsets[j*8*3 + i*3 + k];
      }
    }
  }
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 3; i++) {
      fData[p++] = FrameMedium[j*3 + i];
    }
  }
  for (j = 0; j < 2; j++) {
    for (i = 0; i < 3+1; i++) {
      fData[p++] = StationNominalZpos[j*(3+1) + i];
    }
  }
  fData[p++] = SpokeMedium;

  while(p <= 1000)
    {
      fData[p++] = 0;
    }
  
}
