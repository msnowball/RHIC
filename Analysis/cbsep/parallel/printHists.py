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
  parser.add_option('-f','--file', dest='FILE', type='string', default='' ,help='Histogram ROOT file')
  parser.add_option('-d', '--out', dest='OUTPUT_DIR', type='string', default='plots' ,help='Output directory ')
  parser.add_option('--h1d', dest='drawOpt1D', type='string', default='HIST' ,help='TH1 Draw Options')
  parser.add_option('--h2d', dest='drawOpt2D', type='string', default='SCAT' ,help='TH2 Draw Options')
  parser.add_option('--h3d', dest='drawOpt3D', type='string', default='LEGOZ' ,help='TH3 Draw Options')
  parser.add_option('--logx', action='store_true', dest='LOGX', default=False ,help='Log X')
  parser.add_option('--logy', action='store_true', dest='LOGY', default=False ,help='Log Y')



  
  # store options and arguments as global variables
  global opt, args
  (opt, args) = parser.parse_args()

  if not os.path.isfile(opt.FILE):
    raise RuntimeError, "{0} does not exist!".format(opt.FILE)
  
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
  
  ROOT.gROOT.ProcessLine(".x $HOME/local/include/tdrstyle.cc")
  ROOT.gStyle.SetOptStat(0)

  logString = ''

  if opt.LOGX: 
    ROOT.gPad.SetLogx()
    logString += '_logx'
  if opt.LOGY:
    ROOT.gPad.SetLogy()
    logString += '_logy'

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
      c.SaveAs(opt.OUTPUT_DIR+'/'+hist.GetName()+logString+'.eps')
      c.SaveAs(opt.OUTPUT_DIR+'/'+hist.GetName()+logString+'.png')
      c.SaveAs(opt.OUTPUT_DIR+'/'+hist.GetName()+logString+'.pdf')
      del c


