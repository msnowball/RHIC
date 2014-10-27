// $Id: PHPyOpenMuonTrigger.C,v 1.6 2010/08/12 17:53:14 youzy Exp $


/*!
   \file PHPyOpenMuonTrigger.C
   \brief trigger module to select events that contains a J/Psi in the muon arm acceptance
   \author Hugo Pereira
   \version $Revision: 1.6 $
   \date $Date: 2010/08/12 17:53:14 $
*/


#include <PHPyOpenMuonTrigger.h>
#include <PHPyTrigger.h>
#include <PHPythiaHeader.h>
#include <PHPythiaContainer.h>
#include <PHPyCommon.h>

#include <getClass.h>
#include <Fun4AllReturnCodes.h>
#include <TLorentzVector.h>

#if ROOT_VERSION_CODE >= ROOT_VERSION(5,15,8) 
#include <TMCParticle.h>
#else
#include <TMCParticle6.h>
#endif


#include <cassert>
#include <cmath>
#include <cstdlib>

using namespace std;

//___________________________________________________________________________
PHPyOpenMuonTrigger::PHPyOpenMuonTrigger(const string &name): 
  PHPyTrigger(name),
  _mode( All ),
  _parent( AllB ),
  _pt_min( 0 ),
  _muon_pt_min( 0 ),
  _normalization_file( "pythia.root" ),
  _normalization_tfile( 0 ),
  _fill_normalization_tree( true ),
  _use_internal_vertex_shift( true ),
  _normalization_tree( 0 ),
  _found_parent( false ),
  _eventseq( 1 ),
  _eventnumber( 1 ),
  _pidparent( 0 ),
  _write_history( false ) 
{ 
  // trigger type (?)
  trigger_type = MINBIAS;
  
  // create vertex shift
  _vertex_shift = new PHPyVertexShift( "INTERNAL_VERTEX_SHIFT" );
}

//___________________________________________________________________________
PHPyOpenMuonTrigger::~PHPyOpenMuonTrigger( void )
{ if( _vertex_shift ) delete _vertex_shift; }
  
//___________________________________________________________________________
int PHPyOpenMuonTrigger::Init(PHCompositeNode *top_node)
{
  

  cout << "PHPyOpenMuonTrigger::Init" << endl;
  
  cout << "_mode: " << _mode << endl;
  cout << "  south: " << (_mode&South ? "true":"false" ) << endl; 
  cout << "  north: " << (_mode&North ? "true":"false" ) << endl; 
  cout << "_pt_min: " << _pt_min << endl;
  cout << "_muon_pt_min: " << _muon_pt_min << endl;

  
  cout << "_normalization_file: " << _normalization_file << endl;
  cout << "_fill_normalization_tree: " << _fill_normalization_tree << endl;
  cout << "_use_internal_vertex_shift: " << _use_internal_vertex_shift << endl;
  
  cout << "_parent: " << endl;
  cout << " D+: " << (accept_parent( PY_D ) ? "true":"false" ) << endl;
  cout << " D0: " << (accept_parent( PY_D0 ) ? "true":"false" ) << endl;
  cout << " Ds: " << (accept_parent( PY_Ds ) ? "true":"false" ) << endl;
  cout << " D*+: " << (accept_parent( PY_DSTAR ) ? "true":"false" ) << endl;
  cout << " D*0: " << (accept_parent( PY_DSTAR0 ) ? "true":"false" ) << endl;
  cout << " D*s: " << (accept_parent( PY_DSTARs ) ? "true":"false" ) << endl;

  cout << "_parent: " << endl;
  cout << " B+: " << (accept_parent( PY_B ) ? "true":"false" ) << endl;
  cout << " B0: " << (accept_parent( PY_B0 ) ? "true":"false" ) << endl;
  cout << " Bs: " << (accept_parent( PY_Bs ) ? "true":"false" ) << endl;
  cout << " Bc: " << (accept_parent( PY_Bc ) ? "true":"false" ) << endl;

  cout << " B*+: " << (accept_parent( PY_BSTAR ) ? "true":"false" ) << endl;
  cout << " B*0: " << (accept_parent( PY_BSTAR0 ) ? "true":"false" ) << endl;
  cout << " B*s: " << (accept_parent( PY_BSTARs ) ? "true":"false" ) << endl;
  cout << " B*c: " << (accept_parent( PY_BSTARc ) ? "true":"false" ) << endl;

  cout << " W: " << (accept_parent( PY_W ) ? "true":"false" ) << endl; 
 
  if( _use_internal_vertex_shift ) return  _vertex_shift->Init( top_node );
  else return EVENT_OK;
  
} 
  
