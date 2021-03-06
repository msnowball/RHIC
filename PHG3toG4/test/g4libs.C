// $Id: g4libs.C,v 1.1 2014/11/25 21:28:37 snowball Exp $

//------------------------------------------------
// The Virtual Monte Carlo examples
// Copyright (C) 2007 - 2011 Ivana Hrivnacova
// All rights reserved.
//
// For the licensing terms see geant4_vmc/LICENSE.
// Contact: vmc@pcroot.cern.ch
//-------------------------------------------------

/// \file g4libs.C
/// \brief Macro for loading Geant4 and Geant4 VMC libraries
///
/// New macro for loading Geant4 and Geant4 VMC libraries
/// with using geant4-config script provided in Geant4.
/// Besides loading libraries, the macro also resets 
/// FPE mask to 0, in order to make sure than FPE for
/// FE_OVERFLOW is disabled what is required for Geant4.

#if !defined(__CINT__) || defined(__MAKECINT__)

#include <iostream>

#include <TSystem.h>
#include <TString.h>
#include <TApplication.h>
#include <TROOT.h>
#include <string>

#endif

void loadg4libs()
{
/// Macro function for loading Geant4 libraries
/// from list of libraries build by geant4-config --libs 

  FILE* pipe = gSystem->OpenPipe("geant4-config --libs", "r");
  std::string all_lines;
  char line[1000];
  while ( fgets(line, sizeof(line), pipe ) != NULL ) {
    all_lines += line;
  }
  
  TString all_lines_t(all_lines.data());
  all_lines_t.Remove(all_lines_t.First('\n'));
  //cout << all_lines_t.Data() << endl;
  TObjArray* libs = all_lines_t.Tokenize(" ");

/*
  TString dynamicPath = gSystem->GetDynamicPath();
  TString addPath = ((TObjString*)libs->At(0))->GetString();
  addPath.Remove(0,2);
  //cout << "Adding dynamic path " << addPath.Data() << endl; 
  gSystem->AddDynamicPath(addPath.Data());
*/  

  cout << libs->GetEntriesFast() << endl;
  for (Int_t i=libs->GetEntriesFast()-1; i>0; i-- ) {
    TString lib = ((TObjString*)libs->At(i))->GetString();
    lib.ReplaceAll("-l", "lib");
    //cout << "Loading |" << lib.Data() << "|" << endl; 
    if(lib.BeginsWith("lib"))
      gSystem->Load(lib.Data());
  } 
  
  gSystem->SetFPEMask(0); 
}   

Bool_t isLibrary(const char* libName)
{
/// Helper function which testes the existence of the given library
/// \param libName  The library name

  if (TString(gSystem->DynamicPathName(libName, kTRUE)) != TString(""))
    return kTRUE;
  else  
    return kFALSE;
}    

Bool_t isBatch()
{
/// Helper function which testes if Root was started in batch mode

  for ( Int_t i=0; i<gApplication->Argc(); ++i ) 
    if ( TString(gROOT->GetApplication()->Argv(i)) == "-b" ) return true;
  
  return false;
}    

Bool_t isSet(const char* variable)
{
/// Helper function which checks if the specified environment variable 
/// is set.
/// \param variable  The environment variable name

  TString value = gSystem->Getenv(variable);
  if ( value != "") return true;
  
  return false;
}  

void vgmlibs()
{ 
/// Function for loading VGM libraries.

  if ( isSet("USE_VGM") ) { 
    cout << "Loading VGM libraries ... " << endl;
    gSystem->Load("libClhepVGM");
    gSystem->Load("libBaseVGM");
    gSystem->Load("libGeant4GM");
    gSystem->Load("libRootGM");
    gSystem->Load("libXmlVGM");
  }  
}

void basiclibs()
{
  /// Macro function for loading basic libraries used with all MCs 
  gSystem->Load("libRIO");
  gSystem->Load("libGeom");
  gSystem->Load("libVMC");
  gSystem->Load("libPhysics");
  gSystem->Load("libTree");
}



void g4libs()
{
  cout << "Loading basic libs " << endl;
  basiclibs();
  
  // Load Geant4 libraries
  cout << "Loading Geant4 libraries (using geant4-config) ..." << endl;
  loadg4libs();

  // VGM librares
  vgmlibs();
  
  // G4Root library (if available)
  cout << "Loading g4root library ..." << endl;
  gSystem->Load("libg4root");
    
  // Geant4 VMC library
  cout << "Loading geant4vmc library ..." << endl;
  gSystem->AddIncludePath("-I/direct/phenix+hhj/snowball/local/geant4_vmc/include/geant4vmc");
  gSystem->Load("libgeant4vmc");

  // Geant4 VMC GUI library 
  // (if available and Root is not running in batch mode)
  if ( isLibrary("libgeant4vmc_gui") && ! isBatch() ) {
    cout << "Loading geant4vmc_gui library ... " << endl;
    gSystem->Load("libgeant4vmc_gui");
  }  

}
