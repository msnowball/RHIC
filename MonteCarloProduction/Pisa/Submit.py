#!/usr/bin/python
import sys, os, pwd, commands
import optparse, shlex, re
import math
from array import array
import collections

#Global variables
condorSettings = {}
eventsPerJob_pythia = 1000000
eventsPerPRDFF = 110000
runNumber = 368543

eventsPerJob_pisa = 50000
pisaOutputDir = '/direct/phenix+user08/snowball/storage/MonteCarlo/SIM'

def parseOptions():

  usage = ('usage: %prog [options] \n'
           + '%prog -h for help')
  parser = optparse.OptionParser(usage)

  parser.add_option('-b', action='store_true', dest='noX', default=True ,help='no X11 windows')
  parser.add_option('--submitPythia', action='store_true', dest='SUBMIT_PYTHIA', default=False ,help='Submit Pythia: pass C file with -f')
  parser.add_option('--submitDST', action='store_true', dest='SUBMIT_DST', default=False ,help='Submit DST: pass PRDF file with --prdf')
  parser.add_option('--submitPISA', action='store_true', dest='SUBMIT_PISA', default=False ,help='Submit PISA')

  parser.add_option('--fastSim', action='store_true', dest='FAST_SIM', default=False ,help='Fast SIM - ignore showering')

  parser.add_option('--cfg', dest='PYTHIA_CFG', type='string', default='phpythia_OpenHF.cfg' ,help='Pythia .cfg file')
  parser.add_option('--prdf', dest='PRDF_LIST', type='string', default='prdf.txt' ,help='List of PRDFFs')
  parser.add_option('--submitFile', dest='SUBMIT_FILE', type='string', default='submitFile.csh' ,help='Executable file')
  parser.add_option('--pisaList', dest='PISA_LIST', type='string', default='pisa_list.txt' ,help='List of pythia produced event files to feed to PISA')

  parser.add_option('-f', '--file', dest='C_FILE', type='string', default='phpythia_OpenHF.C' ,help='.C file')
  parser.add_option('-c', '--condor', dest='CONDOR_INPUT', type='string', default='condor.source' ,help='Condor Command File')
  parser.add_option('-p', '--priority', dest='CONDOR_PRIORITY', type='string', default='' ,help='Condor Priority')
  parser.add_option('-e', '--email', dest='CONDOR_EMAIL', type='string', default='msnowball@bnl.gov' ,help='Condor Email')
  parser.add_option('-n', '--nEvents', dest='NEVENTS', type='float', default=1000 ,help='Number of events to produce/per file')
  parser.add_option('-N', '--name', dest='JOB_NAME', type='string', default='pythia' ,help='Job Name')
  

  
  # store options and arguments as global variables
  global opt, args
  (opt, args) = parser.parse_args()

  # Check that the pythia source file exists
  if opt.SUBMIT_PYTHIA:
    if not os.path.isfile(opt.C_FILE):
      raise RuntimeError, "{0} does not exist!".format(opt.C_FILE)
    if not os.path.isfile(opt.PYTHIA_CFG):
      raise RuntimeError, "{0} does not exist! Pythia expects a cfg file with the same name as the script.".format(opt.PYTHIA_CFG)
    if os.path.isdir(opt.JOB_NAME):
      print "Directory {0} exists!".format(opt.JOB_NAME)
      sys.exit()

  # For making DSTs from PRDFFs - Vertex embedding
  if opt.SUBMIT_DST:
    if not os.path.isfile(opt.C_FILE):
      raise RuntimeError, "{0} does not exist!".format(opt.C_FILE)
    if not os.path.isfile(opt.PRDF_LIST):
      raise RuntimeError, "{0} does not exist! Need a list of all PRDFFs to run over.".format(opt.PRDF_LIST)
    if os.path.isdir(opt.JOB_NAME):
      print "Directory {0} exists!".format(opt.JOB_NAME)
      sys.exit()

  # For making SIM events from PISA
  if opt.SUBMIT_PISA:
    if not os.path.isfile(opt.PISA_LIST):
      raise RuntimeError, "{0} does not exist!".format(opt.PISA_LIST)
    if os.path.isdir(opt.JOB_NAME):
      print "Directory {0} exists!".format(opt.JOB_NAME)
      sys.exit()



  if not os.path.isfile(opt.CONDOR_INPUT):
    raise RuntimeError, "{0} does not exist!".format(opt.CONDOR_INPUT)
   

  #Some more variables
  opt.SUBMIT_FILE = 'submitFile.csh'