//___________________________________________________________________________
int PHPyOpenMuonTrigger::End(PHCompositeNode *top_node)
{

  // close TFile
  if( _fill_normalization_tree && _normalization_tfile )
  {
    _normalization_tfile->Write();
    _normalization_tfile->Close();
    delete _normalization_tfile;
  }
  
  // dump out trigger statistics
  cout << "PHPyOpenMuonTrigger::End - number of triggered events: " << ntriggered << "/" << nconsidered << endl;
  if ( nconsidered>0 ) cout << "PHPyOpenMuonTrigger::End - ratio: " << float(ntriggered)/nconsidered << endl;
  return EVENT_OK;
  
}

//___________________________________________________________________________
int PHPyOpenMuonTrigger::process_event(PHCompositeNode *top_node)
{
  _eventnumber++; 
  static int counter(0);
  
  // Get PYTHIA Header (only if you want to trigger on event information)
  phpythiaheader = findNode::getClass<PHPythiaHeader>(top_node,"PHPythiaHeader");
  if (!phpythiaheader)
  {
    cout << "PHPyOpenMuonTrigger::process_event - unable to get PHPythiaHeader, is Node missing?" << endl;
    return ABORTRUN;
  }
  
  // Get PYTHIA Particles
  phpythia = findNode::getClass<PHPythiaContainer>(top_node,"PHPythia");
  if (!phpythia)
  {
    cout << "PHPyOpenMuonTrigger::process_event - unable to get PHPythia, is Node missing?" << endl;
    return ABORTRUN;
  }
  
  // increment counter
  ++nconsidered;
  _Evt_bbcZ = -999;
  _Centrality = -999;
  _pidparent = -9999;
  _Px = -9999;
  _Py = -9999;
  _Pz = -9999;
  _E  = -9999;
  _pT = -9999;
  _rapidity = -9999;
  _pT_muon  = -9999;
  _accepted = -9999;
  for (int ichar = 0; ichar < 128; ichar++) _history[ichar]  = ' ';

  // check if particles fullfil trigger conditions
  _accepted = open_in_muon_arm(phpythia);
            

  // check pT range
  /* 
  if pT is not in range, the normalization ntuple is not filled and one returns directly.
  This is to minimize the size of the normalization file 
  */

  if( _pt_min > 0 && _pT < _pt_min )
  { 
    _accepted = false;
    return ABORTEVENT;
    //return DISCARDEVENT;
  }

  if( _muon_pt_min > 0 && _pT_muon < _muon_pt_min )
  { 
    _accepted = false;
    return ABORTEVENT;
    //return DISCARDEVENT;
  }
    
  if( _accepted ) ++ntriggered;

  if( _accepted && _use_internal_vertex_shift )
  { 
    
    int out( _vertex_shift->process_event( top_node ) );
    if( !( out == EVENT_OK && _vertex_shift->event_valid() ) ) 
    {
    
      cout << "PHPyOpenMuonTrigger::process_event - event: " << ++counter << " vertex_shift event invalid" << endl;
      return out; 
    
    } else {
      
      _Evt_bbcZ = _vertex_shift->event_vertex_z();
      _Centrality = _vertex_shift->event_centrality();
      cout << "PHPyOpenMuonTrigger::process_event - event: " << ++counter << " _Evt_bbcZ: " << _Evt_bbcZ << endl;
      
    }
  }
  
  // fill normalization ntuple for all events
  // must be called after vertex_shift is called, so that accepted events have correct vertex and centrality
  if( _fill_normalization_tree && _accepted && _found_parent ) 
  {
    fill_normalization_tree();
    _eventseq++;
  }

  // output
  return (_accepted) ? EVENT_OK:ABORTEVENT;
  
}

