#include "PHG3toG4BbcPara.h"
#include <vector>


PHG3toG4BbcPara::PHG3toG4BbcPara()
{

  fColor = 26;
  fSeen = 26;
  fMedabs = 26;
  fMedatt = 26;
  fMedbac = 26;
  fMedcov = 32;
  fMedfro = 202;
  fMedmot = 201;
  fMedpmt = 26;
  fMedqua = 1.085e+9;
  fMedstr = 1.097e+9;

  fAbsorb.push_back(0);
  fAbsorb.push_back(0);
  fAbsorb.push_back(360);

  fBackbd.push_back(2.802e-45);
  fBackbd.push_back(0.5);
  fBackbd.push_back(0.2);
  fBackbd.push_back(5.5);
  
  fCovert = 15;
  fFrontb.push_back(0);
  fFrontb.push_back(0.5);
  fFrontb.push_back(1.09);
  fFrontb.push_back(1.2899999);

  fPmtsiz.push_back(1.401e-45);
  fPmtsiz.push_back(2.2);
  fPmtsiz.push_back(0);
  fPmtsiz.push_back(360);

  fQuartz.push_back(6);
  fQuartz.push_back(2);
  fQuartz.push_back(-0.5);
  fQuartz.push_back(0.2);
  fQuartz.push_back(1.399999);
  fQuartz.push_back(0.5);
  fQuartz.push_back(0.2);
  fQuartz.push_back(1.399999);
  fQuartz.push_back(5.5);
  fQuartz.push_back(15);

  fSpacin = 5.5;
  fStruc.push_back(0);
  fStruc.push_back(12.5);
  fStruc.push_back(144.35);
  fStruc.push_back(-144.35);

  fZposit.push_back(0);
  fZposit.push_back(0);
  fZposit.push_back(2.346e-38);

}


PHG3toG4BbcPara::~PHG3toG4BbcPara()
{}


void PHG3toG4BbcPara::InitArrays(int *iData, float *fData)
{

  iData[0] = color();
  iData[1] = seen();
  iData[2] = medabs();
  iData[3] = medatt();
  iData[4] = medbac();
  iData[5] = medcov();
  iData[6] = medfro();
  iData[7] = medmot();
  iData[8] = medpmt();
  iData[9] = medqua();
  iData[10] = medstr();
  
  fData[11] = absorb(1);
  fData[12] = absorb(2);
  fData[13] = absorb(3);

  fData[24] = backbd(2);
  fData[25] = backbd(3);
  fData[26] = backbd(4);

  fData[27] = covert();
  fData[28] = frontb(2);
  fData[29] = frontb(3);
  fData[30] = frontb(4);
  fData[31] = pmtsiz(2);
  fData[32] = pmtsiz(3);
  fData[33] = pmtsiz(4);

  for (int i = 0; i < 10; i++) fData[14+1] = quartz(i+1);

  fData[44] = spacin();
  fData[45] = struc(2);
  fData[46] = struc(3);
  fData[47] = struc(4);
  fData[48] = zposit(2);
  fData[49] = zposit(3);

}