def prepareDir(dirName):
  
  print "Making directory {0}".format(dirName)
  cmd = 'mkdir -p {0}'.format(dirName)
  processCmd(cmd)
  cmd = 'mkdir -p {0}'.format(dirName+'/output')
  processCmd(cmd)
  cmd = 'mkdir -p {0}'.format(dirName+'/log')
  processCmd(cmd)
  cmd = 'mkdir -p {0}'.format(dirName+'/err')
  processCmd(cmd)

def prepareDirPisa(dirName):
  
  print "Making directory {0}".format(dirName)
  cmd = 'mkdir -p {0}'.format(dirName)
  processCmd(cmd)
  cmd = 'mkdir -p {0}'.format(dirName+'/log')
  processCmd(cmd)
  cmd = 'mkdir -p {0}'.format(dirName+'/err')
  processCmd(cmd)



def submitPythia():
  
  #Make a directory for this set of jobs and copy necessary files
  curDir = os.getcwd()
  dirName = opt.JOB_NAME
  submitDir = curDir+'/'+dirName
  prepareDir(dirName)
  cmd = 'cp {0} {1}'.format(opt.C_FILE,dirName)
  processCmd(cmd)
  cmd = 'cp {0} {1}'.format(opt.PYTHIA_CFG,dirName)
  processCmd(cmd)
  cmd = 'cp {0} {1}'.format(opt.SUBMIT_FILE,dirName)
  processCmd(cmd)
  cmd = 'chmod 755 {0}'.format(dirName+'/'+opt.SUBMIT_FILE)
  processCmd(cmd)
  
  #Get settings for condor
  getCondorSettings()
  #Calculate the number of jobs needed
  nJobs = opt.NEVENTS/float(eventsPerJob_pythia)
  nJobs = int(nJobs)
  print  "Requested {0:.2e} events".format(int(opt.NEVENTS))
  actualNumbEvents = eventsPerJob_pythia
  if opt.NEVENTS < eventsPerJob_pythia or nJobs == 0:
    nJobs = 1
    actualNumbEvents = opt.NEVENTS
  
  print "Will run {0} jobs with {1:.2e} events each".format(nJobs,actualNumbEvents)

  #Write out condor file
  jobSettings = {}
  for job in range(nJobs):
    jobName = opt.JOB_NAME+'_'+str(job)
    jobSettings['arguments'] = "{0} {1} {2} {3} {4} {5}".format(actualNumbEvents,jobName,submitDir,submitDir+'/output',submitDir+'/log',submitDir+'/err')
    jobSettings['initialdir'] = submitDir
    jobSettings['executable'] = submitDir+'/'+opt.SUBMIT_FILE
    jobSettings['error'] = submitDir+'/err/'+jobName+'.err'
    jobSettings['log'] = submitDir+'/log/'+jobName+'.log'
    settings = collections.OrderedDict(jobSettings)
    writeCondorFile(submitDir,settings,job)



