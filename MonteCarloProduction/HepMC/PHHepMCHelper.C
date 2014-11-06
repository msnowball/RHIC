#include <iostream>
#include <sstream>
#include <string>

#include <PHCompositeNode.h>
#include <getClass.h>
#include <Fun4AllReturnCodes.h>

#include "PHHepMCHelper.h"
#include "PHHepMCGenEvent.h"
#include "HepMC/IO_GenEvent.h"
#include "HepMC/IO_AsciiParticles.h"
#include "HepMC/GenEvent.h"



using namespace std;

//____________________________________________________________________________________________
bool PHHepMCHelper::_registered = false;

//____________________________________________________________________________________________
PHHepMCHelper::PHHepMCHelper(const string &name): SubsysReco(name)
{
  _verbosity = 1;
  _writeEvent = false;
  _writeReadableEvent = false;
  //_asciiOut = new HepMC::IO_GenEvent(_outputFileName.c_str(),std::ios::out);
  //_asciiReadableOut = new HepMC::IO_AsciiParticles(_outputFileName.c_str(),std::ios::out);
}

//____________________________________________________________________________________________
int PHHepMCHelper::Init( PHCompositeNode* )
{


  if(_outputFileName == _outputHRFileName)
    {
      cout << "PHHepMCHelper::PHHepMCHelper - The output file name for HR and HepMC formats are the same!" << endl;
      return ABORTRUN;
    }

  if( _registered ) 
  {

    cout << "PHHepMCHelper::PHHepMCHelper - there is already one PHHepMCHelper module registered." << endl;
    cout << "PHHepMCHelper::PHHepMCHelper - aborting." << endl;
    cout << "PHHepMCHelper::PHHepMCHelper - (note: the module might have been registered internally by others)." << endl;
    return ABORTRUN;
    
  } else _registered = true;
    
  return EVENT_OK;
}

//____________________________________________________________________________________________
int PHHepMCHelper::End( PHCompositeNode* )
{ 
  _asciiOut->clear();
  if(_asciiOut) delete _asciiOut;

  return EVENT_OK;
}

  
//____________________________________________________________________________________________
int PHHepMCHelper::process_event( PHCompositeNode* top_node )
{

  // get the relevant nodes
  PHHepMCGenEvent* hepmcEvent = findNode::getClass<PHHepMCGenEvent>(top_node,"PHHepMCGenEvent");
  if (!hepmcEvent)
  {
    cout << "PHHepMCHelper::process_event - unable to get PHHepMCGenEvent, is Node missing?" << endl;
    return ABORTRUN;
  }


  _theHepMCEvt = hepmcEvent->getEvent();
  if(_verbosity > 1) cout << "PHHepMCHelper::process_event - writing event to " << _outputFileName << endl;
  if(_writeEvent) _asciiOut->write_event(_theHepMCEvt);
  if(_writeReadableEvent) _asciiReadableOut->write_event(_theHepMCEvt);
  
  
  return EVENT_OK;  
}


void PHHepMCHelper::WriteEventsToText(const std::string& outName)
{
  _outputFileName = outName;
  delete _asciiOut;
  _asciiOut = new HepMC::IO_GenEvent(_outputFileName.c_str(),std::ios::out);
  _writeEvent = true;
}

void PHHepMCHelper::WriteHREventsToText(const std::string& outName)
{
  _outputHRFileName = outName;
  delete _asciiReadableOut;
  _asciiReadableOut = new HepMC::IO_AsciiParticles(_outputHRFileName.c_str(),std::ios::out);
  _writeReadableEvent = true;
}
