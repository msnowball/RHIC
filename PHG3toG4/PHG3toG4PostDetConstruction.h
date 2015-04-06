/************************************************************/
/*     Class: PHG3toG4PostDetConstruction                   */
/*                                                          */
/*     Desc: Constructs detector sensitve volumes           */
/*                                                          */
/*                                                          */
/*   Author: Matt Snowball (snowball.at.rcf.rhic.bnl.gov)   */
/*                                                          */
/************************************************************/

#ifndef PHG3toG4PostDetConstruction_h
#define PHG3toG4PostDetConstruction_h 1

#include "TG4RootDetectorConstruction.h"

#include <string>
#include <vector>

class TPolyLine3D;

class PHG3toG4PostDetConstruction : public TVirtualUserPostDetConstruction
{
private:
   
   PHG3toG4PostDetConstruction();
   static PHG3toG4PostDetConstruction *fgInstance; // Self pointer
   
   bool startsWith(std::string input,std::string stringToCheck);

   void setSvxSDList();
   void setMuonArmSDList();
   void setMuonIDSDList();
   void setMuonRPCSDList();
   void setBbcSDList();

   std::vector<std::string> _sensitiveVolumeListSvx;
   std::vector<std::string> _sensitiveVolumeListMuonArm;
   std::vector<std::string> _sensitiveVolumeListMuonID;
   std::vector<std::string> _sensitiveVolumeListMuonRPC;
   std::vector<std::string> _sensitiveVolumeListBbc;


   G4bool _userLimit_SVX;
   G4bool _userLimit_MUI;
   G4bool _userLimit_MUT;
   G4bool _userLimit_MUPC;
   G4bool _userLimit_BBC;

   G4double _maxStep_SVX, _maxTof_SVX, _minKinEnergy_SVX, _minEnergyDep_SVX;
   G4double _maxStep_MUI, _maxTof_MUI, _minKinEnergy_MUI, _minEnergyDep_MUI;
   G4double _maxStep_MUT, _maxTof_MUT, _minKinEnergy_MUT, _minEnergyDep_MUT;
   G4double _maxStep_MUPC, _maxTof_MUPC, _minKinEnergy_MUPC, _minEnergyDep_MUPC;
   G4double _maxStep_BBC, _maxTof_BBC, _minKinEnergy_BBC, _minEnergyDep_BBC;
   

public:
   virtual ~PHG3toG4PostDetConstruction();

   static PHG3toG4PostDetConstruction *GetInstance();

   virtual void          Initialize(TG4RootDetectorConstruction *dc);

   void SetMaxTof(std::string detector, G4double v);
   void SetMaxStep(std::string detector, G4double v);
   void SetMinKinEnergy(std::string detector, G4double e);
   void SetMinEnergyDep(std::string detector, G4double e);
  
};
#endif
   
   

