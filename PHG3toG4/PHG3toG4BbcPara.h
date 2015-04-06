#ifndef PHG3toG4BbcPara_h
#define PHG3toG4BbcPara_h 1

#include <vector>

class PHG3toG4BbcPara 
{

private:

  int fColor;
  int fSeen;
  int fMedabs;
  int fMedatt;
  int fMedbac;
  int fMedcov;
  int fMedfro;
  int fMedmot;
  int fMedpmt;
  int fMedqua;
  int fMedstr;

  std::vector<float> fAbsorb;
  std::vector<float> fBackbd;
  
  float fCovert;
  std::vector<float> fFrontb;
  std::vector<float> fPmtsiz;
  std::vector<float> fQuartz;
  
  float fSpacin;
  std::vector<float> fStruc;
  std::vector<float> fZposit;


public:
    PHG3toG4BbcPara();
    virtual ~PHG3toG4BbcPara();

    int color(){return fColor;}
    int seen(){return fSeen;}
    int medabs(){return fMedabs;}
    int medatt(){return fMedatt;}
    int medbac(){return fMedbac;}
    int medcov(){return fMedcov;}
    int medfro(){return fMedfro;}
    int medmot(){return fMedmot;}
    int medpmt(){return fMedpmt;}
    int medqua(){return fMedqua;}
    int medstr(){return fMedstr;}

    float absorb(int i){return fAbsorb[i-1];}
    float backbd(int i){return fBackbd[i-1];}
    float covert(){return fCovert;}
    float frontb(int i){return fFrontb[i-1];}
    float pmtsiz(int i){return fPmtsiz[i-1];}
    float quartz(int i){return fQuartz[i-1];}
    float spacin(){return fSpacin;}
    float struc(int i){return fStruc[i-1];}
    float zposit(int i){return fZposit[i-1];}

    void InitArrays(int *iData, float *fData);
    

};


#endif
