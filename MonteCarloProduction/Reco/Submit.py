#!/usr/bin/python
import sys, os, pwd, commands
import optparse, shlex, re
import math
from array import array
import collections, random

#Global variables
condorSettings = {}
eventsPerJob_pythia = 1000000
eventsPerPRDFF = 110000
runNumber = 368543
filesPerJob_ntuple = 2
filesPerJob_analysis = 30
GoodRuns = [364808,364830,365103,365115,365116,365118,365330,365340,365499,365500,365506,365537,365539,365544,365547,365671,365673,365723,365842,365845,365854,365855,365862,365889,365890,365891,365904,365906,365912,
            365914,365915,365929,366034,366035,366039,366041,366042,366053,366586,366632,366977,366978,367037,367038,367062,367067,367076,367099,367134,367161,367165,367232,367234,367236,367237,367238,367318,367319,
            367320,367321,367323,367336,367338,367340,367342,367343,367449,367451,367453,367454,367462,367464,367510,367512,367543,367545,367548,367550,367594,367596,367607,367608,367614,367617,367663,367716,367728,
            367921,367925,367928,368038,368183,368184,368186,368192,368206,368212,368213,368218,368282,368357,368359,368360,368363,368364,368366,368390,368393,368394,368533,368544,368546,368548,368621,368622,368628,
            368630,368683,368686,368689,368692,368742,368787,368789,368790] #Not all good runs, just a sampling of large good runs




def parseOptions():

  usage = ('usage: %prog [options] \n'
           + '%prog -h for help')
  parser = optparse.OptionParser(usage)

  parser.add_option('-b', action='store_true', dest='noX', default=True ,help='no X11 windows')
  parser.add_option('--submitPdst',action='store_true', dest='SUBMIT_PDST', default=False ,help='Submit pdst: pass list of files with --list')  
  parser.add_option('--createPdstInput',action='store_true', dest='CREATE_PDST_INPUT', default=False ,help='Create input file for pdst')

  parser.add_option('-f', '--file', dest='C_FILE', type='string', default='Fun4SingleMuRM.C' ,help='.C file')
  parser.add_option('--list', dest='FILE_LIST', type='string', default='list.txt' ,help='List of DSTs')

  parser.add_option('--submitFile', dest='SUBMIT_FILE', type='string', default='submitFile.csh' ,help='Executable file')

  parser.add_option('-c', '--condor', dest='CONDOR_INPUT', type='string', default='condor.source' ,help='Condor Command File')
  parser.add_option('-p', '--priority', dest='CONDOR_PRIORITY', type='string', default='' ,help='Condor Priority')
  parser.add_option('-e', '--email', dest='CONDOR_EMAIL', type='string', default='msnowball@bnl.gov' ,help='Condor Email')
  parser.add_option('-n', '--nEvents', dest='NEVENTS', type='float', default=1000 ,help='Number of events to produce/per file')
  parser.add_option('-N', '--name', dest='JOB_NAME', type='string', default='pythia' ,help='Job Name')
  parser.add_option('-o', '--out', dest='OUTPUT_DIR', type='string', default='' ,help='Output directory if different than default')
  
  #parser.add_option('--runLow', dest='RUNNUMBER_LOW', type='int', default= ,help='Output directory if different than default')
  #parser.add_option('--runHigh', dest='RUNNUMBER_HIGH', type='int', default= ,help='Output directory if different than default')


  
  # store options and arguments as global variables
  global opt, args
  (opt, args) = parser.parse_args()

  if not os.path.isfile(opt.CONDOR_INPUT):
    raise RuntimeError, "{0} does not exist!".format(opt.CONDOR_INPUT)
   


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
  condorSettings['executable']   = "$(InitialDir)/"+opt.SUBMIT_FILE
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
  


