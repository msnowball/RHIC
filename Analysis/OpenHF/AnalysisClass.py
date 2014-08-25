#!/usr/bin/python
import sys, os, pwd, commands
import optparse, shlex, re
import math
from array import array
import collections
import ROOT


class AnalysisClass:

    #Global Variables
    

    #Constructor
    def __init__(self,rootOutputFile,outputFileOrDir,nEvents):
        ROOT.gROOT.ProcessLine(".L include/tdrstyle.cc")
        self.makeRootFile = rootOutputFile
        self.outFileName = outputFileOrDir
        
        if outputFileOrDir == '':
            self.outFileName = 'hists'
        self.InitHists()        

        self.histDrawOpt1D = 'HIST'
        self.totalEventCounter = 0
        self.totalEventLimit = nEvents
        
        self.histsAlreadyWritten = False


    #Declare histograms
    def InitHists(self):
        self.hists1D = {}
        self.hists2D = {}
        self.hists3D = {}
        
        self.hists1D['muonPt'] = ROOT.TH1F('muonPt',';pT (GeV); N Events / 0.5 GeV', 100, 0, 50)


    #Open file and grab tree
    #Returns True when reached number of events to process
    def ProcessFile(self,fileName,treeName):
        tfile = ROOT.TFile(fileName,"READ")
        if not tfile:
            raise RuntimeError, "{0} could not be opened!".format(fileName)
        
        tree = tfile.Get(treeName)
        if not tree:
            raise RuntimeError, "{0} could not be found in file {1}!".format(treeName,fileName)
        
        finishedProcessing = self.EventLoop(tree)
        return finishedProcessing


    #Event loop
    def EventLoop(self,tree):
        done = False

        for entry in range( tree.GetEntries() ):
            tree.GetEntry(entry)
            if self.totalEventCounter%100000 == 0: print self.totalEventCounter
            if self.totalEventCounter > self.totalEventLimit and self.totalEventLimit > 0: 
                done = True
                break
            self.totalEventCounter += 1

            #Analysis loop per event
            for i in range(tree.muons_nMuons):
                self.hists1D['muonPt'].Fill(tree.muons_pT[i])
                

        #Return True if already processed N Events
        return done


    #End Analysis
    def End(self):

        if not self.histsAlreadyWritten:
            self.WriteHists()
        print '{0} events processed'.format(self.totalEventCounter-1)


    #Write histograms
    def WriteHists(self):


        if self.makeRootFile:
            print 'Opening output file: {0}'.format(self.outFileName+'.root')
            outFile = ROOT.TFile(self.outFileName+'.root',"RECREATE")
            outFile.cd()
        
            for key in self.hists1D:
                print 'Saving Hist: {0}'.format(str(self.hists1D[key].GetName()))
                self.hists1D[key].Write()
            for key in self.hists2D:
                print 'Saving Hist: {0}'.format(str(self.hists2D[key].GetName()))
                self.hists2D[key].Write()
            for key in self.hists3D:
                print 'Saving Hist: {0}'.format(str(self.hists3D[key].GetName()))
                self.hists3D[key].Write()
                
            print 'Closing output file: {0}'.format(self.outFileName+'.root')
            outFile.Close()
            
        else:
            theDir = self.outFileName
            if not os.path.isdir(theDir):
                cmd = 'mkdir -p {0}'.format(theDir)
                self.processCmd(cmd)
            
            for key in self.hists1D:
                print 'Saving Hist: {0}'.format(str(self.hists1D[key].GetName()))
                c = ROOT.TCanvas("c","c",750,750)
                c.cd()
                self.hists1D[key].Draw(self.histDrawOpt1D)
                saveName = theDir+'/'+str(self.hists1D[key].GetName())
                c.SaveAs(saveName+'.eps')
                c.SaveAs(saveName+'.png')
                c.SaveAs(saveName+'.pdf')
                c.SaveAs(saveName+'.C')

        self.histsAlreadyWritten = True




    def processCmd(self,cmd):
        status, output = commands.getstatusoutput(cmd)
        if status !=0:
            raise RuntimeError, 'Error in processing command:\n   ['+cmd+'] \nExiting...'
            

