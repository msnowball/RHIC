// **************************************************************
// 
// GEANT Field Map, for use in converting ROOT maps of 
// PHENIX magnetic field.  Written by Michael Stone, July 2011
// 
// **************************************************************
//
// The structure of the indices has little to do with physics and 
// has much more to do with the way the PHENIX field map is formatted
// in SimMap3D++.root i.e.  The z value is incremented only after
// every phi and r point has been accounted for in that plane.

#ifndef __PHG3toG4MagneticField_H__
#define __PHG3toG4MagneticField_H__

#ifndef __CINT__
#include <G4MagneticField.hh>
#include <globals.hh>
#include <G4ios.hh>
#endif // __CINT__

//root framework
#include <TFile.h>
#include <TNtuple.h>

#ifndef __CINT__
#include <boost/tuple/tuple.hpp>
#include <boost/tuple/tuple_comparison.hpp>
#endif // __CINT__

#include <map> 
#include <string>
#include <vector>

#ifndef __CINT__

class PHG3toG4MagneticField : public G4MagneticField
{
  typedef boost::tuple<float,float,float> trio;
  
 public:
  
  PHG3toG4MagneticField(const std::string  filename);
  virtual ~PHG3toG4MagneticField() {}
  
  void GetFieldValue( const double Point[4],    double *Bfield ) const;
  void GetFieldCyl  ( const double CylPoint[4], double *Bfield ) const;

  void Verbosity(int v)
  {_verbosity = v;}

 protected:
  
  // < i, j, k > , this allows i and i+1 to be neighbors ( <i,j,k>=<z,r,phi> )
  std::vector< std::vector< std::vector<float> > > BFieldZ_; 
  std::vector< std::vector< std::vector<float> > > BFieldR_;
  std::vector< std::vector< std::vector<float> > > BFieldPHI_;
  
  // maps indices to values z_map[i] = z_value that corresponds to ith index
  std::vector<float>   z_map_;   // < i > 
  std::vector<float>   r_map_;   // < j > 
  std::vector<float>   phi_map_; // < k > 
  
  float maxz_, minz_;    // boundaries of magnetic field map cyl
  int _verbosity;

 private:

  bool bin_search( const std::vector<float>& vec, unsigned start, unsigned end, const float& key, unsigned& index ) const;
  void print_map( std::map<trio,trio>::iterator& it ) const;

};

#endif // __CINT__


//---------------------------------------------------
// A wrapper so the class plays nicely with ROOT dictionary.  This can
// be used in a ROOT macro for quick checking of the map.
//
class PHG3toG4MagneticFieldWrapper
{
 public:
  PHG3toG4MagneticFieldWrapper(const std::string &filename);
  
  double GetBx(double x, double y, double z);
  double GetBy(double x, double y, double z);
  double GetBz(double x, double y, double z);
  double GetBCylZ(double z, double r, double phi);
  double GetBCylR(double z, double r, double phi); 
  double GetBCylPHI(double z, double r, double phi); 

#ifndef  __CINT__
 private:
  PHG3toG4MagneticField field;
#endif // __CINT__
};

#endif // __PHFIELD3D_H
