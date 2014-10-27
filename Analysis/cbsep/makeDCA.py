#!/usr/bin/python
import sys, os, pwd, commands
import optparse, shlex, re
import math, time
from array import array
import collections
import ROOT
from config import *


def parseOptions():

  usage = ('usage: %prog [options] \n'
           + '%prog -h for help')
  parser = optparse.OptionParser(usage)

  parser.add_option('-b', action='store_true', dest='noX', default=True ,help='no X11 windows')
  parser.add_option('-d','--dir', dest='OUTPUT_DIR', type='string', default='plots', help='Output director (default:./plots)')

  global opt, args
  (opt, args) = parser.parse_args()

  cmd = "mkdir -p {0}".format(opt.OUTPUT_DIR)
  processCmd(cmd)




def draw_dca_distrib():

  ROOT.gROOT.ProcessLine(".x include/tdrstyle.cc")
  ROOT.gStyle.SetOptStat(000000)
  ROOT.gStyle.SetPadTickX(1)
  ROOT.gStyle.SetPadTickY(1)
  ROOT.gStyle.SetPadGridX(1)
  ROOT.gStyle.SetPadGridY(1)

  
  ttrees = []
  tfiles = [] 

  

  print ">>>> Reading files"
  for iSample in range(len(kTreeFiles)):
    chain = ROOT.TChain(kTreeName)
    if kTreeFiles[iSample].lower().endswith(".txt"):
      for rootFile in open(kTreeFiles[iSample]):
        line = rootFile.split()
        if line[0].startswith("#"): continue
        if len(line[0]) < 1: continue
        print ">> Adding {0} to chain".format(line[0])
        chain.Add(line[0])
      ttrees.append(chain)
  
    else:
      tf = ROOT.TFile(kTreeFiles[iSample],"READ")
      if tf.IsZombie():
        raise RuntimeError, "Could not find file {0}".format(kTreeFiles[iSample])
      tfiles.append(tf)
      print tfiles[iSample].GetName()
      t = tfiles[iSample].Get(kTreeName)
      if not t:
        raise RuntimeError, "Could not find tree {0} in file {1}".format(kTreeName,tfiles[iSample].GetName())
      ttrees.append(t)

  drawDCAPhiValue = "SingleMuons.dca_phi"
  drawDCAValue = "SingleMuons.dca_r"

  drawsDCAPhiValue = "SingleMuons.sdca_phi"
  drawsDCAValue = "SingleMuons.sdca_r"
  
  #cutValue = "abs(Evt_bbcZ)<40"
  cutValue = "abs(Evt_bbcZ)<40&&SingleMuons.nhits_fvtx>2"
  dca_phi_cutValue = "&&abs(SingleMuons.dca_phi)<10"
  mu_cutValue = "&&SingleMuons.trchi2<5&&SingleMuons.lastgap==4&&abs(SingleMuons.rapidity)>1.2&&abs(SingleMuons.rapidity)<2.4&&sqrt(SingleMuons.px*SingleMuons.px+SingleMuons.py*SingleMuons.py)<30"
  mu_cutValue2 = "&&abs(SingleMuons.pz)>2.8&&abs(SingleMuons.pz)<=30&&SingleMuons.DDG0<10&&SingleMuons.ntrhits>=12&&((SingleMuons.rapidity>0&&SingleMuons.DG0<15)||(SingleMuons.rapidity<0&&SingleMuons.DG0<20))"
  mu_cutValue3 = "&&chi2_fvtxmutr<3"
    
  cutValue2 = cutValue+dca_phi_cutValue+mu_cutValue+mu_cutValue2

  cutValue += dca_phi_cutValue
  cutValue += mu_cutValue
  cutValue += mu_cutValue2
  cutValue += mu_cutValue3
  #cut_value += "&&Entry$<100"

  print ">>>> Muon p,pT"
  #Draw p,pT
  hMuonPt = []
  hMuonP = []
  hMuonPz = []
  hMuonEta = []
  hNhitsFvtx = []
  hDG0 = []
  hDDG0 = []
  hChi2 = []
  hdrFvtxSigma = []
  for iSample in range(len(kTreeFiles)):
    print kSampleNames[iSample]
    hMuonPt_name = "mu_pT_"+kSampleNames[iSample]
    hist = ROOT.TH1F(hMuonPt_name,";pT (GeV); N Muons",kNpTbins,kpTMin,kpTMax)
    ttrees[iSample].Draw( kpTVarName + ">>" + hMuonPt_name, cutValue )
    if kNormalize:
      hist.Sumw2()
      scale = kLumi*kSampleSigmas[iSample]/hist.GetEntries()
      hist.Scale(scale)      
    hMuonPt.append(hist)
    
    hMuonP_name = "mu_p_"+kSampleNames[iSample]
    hist = ROOT.TH1F(hMuonP_name,";P (GeV); N Muons",kNpbins,kpMin,kpMax)
    ttrees[iSample].Draw( kpVarName + ">>" + hMuonP_name, cutValue )
    if kNormalize:
      hist.Sumw2()
      scale = kLumi*kSampleSigmas[iSample]/hist.GetEntries()
      hist.Scale(scale)
    hMuonP.append(hist)

    hMuonPz_name = "mu_pz_"+kSampleNames[iSample]
    hist = ROOT.TH1F(hMuonPz_name,";pZ (GeV); N Muons",kNpbins*2,-kpMax,kpMax)
    ttrees[iSample].Draw( kpzVarName + ">>" + hMuonPz_name, cutValue )
    if kNormalize:
      hist.Sumw2()
      scale = kLumi*kSampleSigmas[iSample]/hist.GetEntries()
      hist.Scale(scale)
    hMuonPz.append(hist)

    hMuonEta_name = "mu_eta_"+kSampleNames[iSample]
    hist = ROOT.TH1F(hMuonEta_name,";eta; N Muons",50,-2.5,2.5)
    ttrees[iSample].Draw( "SingleMuons.rapidity>>" + hMuonEta_name, cutValue )
    if kNormalize:
      hist.Sumw2()
      scale = kLumi*kSampleSigmas[iSample]/hist.GetEntries()
      hist.Scale(scale)
    hMuonEta.append(hist)

    hNhitsFvtx_name = "nhits_fvtx_"+kSampleNames[iSample]
    hist = ROOT.TH1F(hNhitsFvtx_name,";N Hits FVTX; N Events",20,0,20)
    ttrees[iSample].Draw( "SingleMuons.nhits_fvtx>>" + hNhitsFvtx_name, cutValue )
    if kNormalize:
      hist.Sumw2()
      scale = kLumi*kSampleSigmas[iSample]/hist.GetEntries()
      hist.Scale(scale)
    hNhitsFvtx.append(hist)

    hDG0_name = "DG0_"+kSampleNames[iSample]
    hist = ROOT.TH1F(hDG0_name,";DG0; N Muons",50,0,50)
    ttrees[iSample].Draw( "SingleMuons.DG0>>" + hDG0_name, cutValue )
    if kNormalize:
      hist.Sumw2()
      scale = kLumi*kSampleSigmas[iSample]/hist.GetEntries()
      hist.Scale(scale)
    hDG0.append(hist)

    hDDG0_name = "DDG0_"+kSampleNames[iSample]
    hist = ROOT.TH1F(hDDG0_name,";DDG0; N Muons",20,0,20)
    ttrees[iSample].Draw( "SingleMuons.DDG0>>" + hDDG0_name, cutValue )
    if kNormalize:
      hist.Sumw2()
      scale = kLumi*kSampleSigmas[iSample]/hist.GetEntries()
      hist.Scale(scale)
    hDDG0.append(hist)

    hChi2_name = "Chi2_fvtxmutr_"+kSampleNames[iSample]
    hist = ROOT.TH1F(hChi2_name,";#chi^{2}; N Tracks",500,0,250)
    ttrees[iSample].Draw( "SingleMuons.chi2_fvtxmutr >>" + hChi2_name, cutValue2 )
    if kNormalize:
      hist.Sumw2()
      scale = kLumi*kSampleSigmas[iSample]/hist.GetEntries()
      hist.Scale(scale)
    hChi2.append(hist)

    hdrFvtxSigma_name = "dr_fvtx_sigma_"+kSampleNames[iSample]
    hist = ROOT.TH1F(hdrFvtxSigma_name,";#sigma; N Muons",300,0,150)
    ttrees[iSample].Draw( "SingleMuons.dr_fvtx_sigma>>" + hdrFvtxSigma_name, cutValue )
    if kNormalize:
      hist.Sumw2()
      scale = kLumi*kSampleSigmas[iSample]/hist.GetEntries()
      hist.Scale(scale)
    hdrFvtxSigma.append(hist)



  theLegend = makeLegend(hMuonPt)


  #Draw DCA 
  print ">>>> DCA "
  hDCA = []
  hDCAPhi = []
  for iSample in range(len(kTreeFiles)):
    dName = "DCA_"+kSampleNames[iSample]
    dPhiName = "DCA_phi_"+kSampleNames[iSample]
    sdName = "sDCA_"+kSampleNames[iSample]
    sdPhiName = "sDCA_phi_"+kSampleNames[iSample]
    hDCAPtBin = []
    hDCAPtBinPhi = []
    for iPtBin in range(0,hMuonPt[iSample].GetNbinsX()):
      xLowEdge = hMuonPt[iSample].GetXaxis().GetBinLowEdge(iPtBin+1)
      xHighEdge = hMuonPt[iSample].GetXaxis().GetBinUpEdge(iPtBin+1)
      
      if iSample == 0: print ">> bin {0}: {1:.2} to {2:.2}".format(iPtBin+1,xLowEdge,xHighEdge)
      
      dcaName = dName+"_{0:.2}_{1:.2}".format(xLowEdge,xHighEdge)
      dcaPhiName = dPhiName+"_{0:.2}_{1:.2}".format(xLowEdge,xHighEdge)
      sdcaName = sdName+"_{0:.2}_{1:.2}".format(xLowEdge,xHighEdge)
      sdcaPhiName = sdPhiName+"_{0:.2}_{1:.2}".format(xLowEdge,xHighEdge)

      hist = ROOT.TH1F(dcaName,";dca_{r}; N Events",kNDCAbins,kDCAMin,kDCAMax)
      hist2 = ROOT.TH1F(dcaPhiName,";dca_{#phi}; N Events",kNDCAbins,kDCAMin,kDCAMax)

      shist = ROOT.TH1F(sdcaName,";dca_{r}; N Events",kNDCAbins,kDCAMin,kDCAMax)
      shist2 = ROOT.TH1F(sdcaPhiName,";dca_{#phi}; N Events",kNDCAbins,kDCAMin,kDCAMax)

      pTcut = kpTVarName+">="+str(xLowEdge)+"&&"+kpTVarName+"<"+str(xHighEdge)
      ttrees[iSample].Draw( drawDCAValue + ">>" + dcaName, cutValue+"&&"+pTcut )
      ttrees[iSample].Draw( drawDCAPhiValue + ">>" + dcaPhiName, cutValue+"&&"+pTcut )

      ttrees[iSample].Draw( drawsDCAValue + ">>" + sdcaName, cutValue+"&&"+pTcut )
      ttrees[iSample].Draw( drawsDCAPhiValue + ">>" + sdcaPhiName, cutValue+"&&"+pTcut )
      
      hist.Add(shist,-1)
      hist2.Add(shist2,-1)
      
      if kNormalize:
        hist.Sumw2()
        scale = kLumi*kSampleSigmas[iSample]/hist.GetEntries()
        hist.Scale(scale)
        hist2.Sumw2()
        scale = kLumi*kSampleSigmas[iSample]/hist2.GetEntries()
        hist2.Scale(scale)

      hDCAPtBin.append(hist)
      hDCAPtBinPhi.append(hist2)
    hDCA.append(hDCAPtBin)
    hDCAPhi.append(hDCAPtBinPhi)
    

  print ">>>> DCA Total"
  #Total DCA Distributions
  hDCATotal = []
  hDCAPhiTotal = []
  for iPtBin in range(0,kNpTbins):
    dcaTotName = "DCA_total_"
    dcaPhiTotName = "DCA_phi_total_"
    lowEdge = hMuonPt[0].GetXaxis().GetBinLowEdge(iPtBin+1)
    highEdge = hMuonPt[0].GetXaxis().GetBinUpEdge(iPtBin+1)
    dcaTotName += "_{0:.2}_{1:.2}".format(lowEdge,highEdge)
    dcaPhiTotName += "_{0:.2}_{1:.2}".format(lowEdge,highEdge)
    hist = ROOT.TH1F(dcaTotName,";dca_{r}; N Events",kNDCAbins,kDCAMin,kDCAMax)
    hDCATotal.append(hist)
    hist2 = ROOT.TH1F(dcaPhiTotName,";dca_{#phi}; N Events",kNDCAbins,kDCAMin,kDCAMax)
    hDCAPhiTotal.append(hist2)
    

  for iSample in range(len(kTreeFiles)):
    if kSampleTypes[iSample] == "D": continue
    for iPtBin in range(0,kNpTbins):
      if kNormalize: 
        hDCA[iSample][iPtBin].Sumw2()
        scale = kLumi*kSampleSigmas[iSample]/hDCA[iSample][iPtBin].GetEntries()
        hDCA[iSample][iPtBin].Scale(scale)
        hDCAPhi[iSample][iPtBin].Sumw2()
        scale = kLumi*kSampleSigmas[iSample]/hDCAPhi[iSample][iPtBin].GetEntries()
        hDCAPhi[iSample][iPtBin].Scale(scale)
      hDCATotal[iPtBin].Add(hDCA[iSample][iPtBin])
      hDCAPhiTotal[iPtBin].Add(hDCAPhi[iSample][iPtBin])


  print ">>>> DCA Summary"
  #Summary Hists
  hDCASummary = []
  hDCAPhiSummary = []
  for iSample in range(len(kTreeFiles)):
    dcaSummaryName = "DCA_summary_"
    dcaSummaryName += kSampleNames[iSample]
    dcaPhiSummaryName = "DCA_phi_summary_"
    dcaPhiSummaryName += kSampleNames[iSample]
    hist = ROOT.TH1F(dcaSummaryName,";muon pT (GeV); dca_{r}",kNpTbins,kpTMin,kpTMax)
    hist2 = ROOT.TH1F(dcaPhiSummaryName,";muon pT (GeV); dca_{#phi}",kNpTbins,kpTMin,kpTMax)
    for iPtBin in range(0,kNpTbins):
      hist.SetBinContent(iPtBin+1,hDCA[iSample][iPtBin].GetRMS())      
      hist.SetBinError(iPtBin+1,hDCA[iSample][iPtBin].GetRMSError())      
      hist2.SetBinContent(iPtBin+1,hDCAPhi[iSample][iPtBin].GetRMS())      
      hist2.SetBinError(iPtBin+1,hDCAPhi[iSample][iPtBin].GetRMSError())      
    hDCASummary.append(hist)
    hDCAPhiSummary.append(hist2)
    


  print ">>>> Drawing Hists"
  #Draw Hists
  fOut = ROOT.TFile(opt.OUTPUT_DIR+'/'+'AllHists.root',"RECREATE")
  printHists(hMuonPt,kDrawOpt,-999,-999,False,False,theLegend)
  printHists(hMuonPt,kDrawOpt,-999,-999,False,True,theLegend)
  writeHists(hMuonPt,fOut)

  printHists(hMuonP,kDrawOpt,-999,-999,False,False,theLegend)
  printHists(hMuonP,kDrawOpt,-999,-999,False,True,theLegend)
  writeHists(hMuonP,fOut)

  printHists(hMuonPz,kDrawOpt,-999,-999,False,False,theLegend)
  printHists(hMuonPz,kDrawOpt,-999,-999,False,True,theLegend)
  writeHists(hMuonPz,fOut)

  printHists(hDG0,kDrawOpt,-999,-999,False,False,theLegend)
  printHists(hDG0,kDrawOpt,-999,-999,False,True,theLegend)
  writeHists(hDG0,fOut)

  printHists(hDDG0,kDrawOpt,-999,-999,False,False,theLegend)
  printHists(hDDG0,kDrawOpt,-999,-999,False,True,theLegend)
  writeHists(hDDG0,fOut)

  printHists(hChi2,kDrawOpt,-999,-999,False,False,theLegend)
  printHists(hChi2,kDrawOpt,-999,-999,False,True,theLegend)
  writeHists(hChi2,fOut)

  printHists(hdrFvtxSigma,kDrawOpt,-999,-999,False,False,theLegend)
  printHists(hdrFvtxSigma,kDrawOpt,-999,-999,False,True,theLegend)
  writeHists(hdrFvtxSigma,fOut)

  printHists(hMuonEta,kDrawOpt,-999,-999,False,False,theLegend)
  writeHists(hMuonEta,fOut)

  printHists(hNhitsFvtx,kDrawOpt,-999,-999,False,False,theLegend)
  printHists(hNhitsFvtx,kDrawOpt,-999,-999,False,True,theLegend)
  writeHists(hNhitsFvtx,fOut)

  #printDCAHists(hDCAPhi,"e1p",-999,-999,False,True,theLegend)
  #writeDCAHists(hDCAPhi,fOut)

  #printDCAHists(hDCA,"e1p",-999,-999,False,True,theLegend)
  #writeDCAHists(hDCA,fOut)

  graphDCA = printGraph(hDCASummary,"ap",0,0.1,False,False,theLegend)
  fOut.cd()
  graphDCA.Write()

  graphDCAPhi = printGraph(hDCAPhiSummary,"ap",0,1,False,False,theLegend)
  fOut.cd()
  graphDCAPhi.Write()
  
  printDCAHistsTot(hDCA,0,"e1p",-999,-999,False,True,theLegend)
  printDCAHistsTot(hDCAPhi,0,"e1p",-999,-999,False,True,theLegend)
  
  fOut.Close()







