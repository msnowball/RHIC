#include <iostream>
#include <climits>

#include <getClass.h>
#include <Fun4AllReturnCodes.h>

#include <TLorentzVector.h>

#if ROOT_VERSION_CODE >= ROOT_VERSION(5,15,8) 
#include <TMCParticle.h>
#else
#include <TMCParticle6.h>
#endif

#include <PHPyTrigger.h>
#include <PHPythiaHeader.h>
#include <PHPythiaContainer.h>
#include <PHPyCommon.h>

using namespace std;

PHPyTrigger::PHPyTrigger(const std::string &name): SubsysReco(name)
{
  phpythia = 0;		// array of pythia particles
  phpythiaheader = 0;	// pythia header

  max_events = UINT_MAX;
  ntriggered = 0;
  nconsidered = 0;

  e_threshold = 0.;	// default energy threshold
  p_threshold = 0.;	// default momentum threshold
  pt_threshold = 0.;	// default pt threshold

  trigger_type = CHIC_MUONARM;
}

PHPyTrigger::~PHPyTrigger()
{
}

int PHPyTrigger::Init(PHCompositeNode *topNode)
{
  return EVENT_OK;
}

int PHPyTrigger::End(PHCompositeNode *topNode)
{

  if ( trigger_type == 0 ) cout<<" PHPyTrigger: keep all events "<<endl;	// accept all events
  else if ( trigger_type == PI0_CENTARM ) cout<<" PHPyTrigger: PI0_CENTARM "<<endl;
  else if ( trigger_type == CHIC_MUONARM ) cout<<" PHPyTrigger: CHIC_MUONARM "<<endl;
  else if ( trigger_type == CHIC_MUONARM2 ) cout<<" PHPyTrigger: CHIC_MUONARM2 "<<endl;
  else if ( trigger_type == CHIC_MUONARM3 ) cout<<" PHPyTrigger: CHIC_MUONARM3 "<<endl;
  else if ( trigger_type == CHIC_MUONARM4 ) cout<<" PHPyTrigger: CHIC_MUONARM4 "<<endl;
  else if ( trigger_type == GAMMA_NCC ) cout<<" PHPyTrigger: GAMMA_NCC "<<endl;
  else if ( trigger_type == UPSILON_MUONARM ) cout<<" PHPyTrigger: UPSILON_MUONARM "<<endl;
  else if ( trigger_type == KorPi_MUONARM ) cout<<" PHPyTrigger: KorPi_MUONARM "<<endl;
  else{
    cout<<" UNKNOWN TRIGGER TYPE"<<endl;
    return ABORTRUN;
  }

  //-* dump out trigger statistics
  cout << "PHPyTrigger: Number_Triggered Number_Considered Percentage "
       << ntriggered << " " << nconsidered;
  if ( nconsidered>0 ) cout << " " << float(ntriggered)/nconsidered << endl;
  else                 cout << " nan" << endl;

  return EVENT_OK;
}

