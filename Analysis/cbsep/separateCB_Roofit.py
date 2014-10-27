import sys, os, pwd, commands
import optparse, shlex, re
import math, time
from array import array
import collections
import ROOT


def parseOptions():

  usage = ('usage: %prog [options] \n'
           + '%prog -h for help')
  parser = optparse.OptionParser(usage)

  parser.add_option('-b', action='store_true', dest='noX', default=True ,help='no X11 windows')

  # store options and arguments as global variables
  global opt, args
  (opt, args) = parser.parse_args()



k_n_smooth = 1;
k_pt_min = 0.75;
k_pt_max = 5.25;
k_n_pptbin = 40;
k_ppt_min = 0.0;
k_ppt_max = 20.0;

k_flavor_name = ["D", "B", "Rl", "p"]

k_nbin_max = 500
k_bin_start = 1
k_bin_end = 500
k_bin_count_min = 0

k_xmin = [-0.5, -0.50, -0.30, -0.30, -0.20, -0.10, -0.08, -0.05, -0.03]
k_xmax = [ 0.3,  0.30,  0.20,  0.20,  0.15,  0.08,  0.05,  0.03,  0.01]
k_n_ptbin = len(k_xmin);
k_n_flavor = len(k_flavor_name);

nbin_used = 0


def loadLibs():

    ROOT.gSystem.Load("libRoofit")