//_________________________________________________________________________
bool PHPyOpenMuonTrigger::accept_parent( int parent ) const
{
  
  if( (_parent & D) && std::abs(parent) == PY_D ) return true;
  if( (_parent & D0) && std::abs( parent) == PY_D0 ) return true;
  if( (_parent & Ds) && std::abs(parent) == PY_Ds ) return true;
  if( (_parent & DSTAR) && std::abs(parent) == PY_DSTAR ) return true;
  if( (_parent & DSTAR0) && std::abs(parent) == PY_DSTAR0 ) return true;
  if( (_parent & DSTARs) && std::abs(parent) == PY_DSTARs ) return true;
  

  if( (_parent & B) && std::abs(parent) == PY_B ) return true;
  if( (_parent & B0) && std::abs( parent) == PY_B0 ) return true;
  if( (_parent & Bs) && std::abs(parent) == PY_Bs ) return true;
  if( (_parent & Bc) && std::abs(parent) == PY_Bc ) return true;

  if( (_parent & BSTAR) && std::abs(parent) == PY_BSTAR ) return true;
  if( (_parent & BSTAR0) && std::abs(parent) == PY_BSTAR0 ) return true;
  if( (_parent & BSTARs) && std::abs(parent) == PY_BSTARs ) return true;
  if( (_parent & BSTARc) && std::abs(parent) == PY_BSTARc ) return true;
  
  if( (_parent & W) && std::abs(parent) == PY_W ) return true;

  return false;
}

//_________________________________________________________________________
bool PHPyOpenMuonTrigger::open_in_muon_arm(PHPythiaContainer *phpylist)
{
  // store momentum and flags whether or not muons have been found.
  TLorentzVector momentum_muon;
  TLorentzVector momentum_parent;
  
  //bool found_muon = false;
  _found_parent = false;
   
  // loop over particles, store mu_plus and mu_minus candidate
  for( unsigned int ipart=0; ipart<phpylist->size(); ipart++)
  {
    
    TMCParticle *part = phpylist->getParticle(ipart);
    
    // check if partices are muons
    int kf = part->GetKF();
    if ( kf==PY_MU || kf==-PY_MU )
    {
      // get momentum and energy
      TLorentzVector momentum( part->GetPx(), part->GetPy(), part->GetPz(), part->GetEnergy());
      
      // THIS IS A PAIN, you have to subtract 1 from the line number of the parent!	  
      TMCParticle *part2 =  phpylist->getParticle( part->GetParent()-1 );
      Int_t kfparent =  part2->GetKF();

      if( accept_parent( kfparent ) )
      {
        //cout << "kfparent = " << kfparent << endl;
        // store parent momentum
        TLorentzVector momentum_parent_local( part2->GetPx(), part2->GetPy(), part2->GetPz(), part2->GetEnergy() ); 
        _pidparent = part2->GetKF();
      
        if( !_found_parent )
        {
          
          _found_parent = true;
          momentum_parent = momentum_parent_local;
          //cout << phpylist->getHistoryString(part) << endl;          
          if ( _write_history ) 
          {
            int nchar = phpylist->getHistoryString(part).Length();
            if ( nchar > 128 ) nchar = 128;
            for (int ichar = 0; ichar < nchar; ichar++) _history[ichar] = (phpylist->getHistoryString(part))[ichar];
          }
        } else if( momentum_parent != momentum_parent_local ) {
          
          cout << "PHPyOpenMuonTrigger::open_in_muon_arm - warning: multiple parents found." << endl;
          return false;
        
        }  
      }
      momentum_muon = momentum;
    }
    
  }
  
  // check if parent has been found
  if( ! _found_parent )
  { 
    //cout << "PHPyOpenMuonTrigger::open_in_muon_arm - warning: orphan muon candidated." << endl;
    return false;
  }
   
  // store relevant ntuple variables
  _Px = momentum_parent.Px();
  _Py = momentum_parent.Py();
  _Pz = momentum_parent.Pz();
  _E = momentum_parent.E();
  _pT = momentum_parent.Pt();
  _rapidity = momentum_parent.Rapidity();

  _pT_muon = momentum_muon.Pt();

  // check momentum against selected mode
  if( ( _mode & South ) && muon_in_muon_arm( momentum_muon, South ) ) return true;
  if( ( _mode & North ) && muon_in_muon_arm( momentum_muon, North ) ) return true;

  return false;    

}