int PHPyTrigger::process_event(PHCompositeNode *topNode)
{
  // Get PYTHIA Header (only if you want to trigger on event information)
  phpythiaheader = findNode::getClass<PHPythiaHeader>(topNode,"PHPythiaHeader");
  if (!phpythiaheader)
    {
      cout << PHWHERE << "Unable to get PHPythiaHeader, is Node missing?" << endl;
      return ABORTEVENT;
    }

  // Get PYTHIA Particles
  phpythia = findNode::getClass<PHPythiaContainer>(topNode,"PHPythia");
  if (!phpythia)
    {
      cout << PHWHERE << "Unable to get PHPythia, is Node missing?" << endl;
      return ABORTEVENT;
    }

  ++nconsidered;

  int triggered = 0;
  if ( trigger_type == 0 ) triggered = 1;	// accept all events
  else if ( trigger_type == PI0_CENTARM ) triggered = Pi0InCentralArm(phpythia);
  else if ( trigger_type == CHIC_MUONARM ) triggered = ChiCInMuonArm(phpythia);
  else if ( trigger_type == CHIC_MUONARM2 ) triggered = ChiCInMuonArm2(phpythia);
  else if ( trigger_type == CHIC_MUONARM3 ) triggered = ChiCInMuonArm3(phpythia);
  else if ( trigger_type == CHIC_MUONARM4 ) triggered = ChiCInMuonArm4(phpythia);
  else if ( trigger_type == GAMMA_NCC ) triggered = GammaInNCC(phpythia);
  else if ( trigger_type == UPSILON_MUONARM ) triggered = UpsilonInMuonArm(phpythia);
  else if ( trigger_type == KorPi_MUONARM ) triggered = KorPiInMuonArm(phpythia);
  else{
    cout<<" UNKNOWN TRIGGER TYPE"<<endl;
    return ABORTRUN;
  }

  if ( triggered )
    {
      ++ntriggered;

      // Quit if we reach the desired number of events
      if ( ntriggered > max_events )
        {
          return ABORTRUN;
        }

      return EVENT_OK;
    }

  return ABORTEVENT;	// trigger condition not found, don't write out event
}
//----------------------------------------------------------------------------
// Trigger on a pi0 above threshold in the central arm acceptance.
int PHPyTrigger::Pi0InCentralArm(PHPythiaContainer *phpylist)
{
  // Print Out Trigger Information Once, for Posterity
  static int trig_info_printed = 0;
  if ( trig_info_printed==0 )
    {
      cout << "Pi0InCentralArm, |eta|<0.35 ";
      cout << "Energy Threshold " << e_threshold << endl;
      trig_info_printed = 1;
    }

  int npart = phpylist->size();
  for (int ipart=0; ipart<npart; ipart++)
    {
      TMCParticle *part = phpylist->getParticle(ipart);

      // Check that there is a pi0
      if ( part->GetKF()==PY_PIZERO )
        {
          Float_t px = part->GetPx();
          Float_t py = part->GetPy();
          Float_t pz = part->GetPz();
          Float_t energy = part->GetEnergy();
          TLorentzVector pi0v(px,py,pz,energy);

          // check that pi0 is above threshold
          if ( pi0v.Energy()<e_threshold ) continue;

          // check that pi0 is in acceptance
          if ( InCentralArmAcceptance(pi0v) == 0 ) continue;

          // We found a trigger pi0 above threshold and in acceptance.
          // Return success
          return 1;
        }
    }

  // Went through all particles and did not find a pi0 trigger
  return 0;
}
//----------------------------------------------------------------------------
// Trigger for ChiC in Muon Arm direction
int PHPyTrigger::ChiCInMuonArm(PHPythiaContainer *phpylist)
{
  // Print Out Trigger Information Once, for Posterity
  static int trig_info_printed = 0;
  if ( trig_info_printed==0 )
    {
      cout << "PHPyTrigger::ChiCInMuonArm, any chi0c0, chi1c0, chi2c0 in 1.2<|eta|<2.4";
      trig_info_printed = 1;
    }

  int npart = phpylist->size();
  for (int ipart=0; ipart<npart; ipart++)
    {
      TMCParticle *part = phpylist->getParticle(ipart);

      // Check that there is a chi_C
      int kf = part->GetKF();
      if ( kf==PY_CHI0C0 || kf==PY_CHI1C0 || kf==PY_CHI2C0 )
        {
          Float_t px = part->GetPx();
          Float_t py = part->GetPy();
          Float_t pz = part->GetPz();
          Float_t energy = part->GetEnergy();
          TLorentzVector chic_v(px,py,pz,energy);

          // check that chic is in acceptance
          if ( InMuonArmAcceptance(chic_v) == 0 ) continue;

          // We found a trigger chic in the acceptance.
          // Return success
          return 1;
        }
    }

  // Went through all particles and did not find a trigger
  return 0;
} // end of ChiCInMuonArm
//----------------------------------------------------------------------------