def printHists(hists,drawOpt,yMin,yMax,logx,logy,legend=0):
  

  c = ROOT.TCanvas("c","c",750,750)
  c.cd()
  for i in range(len(hists)):
    drawOption = drawOpt
    if i > 0:
      drawOption = drawOpt+"SAME"
      
    hists[i].SetLineColor(kSampleColors[i])
    hists[i].SetMarkerColor(kSampleColors[i])
    hists[i].SetLineWidth(kSampleWidths[i])
    if yMin > -999:
      hists[i].SetMinimum(yMin)
    if yMax > -999:
      if hists[i].GetBinContent(hists[i].GetMaximumBin()) > yMax:
        yMax = hists[i].GetBinContent(hists[i].GetMaximumBin())*1.25
      hists[i].SetMaximum(yMax)
    hists[i].Draw(drawOption)
    name = hists[0].GetName()
    if logx: 
      c.SetLogx()
      c.Update()
      name += '_logx'
    if logy:
      c.SetLogy()
      c.Update()
      name += '_logy'

    if legend: legend.Draw()
    c.SaveAs(opt.OUTPUT_DIR+'/'+name+'.eps')
    c.SaveAs(opt.OUTPUT_DIR+'/'+name+'.png')
    c.SaveAs(opt.OUTPUT_DIR+'/'+name+'.pdf')
    c.SaveAs(opt.OUTPUT_DIR+'/'+name+'.C')