//_________________________________________________________________________
bool PHPyOpenMuonTrigger::muon_in_muon_arm( const TLorentzVector& momentum, PHPyOpenMuonTrigger::Arm arm )
{
  
  const double DEG_TO_RAD = M_PI/180.0;
  const double south_theta_range[2] = { DEG_TO_RAD*143, DEG_TO_RAD*171 };
  const double north_theta_range[2] = { DEG_TO_RAD*9, DEG_TO_RAD*37 };
  
  return (
    (arm == South && std::abs( momentum.Theta() ) >= south_theta_range[0] && std::abs( momentum.Theta() ) <= south_theta_range[1] ) ||
    (arm == North && std::abs( momentum.Theta() ) >= north_theta_range[0] && std::abs( momentum.Theta() ) <= north_theta_range[1] ) );
  
};

//_________________________________________________________________________
void PHPyOpenMuonTrigger::book_normalization_tree( void )
{
  assert( !( _normalization_file.empty() || _normalization_tree ) );
  
  cout << "PHPyOpenMuonTrigger::book_normalization_tree - writing to " << _normalization_file << endl;

  _normalization_tfile = new TFile( _normalization_file.c_str(), "RECREATE" ); 

  enum { BUFFER_SIZE=32000 };
  enum { AUTO_SAVE=16000 };
  _normalization_tree = new TTree(  "h10", "J/Psi before selection" );	
  _normalization_tree->Branch( "eventseq", &_eventseq, "eventseq/I", BUFFER_SIZE );
  _normalization_tree->Branch( "eventnumber", &_eventnumber, "eventnumber/I", BUFFER_SIZE );
  _normalization_tree->Branch( "pidparent", &_pidparent, "pidparent/I", BUFFER_SIZE );
  _normalization_tree->Branch( "Px", &_Px, "Px/D", BUFFER_SIZE );
  _normalization_tree->Branch( "Py", &_Py, "Py/D", BUFFER_SIZE );
  _normalization_tree->Branch( "Pz", &_Pz, "Pz/D", BUFFER_SIZE );
  _normalization_tree->Branch( "E", &_E, "E/D", BUFFER_SIZE );
  _normalization_tree->Branch( "pT", &_pT, "pT/D", BUFFER_SIZE );
  _normalization_tree->Branch( "rapidity", &_rapidity, "rapidity/D", BUFFER_SIZE );
  _normalization_tree->Branch( "Evt_bbcZ", &_Evt_bbcZ, "Evt_bbcZ/D", BUFFER_SIZE );
  _normalization_tree->Branch( "Centrality", &_Centrality, "Centrality/D", BUFFER_SIZE );
  _normalization_tree->Branch( "accepted", &_accepted, "accepted/I", BUFFER_SIZE );
  _normalization_tree->Branch( "history", _history, "history/C", BUFFER_SIZE );
  _normalization_tree->SetAutoSave( AUTO_SAVE );
  cout << "PHPyOpenMuonTrigger::book_normalization_tree - h10 ntuple created" << endl;
  
}

//_________________________________________________________________________
void PHPyOpenMuonTrigger::fill_normalization_tree( void )
{ 
  
  if( _normalization_file.empty() ) return;
  if( !( _normalization_tree ) ) book_normalization_tree();
  _normalization_tree->Fill();
  
}