def submitPdst():
  
  #Make a directory for this set of jobs and copy necessary files
  curDir = os.getcwd()
  dirName = opt.JOB_NAME
  submitDir = curDir+'/'+dirName
  prepareDir(dirName)
  cmd = 'cp Fun4All_*.C {0}'.format(dirName)
  processCmd(cmd)
  cmd = 'cp {0} {1}'.format(opt.SUBMIT_FILE,dirName)
  processCmd(cmd)
  cmd = 'cp {0} {1}'.format(opt.FILE_LIST,dirName)
  processCmd(cmd)
  cmd = 'chmod 755 {0}'.format(dirName+'/'+opt.SUBMIT_FILE)
  processCmd(cmd)

  outDir = submitDir+'/output'

  if opt.OUTPUT_DIR != "":
    cmd = 'mkdir -p {0}'.format(opt.OUTPUT_DIR)
    outDir = opt.OUTPUT_DIR
  
  #Get settings for condor
  getCondorSettings()
  #Calculate the number of jobs needed
  fileListPythia = []
  fileListPisa = []
  fileListData = []

  for line in open(opt.FILE_LIST,'r'):
    f = line.split()
    if len(f) < 1: continue
    if f[0].startswith("#"): continue
    fileListPisa.append(f[0])
    print f[0], "added to list"

  #Write out condor file
  jobSettings = {}
  fileCounter = 0

  nJobs = len(fileListPisa)

  for job in range(nJobs):
    #randNumb = random.randint(0, len(GoodRuns)-1)
    #print randNumb, len(GoodRuns)
    jobName = opt.JOB_NAME+'_'+str(job)
    jobSettings['arguments'] = "{0} {1} {2} {3} {4} {5} {6}".format("0",fileListPisa[job],jobName,submitDir,outDir,submitDir+'/log',submitDir+'/err')
    jobSettings['initialdir'] = submitDir
    jobSettings['executable'] = submitDir+'/'+opt.SUBMIT_FILE
    jobSettings['error'] = submitDir+'/err/'+jobName+'.err'
    jobSettings['log'] = submitDir+'/log/'+jobName+'.log'
    settings = collections.OrderedDict(jobSettings)
    writeCondorFile(submitDir,settings,job)
    




def createPdstInput():
  
  outFileName = ['pdstInputFile_bbbar.txt','pdstInputFile_ccbar.txt']
  pisafiles = ['pisa_bbbar.txt','pisa_ccbar.txt']
  dstfiles = ['MinBiasList.txt']
  

  fileListPythia = []
  fileListPisa = []  
  fileListData = []

  counter = 0
  for pisaFile in range(len(pisafiles)):
    fileListPythia.append([])
    fileListPisa.append([])
    for line in open(pisafiles[pisaFile]):
      f = line.split()
      if len(f) < 1: continue
      if f[0].startswith("#"): continue
      fileListPythia[counter].append(f[0])
      fileListPisa[counter].append(f[1])
    counter+=1
      
  for dstFile in range(len(dstfiles)):
    for line in open(dstfiles[dstFile]):
      f = line.split()
      if len(f) < 1: continue
      if f[0].startswith("#"): continue
      fileListData.append(f[0])


  counterNew = 0
  totalCounter = 0
  for i in range(counter):
    f = open(outFileName[counterNew],'w')
    for l in range(len(fileListPythia[counterNew])):
      myLine = fileListPythia[counterNew][l]+' '+fileListPisa[counterNew][l]+' '+fileListData[totalCounter]+'\n'
      totalCounter+=1
      f.write(myLine)
    counterNew+=1
    f.close()
  

    






def processCmd(cmd):
    status, output = commands.getstatusoutput(cmd)
    if status !=0:
        print 'Error in processing command:\n   ['+cmd+'] \nExiting...'
        sys.exit()



if __name__ == "__main__":
  parseOptions()
  

  if opt.SUBMIT_PDST:
    submitPdst()
  elif opt.CREATE_PDST_INPUT:
    createPdstInput()