def printDCAHistsTot(hists,histsTotal,drawOpt,yMin,yMax,logx,logy,legend=0):
  
  for iPtBin in range(kNpTbins):
    c = ROOT.TCanvas("c","c",750,750)
    c.cd()
    for iSample in range(len(kTreeFiles)):
      drawOption = drawOpt
      if iSample > 0:
        drawOption = drawOpt+"SAME"
      
      hists[iSample][iPtBin].SetLineColor(kSampleColors[iSample])
      hists[iSample][iPtBin].SetMarkerColor(kSampleColors[iSample])
      hists[iSample][iPtBin].SetLineWidth(kSampleWidths[iSample])
      hists[iSample][iPtBin].SetMarkerStyle(20)
      
      if histsTotal:
        histsTotal[iPtBin].SetLineColor(1)
        histsTotal[iPtBin].SetMarkerColor(1)
        histsTotal[iPtBin].SetMarkerStyle(20)

      if yMin > -999:
        hists[iSample][iPtBin].SetMinimum(yMin)
      if yMax > -999:
        if hists[iSample][iPtBin].GetBinContent(hists[iSample][iPtBin].GetMaximumBin()) > yMax:
          yMax = hists[iSample][iPtBin].GetBinContent(hists[iSample][iPtBin].GetMaximumBin())*1.25
        hists[iSample][iPtBin].SetMaximum(yMax)
      hists[iSample][iPtBin].Draw(drawOption)
    if histsTotal: histsTotal[iPtBin].Draw(drawOpt+"SAME")
    name = hists[0][iPtBin].GetName()
    if logx: 
      c.SetLogx()
      c.Update()
      name += '_logx'
    if logy:
      c.SetLogy()
      c.Update()
      name += '_logy'
    if legend: legend.Draw()
    c.SaveAs(opt.OUTPUT_DIR+'/'+name+'.eps')
    c.SaveAs(opt.OUTPUT_DIR+'/'+name+'.png')
    c.SaveAs(opt.OUTPUT_DIR+'/'+name+'.pdf')
    c.SaveAs(opt.OUTPUT_DIR+'/'+name+'.C')



