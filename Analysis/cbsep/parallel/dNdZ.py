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
  parser.add_option('--dndz', action='store_true', dest='DNDZ', default=True ,help='Draw and Normalize dN/dZ')
  parser.add_option('--fit', action='store_true', dest='FIT', default=False ,help='Do linear fit')


  
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
  
  ROOT.gROOT.ProcessLine(".x $HOME/local/include/tdrstyle.cc")
  ROOT.gStyle.SetOptStat(0)

  dndzHists_LG3_N = []
  dndzHists_LG3_S = []

  dndzHists_LG4_N = []
  dndzHists_LG4_S = []

  tfile = ROOT.TFile(opt.FILE,"READ")
  tfile.cd()
  dirList = ROOT.gDirectory.GetListOfKeys()
  for d in dirList: 
      hist = d.ReadObj()
      print hist.ClassName(), hist.GetName()
      if str(hist.ClassName()).startswith('TH1'):
        if opt.DNDZ and str(hist.GetName()).startswith('dNdZ'):
            if str(hist.GetName()).endswith('LG3_N'):
                dndzHists_LG3_N.append(hist)
            if str(hist.GetName()).endswith('LG3_S'):
                dndzHists_LG3_S.append(hist)
            if str(hist.GetName()).endswith('LG4_N'):
                dndzHists_LG4_N.append(hist)
            if str(hist.GetName()).endswith('LG4_S'):
                dndzHists_LG4_S.append(hist)

  

  latex = ROOT.TLatex()
  latex.SetTextAlign(11)
  latex.SetTextFont(42)
  latex.SetTextSize(0.08)
  theString = ''
  #North
  dndzCanvas_LG3_N = ROOT.TCanvas('cN1','cN1',1000,1200)
  theDivide = len(dndzHists_LG3_N)%2 
  canvDiv = len(dndzHists_LG3_N)/2 + theDivide
  #print canvDiv
  dndzCanvas_LG3_N.Divide(2,canvDiv,0.01,0.01)
  for h in range(len(dndzHists_LG3_N)):
    dndzCanvas_LG3_N.cd(h+1)
    dndzCanvas_LG3_N.SetTopMargin(0.)
    dndzCanvas_LG3_N.SetBottomMargin(0.)
    dndzHists_LG3_N[h].Sumw2()
    dndzHists_LG3_N[h].Scale(1/dndzHists_LG3_N[h].Integral())
    thePts = str(dndzHists_LG3_N[h].GetName()).split('_')
    thePtLow = thePts[2]
    thePtHigh = thePts[3]
    theTitle = 'pT: {0}-{1}'.format(thePtLow,thePtHigh)
    if opt.FIT:
        dndzHists_LG3_N[h].Fit('pol1')
        theFunc = dndzHists_LG3_N[h].GetFunction('pol1')
        theFunc.SetLineColor(ROOT.kRed)
        theSlope = theFunc.GetParameter(0)
        theSlopeErr = theFunc.GetParError(0)
        theString = 'slope = {0} #pm {1}'.format(theSlope,theSlopeErr)
    dndzHists_LG3_N[h].Draw('el')
    latex.DrawLatex(0.2,0.3,theTitle)    
    if opt.FIT: 
      latex.DrawLatex(0.1,0.2,theString)    
      theFunc.Draw('SAME')

    

  dndzCanvas_LG3_N.SaveAs(opt.OUTPUT_DIR+'/dNdZ_LG3_N.eps')
  dndzCanvas_LG3_N.SaveAs(opt.OUTPUT_DIR+'/dNdZ_LG3_N.png')


  #LG4
  dndzCanvas_LG4_N = ROOT.TCanvas('cN2','cN2',1000,1200)
  theDivide = len(dndzHists_LG4_N)%2 
  canvDiv = len(dndzHists_LG4_N)/2 + theDivide
  #print canvDiv
  dndzCanvas_LG4_N.Divide(2,canvDiv,0.01,0.01)
  for h in range(len(dndzHists_LG4_N)):
    dndzCanvas_LG4_N.cd(h+1)
    dndzCanvas_LG4_N.SetTopMargin(0.)
    dndzCanvas_LG4_N.SetBottomMargin(0.)
    dndzHists_LG4_N[h].Sumw2()
    dndzHists_LG4_N[h].Scale(1/dndzHists_LG4_N[h].Integral())
    thePts = str(dndzHists_LG4_N[h].GetName()).split('_')
    thePtLow = thePts[2]
    thePtHigh = thePts[3]
    theTitle = 'pT: {0}-{1}'.format(thePtLow,thePtHigh)
    if opt.FIT:
        dndzHists_LG4_N[h].Fit('pol1')
        theFunc = dndzHists_LG4_N[h].GetFunction('pol1')
        theFunc.SetLineColor(ROOT.kRed)
        theSlope = theFunc.GetParameter(0)
        theSlopeErr = theFunc.GetParError(0)
        theString = 'slope = {0} #pm {1}'.format(theSlope,theSlopeErr)
    dndzHists_LG4_N[h].Draw('el')
    latex.DrawLatex(0.2,0.3,theTitle)    
    if opt.FIT: 
      latex.DrawLatex(0.1,0.2,theString)    
      theFunc.Draw('SAME')

    

  dndzCanvas_LG4_N.SaveAs(opt.OUTPUT_DIR+'/dNdZ_LG4_N.eps')
  dndzCanvas_LG4_N.SaveAs(opt.OUTPUT_DIR+'/dNdZ_LG4_N.png')




  #South
  dndzCanvas_LG3_S = ROOT.TCanvas('cS1','cS1',1000,1200)
  theDivide = len(dndzHists_LG3_S)%2 
  canvDiv = len(dndzHists_LG3_S)/2 + theDivide
  #print canvDiv
  dndzCanvas_LG3_S.Divide(2,canvDiv,0.01,0.01)
  for h in range(len(dndzHists_LG3_S)):
    dndzCanvas_LG3_S.cd(h+1)
    dndzCanvas_LG3_S.SetTopMargin(0.)
    dndzCanvas_LG3_S.SetBottomMargin(0.)
    dndzHists_LG3_S[h].Sumw2()
    dndzHists_LG3_S[h].Scale(1/dndzHists_LG3_S[h].Integral())
    thePts = str(dndzHists_LG3_S[h].GetName()).split('_')
    thePtLow = thePts[2]
    thePtHigh = thePts[3]
    theTitle = 'pT: {0}-{1}'.format(thePtLow,thePtHigh)
    if opt.FIT:
        dndzHists_LG3_S[h].Fit('pol1')
        theFunc = dndzHists_LG3_S[h].GetFunction('pol1')
        theFunc.SetLineColor(ROOT.kRed)
        theSlope = theFunc.GetParameter(0)
        theSlopeErr = theFunc.GetParError(0)
        theString = 'slope = {0} #pm {1}'.format(theSlope,theSlopeErr)
    dndzHists_LG3_S[h].Draw('el')
    latex.DrawLatex(0.2,0.3,theTitle)    
    if opt.FIT: 
      latex.DrawLatex(0.1,0.2,theString)    
      theFunc.Draw('SAME')

    

  dndzCanvas_LG3_S.SaveAs(opt.OUTPUT_DIR+'/dNdZ_LG3_S.eps')
  dndzCanvas_LG3_S.SaveAs(opt.OUTPUT_DIR+'/dNdZ_LG3_S.png')


  #LG4
  dndzCanvas_LG4_S = ROOT.TCanvas('cS2','cS2',1000,1200)
  theDivide = len(dndzHists_LG4_S)%2 
  canvDiv = len(dndzHists_LG4_S)/2 + theDivide
  #print canvDiv
  dndzCanvas_LG4_S.Divide(2,canvDiv,0.01,0.01)
  for h in range(len(dndzHists_LG4_S)):
    dndzCanvas_LG4_S.cd(h+1)
    dndzCanvas_LG4_S.SetTopMargin(0.)
    dndzCanvas_LG4_S.SetBottomMargin(0.)
    dndzHists_LG4_S[h].Sumw2()
    dndzHists_LG4_S[h].Scale(1/dndzHists_LG4_S[h].Integral())
    thePts = str(dndzHists_LG4_S[h].GetName()).split('_')
    thePtLow = thePts[2]
    thePtHigh = thePts[3]
    theTitle = 'pT: {0}-{1}'.format(thePtLow,thePtHigh)
    if opt.FIT:
        dndzHists_LG4_S[h].Fit('pol1')
        theFunc = dndzHists_LG4_S[h].GetFunction('pol1')
        theFunc.SetLineColor(ROOT.kRed)
        theSlope = theFunc.GetParameter(0)
        theSlopeErr = theFunc.GetParError(0)
        theString = 'slope = {0} #pm {1}'.format(theSlope,theSlopeErr)
    dndzHists_LG4_S[h].Draw('el')
    latex.DrawLatex(0.2,0.3,theTitle)    
    if opt.FIT: 
      latex.DrawLatex(0.1,0.2,theString)    
      theFunc.Draw('SAME')

    

  dndzCanvas_LG4_S.SaveAs(opt.OUTPUT_DIR+'/dNdZ_LG4_S.eps')
  dndzCanvas_LG4_S.SaveAs(opt.OUTPUT_DIR+'/dNdZ_LG4_S.png')