// Trigger for J/psi muons in muon arm (including j/psi from chic)  and ChiC photon in NCC 
int PHPyTrigger::ChiCInMuonArm2(PHPythiaContainer *phpylist)
{
  // Print Out Trigger Information Once, for Posterity
  static int trig_info_printed = 0;
  if ( trig_info_printed==0 )
    {
      cout << "PHPyTrigger::ChiCInMuonArm, any chi0c0, chi1c0, chi2c0 in 1.2<|eta|<2.4";
      trig_info_printed = 1;
    }

  bool muplusaccepted=false;
  bool muminusaccepted=false;
  bool gammaaccepted=false;

  int npart = phpylist->size();
  for (int ipart=0; ipart<npart; ipart++)
    {
      TMCParticle *part = phpylist->getParticle(ipart);
      
      int kf = part->GetKF();
      if ( kf==PY_MU || kf==-PY_MU || kf==PY_GAMMA )
        {
          Float_t px = part->GetPx();
          Float_t py = part->GetPy();
          Float_t pz = part->GetPz();
          Float_t energy = part->GetEnergy();
          TLorentzVector chic_v(px,py,pz,energy);

	  // THIS IS A PAIN, you have to subtract 1 from the line number of the parent!	  
	  int ipart2 = part->GetParent()-1;
	  TMCParticle *part2 =  phpylist->getParticle(ipart2);
	  Int_t kfparent =  part2->GetKF();
	  //	  cout<<" parent id="<<ipart2<<" kfparent="<<kfparent<<endl;
	  //	  Int_t kfparent =  phpylist->getParticle(part->GetParent())->GetKF();
	  //	  cout<<" kfparent="<<kfparent<<endl;
	  
	  if ( kfparent==PY_JPSI && kf==PY_MU){
	    // check that particle is in acceptance
	    if ( InMuonArmAcceptance(chic_v) == 1 ) muminusaccepted=true;
	    //	    cout<<" mu- "<<endl;
	  }
	  
	  if ( kfparent==PY_JPSI && kf== -PY_MU){
	    // check that particle is in acceptance
	    if ( InMuonArmAcceptance(chic_v) == 1 ) muplusaccepted=true;
	    //	    cout<<" mu+ "<<endl;
	  }
	  
	  if ( (kfparent==PY_CHI0C0 || kfparent==PY_CHI1C0 || kfparent==PY_CHI2C0) && kf==PY_GAMMA){
	    // check that particle is in acceptance
	    if ( InNCCAcceptance(chic_v) == 1 ) gammaaccepted=true;
	    //	    cout<<" gamma "<<endl;
	  }
	  
	  
        }
    }



  //  cout<<muplusaccepted<<endl;
  //  cout<<muminusaccepted<<endl;
  //  cout<<gammaaccepted<<endl;


  // We found a trigger where chic muons are in the muon arm and photon is in NCC acceptance.
  // Return success
  if(muplusaccepted && muminusaccepted && gammaaccepted ){
    if ( verbosity ) cout<<" got one "<<endl;
    return 1;
  }

  // Went through all particles and did not find trigger
  return 0;
} // end of ChiCInMuonArm2

//----------------------------------------------------------------------------

// Trigger for J/psi muons in muon arm (including j/psi from chic) 
int PHPyTrigger::ChiCInMuonArm3(PHPythiaContainer *phpylist)
{
  // Print Out Trigger Information Once, for Posterity
  static int trig_info_printed = 0;
  if ( trig_info_printed==0 )
    {
      cout << "PHPyTrigger::ChiCInMuonArm, any chi0c0, chi1c0, chi2c0 in 1.2<|eta|<2.4";
      trig_info_printed = 1;
    }

  bool muplusaccepted=false;
  bool muminusaccepted=false;
  bool gammaaccepted=false;

  int npart = phpylist->size();
  for (int ipart=0; ipart<npart; ipart++)
    {
      TMCParticle *part = phpylist->getParticle(ipart);
      
      int kf = part->GetKF();
      if ( kf==PY_MU || kf==-PY_MU || kf==PY_GAMMA )
        {
          Float_t px = part->GetPx();
          Float_t py = part->GetPy();
          Float_t pz = part->GetPz();
          Float_t energy = part->GetEnergy();
          TLorentzVector chic_v(px,py,pz,energy);

	  // THIS IS A PAIN, you have to subtract 1 from the line number of the parent!	  
	  int ipart2 = part->GetParent()-1;
	  TMCParticle *part2 =  phpylist->getParticle(ipart2);
	  Int_t kfparent =  part2->GetKF();
	  //	  cout<<" parent id="<<ipart2<<" kfparent="<<kfparent<<endl;
	  //	  Int_t kfparent =  phpylist->getParticle(part->GetParent())->GetKF();
	  //	  cout<<" kfparent="<<kfparent<<endl;
	  
	  if ( kfparent==PY_JPSI && kf==PY_MU){
	    // check that particle is in acceptance
	    if ( InMuonArmAcceptance(chic_v) == 1 ) muminusaccepted=true;
	    //	    cout<<" mu- "<<endl;
	  }
	  
	  if ( kfparent==PY_JPSI && kf== -PY_MU){
	    // check that particle is in acceptance
	    if ( InMuonArmAcceptance(chic_v) == 1 ) muplusaccepted=true;
	    //	    cout<<" mu+ "<<endl;
	  }
	  
	  if ( (kfparent==PY_CHI0C0 || kfparent==PY_CHI1C0 || kfparent==PY_CHI2C0) && kf==PY_GAMMA){
	    // check that particle is in acceptance
	    if ( InNCCAcceptance(chic_v) == 1 ) gammaaccepted=true;
	    //	    cout<<" gamma "<<endl;
	  }
	  
	  
        }
    }



  //  cout<<muplusaccepted<<endl;
  //  cout<<muminusaccepted<<endl;
  //  cout<<gammaaccepted<<endl;


  // We found a trigger where chic muons are in the muon arm and photon is in NCC acceptance.
  // Return success
  if(muplusaccepted && muminusaccepted){
    if ( verbosity ) cout<<" got one "<<" gamma also? "<<gammaaccepted<<endl;
    return 1;
  }

  // Went through all particles and did not find trigger
  return 0;
} // end of ChiCInMuonArm3