def printDCAHists(hists,drawOpt,yMin,yMax,logx,logy,legend=0):
  
  
  for iPtBin in range(kNpTbins):
    c = ROOT.TCanvas("c","c",750,750)
    c.cd()
    for iSample in range(len(kTreeFiles)):
      drawOption = drawOpt
      if iSample > 0:
        drawOption = drawOpt+"SAME"
      
      hists[iSample][iPtBin].SetLineColor(kSampleColors[iSample])
      hists[iSample][iPtBin].SetMarkerColor(kSampleColors[iSample])
      hists[iSample][iPtBin].SetLineWidth(kSampleWidths[iSample])
      hists[iSample][iPtBin].SetMarkerStyle(20)

      if yMin > -999:
        hists[iSample][iPtBin].SetMinimum(yMin)
      if yMax > -999:
        if hists[iSample][iPtBin].GetBinContent(hists[iSample][iPtBin].GetMaximumBin()) > yMax:
          yMax = hists[iSample][iPtBin].GetBinContent(hists[iSample][iPtBin].GetMaximumBin())*1.25
        hists[iSample][iPtBin].SetMaximum(yMax)
      hists[iSample][iPtBin].Draw(drawOption)
    name = hists[0][iPtBin].GetName()
    if logx: 
      c.SetLogx()
      c.Update()
      name += '_logx'
    if logy:
      c.SetLogy()
      c.Update()
      name += '_logy'

    if legend: legend.Draw()
    c.SaveAs(opt.OUTPUT_DIR+'/'+name+'.eps')
    c.SaveAs(opt.OUTPUT_DIR+'/'+name+'.png')
    c.SaveAs(opt.OUTPUT_DIR+'/'+name+'.pdf')
    c.SaveAs(opt.OUTPUT_DIR+'/'+name+'.C')



