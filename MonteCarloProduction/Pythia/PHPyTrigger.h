#ifndef __PHPYTRIGGER_H__
#define __PHPYTRIGGER_H__

#include <SubsysReco.h>

#include <Rtypes.h> // for Double_t def

/**

Example Reco Module for Triggering on PYTHIA events.

In Fun4All it is very simple to select and write out only
the interesting events by going through the PHPythia Event
and returning EVENTOK for a triggered event, and ABORTEVENT
for an untriggered event.

We recommend that you derive your trigger from this class, and simply
override the process_event method.  This will allow everyone to
share the same acceptance methods given here, etc., and also
for people to contribute and improve these methods.

*/

class PHCompositeNode;
class PHPythiaHeader;
class PHPythiaContainer;
class TLorentzVector;

class PHPyTrigger: public SubsysReco
{
  
  public:
  
  PHPyTrigger(const std::string &name = "PHPyTrigger");
  virtual ~PHPyTrigger();

  virtual void SetEnergyThreshold(const double e) { e_threshold = e; }
  virtual void SetPThreshold(const double p) { p_threshold = p; }
  virtual void SetPtThreshold(const double pt) { pt_threshold = pt; }

  virtual void SetTrigger(const int t) { trigger_type = t; }
  virtual void SetMaxEvents(const int m) { max_events = m; }

  // Trigger Methods (return 0 for false, 1 for true)
  int Pi0InCentralArm(PHPythiaContainer *phpylist);
  int ChiCInMuonArm(PHPythiaContainer *phpylist);
  int ChiCInMuonArm2(PHPythiaContainer *phpylist);
  int ChiCInMuonArm3(PHPythiaContainer *phpylist);
  int ChiCInMuonArm4(PHPythiaContainer *phpylist);
  int GammaInNCC(PHPythiaContainer *phpylist);
  int UpsilonInMuonArm(PHPythiaContainer *phpylist);
  int KorPiInMuonArm(PHPythiaContainer *phpylist);

  // Acceptance Tests (return 0 for false, 1 for true)
  virtual int PhotonInCentralArmAcceptance(const TLorentzVector& v, const Double_t zvtx = 0.);
  virtual int InCentralArmAcceptance(const TLorentzVector& v);
  virtual int InMuonArmAcceptance(const TLorentzVector& v);
  virtual int InNCCAcceptance(const TLorentzVector& v, const Double_t zvtx = 0.);
  virtual int InMPCAcceptance(const TLorentzVector& v, const Double_t zvtx = 0.);

  // Methods Derived from SubsysReco
  int Init(PHCompositeNode *topNode);
  //int InitRun(PHCompositeNode *topNode);
  int process_event(PHCompositeNode *topNode);
  int ResetEvent(PHCompositeNode *topNode);
  int End(PHCompositeNode *topNode);

  enum TriggerTypes {
    MINBIAS = 0,
    PI0_CENTARM,
    CHIC_MUONARM, 
    CHIC_MUONARM2,
    CHIC_MUONARM3,
    CHIC_MUONARM4,
    GAMMA_NCC,
    UPSILON_MUONARM,
    EFROMB_CENTRALARM,
    D02KPI_CENTRALARM,
    EFROMHF_UNITRAP,
    EFROMC_CENTRALARM,
    NPART_CENTRALARM,
    DBCorr_CENTRALARM,
    Dstar_CENTRALARM,
    KorPi_MUONARM
  };

  protected:
  
  PHPythiaHeader *phpythiaheader;
  PHPythiaContainer *phpythia;

  double e_threshold;		// energy threshold
  double p_threshold;		// momentum threshold
  double pt_threshold;		// pt threshold

  int trigger_type;		//  trigger type

  unsigned int max_events;	// maximum num events to generate
  unsigned int ntriggered;	// number of triggered events
  unsigned int nconsidered;	// number of considered events
};

#endif	/* __PHPYTRIGGER_H__ */

