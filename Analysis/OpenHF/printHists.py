#!/usr/bin/python
import sys, os, pwd, commands
import optparse, shlex, re
import math
from array import array
import collections
import ROOT


def parseOptions():

  usage = ('usage: %prog [options] \n'
           + '%prog -h for help')
  parser = optparse.OptionParser(usage)

  parser.add_option('-b', action='store_true', dest='noX', default=True ,help='no X11 windows')
  parser.add_option('-f','--file', dest='FILE', type='string', default='test2.root' ,help='Histogram ROOT file')
  parser.add_option('-d', '--out', dest='OUTPUT_DIR', type='string', default='test' ,help='Output directory ')
  parser.add_option('--h1d', dest='drawOpt1D', type='string', default='HIST' ,help='TH1 Draw Options')
  parser.add_option('--h2d', dest='drawOpt2D', type='string', default='SCAT' ,help='TH2 Draw Options')
  parser.add_option('--h3d', dest='drawOpt3D', type='string', default='LEGOZ' ,help='TH3 Draw Options')
  

  
  # store options and arguments as global variables
  global opt, args
  (opt, args) = parser.parse_args()

  #if os.path.isdir(opt.OUTPUT_DIR):
  #    raise RuntimeError, "{0} exists!".format(opt.OUTPUT_DIR)
  #else:
  cmd = 'mkdir -p {0}'.format(opt.OUTPUT_DIR)
  processCmd(cmd)
  cmd = 'cp $HOME/WWW/index.php {0}'.format(opt.OUTPUT_DIR)
  processCmd(cmd)


def processCmd(cmd):
    status, output = commands.getstatusoutput(cmd)
    if status !=0:
        print 'Error in processing command:\n   ['+cmd+'] \nExiting...'
        sys.exit()


if __name__ == "__main__":
  parseOptions()
  
  ROOT.gROOT.ProcessLine(".x include/tdrstyle.cc")
  

  tfile = ROOT.TFile(opt.FILE,"READ")
  tfile.cd()
  dirList = ROOT.gDirectory.GetListOfKeys()
  for d in dirList: 
      hist = d.ReadObj()
      print hist.ClassName(), hist.GetName()
      c = ROOT.TCanvas('c','c',750,750)
      c.cd()
      if str(hist.ClassName()).startswith('TH1'):
        drawOpt = opt.drawOpt1D
      elif str(hist.ClassName()).startswith('TH2'):
        drawOpt = opt.drawOpt2D
      elif str(hist.ClassName()).startswith('TH3'):
        drawOpt = opt.drawOpt3D

      hist.Draw(drawOpt)
      c.SaveAs(opt.OUTPUT_DIR+'/'+hist.GetName()+'.eps')
      c.SaveAs(opt.OUTPUT_DIR+'/'+hist.GetName()+'.png')
      c.SaveAs(opt.OUTPUT_DIR+'/'+hist.GetName()+'.pdf')
      del c