def writeHists(hists,fileOut):
  
  fileOut.cd()
  for i in range(len(hists)):
    print "Writing {0} to {1}".format(hists[i].GetName(),fileOut.GetName())
    hists[i].Write()

def writeDCAHists(hists,fileOut):
  
  fileOut.cd()
  for iSample in range(len(kTreeFiles)):
    for i in range(len(hists[iSample])):
      print "Writing {0} to {1}".format(hists[iSample][i].GetName(),fileOut.GetName())
      hists[iSample][i].Write()



def printHist(hist,drawOpt,yMin,yMax,logx,logy,legend=0):
  

  c = ROOT.TCanvas("c","c",750,750)
  c.cd()
  drawOption = drawOpt
  if yMin > -999:
    hist.SetMinimum(yMin)
  if yMax > -999:
    if hist.GetBinContent(hist.GetMaximumBin()) > yMax:
      yMax = hist.GetBinContent(hist.GetMaximumBin())*1.25
  hist.SetMaximum(yMax)
  hist.Draw(drawOption)
  name = hist.GetName()
  if logx: 
    c.SetLogx()
    c.Update()
    name += '_logx'
  if logy:
    c.SetLogy()
    c.Update()
    name += '_logy'

  if legend: legend.Draw()
  c.SaveAs(opt.OUTPUT_DIR+'/'+name+'.eps')
  c.SaveAs(opt.OUTPUT_DIR+'/'+name+'.png')
  c.SaveAs(opt.OUTPUT_DIR+'/'+name+'.pdf')
  c.SaveAs(opt.OUTPUT_DIR+'/'+name+'.C')