def submitPisa():
  
  #Make a directory for this set of jobs and copy necessary files
  curDir = os.getcwd()
  dirName = opt.JOB_NAME
  theOutputDir = pisaOutputDir+'/{0}'.format(opt.JOB_NAME)
  #pisaOutputDir = 'test'
  submitDir = curDir+'/'+dirName
  cmd = 'mkdir -p {0}'.format(dirName)
  processCmd(cmd)
  prepareDirPisa(theOutputDir)
  


  #Iterate over list of pythia files to SIM
  inFiles = []
  vertexFiles = []
  jobCounter = 0
  for line in open(opt.PISA_LIST,'r'):
    f = line.split()
    if len(f) < 1: continue
    if f[0].startswith('#'): continue
    jobCounter+=1
    inFiles.append(f[0])
    for line2 in open(f[0]+'_filetracking.txt','r'):
      f2 = line2.split()
      if len(f2) < 1: continue
      vertexFiles.append(f2[0])
    
    


  #Make glogon.kumac file
  glogon = open('glogon.kumac','w')
  myLine = "phpythia 100 phpythia.root\n"
  glogon.write(myLine)
  myLine = "ptrig {0}\n".format(eventsPerJob_pisa)
  glogon.write(myLine)
  myLine = "exit \nreturn \n"
  glogon.write(myLine)
  glogon.close()

  #Copy all needed files
  cpFiles = ['Sim3D++.root','event.par','flukaaf.dat','gffgo.dat','phnx.par','pisa.input','pisa.kumac','xsneut95.dat','glogon.kumac']
  if opt.FAST_SIM: cpFiles = ['Sim3D++.root','event.par','flukaaf.dat','gffgo.dat','phnx.par','pisa.input','pisa.kumac.fastsim','xsneut95.dat','glogon.kumac']
  for i in range(len(cpFiles)):
    cmd = 'cp {0} {1}'.format(cpFiles[i],dirName)
    processCmd(cmd)
  if opt.FAST_SIM: 
    cmd = "mv {0}/{1} {0}/{2}".format(dirName,"pisa.kumac.fastsim","pisa.kumac")
    processCmd(cmd)
  cmd = 'cp {0} {1}'.format(opt.SUBMIT_FILE,dirName)
  processCmd(cmd)
  cmd = 'chmod 755 {0}'.format(dirName+'/'+opt.SUBMIT_FILE)
  processCmd(cmd)
  
  
  #Get settings for condor
  getCondorSettings()
  print "Will run {0} jobs with {1:.2e} events each".format(jobCounter,eventsPerJob_pisa)
  print "Output directory = {0}".format(theOutputDir)
  print "Log directory = {0}".format(theOutputDir+'/log')
  if opt.FAST_SIM: print "DANGER WILL ROBINSON: You're going to run FASTSIM!" 

  #Write out condor file
  jobSettings = {}
  for job in range(jobCounter):
    jobName = opt.JOB_NAME+'_'+str(job+1)
    jobSettings['arguments'] = "{0} {1} {2} {3} {4} {5} {6}".format(jobName,inFiles[job],vertexFiles[job],submitDir,theOutputDir,theOutputDir+'/log',theOutputDir+'/err')
    jobSettings['initialdir'] = submitDir
    jobSettings['executable'] = submitDir+'/'+opt.SUBMIT_FILE
    jobSettings['error'] = theOutputDir+'/err/'+jobName+'.err'
    jobSettings['log'] = theOutputDir+'/log/'+jobName+'.log'
    settings = collections.OrderedDict(jobSettings)
    writeCondorFile(submitDir,settings,job)
  
  



def getCondorSettings():
  
  #All settings needed
  condorKeyWords = ['universe','getenv','notification','email','requirements','rank','priority','executable','experiment','job_type']
  #Set some defaults in case the source file is missing
  condorSettings['universe']     = "vanilla"
  condorSettings['getenv']       = "True"
  condorSettings['notification'] = "Error"
  condorSettings['email']        = "snowball@rcf.rhic.bnl.gov"
  condorSettings['requirements'] = "CPU_Speed >= 1 && Memory >= 1024"
  condorSettings['rank']         = "CPU_Speed"
  condorSettings['priority']     = "+20"
  condorSettings['executable']   = "submitFile.csh"
  condorSettings['experiment']   = "phenix"
  condorSettings['job_type']     = "cas"

  #Check the source file 
  for line in open(opt.CONDOR_INPUT,'r'):
    f = line.split()
    if len(f) < 1: continue
    if f[0].startswith("#"): continue

    found = False
    for key in condorKeyWords:
      if f[0].lower().startswith(key):       
        condorSettings[key.lower()] = f[1]
        found = True

    if not found:
      print "Condor Keyword {0} not found!\n".format(f[0])

    for key in condorKeyWords:
      if condorSettings[key] == "":
        raise RuntimeError, "Condor Keyword {0} not set!".format(key)
        


