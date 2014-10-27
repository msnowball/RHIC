#ifndef PHPyOpenMuonTrigger_h
#define PHPyOpenMuonTrigger_h

// $Id: PHPyOpenMuonTrigger.h,v 1.5 2014/02/08 21:06:17 keyaaron Exp $

#include <TTree.h>
#include <TFile.h>
#include <PHPyVertexShift.h>

#include "PHPyTrigger.h"
#include "PHPyVertexShift.h"

/*!
   \file PHPyOpenMuonTrigger.h
   \brief trigger module to select events that contains a J/Psi in the muon arm acceptance
   \author Hugo Pereira
   \version $Revision: 1.5 $
   \date $Date: 2014/02/08 21:06:17 $
*/

//!  trigger module to select events that contains a J/Psi in the muon arm acceptance
class PHPyOpenMuonTrigger: public PHPyTrigger
{
  
  public:
  
  //! constructor
  PHPyOpenMuonTrigger( const std::string& name = "PHPyOpenMuonTrigger" );
  
  //! destructor 
  virtual ~PHPyOpenMuonTrigger( void );
  
  //! mode
  enum Arm
  {
    //! south arm
    South = 1<<0,
    
    //! north arm
    North = 1<<1,
    
    //! all
    All = North|South
    
  };
    
  //! set arm selection
  /*! value must be bitwise or of the above */
  void set_arm_selection( unsigned int value )
  { _mode = value; }
  
  //! minimum pT (GeV/c)
  void set_pt_min( double value )
  { _pt_min = value; }
  
  //! minimum muon pT (GeV/c)
  void set_muon_pt_min( double value )
  { _muon_pt_min = value; }
  
  //! parent particle type
  enum ParentType
  {
    D = 1<<0,
    D0 = 1<<1,
    Ds = 1<<2,
    DSTAR = 1<<3,
    DSTAR0 = 1<<4,
    DSTARs = 1 <<5,
    B = 1<<6,
    B0 = 1<<7,
    Bs = 1<<8,
    Bc = 1 <<9,
    BSTAR = 1<<10,
    BSTAR0 = 1<<11,
    BSTARs = 1 <<12,
    BSTARc =1 <<13, 
    W = 1<<14,
    AllD = D |D0 |Ds |DSTAR| DSTAR0|DSTARs,
    AllB = B |B0 |Bs |Bc|BSTAR| BSTAR0|BSTARs|BSTARc

  };
  
  //! select parent particle
  /*! must be a bitwise or of the above */
  void set_parent_selection( unsigned int parent )
  { _parent = parent; }
  
  //!@name Methods Derived from SubsysReco
  //@{
  
  //! run initialization
  virtual int Init( PHCompositeNode* );
  
  //! event method
  int process_event(PHCompositeNode* );
  
  //! end of job
  int End(PHCompositeNode* ); 
  
  //@}
  
  //! normalization ntuple
  void set_normalization_filename( const char* file )
  { if( file ) _normalization_file = file; }
  
  //! normalization tree filling
  void set_fill_normalization_tree( bool value )
  { _fill_normalization_tree = value; }
  
  //! set to true to use internal vertex shift module
  /*! 
  this is needed so that vertex used in the output also appears in the normalisation ntuple
  however, it might conflict with possible vertex shift used in the macro 
  */
  void set_use_internal_vertex_shift( bool value )
  { _use_internal_vertex_shift = value; }

  //! pointer to internal vertex shift
  PHPyVertexShift* vertex_shift_module( void )
  { return _vertex_shift; }

  void set_write_history( bool value )
  { _write_history = value; }
  
  protected:
  
  //! true if parent particle is accepted
  bool accept_parent( int ) const;
  
  //! true if open charm(beauty) is found in final state that enters muon arm acceptance
  bool open_in_muon_arm( PHPythiaContainer* );

  //! true if muon is found in given muon arm
  bool muon_in_muon_arm( const TLorentzVector&, Arm );
  
  //! normalization ntuple
  void fill_normalization_tree( void );

  //! book normalization ntuple
  void book_normalization_tree( void );

  //! mode
  unsigned int _mode;
  
  //! parent particle
  unsigned int _parent;
  
  //! minimum pT
  /*! used to only select particles produced with a minimum pT */
  double _pt_min;

  double _muon_pt_min;

  
  //! normalization file 
  std::string _normalization_file;
  
  //! normalization file
  TFile* _normalization_tfile;
  
  //! fill_normalization_tree
  bool _fill_normalization_tree;
  
  //! internal vertex shift
  bool _use_internal_vertex_shift; 
  
  //! normalization tree
  TTree* _normalization_tree;

  //! the requried parent found
  bool _found_parent;
  
  //!@name normalization ntuple variables
  //@{
  int _eventseq;
  int _eventnumber;
  int _pidparent;
  double _Px;
  double _Py;
  double _Pz;
  double _E;
  double _pT;
  double _rapidity;
  
  double _pT_muon;
  char   _history[128];  
  bool   _write_history;

  //! vertex position
  double _Evt_bbcZ;
  
  //! centrality. This variable makes sense only if internal vertex shift is used
  double _Centrality;
  
  //! true if event is accepted
  int _accepted;

  //! true if only specified parents are accepted
  bool _cut_on_parent;
  
  //@}
  
  //! internal vertex shift module
  PHPyVertexShift* _vertex_shift;
  
};

#endif