def printGraph(hist,drawOpt,yMin,yMax,logx,logy,legend=0):
  
  c = ROOT.TCanvas("c","c",750,750)
  c.cd()
  
  mg = ROOT.TMultiGraph()

  for i in range(len(hist)):
    graph = histo_to_tgraph(hist[i],False)
    #drawOption = drawOpt
    #if i > 0:
    #  drawOption = "p"
    graph.SetMarkerStyle(20)
    graph.SetMarkerSize(2)
    graph.SetMarkerColor(kSampleColors[i])
    mg.Add(graph)
  
    if yMin > -999:
      mg.SetMinimum(yMin)
    if yMax > -999:
      mg.SetMaximum(yMax)


  mg.Draw(drawOpt)
  mg.GetXaxis().SetTitle(hist[0].GetXaxis().GetTitle())
  mg.GetYaxis().SetTitle(hist[0].GetYaxis().GetTitle())
  c.Modified();
  name = hist[0].GetName()
  if logx: 
    c.SetLogx()
    c.Update()
    name += '_logx'
  if logy:
    c.SetLogy()
    c.Update()
    name += '_logy'

  if legend: legend.Draw()
  c.SaveAs(opt.OUTPUT_DIR+'/'+name+'.eps')
  c.SaveAs(opt.OUTPUT_DIR+'/'+name+'.png')
  c.SaveAs(opt.OUTPUT_DIR+'/'+name+'.pdf')
  c.SaveAs(opt.OUTPUT_DIR+'/'+name+'.C')

  return mg


