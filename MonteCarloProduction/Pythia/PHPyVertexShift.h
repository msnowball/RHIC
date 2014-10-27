#ifndef PHPyVertexShift_h
#define PHPyVertexShift_h

// $Id: PHPyVertexShift.h,v 1.1 2011/08/05 17:47:36 pinkenbu Exp $

/*!
   \file PHPyVertexShift.h
   \brief  shifts particles vertex based on values read from a file
   \author Hugo Pereira
   \version $Revision: 1.1 $
   \date $Date: 2011/08/05 17:47:36 $
*/

#include <fstream>
#include <string>
#include "SubsysReco.h"

class PHCompositeNode;

//! shifts particles vertex based on values read from a file
class PHPyVertexShift: public SubsysReco
{
  
  public:
  
  //! constructor
  PHPyVertexShift( const std::string& name = "PHPyVertexShift", const std::string& filename = "" );
  
  //! destructor
  virtual ~PHPyVertexShift( void )
  {}
  
  //!@name Methods Derived from SubsysReco
  //@{
  
  //! full initialization
  int Init(PHCompositeNode *topNode);
    
  //! event method
  int process_event(PHCompositeNode *topNode);
  
  //! end-of-job method
  int End(PHCompositeNode *topNode);
  //@}

  //! vertex file
  void SetVtxFile(const std::string o = "") 
  { fVtx_fname  = o; }

  //! set the vertex file version
  void SetVtxFileVersion(const int v)
  { _file_version = v; }

  //! validity
  bool event_valid( void ) const
  { return _event_valid; }
  
  //! event vertex
  double event_vertex_z( void ) const
  { return _event_vertex_z; }
  
  //! event centrality
  double event_centrality( void ) const
  { return _event_centrality; }

  void SetVerbosity(int v = 1)
  { _verbosity = v; }

  void set_vertex_name( std::string vname = "FVTX" ) 
  { _vertexname = vname;}
  
  enum { VERSION1 = 0, VERSION2 };

  private:
  
  //! used to ensure only one module is registered per job
  static bool _registered;
  
  //! vtx file, derived from mutoo framework vtx file
  std::string fVtx_fname;	
  
  //! vtx file, derived from mutoo framework vtx file
  std::ifstream fVtx_file;	

  //! version of file (zvertex only, 3vertex, etc);
  int _file_version;

  // true if valid
  bool _event_valid;
  
  // current event vertex
  double _event_vertex_x;
  double _event_vertex_y;
  double _event_vertex_z;
  
  // current event centrality
  double _event_centrality;

  bool _isVtxFileRoot;

  std::string _vertexname;

  int _verbosity;
  
};

#endif
