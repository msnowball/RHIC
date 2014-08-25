#!/usr/bin/python
import sys, os, pwd, commands
import optparse, shlex, re
import math
from array import array
import collections
import ROOT
from AnalysisClass import *
import time

def parseOptions():

  usage = ('usage: %prog [options] \n'
           + '%prog -h for help')
  parser = optparse.OptionParser(usage)

  parser.add_option('-b', action='store_true', dest='noX', default=True ,help='no X11 windows')
  parser.add_option('-f','--list', dest='FILE_LIST', type='string', default='Run12ppNtupleList.txt' ,help='List of ntuples')
  parser.add_option('-n', '--nEvents', dest='NEVENTS', type='float', default=-1 ,help='Number of events to process')
  parser.add_option('-N', '--name', dest='JOB_NAME', type='string', default='analysisTest' ,help='Job Name')
  parser.add_option('-d', '--out', dest='OUTPUT_DIR', type='string', default='' ,help='Output directory if different than default')
  parser.add_option('--time', action='store_true', dest='DO_TIMING', default=True ,help='Run timing analysis')
  

  
  # store options and arguments as global variables
  global opt, args
  (opt, args) = parser.parse_args()

  if not os.path.isfile(opt.FILE_LIST):
    raise RuntimeError, "{0} does not exist! Need a list of all files to run over.".format(opt.FILE_LIST)



def memory_usage_resource():
    import resource
    rusage_denom = 1024.
    if sys.platform == 'darwin':
        # ... it seems that in OSX the output is different units ...
        rusage_denom = rusage_denom * rusage_denom
    mem = resource.getrusage(resource.RUSAGE_SELF).ru_maxrss / rusage_denom
    return mem



if __name__ == "__main__":
  parseOptions()
  
  ROOT.gROOT.ProcessLine(".L include/tdrstyle.cc")

  treeName = 'passedEvents'
  files = []
  makeRootFile = False
  
  
  if opt.FILE_LIST.find(".txt"):
    makeRootFile = True
    for line in open(opt.FILE_LIST,'r'):
      f = line.split()
      if len(f) < 1: continue
      if f[0].startswith("#"): continue
      files.append(f[0])
      print '{0} added to list'.format(f[0])
  else:
    files.append(opt.FILE_LIST)
    print '{0} added to list'.format(opt.FILE_LIST)

    print 'Will run over {0} events from {1} total files'.format(opt.NEVENTS,len(files))
    print 'Make final root file: '.format(makeRootFile)

  
  if opt.DO_TIMING:
    t = time.time()
    tp = time.clock()
  done = False
  myClass = AnalysisClass(makeRootFile,opt.JOB_NAME,opt.NEVENTS)
  j = 0
  while not done:
    if j == len(files): break
    print "Opening file: {0}".format(files[j])
    done = myClass.ProcessFile(files[j],treeName)
    print "Closing file: {0}".format(files[j])
    j += 1
    
  myClass.End()
  if opt.DO_TIMING: 
    et = time.time() - t
    etp = time.clock() - tp
    print ">>>>>>>>>>>>>>> Time Report <<<<<<<<<<<<<<<<<<"
    print et, "sec wall time to execute", opt.NEVENTS
    print etp, "sec process time to execute", opt.NEVENTS
    perEvent = et/float(opt.NEVENTS)
    print perEvent, "sec wall time per event"
  print ">>>>>>>>>>>>>>> Mem Report <<<<<<<<<<<<<<<<<<"
  print memory_usage_resource(), "MB mem used"
  
  