def histo_to_tgraph( h, zero_sup ):

  if not h:
    raise RuntimeError, "histo_to_tgraph: h not found!"


  tg = ROOT.TGraphErrors()
  point = 0
  for i in range(h.GetNbinsX()):
    x = h.GetXaxis().GetBinCenter( i+1 )
    y = float( h.GetBinContent( i+1 ) )
    error = float( h.GetBinError( i+1 ) )
    if zero_sup and not y: continue
    tg.SetPoint( point, x, y )
    tg.SetPointError( point, 0, error )
    point+=1
    
  return tg


def makeLegend(hists):
  
  leg = ROOT.TLegend(kLegX1,kLegY1,kLegX2,kLegY2)
  leg.SetTextSize(0.04);
  leg.SetTextFont(42);
  leg.SetFillColor(10);
  leg.SetBorderSize(0);
  leg.SetFillStyle(0);

  for i in range(len(hists)):
    leg.AddEntry(hists[i],kSampleNames[i],"p")

  return leg


def processCmd(cmd):
  status, output = commands.getstatusoutput(cmd)
  if status !=0:
    print 'Error in processing command:\n   ['+cmd+'] \nExiting...'
    print output
    sys.exit()





if __name__ == "__main__":
  parseOptions()
  draw_dca_distrib()