//----------------------------------------------------------------------------
// Trigger on a gamma above threshold in the NCC acceptance.
int PHPyTrigger::GammaInNCC(PHPythiaContainer *phpylist)
{
  // Print Out Trigger Information Once, for Posterity
  static int trig_info_printed = 0;
  if ( trig_info_printed==0 )
    {
      cout << "GammaInNCC, 1.0<|eta|<3.0 ";
      cout << "Energy Threshold " << e_threshold << endl;
      trig_info_printed = 1;
    }

  int npart = phpylist->size();
  for (int ipart=0; ipart<npart; ipart++)
    {
      TMCParticle *part = phpylist->getParticle(ipart);

      // Check that there is a gamma
      if ( part->GetKF()==PY_GAMMA )
        {
          Float_t px = part->GetPx();
          Float_t py = part->GetPy();
          Float_t pz = part->GetPz();
          Float_t energy = part->GetEnergy();
          TLorentzVector gamv(px,py,pz,energy);

          // check that gamma is above threshold
          if ( gamv.Energy()<e_threshold ) continue;

          // check that pi0 is in acceptance
          if ( InNCCAcceptance(gamv) == 0 ) continue;

cout << "found trigger!" << endl;
          // We found a trigger gamma above threshold and in acceptance.
          // Return success
          return 1;
        }
    }

  // Went through all particles and did not find a pi0 trigger
  return 0;
}

//----------------------------------------------------------------------------

// Trigger for J/psi muons in muon arm (including j/psi from chic)  and ChiC photon in MPC
int PHPyTrigger::ChiCInMuonArm4(PHPythiaContainer *phpylist)
{
  // Print Out Trigger Information Once, for Posterity
  static int trig_info_printed = 0;
  if ( trig_info_printed==0 )
    {
      cout << "PHPyTrigger::ChiCInMuonArm, any chi0c0, chi1c0, chi2c0 in 1.2<|eta|<2.4";
      trig_info_printed = 1;
    }

  bool muplusaccepted=false;
  bool muminusaccepted=false;
  bool gammaaccepted=false;

  int npart = phpylist->size();
  for (int ipart=0; ipart<npart; ipart++)
    {
      TMCParticle *part = phpylist->getParticle(ipart);

      int kf = part->GetKF();
      if ( kf==PY_MU || kf==-PY_MU || kf==PY_GAMMA )
        {
          Float_t px = part->GetPx();
          Float_t py = part->GetPy();
          Float_t pz = part->GetPz();
          Float_t energy = part->GetEnergy();
          TLorentzVector chic_v(px,py,pz,energy);

          // THIS IS A PAIN, you have to subtract 1 from the line number of the parent!
          int ipart2 = part->GetParent()-1;
          TMCParticle *part2 =  phpylist->getParticle(ipart2);
          Int_t kfparent =  part2->GetKF();
          //      cout<<" parent id="<<ipart2<<" kfparent="<<kfparent<<endl;
          //      Int_t kfparent =  phpylist->getParticle(part->GetParent())->GetKF();
          //      cout<<" kfparent="<<kfparent<<endl;

          if ( kfparent==PY_JPSI && kf==PY_MU){
            // check that particle is in acceptance
            if ( InMuonArmAcceptance(chic_v) == 1 ) muminusaccepted=true;
            //      cout<<" mu- "<<endl;
          }

          if ( kfparent==PY_JPSI && kf== -PY_MU){
            // check that particle is in acceptance
            if ( InMuonArmAcceptance(chic_v) == 1 ) muplusaccepted=true;
            //      cout<<" mu+ "<<endl;
          }

          if ( (kfparent==PY_CHI0C0 || kfparent==PY_CHI1C0 || kfparent==PY_CHI2C0) && kf==PY_GAMMA){
            // check that particle is in acceptance
            if ( InMPCAcceptance(chic_v) == 1 ) gammaaccepted=true;
            //      cout<<" gamma "<<endl;
          }

        }
    }



  //  cout<<muplusaccepted<<endl;
  //  cout<<muminusaccepted<<endl;
  //  cout<<gammaaccepted<<endl;


  // We found a trigger where chic muons are in the muon arm and photon is in NCC acceptance.
  // Return success
  if(muplusaccepted && muminusaccepted && gammaaccepted ){
    if ( verbosity ) cout<<" got one "<<endl;
    return 1;
  }

  // Went through all particles and did not find trigger
  return 0;
} // end of ChiCInMuonArm4