def writeCondorFile(submitDir,jobSettings,jobCounter):

  condorStartLine = {}
  condorLine = {}

  condorStartLine['Universe']     = "Universe      =  {0} ".format(condorSettings['universe'])
  condorStartLine['GetEnv']       = "GetEnv        =  {0} ".format(condorSettings['getenv'])
  condorStartLine['Notification'] = "Notification  =  {0} ".format(condorSettings['notification'])
  condorStartLine['Notify_user']  = "Notify_user   =  {0} ".format(condorSettings['email'])
  condorStartLine['Initialdir']   = "Initialdir     = {0} ".format(jobSettings['initialdir'])
  condorStartLine['Requirements'] = "Requirements  =  {0} ".format(condorSettings['requirements'])
  condorStartLine['Rank']         = "Rank          =  {0} ".format(condorSettings['rank'])
  condorStartLine['Priority']     = "Priority      =  {0} ".format(condorSettings['priority'])
  condorStartLine['Experiment']   = "+Experiment    = \"{0}\" ".format(condorSettings['experiment'])
  condorStartLine['Job_Type']     = "+Job_Type      = \"{0}\" ".format(condorSettings['job_type'])
  

  condorLine['Arguments']    = "Arguments      = {0} ".format(jobSettings['arguments'])
  condorLine['Executable']   = "Executable     = {0} ".format(condorSettings['executable'])
  condorLine['Error']        = "Error          = {0} ".format(jobSettings['error'])
  condorLine['Log']          = "Log            = {0} ".format(jobSettings['log'])


  with open(submitDir+"/condor.job", "a") as f:
    if jobCounter < 1:
      for key in condorStartLine:
        f.write(condorStartLine[key]+'\n')
      f.write('\n\n')
    for key2 in condorLine:
      f.write(condorLine[key2]+'\n')
    f.write('Queue\n')
    f.write('\n\n')
  


def submitDST():
  
  #Make a directory for this set of jobs and copy necessary files
  curDir = os.getcwd()
  dirName = opt.JOB_NAME
  submitDir = curDir+'/'+dirName
  prepareDir(dirName)
  cmd = 'cp {0} {1}'.format(opt.C_FILE,dirName)
  processCmd(cmd)
  cmd = 'cp {0} {1}'.format(opt.SUBMIT_FILE,dirName)
  processCmd(cmd)
  cmd = 'cp {0} {1}'.format(opt.PRDF_LIST,dirName)
  processCmd(cmd)
  cmd = 'chmod 755 {0}'.format(dirName+'/'+opt.SUBMIT_FILE)
  processCmd(cmd)
  
  #Get settings for condor
  getCondorSettings()
  #Calculate the number of jobs needed
  eventsPerJob = opt.NEVENTS
  jobsPerFile = int(float(eventsPerPRDFF)/eventsPerJob)
  prdffList = []

  for line in open(opt.PRDF_LIST,'r'):
    f = line.split()
    if len(f) < 1: continue
    if f[0].startswith("#"): continue
    prdffList.append(f[0])
    print f[0],"added to list"


  nFiles = len(prdffList)
  nJobs = jobsPerFile*nFiles
    
  print "Will run {0} jobs with {1:.2e} events in each DST, on a total of {2} PRDFF files with {3} jobs per file".format(nJobs,eventsPerJob,nFiles,jobsPerFile)

  #Write out condor file
  jobSettings = {}
  fileCounter = 0
  for prdfFile in prdffList:
    eventsToSkip = 0
    fileCounter += 1
    for job in range(jobsPerFile):
      jobName = opt.JOB_NAME+'_'+str(fileCounter)+'_'+str(job)
      jobSettings['arguments'] = "{0} {1} {2} {3} {4} {5} {6} {7} {8}".format(eventsPerJob,eventsToSkip,jobName,submitDir,submitDir+'/output',submitDir+'/log',submitDir+'/err',prdfFile, runNumber)
      jobSettings['initialdir'] = submitDir
      jobSettings['executable'] = submitDir+'/'+opt.SUBMIT_FILE
      jobSettings['error'] = submitDir+'/err/'+jobName+'.err'
      jobSettings['log'] = submitDir+'/log/'+jobName+'.log'
      settings = collections.OrderedDict(jobSettings)
      writeCondorFile(submitDir,settings,job)
      eventsToSkip += eventsPerJob

  




def processCmd(cmd):
    status, output = commands.getstatusoutput(cmd)
    if status !=0:
        print 'Error in processing command:\n   ['+cmd+'] \nExiting...'
        sys.exit()



if __name__ == "__main__":
  parseOptions()
  

  if opt.SUBMIT_PYTHIA:
    submitPythia()
  elif opt.SUBMIT_DST:
    submitDST()
  elif opt.SUBMIT_PISA:
    submitPisa()
