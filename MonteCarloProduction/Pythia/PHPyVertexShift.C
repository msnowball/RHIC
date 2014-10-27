// $Id: PHPyVertexShift.C,v 1.1 2011/08/05 17:47:36 pinkenbu Exp $

/*!
   \file PHPyVertexShift.C
   \brief  shifts particles vertex based on values read from a file
   \author Hugo Pereira
   \version $Revision: 1.1 $
   \date $Date: 2011/08/05 17:47:36 $
*/

#include <iostream>
#include <sstream>

#include <PHCompositeNode.h>
#include <getClass.h>
#include <Fun4AllReturnCodes.h>
#include <VtxOut.h>
#include <MuonUtil.h>

#include "PHPyVertexShift.h"
#include "PHPythiaContainer.h"
#include "PHPythiaHeader.h"



using namespace std;

//____________________________________________________________________________________________
bool PHPyVertexShift::_registered = false;

//____________________________________________________________________________________________
PHPyVertexShift::PHPyVertexShift(const string &name, const string& filename ): SubsysReco(name)
{
  SetVtxFile( filename );
  _file_version = 0;
  _event_vertex_x = 0.;
  _event_vertex_y = 0.;
  _event_vertex_z = 0.;
  _isVtxFileRoot = false;

  _verbosity = 1;
  _vertexname = "FVTX";



}

//____________________________________________________________________________________________
int PHPyVertexShift::Init( PHCompositeNode* )
{
  
  if( _registered ) 
  {

    cout << "PHPyVertexShift::PHPyVertexShift - there is already one PHPyVertexShift module registered." << endl;
    cout << "PHPyVertexShift::PHPyVertexShift - this will ruin vertex shifting. Macro needs to be fixed." << endl;
    cout << "PHPyVertexShift::PHPyVertexShift - aborting." << endl;
    cout << "PHPyVertexShift::PHPyVertexShift - (note: the module might have been registered internally by others)." << endl;
    return ABORTRUN;
    
  } else _registered = true;
    

   // If the vtx file is set, open up the vtx file
  if ( ! fVtx_fname.empty() )
  {
    std::size_t found = fVtx_fname.find(".root");
    if (found!=std::string::npos) 
      {
	if( _verbosity > 0) 
	  { 
	    cout << "PHPyVertexShift::Init - file is a .root file, ignoring.  " << endl 
		 << "                      - ***make sure you have put it into a DSTInputManager***" << endl;
	  }
	_isVtxFileRoot = true;
      }
    else{
      fVtx_file.open( fVtx_fname.c_str() );
      if ( !fVtx_file.good() )
	{
	  cout << "PHPyVertexShift::Init - unable to open " << fVtx_fname << endl;
	  return ABORTRUN;
	}
    }
  }
  else {
    cout << "PHPyVertexShift::Init - no filename specified. Checking for DST with vtxOut..." << endl; 
    _isVtxFileRoot = true;
  }

  return EVENT_OK;
}