//----------------------------------------------------------------------------
// Trigger for Upsilon in Muon Arm direction
int PHPyTrigger::UpsilonInMuonArm(PHPythiaContainer *phpylist)
{
  // Print Out Trigger Information Once, for Posterity
  static int trig_info_printed = 0;
  if ( trig_info_printed==0 )
    {
      cout << "PHPyTrigger::UpsilonInMuonArm, Upsilon in 1.2<|eta|<2.4";
      trig_info_printed = 1;
    }

  int npart = phpylist->size();
  for (int ipart=0; ipart<npart; ipart++)
    {
      TMCParticle *part = phpylist->getParticle(ipart);

      // Check that there is a Upsilon
      int kf = part->GetKF();
      if ( kf==PY_UPSILON1S || kf==PY_UPSILON2S )
        {
          Float_t px = part->GetPx();
          Float_t py = part->GetPy();
          Float_t pz = part->GetPz();
          Float_t energy = part->GetEnergy();
          TLorentzVector chic_v(px,py,pz,energy);

          // check that Upsilon is in acceptance
          if ( InMuonArmAcceptance(chic_v) == 0 ) continue;

          // We found a trigger Upsilon in the acceptance.
          // Return success
          return 1;
        }
    }

  // Went through all particles and did not find a trigger
  return 0;
} // end of UpsilonInMuonArm

//----------------------------------------------------------------------------

// Check for particle in Canonical Central Arm Acceptance
int PHPyTrigger::InCentralArmAcceptance(const TLorentzVector& v)
{
  Double_t Eta = v.Eta();
  if ( fabs(Eta)<0.35 ) return 1;

  return 0;
}

//----------------------------------------------------------------------------

// Check for particle in Canonical Central Arm Acceptance
// This works only for photons or very high pT charged particles
// Vertex position is currently neglected, since EMCal is far away 
// from the event vertex.
int PHPyTrigger::PhotonInCentralArmAcceptance(const TLorentzVector& v, const Double_t zvtx)
{
  Double_t Eta = v.Eta();
  Double_t Phi = v.Phi();
  //std::cout << "phi = " << Phi << std::endl;
  // Phi goes from -M_PI to M_PI
  //                            west arm              upper part of east arm         lower part of east arm
  if ( fabs(Eta)<0.35 && ( (Phi>-0.58 && Phi<0.98) || ((Phi>2.15 && Phi<M_PI) || (Phi>-M_PI && Phi<(3.73-2*M_PI))) ) ) return 1;

  return 0;
}

