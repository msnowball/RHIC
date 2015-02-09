#!/usr/bin/python
import sys, os, pwd, commands
import optparse, shlex, re
import math
from array import array
import collections, random

#Global variables
condorSettings = {}

def parseOptions():

  usage = ('usage: %prog [options] filelist.txt \n'
           + '%prog -h for help')
  parser = optparse.OptionParser(usage)

  parser.add_option('-b', action='store_true', dest='noX', default=True ,help='no X11 windows')
  parser.add_option('--submitHists',action='store_true', dest='SUBMIT_HISTS', default=True ,help='Submit hists')
  parser.add_option('-n', '--nFiles', dest='FILES_PER_JOB', type='int', default=20 ,help='N Files Per Job (def:10)')
  parser.add_option('-N', '--name', dest='JOB_NAME', type='string', default='' ,help='Job Name (def:)')
  parser.add_option('--submitFile', dest='SUBMIT_FILE', type='string', default='submitFile.csh' ,help='Submit file (def:submitFile.sh)')
  parser.add_option('-c', '--condor', dest='CONDOR_INPUT', type='string', default='condor.source' ,help='Condor input settings (def:condor.source)')
  
  # store options and arguments as global variables
  global opt, args
  (opt, args) = parser.parse_args()

  if opt.JOB_NAME == '':
    opt.JOB_NAME = args[0].split('.txt')[0]


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
  condorKeyWords = ['universe','getenv','notification','email','requirements','request_memory','rank','priority','executable','experiment','job_type']
  #Set some defaults in case the source file is missing
  condorSettings['universe']     = "vanilla"
  condorSettings['getenv']       = "True"
  condorSettings['notification'] = "Error"
  condorSettings['email']        = "snowball@rcf.rhic.bnl.gov"
  condorSettings['requirements'] = "CPU_Speed >= 1"
  condorSettings['request_memory'] = "1024"
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
  condorStartLine['request_memory'] = "request_memory  =  {0} ".format(condorSettings['request_memory'])
  condorStartLine['Rank']         = "Rank          =  {0} ".format(condorSettings['rank'])
  condorStartLine['Priority']     = "Priority      =  {0} ".format(condorSettings['priority'])
  #condorStartLine['Experiment']   = "+Experiment    = \"{0}\" ".format(condorSettings['experiment'])
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
  

def submitHists():
  
  #Make a directory for this set of jobs and copy necessary files
  curDir = os.getcwd()
  dirName = opt.JOB_NAME
  submitDir = curDir+'/'+dirName
  prepareDir(dirName)
  cmd = 'cp {0} {1}'.format('makeAna.py',dirName)
  processCmd(cmd)
  cmd = 'cp {0} {1}'.format('anaClass.py',dirName)
  processCmd(cmd)
  cmd = 'cp {0} {1}'.format('copyFromDcache.sh',dirName)
  processCmd(cmd)
  cmd = 'cp {0} {1}'.format(opt.SUBMIT_FILE,dirName)
  processCmd(cmd)
  cmd = 'chmod 755 {0}'.format(dirName+'/'+opt.SUBMIT_FILE)
  processCmd(cmd)

  outDir = submitDir+'/output'

  #Get settings for condor
  getCondorSettings()
  #Calculate the number of jobs needed
  fileList = []
  filesPerJob = opt.FILES_PER_JOB

  for line in open(args[0],'r'):
    f = line.split()
    if len(f) < 1: continue
    if f[0].startswith("#"): continue
    fileList.append(f[0])
    print f[0],"added to list"

  nJobs = 0
  leftover = 0
  if len(fileList)%filesPerJob == 0:
    nJobs = len(fileList)/filesPerJob
  else:
    nJobs = 1+(len(fileList)/filesPerJob) 
    leftover = len(fileList)%filesPerJob 
    
  print "Will run {0} jobs with {1} files each, on a total of {2} files".format(nJobs,filesPerJob,len(fileList))

  #Write out condor file
  jobSettings = {}
  fileCounter = 0

  for job in range(nJobs):
    theList = submitDir+'/'+opt.JOB_NAME+'_'+str(job)+'.txt'
    out = open(theList,'a')
    f = 0
    while f < filesPerJob:
      out.write("{0}\n".format(fileList[fileCounter]))
      fileCounter += 1
      f += 1
      if fileCounter == len(fileList): break
    
    jobName = opt.JOB_NAME+'_'+str(job)
    jobSettings['arguments'] = "{0} {1} {2}".format(jobName,submitDir,theList)
    jobSettings['initialdir'] = submitDir
    jobSettings['executable'] = submitDir+'/'+opt.SUBMIT_FILE
    jobSettings['error'] = submitDir+'/err/'+jobName+'.err'
    jobSettings['log'] = submitDir+'/log/'+jobName+'.log'
    settings = collections.OrderedDict(jobSettings)
    writeCondorFile(submitDir,settings,job)
    




def processCmd(cmd):
    status, output = commands.getstatusoutput(cmd)
    if status !=0:
        print 'Error in processing command:\n   ['+cmd+'] \nExiting...'
        sys.exit()



if __name__ == "__main__":
  parseOptions()
  

  if opt.SUBMIT_HISTS:
    submitHists()