def cbsep_roofit():

    f_in = ROOT.TFile("../bin500_10pb/draw_dca_distrib_train.root")
    f_in2 = ROOT.TFile("../bin500_10pb/draw_dca_distrib_3.root")
    f_ppT = ROOT.TFile("../bin500_10pb/draw_dca_distrib_train.root")

    h_p = [[]]
    h_p2 = [[]]
    h_p_p = [[]]
    
    fit_ratio = [[]] 
    fit_error = [[]] 

    for i_ptbin in range(k_n_ptbin):
        for i_flavor in range(k_n_flavor):

            h_name = ROOT.TString("h_")
            h_name += k_flavor_name[i_flavor]
            h_name += "_pt_"
            h_name += int((i_ptbin+2)/2)
            if i_ptbin%2 == 1: h_name += ".5"
            
            h_p2[i_ptbin][i_flavor] = (f_in2.Get("dca")).GetPad(i_ptbin+1).FindObject(h_name)
            h_p2_name = h_p2[i_ptbin][i_flavor].GetName()
            h_p2_name += "_2"
            #h_p2[i_ptbin][i_flavor].SetName( h_p2_name )
            #cout << h_p2_name << endl

            h_p[i_ptbin][i_flavor] = (f_in.Get("dca")).GetPad(i_ptbin+1).FindObject(h_name)
            if not h_p[i_ptbin][i_flavor]:
                
                h_name = ROOT.TString("h_")
                h_name += k_flavor_name[i_flavor]
                h_name += "_pt_"
                h_name += i_ptbin
                h_p[i_ptbin][i_flavor] = (f_in.Get("dca")).GetPad(i_ptbin+1).FindObject(h_name)
                if not h_p[i_ptbin][i_flavor]: 
                    h_name.ReplaceAll("p_", "Bg_")
                    h_p[i_ptbin][i_flavor] = (f_in.Get("dca")).GetPad(i_ptbin+1).FindObject(h_name)
                    

            print h_p[i_ptbin][i_flavor].GetName()
            print h_p[i_ptbin][i_flavor].GetBinContent(k_nbin_max/2)+" error "+h_p[i_ptbin][i_flavor].GetBinError(k_nbin_max/2)
            print h_p2[i_ptbin][i_flavor].GetName()
            print h_p2[i_ptbin][i_flavor].GetBinContent(k_nbin_max/2)+"  error "+h_p2[i_ptbin][i_flavor].GetBinError(k_nbin_max/2)
            print "integral ", h_p2[i_ptbin][i_flavor].Integral()


            h_p_name = ROOT.TString("h_")
            h_p_name += k_flavor_name[i_flavor]
            h_p_name += "_p_pt_"
            h_p_name += int((i_ptbin+2)/2)
            if i_ptbin%2 == 1: h_p_name += ".5"
            if i_flavor == 0 or i_flavor == 1: h_p_p[i_ptbin][i_flavor] = (f_ppT.Get("ppT")).GetPad(i_ptbin+1).FindObject(h_p_name)
            else:
                h_p_p[i_ptbin][i_flavor] = ROOT.TH1F(h_p_name, h_p_name, k_n_pptbin, k_ppt_min, k_ppt_max)
                h_p_p[i_ptbin][i_flavor].SetLineColor(5)
      
            print h_p_p[i_ptbin][i_flavor].GetName(), h_p_p[i_ptbin][i_flavor].Integral()


  h_c_percent_true = ROOT.TH1F("h_c_percent_true", "h_c_percent_true", k_n_ptbin, k_pt_min, k_pt_max)
  h_b_percent_true = ROOT.TH1F("h_b_percent_true", "h_b_percent_true", k_n_ptbin, k_pt_min, k_pt_max)
  h_m_percent_true = ROOT.TH1F("h_m_percent_true", "h_m_percent_true", k_n_ptbin, k_pt_min, k_pt_max)

  h_c_spectrum_true = ROOT.TH1F("h_c_spectrum_true", "h_c_spectrum_true", k_n_ptbin, k_pt_min, k_pt_max)
  h_b_spectrum_true = ROOT.TH1F("h_b_spectrum_true", "h_b_spectrum_true", k_n_ptbin, k_pt_min, k_pt_max)
  h_m_spectrum_true = ROOT.TH1F("h_m_spectrum_true", "h_m_spectrum_true", k_n_ptbin, k_pt_min, k_pt_max)

  for i_ptbin in range(k_n_ptbin):
      
      n_c_tot = h_p[i_ptbin][0].Integral()
      n_b_tot = h_p[i_ptbin][1].Integral()
      n_rl_tot = h_p[i_ptbin][2].Integral()
      n_mb_tot = h_p[i_ptbin][3].Integral()
      print n_c_tot, n_b_tot, n_rl_tot, n_mb_tot
      
      n_c_tot2 = h_p2[i_ptbin][0].Integral()
      n_b_tot2 = h_p2[i_ptbin][1].Integral()
      n_rl_tot2 = h_p2[i_ptbin][2].Integral()
      n_mb_tot2 = h_p2[i_ptbin][3].Integral()
      print n_c_tot2, n_b_tot2, n_rl_tot2, n_mb_tot2

      for j in range(4):
          h_p[i_ptbin][j].Smooth(k_n_smooth)
          h_p2[i_ptbin][j].Smooth(k_n_smooth)
          

      if False:
          h_p[i_ptbin][0].Scale(1.0/n_c_tot)
          h_p[i_ptbin][1].Scale(1.0/n_b_tot)
          h_p[i_ptbin][2].Scale(1.0/n_rl_tot)
          h_p[i_ptbin][3].Scale(1.0/n_mb_tot)
          
          h_p2[i_ptbin][0].Scale(1.0/n_c_tot2)
          h_p2[i_ptbin][1].Scale(1.0/n_b_tot2)
          h_p2[i_ptbin][2].Scale(1.0/n_rl_tot2)
          h_p2[i_ptbin][3].Scale(1.0/n_mb_tot2)
    

      if n_c_tot2+n_b_tot2+n_mb_tot2 != 0:
          h_c_percent_true.SetBinContent(i_ptbin+1, double(n_c_tot2)/(n_c_tot2+n_b_tot2+n_mb_tot2))
          h_b_percent_true.SetBinContent(i_ptbin+1, double(n_b_tot2)/(n_c_tot2+n_b_tot2+n_mb_tot2))
          h_m_percent_true.SetBinContent(i_ptbin+1, double(n_mb_tot2)/(n_c_tot2+n_b_tot2+n_mb_tot2))
      else:
          h_c_percent_true.SetBinContent(i_ptbin+1, 0)
          h_b_percent_true.SetBinContent(i_ptbin+1, 0)
          h_m_percent_true.SetBinContent(i_ptbin+1, 0)
    
      h_c_spectrum_true.SetBinContent(i_ptbin+1, n_c_tot2)
      h_b_spectrum_true.SetBinContent(i_ptbin+1, n_b_tot2)
      h_m_spectrum_true.SetBinContent(i_ptbin+1, n_mb_tot2)

      xmin = -0.0
      xmax = 0.0

      bin_start = k_bin_start
      bin_end = k_bin_end
      for _bin in range(k_bin_start,k_bin_end+1):
          c_tmp = h_p[i_ptbin][0].GetBinContent(i_bin)
          b_tmp = h_p[i_ptbin][1].GetBinContent(i_bin)
          m_tmp = h_p[i_ptbin][3].GetBinContent(i_bin)
          rl_tmp = h_p2[i_ptbin][2].GetBinContent(i_bin)

          if( c_tmp >= k_bin_count_min and b_tmp >= k_bin_count_min and m_tmp >= k_bin_count_min and bin_start == k_bin_start ): bin_start = i_bin 
          if( !(c_tmp >= k_bin_count_min and b_tmp >= k_bin_count_min and m_tmp >= k_bin_count_min) and i_bin > (k_bin_end-k_bin_start)/2 and bin_end == k_bin_end ): bin_end = i_bin
    
      xmin = -0.5+1.0/k_nbin_max*(bin_start-k_bin_start)
      xmax = -0.5+1.0/k_nbin_max*(bin_end-k_bin_start)

      xmin = k_xmin[i_ptbin]
      xmax = k_xmax[i_ptbin]
      print "xmin "+xmin+"   xmax "+xmax

      ratio_signal = ROOT.RooFit.RooRealVar("ratio_signal", "ratio_signal", 0.5, 0.0, 1.0)
      ratio_charm = ROOT.RooFit.RooRealVar("ratio_charm", "ratio_charm", 0.5, 0.0, 1.0)

      x = ROOT.RooFit.RooRealVar("x", "x", xmin, xmax)
      hist_c = ROOT.RooFit.RooDataHist("hist_c", "hist_c", x, h_p[i_ptbin][0])
      hist_b = ROOT.RooFit.RooDataHist("hist_b", "hist_b", x, h_p[i_ptbin][1])
      hist_m = ROOT.RooFit.RooDataHist("hist_m", "hist_m", x, h_p[i_ptbin][3])
      hist_rl = ROOT.RooFit.RooDataHist("hist_rl", "hist_rl", x, h_p2[i_ptbin][2])

      pdf_c = ROOT.RooFit.RooHistPdf("pdf_c", "pdf_c", x, hist_c, 0)
      pdf_b = ROOT.RooFit.RooHistPdf("pdf_b", "pdf_b", x, hist_b, 0)
      pdf_m = ROOT.RooFit.RooHistPdf("pdf_m", "pdf_m", x, hist_m, 0)

      pdf_signal = ROOT.RooFit.RooAddPdf("pdf_signal", "charm+beauty", RooArgList(pdf_c, pdf_b), ratio_charm)
      pdf_all = ROOT.RooFit.RooAddPdf("pdf_all", "charm+beauty+background", RooArgList(pdf_signal, pdf_m), ratio_signal)

      pdf_all.fitTo( hist_rl )

      #ratio_signal.Print()
      #ratio_charm.Print()

      fit_ratio_signal = ratio_signal.getVal()
      fit_error_signal = ratio_signal.getError()
      fit_ratio_charm  = ratio_charm.getVal()
      fit_error_charm  = ratio_charm.getError()
      
      fit_ratio[i_ptbin][0] = fit_ratio_signal * fit_ratio_charm
      fit_ratio[i_ptbin][1] = fit_ratio_signal * (1.0-fit_ratio_charm)
      fit_ratio[i_ptbin][2]  = 1.0-fit_ratio_signal    

      fit_error[i_ptbin][0] = fit_ratio[i_ptbin][0] * sqrt( (fit_error_signal/fit_ratio_signal)**2 + (fit_error_charm/fit_ratio_charm)**2 )
      fit_error[i_ptbin][1] = fit_ratio[i_ptbin][1] * sqrt( (fit_error_signal/fit_ratio_signal)**2 + (fit_error_charm/(1-fit_ratio_charm))**2 )
      fit_error[i_ptbin][2] = fit_error_signal 

      print "pT "+i_ptbin
      print "ratio_D   = "+fit_ratio[i_ptbin][0]+" +- "+fit_error[i_ptbin][0]+" true "+h_c_percent_true.GetBinContent(i_ptbin+1)
      print "ratio_B   = "+fit_ratio[i_ptbin][1]+" +- "+fit_error[i_ptbin][1]+" true "+h_b_percent_true.GetBinContent(i_ptbin+1)
      print "ratio_Mb  = "+fit_ratio[i_ptbin][2]+" +- "+fit_error[i_ptbin][2]+" true "+h_m_percent_true.GetBinContent(i_ptbin+1)


  c_c_percent = ROOT.TCanvas( "c_c_percent", "c_c_percent", 1200, 900)
  h_c_percent = ROOT.TH1F("h_c_percent", "h_c_percent", k_n_ptbin, k_pt_min, k_pt_max)
  h_b_percent = ROOT.TH1F("h_b_percent", "h_b_percent", k_n_ptbin, k_pt_min, k_pt_max)
  h_m_percent = ROOT.TH1F("h_m_percent", "h_m_percent", k_n_ptbin, k_pt_min, k_pt_max)

  h_c_spectrum = ROOT.TH1F("h_c_spectrum", "h_c_spectrum", k_n_ptbin, k_pt_min, k_pt_max)
  h_b_spectrum = ROOT.TH1F("h_b_spectrum", "h_b_spectrum", k_n_ptbin, k_pt_min, k_pt_max)
  h_m_spectrum = ROOT.TH1F("h_m_spectrum", "h_m_spectrum", k_n_ptbin, k_pt_min, k_pt_max)

  for i_ptbin in range(k_n_ptbin):
      #if (i_ptbin == 0) h_c_percent.SetBinContent(1, 0.9853)
      #else if (i_ptbin == 1) h_c_percent.SetBinContent(2, 0.9675)
      #else 
      h_c_percent.SetBinContent(i_ptbin+1, fit_ratio[i_ptbin][0])
      h_b_percent.SetBinContent(i_ptbin+1, fit_ratio[i_ptbin][1])
      h_m_percent.SetBinContent(i_ptbin+1, fit_ratio[i_ptbin][2])
      
      h_c_percent.SetBinError(i_ptbin+1, fit_error[i_ptbin][0])
      h_b_percent.SetBinError(i_ptbin+1, fit_error[i_ptbin][1])
      h_m_percent.SetBinError(i_ptbin+1, fit_error[i_ptbin][2])
      
      h_c_spectrum.SetBinContent(i_ptbin+1, fit_ratio[i_ptbin][0] * h_p2[i_ptbin][2].Integral() )
      h_b_spectrum.SetBinContent(i_ptbin+1, fit_ratio[i_ptbin][1] * h_p2[i_ptbin][2].Integral() )
      h_m_spectrum.SetBinContent(i_ptbin+1, fit_ratio[i_ptbin][2] * h_p2[i_ptbin][2].Integral() )
      
      h_c_spectrum.SetBinError(i_ptbin+1, fit_error[i_ptbin][0] * h_p2[i_ptbin][2].Integral() )
      h_b_spectrum.SetBinError(i_ptbin+1, fit_error[i_ptbin][1] * h_p2[i_ptbin][2].Integral() )
      h_m_spectrum.SetBinError(i_ptbin+1, fit_error[i_ptbin][2] * h_p2[i_ptbin][2].Integral() )



  tg_c_percent = histo_to_tgraph( h_c_percent, false )
  tg_b_percent = histo_to_tgraph( h_b_percent, false )
  tg_m_percent = histo_to_tgraph( h_m_percent, false )

  tg_c_percent.SetTitle("Extracted fraction #mu from D / B / Bkgnd")
  tg_c_percent.GetXaxis().SetTitle("#mu p_{T} (GeV/c)")
  tg_c_percent.GetYaxis().SetTitle("N_{i} / N_{total}")

  tg_c_percent.SetLineColor(4)
  tg_c_percent.SetLineWidth(3)
  tg_c_percent.SetMarkerStyle(20)
  tg_c_percent.SetMarkerSize(2)
  tg_c_percent.SetMarkerColor(4)
  tg_c_percent.SetMinimum(-0.05)
  tg_c_percent.SetMaximum(1.0)
  tg_c_percent.Draw("AP")
  tg_b_percent.SetLineColor(2)
  tg_b_percent.SetLineWidth(3)
  tg_b_percent.SetMarkerStyle(21)
  tg_b_percent.SetMarkerSize(2)
  tg_b_percent.SetMarkerColor(2)
  tg_b_percent.Draw("Psame")
  tg_m_percent.SetLineColor(8)
  tg_m_percent.SetLineWidth(3)
  tg_m_percent.SetMarkerStyle(22)
  tg_m_percent.SetMarkerSize(2)
  tg_m_percent.SetMarkerColor(8)
  tg_m_percent.Draw("Psame")

  h_c_percent_true.SetLineColor(4)
  h_b_percent_true.SetLineColor(2)
  h_m_percent_true.SetLineColor(8)
  h_c_percent_true.SetLineStyle(2)
  h_b_percent_true.SetLineStyle(2)
  h_m_percent_true.SetLineStyle(2)

  h_c_percent_true.Draw("plsame")
  h_b_percent_true.Draw("plsame")
  h_m_percent_true.Draw("plsame")

  ll = []
  for l in range(4):
      ll[l] = ROOT.TLatex()
  lable_x = 2
  #ll[0].SetTextSize(0.045)
  #ll[0].DrawLatex(lable_x, 1e5,"Merged events")
  ll[1].SetTextColor(2)
  ll[1].DrawLatex(lable_x, 0.9, "N_{B} / N_{total}")
  ll[2].SetTextColor(4)
  ll[2].DrawLatex(lable_x, 0.8, "N_{D} / N_{total}")
  ll[3].SetTextColor(8)
  ll[3].DrawLatex(lable_x, 0.7, "N_{BG} / N_{total}")
   
  
  f_out = ROOT.TFile("cbsep.root", "RECREATE")
  #c_cbsep.Write()
  h_c_percent.Write()
  h_b_percent.Write()
  h_m_percent.Write()
  c_c_percent.Write()
  
  h_c_percent_true.Write()
  h_b_percent_true.Write()
  h_m_percent_true.Write()

  h_c_spectrum.Write()
  h_b_spectrum.Write()
  h_m_spectrum.Write()

  h_c_spectrum_true.Write()
  h_b_spectrum_true.Write()
  h_m_spectrum_true.Write()

  c_c_percent.SaveAs("cbsep.gif")
  
  f_cut_ratio = ROOT.TFile("../make_pt_spectrum/make_pt_spectrum.root")
  h_reco_vs_gen_ratio_c = f_cut_ratio.Get("cut_ratio_charm")
  h_reco_vs_gen_ratio_b = f_cut_ratio.Get("cut_ratio_beauty")
  
  c_bratio = ROOT.TCanvas( "c_bratio", "c_bratio", 800, 600)
  h_bratio = ROOT.TH1F( "h_bratio", "h_bratio", k_n_ptbin, k_pt_min, k_pt_max)
  for i_ptbin in range(k_n_ptbin):
  
      mu_before_reco_c = h_c_percent.GetBinContent(i_ptbin+1) / h_reco_vs_gen_ratio_c.GetBinContent(i_ptbin+1)
      mu_before_reco_b = h_b_percent.GetBinContent(i_ptbin+1) / h_reco_vs_gen_ratio_b.GetBinContent(i_ptbin+1)
      
      print "pT ",i_ptbin
      print "mu_after_reco_c "+h_c_percent.GetBinContent(i_ptbin+1)+" mu_after_reco_b "+h_b_percent.GetBinContent(i_ptbin+1)
      print "mu_reco_ratio_c "+h_reco_vs_gen_ratio_c.GetBinContent(i_ptbin+1)+" mu_reco_ratio_b "+h_reco_vs_gen_ratio_b.GetBinContent(i_ptbin+1)
      print "mu_before_reco_c "+mu_before_reco_c+" mu_before_reco_b "+mu_before_reco_b
      bratio_content = mu_before_reco_b / (mu_before_reco_b+mu_before_reco_c)
      h_bratio.SetBinContent(i_ptbin+1, bratio_content )
      #print "bratio = ",bratio_content
      
      bratio_after_reco = h_b_percent.GetBinContent(i_ptbin+1) / (h_c_percent.GetBinContent(i_ptbin+1)+h_b_percent.GetBinContent(i_ptbin+1))
      bratio_after_reco_error = sqrt( (h_c_percent.GetBinError(i_ptbin+1)/h_c_percent.GetBinContent(i_ptbin+1))**2 + (h_b_percent.GetBinError(i_ptbin+1)/h_b_percent.GetBinContent(i_ptbin+1))**2 ) * h_bratio.GetBinContent(i_ptbin+1)
      ratio_reco_vs_gen = h_reco_vs_gen_ratio_b.GetBinContent(i_ptbin+1) / (h_reco_vs_gen_ratio_c.GetBinContent(i_ptbin+1)+h_reco_vs_gen_ratio_b.GetBinContent(i_ptbin+1))
      ratio_reco_vs_gen_error = ratio_reco_vs_gen * sqrt( (h_reco_vs_gen_ratio_c.GetBinError(i_ptbin+1)/h_reco_vs_gen_ratio_c.GetBinContent(i_ptbin+1))**2 + (h_reco_vs_gen_ratio_b.GetBinError(i_ptbin+1)/h_reco_vs_gen_ratio_b.GetBinContent(i_ptbin+1))**2 )
      bratio_error = bratio_content * sqrt( (bratio_after_reco_error/bratio_after_reco)**2 + (ratio_reco_vs_gen_error/ratio_reco_vs_gen)**2 )
      h_bratio.SetBinError(i_ptbin+1, bratio_error)
      

  f_pt_spectrum = ROOT.TFile("../make_pt_spectrum/make_pt_spectrum.root")
  h_pT_true_bratio = ROOT.TH1F( "h_pT_true_bratio", "h_pT_true_bratio", k_n_ptbin, k_pt_min, k_pt_max)
  h_pT_true_c = f_pt_spectrum.Get("pythia_charm_muon_pt")
  h_pT_true_b = f_pt_spectrum.Get("pythia_beauty_muon_pt")
  *h_pT_true_bratio = *h_pT_true_b / (*h_pT_true_c + *h_pT_true_b)
  print "b ratio "
  for i_ptbin in range(k_n_ptbin):
      print "pT ", 0.5*i_ptbin+1, "\t", h_bratio.GetBinContent(i_ptbin+1)," +- ",h_bratio.GetBinError(i_ptbin+1)," true ",h_pT_true_bratio.GetBinContent(i_ptbin+1) 
  

  h_pT_true_bratio.SetLineColor(4)
  h_pT_true_bratio.SetLineWidth(2)
  
  h_bratio.SetMinimum(0.0)
  h_bratio.SetMaximum(1.0)
  h_bratio.SetLineColor(6)
  h_bratio.GetXaxis().SetTitle("#mu p_{T} (GeV/c)")
  h_bratio.GetYaxis().SetTitle("b ratio")
  h_bratio.Draw("e")
  h_pT_true_bratio.Draw("plsame")
  c_bratio.SaveAs("c_mu_pT_bratio.gif")
  f_out.Close()
}




def histo_to_tgraph( TH1* h, bool zero_sup ):

    if not h: return 0
    
    tg = ROOT.TGraphErrors(0)
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




if __name__ == "__main__":
    parseOptions()
    loadLibs()
    cbsep_roofit()