// Trigger for K or Pi in Muon Arm direction
int PHPyTrigger::KorPiInMuonArm(PHPythiaContainer *phpylist)
{
  // Print Out Trigger Information Once, for Posterity
  static int trig_info_printed = 0;
  if ( trig_info_printed==0 )
    {
      cout << "PHPyTrigger::KorPiInMuonArm, any K+/-, pi+/- in 1.2<|eta|<2.4";
      trig_info_printed = 1;
    }

  int npart = phpylist->size();
  for (int ipart=0; ipart<npart; ipart++)
    {
      TMCParticle *part = phpylist->getParticle(ipart);

      // Check that there is a chi_C
      int kf = part->GetKF();
      if ( kf==PY_K || kf==PY_PI )
        {
          Float_t px = part->GetPx();
          Float_t py = part->GetPy();
          Float_t pz = part->GetPz();
          Float_t energy = part->GetEnergy();
          TLorentzVector korpi_v(px,py,pz,energy);

          // check that chic is in acceptance
          if ( InMuonArmAcceptance(korpi_v) == 0 ) continue;

          // We found a trigger chic in the acceptance.
          // Return success
          return 1;
        }
    }

  // Went through all particles and did not find a trigger
  return 0;
} // end of KorPiInMuonArm
//----------------------------------------------------------------------------


//----------------------------------------------------------------------------
// Check for particle in Canonical Muon Arm Acceptance
int PHPyTrigger::InMuonArmAcceptance(const TLorentzVector& v)
{
  
  Double_t Eta = v.Eta();
  
  // is this the correct acceptance?
  if ( fabs(Eta)>1.2 && fabs(Eta)<2.4 ) return 1;

  return 0;
}
//----------------------------------------------------------------------------
// Check for particle in Canonical NCC Acceptance. (x,y) is assumed to be 0,0.
// zvtx is in cm
int PHPyTrigger::InNCCAcceptance(const TLorentzVector& v, const Double_t zvtx)
{
  // Canonical NCC size, please double-check since NCC is currently changing
  const Double_t NCC_Z = 41.0;		// in cm.
  const Double_t NCC_RMIN = 6.0;
  const Double_t NCC_RMAX = 48.0;

  Double_t pt = sqrt( v.Px()*v.Px() + v.Py()*v.Py() );
  Double_t pz = v.Pz();
  Double_t z = (pz>0.) ? NCC_Z - zvtx: NCC_Z + zvtx;
  Double_t ncc_r = fabs((pt/pz)*z);

  if ( verbosity ) 
    {
      cout << "eta\t" << v.Px() << "\t" << v.Py() << "\t" << v.Pz() << "\t" << v.Eta() << "\t";
    }

  // is this the correct acceptance?
  if ( ncc_r >= NCC_RMIN && ncc_r <= NCC_RMAX )
    {
      if ( verbosity ) cout << "accepted" << endl;
      return 1;
    }

  if ( verbosity ) cout << "rejected" << endl;
  return 0;
}
//----------------------------------------------------------------------------
// Check for particle in Canonical MPC Acceptance
int PHPyTrigger::InMPCAcceptance(const TLorentzVector& v, const Double_t zvtx)
{
  // Canonical MPC size and location, please double-check since MPC
  // changed between runs 06 to 08. The below numbers are for Run08
  // and beyond.
  const Double_t MPC_Z = 220.947;          // in cm.
  const Double_t MPC_S_RMIN = 6.4;
  const Double_t MPC_N_RMIN = 8.7;
  const Double_t MPC_RMAX = 22.5;

  Double_t pt = sqrt( v.Px()*v.Px() + v.Py()*v.Py() );
  Double_t pz = v.Pz();
  Double_t rmin = MPC_N_RMIN;
  Double_t z = MPC_Z - zvtx;
  if ( pz < 0. )	// South Arm
    {
      z = MPC_Z + zvtx;
      rmin = MPC_S_RMIN;
    }

  Double_t mpc_r = fabs((pt/pz)*z);

  if ( verbosity )
    {
      cout << "eta\t" << v.Px() << "\t" << v.Py() << "\t" << v.Pz() << "\t" << v.Eta() << "\t";
    }

  // is this the correct acceptance?
  if ( mpc_r >= rmin && mpc_r <= MPC_RMAX )
    {
      if ( verbosity ) cout << "accepted" << endl;
      return 1;
    }

  if ( verbosity ) cout << "rejected" << endl;

  return 0;
}

//----------------------------------------------------------------------------
int PHPyTrigger::ResetEvent(PHCompositeNode *topNode)
{
  return 0;
}

