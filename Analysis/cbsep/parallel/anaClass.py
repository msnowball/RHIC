#!/usr/bin/python
import sys, os, pwd, commands
import optparse, shlex, re
import math, time
from array import array
import collections
import ROOT




class anaClass:

    def __init__(self,theInputFile,theTreeName,theOutputFile):
        
        ROOT.gStyle.SetOptStat(000000)
        ROOT.gStyle.SetPadTickX(1)
        ROOT.gStyle.SetPadTickY(1)
        ROOT.gStyle.SetPadGridX(1)
        ROOT.gStyle.SetPadGridY(1)

        self.hists1D = {}
        self.hists2D = {}
        
        self.eventCounter = 0

        self.hists1D["providence"] = ROOT.TH1F("providence","",10,0,10)
        
        self.inputFile = theInputFile
        self.treeName = theTreeName
        self.outputFile = theOutputFile
        
        ###########################################

        self.drawDCAPhiValue = "SingleMuons.dca_phi"
        self.drawDCAValue = "SingleMuons.dca_r"

        self.drawsDCAPhiValue = "SingleMuons.sdca_phi"
        self.drawsDCAValue = "SingleMuons.sdca_r"

        self.cutValue = "abs(Evt_bbcZ)<40&&SingleMuons.nhits_fvtx>2"
        self.cutValueDCAPhi = "&&abs(SingleMuons.dca_phi)<10"
        self.cutValueAcceptance = "&&abs(SingleMuons.rapidity)>1.2&&abs(SingleMuons.rapidity)<2.4&&sqrt(SingleMuons.px*SingleMuons.px+SingleMuons.py*SingleMuons.py)<30"
        self.cutValueQuality = "&&abs(SingleMuons.pz)>2.8&&abs(SingleMuons.pz)<=30&&SingleMuons.DDG0<10&&SingleMuons.ntrhits>=12&&((SingleMuons.rapidity>0&&SingleMuons.DG0<15)||(SingleMuons.rapidity<0&&SingleMuons.DG0<20))"
        self.cutValueChi2 = "&&chi2_fvtxmutr<3&&SingleMuons.trchi2<5"
        self.cutValueLastGap = "&&SingleMuons.lastgap==4"

        self.veryLooseCutVal = self.cutValue+self.cutValueAcceptance+self.cutValueQuality
        self.looseCutVal = self.cutValue+self.cutValueAcceptance
        self.cutValueSH = self.cutValue+self.cutValueDCAPhi+self.cutValueAcceptance+self.cutValueQuality+self.cutValueChi2+"&&SingleMuons.lastgap<4"

        self.cutValue += self.cutValueDCAPhi+self.cutValueAcceptance+self.cutValueQuality+self.cutValueChi2+self.cutValueLastGap

        #pT/DCA
        ###########################################

        self.pTVar = "sqrt(SingleMuons.px*SingleMuons.px+SingleMuons.py*SingleMuons.py)"
        self.pVar = "sqrt(SingleMuons.px*SingleMuons.px+SingleMuons.py*SingleMuons.py+SingleMuons.pz*SingleMuons.pz)"

        self.kNpTbins = 9
        self.kpTMin = 0.75
        self.kpTMax = 5.25
        self.kNpbins = 40
        self.kpMin = 0
        self.kpMax = 20
        self.kNDCAbins = 100
        self.kDCAMin = -.5
        self.kDCAMax = 0.5
        self.kNDCAResbins = 100
        self.kDCAResMin = -1.0
        self.kDCAResMax = 1.0
        
        #dNdZ
        ###########################################
        self.ZVar = 'Evt_bbcZ'
        self.dNdZ_low_pT = 1
        self.dNdZ_high_pT = 5
        self.dNdZ_pT_step = 0.5 
        self.dNdZ_pT_NStep = 0

        self.dNdZ_NBins = 12
        self.dNdZ_Zmin = -30
        self.dNdZ_Zmax = 30

        self.dNdZ_lowpTCut = []
        self.dNdZ_highpTCut = []

        self.dNdZ_LG3Cuts_N = 'SingleMuons.lastgap==3&&SingleMuons.ntrhits>=12&&SingleMuons.rapidity>0&&SingleMuons.DG0<15&&SingleMuons.trchi2<5'
        self.dNdZ_LG4Cuts_N = 'SingleMuons.lastgap==4&&SingleMuons.ntrhits>=12&&SingleMuons.rapidity>0&&SingleMuons.DG0<15&&SingleMuons.trchi2<5'

        self.dNdZ_LG3Cuts_S = 'SingleMuons.lastgap==3&&SingleMuons.ntrhits>=12&&SingleMuons.rapidity<0&&SingleMuons.DG0<20&&SingleMuons.trchi2<5'
        self.dNdZ_LG4Cuts_S = 'SingleMuons.lastgap==4&&SingleMuons.ntrhits>=12&&SingleMuons.rapidity<0&&SingleMuons.DG0<20&&SingleMuons.trchi2<5'

        i=self.dNdZ_low_pT
        while i <= self.dNdZ_high_pT:
            i+=self.dNdZ_pT_step
            self.dNdZ_pT_NStep += 1
            self.dNdZ_lowpTCut.append(i)
            self.dNdZ_highpTCut.append(i+self.dNdZ_pT_step)
        




    def getTree(self):

        print ">>>> Reading files"
        self.chain = ROOT.TChain(self.treeName)
        if self.inputFile.lower().endswith(".txt"):
            for rootFile in open(self.inputFile):
                line = rootFile.split()
                if line[0].startswith("#"): continue
                if len(line[0]) < 1: continue
                theFile = os.path.basename(line[0])
                print ">> Adding {0} to chain".format(theFile)
                self.chain.Add(theFile)
        else:
            self.chain.Add(self.inputFile)




    def doAnalysis(self):

        self.getTree()

        #Providence info tracking
        #########################################################
        self.hists1D["providence"].SetBinContent(1,self.chain.GetEntriesFast())

        self.addHist1D("mu_pT",";pT (GeV); N Muons",self.pTVar,self.kNpTbins,self.kpTMin,self.kpTMax)
        self.addHist1D("mu_p",";P (GeV); N Muons",self.pVar,self.kNpbins,self.kpMin,self.kpMax)
        self.addHist1D("mu_pz",";pZ (GeV); N Muons","SingleMuons.pz",self.kNpbins*2,-self.kpMax,self.kpMax)
        self.addHist1D("mu_eta",";eta; N Muons","SingleMuons.rapidity",50,-2.5,2.5)
        self.addHist1D("nhits_fvtx",";N Hits FVTX; N Events","SingleMuons.nhits_fvtx",20,0,20)
        self.addHist1D("DG0",";DG0; N Muons","SingleMuons.DG0",50,0,50)
        self.addHist1D("DDG0",";DDG0; N Muons","SingleMuons.DDG0",20,0,20)
        self.addHist1D("Chi2_fvtxmutr",";#chi^{2}; N Tracks","SingleMuons.chi2_fvtxmutr",1000,0,500)
        self.addHist1D("dr_fvtx_sigma",";#sigma; N Muons","SingleMuons.dr_fvtx_sigma",300,0,150)
        self.addHist1D("mu_pT_nHitsgte10",";pT (GeV); N Muons",self.pTVar,30,0,15,self.looseCutVal+"&&SingleMuons.ntrhits>=10")
        self.addHist1D("mu_pT_nHitsgte12",";pT (GeV); N Muons",self.pTVar,30,0,15,self.looseCutVal+"&&SingleMuons.ntrhits>=12")
        self.addHist1D("mu_pT_nHitsgte14",";pT (GeV); N Muons",self.pTVar,30,0,15,self.looseCutVal+"&&SingleMuons.ntrhits>=14")

        self.addHist2D("chi2_vs_LG",";Last Gap; #chi^{2}","SingleMuons.lastgap",4,0,4,"SingleMuons.chi2_fvtxmutr",50,0,25,self.veryLooseCutVal)

        #########################################################
        dName = "DCA"
        dPhiName = "DCA_phi"
        sdName = "sDCA"
        sdPhiName = "sDCA_phi"
        for iPtBin in range(0,self.hists1D["mu_pT"].GetNbinsX()):
            xLowEdge = self.hists1D["mu_pT"].GetXaxis().GetBinLowEdge(iPtBin+1)
            xHighEdge = self.hists1D["mu_pT"].GetXaxis().GetBinUpEdge(iPtBin+1)
            print ">> bin {0}: {1:.2} to {2:.2}".format(iPtBin+1,xLowEdge,xHighEdge)
      
            dcaName = dName+"_{0:.2}_{1:.2}".format(xLowEdge,xHighEdge)
            dcaPhiName = dPhiName+"_{0:.2}_{1:.2}".format(xLowEdge,xHighEdge)
            sdcaName = sdName+"_{0:.2}_{1:.2}".format(xLowEdge,xHighEdge)
            sdcaPhiName = sdPhiName+"_{0:.2}_{1:.2}".format(xLowEdge,xHighEdge)
            
            hist = ROOT.TH1F(dcaName,";dca_{r}; N Muons",self.kNDCAbins,self.kDCAMin,self.kDCAMax)
            hist2 = ROOT.TH1F(dcaPhiName,";dca_{#phi}; N Muons",self.kNDCAbins,self.kDCAMin,self.kDCAMax)
            
            shist = ROOT.TH1F(sdcaName,";dca_{r}; N Muons",self.kNDCAbins,self.kDCAMin,self.kDCAMax)
            shist2 = ROOT.TH1F(sdcaPhiName,";dca_{#phi}; N Muons",self.kNDCAbins,self.kDCAMin,self.kDCAMax)
            
            pTcut = self.pTVar+">="+str(xLowEdge)+"&&"+self.pTVar+"<"+str(xHighEdge)
            self.chain.Draw( self.drawDCAValue + ">>" + dcaName, self.cutValue+"&&"+pTcut )
            self.chain.Draw( self.drawDCAPhiValue + ">>" + dcaPhiName, self.cutValue+"&&"+pTcut )
            
            self.chain.Draw( self.drawsDCAValue + ">>" + sdcaName, self.cutValue+"&&"+pTcut )
            self.chain.Draw( self.drawsDCAPhiValue + ">>" + sdcaPhiName, self.cutValue+"&&"+pTcut )
      
            hist.Add(shist,-1)
            hist2.Add(shist2,-1)
      
            self.hists1D[dcaName] = hist
            self.hists1D[dcaPhiName] = hist2



        #Stopped Hadrons
        ###########################################################################
        self.addHist1D("SH_mu_pT",";pT (GeV); N Muons",self.pTVar,self.kNpTbins,self.kpTMin,self.kpTMax,self.cutValueSH)
        self.addHist1D("SH_mu_p",";P (GeV); N Muons",self.pVar,self.kNpbins,self.kpMin,self.kpMax,self.cutValueSH)
        self.addHist1D("SH_mu_pz",";pZ (GeV); N Muons","SingleMuons.pz",self.kNpbins*2,-self.kpMax,self.kpMax,self.cutValueSH)
        self.addHist1D("SH_mu_eta",";eta; N Muons","SingleMuons.rapidity",50,-2.5,2.5,self.cutValueSH)
        self.addHist1D("SH_nhits_fvtx",";N Hits FVTX; N Events","SingleMuons.nhits_fvtx",20,0,20,self.cutValueSH)
        self.addHist1D("SH_DG0",";DG0; N Muons","SingleMuons.DG0",50,0,50,self.cutValueSH)
        self.addHist1D("SH_DDG0",";DDG0; N Muons","SingleMuons.DDG0",20,0,20,self.cutValueSH)
        self.addHist1D("SH_Chi2_fvtxmutr",";#chi^{2}; N Tracks","SingleMuons.chi2_fvtxmutr",1000,0,500,self.cutValueSH)
        self.addHist1D("SH_dr_fvtx_sigma",";#sigma; N Muons","SingleMuons.dr_fvtx_sigma",300,0,150,self.cutValueSH)


        dName = "SH_DCA"
        dPhiName = "SH_DCA_phi"
        sdName = "SH_sDCA"
        sdPhiName = "SH_sDCA_phi"
        for iPtBin in range(0,self.hists1D["mu_pT"].GetNbinsX()):
            xLowEdge = self.hists1D["mu_pT"].GetXaxis().GetBinLowEdge(iPtBin+1)
            xHighEdge = self.hists1D["mu_pT"].GetXaxis().GetBinUpEdge(iPtBin+1)
            print ">> bin {0}: {1:.2} to {2:.2}".format(iPtBin+1,xLowEdge,xHighEdge)
      
            dcaName = dName+"_{0:.2}_{1:.2}".format(xLowEdge,xHighEdge)
            dcaPhiName = dPhiName+"_{0:.2}_{1:.2}".format(xLowEdge,xHighEdge)
            sdcaName = sdName+"_{0:.2}_{1:.2}".format(xLowEdge,xHighEdge)
            sdcaPhiName = sdPhiName+"_{0:.2}_{1:.2}".format(xLowEdge,xHighEdge)
            
            hist = ROOT.TH1F(dcaName,";dca_{r}; N Muons",self.kNDCAbins,self.kDCAMin,self.kDCAMax)
            hist2 = ROOT.TH1F(dcaPhiName,";dca_{#phi}; N Muons",self.kNDCAbins,self.kDCAMin,self.kDCAMax)
            
            shist = ROOT.TH1F(sdcaName,";dca_{r}; N Muons",self.kNDCAbins,self.kDCAMin,self.kDCAMax)
            shist2 = ROOT.TH1F(sdcaPhiName,";dca_{#phi}; N Muons",self.kNDCAbins,self.kDCAMin,self.kDCAMax)
            
            pTcut = self.pTVar+">="+str(xLowEdge)+"&&"+self.pTVar+"<"+str(xHighEdge)
            self.chain.Draw( self.drawDCAValue + ">>" + dcaName, self.cutValueSH+"&&"+pTcut )
            self.chain.Draw( self.drawDCAPhiValue + ">>" + dcaPhiName, self.cutValueSH+"&&"+pTcut )
            
            self.chain.Draw( self.drawsDCAValue + ">>" + sdcaName, self.cutValue+"&&"+pTcut )
            self.chain.Draw( self.drawsDCAPhiValue + ">>" + sdcaPhiName, self.cutValueSH+"&&"+pTcut )
      
            hist.Add(shist,-1)
            hist2.Add(shist2,-1)
      
            self.hists1D[dcaName] = hist
            self.hists1D[dcaPhiName] = hist2



        #dN/dZ
        ###############################################################################
        for i in range(self.dNdZ_pT_NStep):
            theName="dNdZ_pT_{0:.2}_{1:.2}_LG3_N".format(self.dNdZ_lowpTCut[i],self.dNdZ_highpTCut[i])
            self.addHist1D(theName,";Z (cm); dN/dZ",self.ZVar,self.dNdZ_NBins,self.dNdZ_Zmin,self.dNdZ_Zmax,self.dNdZ_LG3Cuts_N)
            
            theName="dNdZ_pT_{0:.2}_{1:.2}_LG4_N".format(self.dNdZ_lowpTCut[i],self.dNdZ_highpTCut[i])
            self.addHist1D(theName,";Z (cm); dN/dZ",self.ZVar,self.dNdZ_NBins,self.dNdZ_Zmin,self.dNdZ_Zmax,self.dNdZ_LG4Cuts_N)

            theName="dNdZ_pT_{0:.2}_{1:.2}_LG3_S".format(self.dNdZ_lowpTCut[i],self.dNdZ_highpTCut[i])
            self.addHist1D(theName,";Z (cm); dN/dZ",self.ZVar,self.dNdZ_NBins,self.dNdZ_Zmin,self.dNdZ_Zmax,self.dNdZ_LG3Cuts_S)
            
            theName="dNdZ_pT_{0:.2}_{1:.2}_LG4_S".format(self.dNdZ_lowpTCut[i],self.dNdZ_highpTCut[i])
            self.addHist1D(theName,";Z (cm); dN/dZ",self.ZVar,self.dNdZ_NBins,self.dNdZ_Zmin,self.dNdZ_Zmax,self.dNdZ_LG4Cuts_S)
        




        #Write Hists 
        ###############################################################################
        fOut = ROOT.TFile(self.outputFile,"RECREATE")
        self.writeHists(self.hists1D,fOut)
        fOut.Close()



    def addHist1D(self, histName, histTitles, varName, histNbins, histBinMin, histBinMax, theCutValue=None):

        if theCutValue == None:
            theCutValue = self.cutValue
        hist = ROOT.TH1F(histName,histTitles,histNbins,histBinMin,histBinMax)
        self.chain.Draw( varName + ">>" + histName, theCutValue )
        self.hists1D[histName] = hist


    def addHist2D(self, histName, histTitles, varNameX, histNbinsX, histBinMinX, histBinMaxX, varNameY, histNbinsY, histBinMinY, histBinMaxY, theCutValue=None):

        if theCutValue == None:
            theCutValue = self.cutValue
        hist = ROOT.TH2F(histName,histTitles,histNbinsX,histBinMinX,histBinMaxX,histNbinsY,histBinMinY,histBinMaxY)
        self.chain.Draw( varNameX+":"+varNameY + ">>" + histName, theCutValue )
        self.hists2D[histName] = hist

        
    def writeHists(self,hists,fileOut):
            
        fileOut.cd()
        for key in hists:
            print "Writing {0} to {1}".format(hists[key].GetName(),fileOut.GetName())
            hists[key].Write()