//____________________________________________________________________________________________
int PHPyVertexShift::End( PHCompositeNode* )
{ 
  if ( fVtx_file.is_open() ) fVtx_file.close();
  return EVENT_OK;
}

  
//____________________________________________________________________________________________
int PHPyVertexShift::process_event( PHCompositeNode* top_node )
{

  static int counter(0);
  
  // get the relevant nodes
  PHPythiaHeader* phpythiaheader = findNode::getClass<PHPythiaHeader>(top_node,"PHPythiaHeader");
  if (!phpythiaheader)
  {
    cout << "PHPyVertexShift::process_event - unable to get PHPythiaHeader, is Node missing?" << endl;
    return ABORTRUN;
  }
  
  // get the relevant nodes
  PHPythiaContainer* phpythia = findNode::getClass<PHPythiaContainer>(top_node,"PHPythia");
  if (!phpythia)
  {
    cout << "PHPyVertexShift::process_event - unable to get PHPythia, is Node missing?" << endl;
    return ABORTRUN;
  }

  VtxOut* vtx = TMutNode<VtxOut>::find_io_node(top_node, "VtxOut" );
  if (_isVtxFileRoot && !vtx)
    {
      cout << "PHPyVertexShift::process_event - VtxOut not in Node Tree, did you load DST?" << endl;
      return ABORTRUN;
    }


  // Move the Vertex if fVtx_file was specified
  _event_valid = false;
  _event_vertex_x = 0.;
  _event_vertex_y = 0.;
  _event_vertex_z = -999.;
  _event_centrality = -999;
  if ( !fVtx_fname.empty() && !_isVtxFileRoot )
  {
    char line[512];
    if ( fVtx_file.good() )
    {
      _event_valid = true;
      
      // read file line by line. This is more robust against input file format
      // notably when additional information per event is added
      fVtx_file.getline( line, 512, '\n' );
      
      // check file stream
      if( !fVtx_file.good() )
      {
        
        cout << "PHPyVertexShift::process_event - event: " << ++counter << " invalid line:" << line << endl;
        
      }
      else {
        
	if(_verbosity > 2) cout << "PHPyVertexShift::process_event - reading line from txt file" << endl;
        istringstream line_stream( line );
        Int_t   vtx_evt;
        if ( _file_version == VERSION2 )
          {
            line_stream >> vtx_evt
                        >> _event_vertex_x
                        >> _event_vertex_y 
                        >> _event_vertex_z
                        >> _event_centrality;
          }
        else {
	  line_stream >> vtx_evt >> _event_vertex_z >> _event_centrality;
	}
        _event_valid = !(line_stream.rdstate() & ios::failbit );
	
        // check line stream
        if( !_event_valid )  cout << "PHPyVertexShift::process_event - event: " << ++counter << " invalid line:" << line << endl;
        else {
	  phpythiaheader->moveVertex( _event_vertex_x, _event_vertex_y, _event_vertex_z );
	  phpythia->moveVertex( _event_vertex_x, _event_vertex_y, _event_vertex_z );
	  if(_verbosity > 1){ cout << "PHPyVertexShift::process_event - shifting vertex (0,0,0) --> ("<< _event_vertex_x<<","<<_event_vertex_y<<","<<_event_vertex_z<<")"<< endl; }
	}
        
      }
      
      // validity
      _event_valid &= fVtx_file.good();
      
    } 
    else {
      if ( fVtx_file.eof() ) cout << "PHPyVertexShift::process_event - not enough entries in vtx file " << fVtx_fname << endl;
      else  cout << "PHPyVertexShift::process_event - error in reading vtx file " << fVtx_fname << endl;
      return ABORTRUN;
    }
    
  }
  else if(vtx)
    {
      if(_verbosity > 2) cout<< "PHPyVertexShift::process_event - reading vertex from DST, vertex_name = " << _vertexname << endl;

      if (_vertexname.empty())
	{
	  cout << "PHPyVertexShift::process_event - _vertexname is empty, aborting" << endl;
	  return ABORTRUN;
	}
      double vtx_x( vtx->get_Vertex(_vertexname.c_str()).getX() );
      double vtx_y( vtx->get_Vertex(_vertexname.c_str()).getY() );
      double vtx_z( vtx->get_Vertex(_vertexname.c_str()).getZ() );
      phpythiaheader->moveVertex( vtx_x, vtx_y, vtx_z );
      phpythia->moveVertex( vtx_x, vtx_y, vtx_z );
      //double centrality( MuonUtil::get_centrality( top_node ) );
      double centrality = -999;

      _event_vertex_z = vtx_z;
      _event_centrality = centrality;

      if(_verbosity > 1)
	{
	  cout << "PHPyVertexShift::process_event - shifting vertex (0,0,0) --> ("<<vtx_x<<","<<vtx_y<<","<<vtx_z<<")"<< endl; 
	  cout << "PHPyVertexShift::process_event - centrality will be ignored" << endl;
	}
      _event_valid = true;
    }
  else {
    cout << "PHPyVertexShift::process_event - could not find a txt vertex file or vtxOut node!" << endl
	 << "PHPyVertexShift::process_event - Aborting event" << endl;
    return ABORTEVENT;
  }


  return EVENT_OK;  
}
