#include "PHPy8GenTrigger.h"
#include "Pythia8/Pythia.h"
#include <iterator>

using namespace std;

//__________________________________________________________
PHPy8GenTrigger::PHPy8GenTrigger(const std::string &name):
_name(name)
{}

//__________________________________________________________
PHPy8GenTrigger::~PHPy8GenTrigger()
{}

std::vector<int> PHPy8GenTrigger::convertToInts(std::string s)
{
  
  vector<int> theVec;
  stringstream ss(s);
  int i;  
  while (ss >> i)
    {
      theVec.push_back(i);
      if (ss.peek() == ',' || ss.peek() == ' ' || ss.peek() == ':' || ss.peek() == ';') ss.ignore();
    }
  return theVec;
  /*
  cout << "s " << s << endl;
  s.erase(remove(s.begin(), s.end(), ',')); // get rid of commas
  cout << "s " << s << endl;
  istringstream buf(s); // convert to input stream
  //cout << vector<int>(istream_iterator<int>(buf), istream_iterator<int>()) << endl;
  return vector<int>(istream_iterator<int>(buf), istream_iterator<int>()); 
  */
}

