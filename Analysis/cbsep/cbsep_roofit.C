const int k_n_smooth = 1;

const double k_pt_min = 0.75;
const double k_pt_max = 5.25;
const int k_n_ptbin = 9;
const int k_n_flavor = 4;
const int k_n_pptbin = 40;
const double k_ppt_min = 0.0;
const double k_ppt_max = 20.0;

const char* k_flavor_name[] = {"D", "B", "Rl", "p"};

const int k_nbin_max = 500;
const int k_bin_start = 1;
const int k_bin_end = 500;
const double k_bin_count_min = 0;

// all
//const double k_xmin[] = {-0.5, -0.5, -0.50, -0.20, -0.30, -0.13, -0.06, -0.05, -0.05};
//const double k_xmax[] = { 0.3,  0.3,  0.30,  0.15,  0.20,  0.08,  0.02,  0.02,  0.02};
const double k_xmin[] = {-0.5, -0.5, -0.50, -0.30, -0.30, -0.20, -0.10, -0.08, -0.05, -0.03, -0.03, -0.03, -0.03, -0.03, -0.03, -0.03, -0.03, -0.03};
const double k_xmax[] = { 0.3,  0.3,  0.30,  0.20,  0.20,  0.15,  0.08,  0.05,  0.03,  0.01,  0.01,  0.01,  0.01,  0.01,  0.01,  0.01,  0.01,  0.01};


int nbin_used = 0;

//____________________________________________________________
TGraphErrors* histo_to_tgraph( TH1* h, bool zero_sup )
{
 if( !h ) return 0;

 TGraphErrors *tg = new TGraphErrors( 0 );
 int point( 0 );
 for( int i=0; i<h->GetNbinsX(); i++ )
 {
 double x = h->GetXaxis()->GetBinCenter( i+1 );
 double y = double( h->GetBinContent( i+1 ) );
 double error = double( h->GetBinError( i+1 ) );
 if( zero_sup && !y ) continue;
 tg->SetPoint( point, x, y );
 tg->SetPointError( point, 0, error );
 point++;
 }
 
 return tg;
}

void cbsep_roofit()
{
  TFile *f_in  = new TFile("../bin500_10pb/draw_dca_distrib_train.root"); //_B_shape_tilt_p_0.040.root"); // draw_dca_distrib_ba_B_shape_tilt_m_0.020.root"); draw_dca_distrib_1_fullpt.root
  TFile *f_in2 = new TFile("../bin500_10pb/draw_dca_distrib_3.root");
  TFile *f_ppT = new TFile("../bin500_10pb/draw_dca_distrib_train.root"); //_B_shape_tilt_p_0.040.root"); //draw_dca_distrib_ba_B_shape_tilt_m_0.020.root"); 
//  TFile *f_ppT = new TFile("../draw_dca_distrib_ppT.root");

  
  TH1F *h_p[k_n_ptbin][k_n_flavor];
  TH1F *h_p2[k_n_ptbin][k_n_flavor];
  TH1F *h_p_p[k_n_ptbin][k_n_flavor];

  double fit_ratio[k_n_ptbin][3];
  double fit_error[k_n_ptbin][3];

  for (int i_ptbin = 0; i_ptbin < k_n_ptbin; i_ptbin++)
  {
    for (int i_flavor = 0; i_flavor < k_n_flavor; i_flavor++)
    {
      TString h_name("h_");
      h_name += k_flavor_name[i_flavor];
      h_name += "_pt_";
      h_name += int((i_ptbin+2)/2);
      if (i_ptbin%2 == 1) h_name += TString(".5");
      {
        h_p2[i_ptbin][i_flavor] = (TH1F*)((TCanvas*)f_in2->Get("dca"))->GetPad(i_ptbin+1)->FindObject(h_name);
        TString h_p2_name = h_p2[i_ptbin][i_flavor]->GetName();
        h_p2_name += "_2";
        //h_p2[i_ptbin][i_flavor]->SetName( h_p2_name );
        //cout << h_p2_name << endl;

        h_p[i_ptbin][i_flavor] = (TH1F*)((TCanvas*)f_in->Get("dca"))->GetPad(i_ptbin+1)->FindObject(h_name);
        if (!h_p[i_ptbin][i_flavor])
        {
          TString h_name("h_");
          h_name += k_flavor_name[i_flavor];
          h_name += "_pt_";
          h_name += i_ptbin;
          h_p[i_ptbin][i_flavor] = (TH1F*)((TCanvas*)f_in->Get("dca"))->GetPad(i_ptbin+1)->FindObject(h_name);
          if (!h_p[i_ptbin][i_flavor]) 
          {
            h_name.ReplaceAll("p_", "Bg_");
            h_p[i_ptbin][i_flavor] = (TH1F*)((TCanvas*)f_in->Get("dca"))->GetPad(i_ptbin+1)->FindObject(h_name);
          }
        } 
      }

      cout << h_p[i_ptbin][i_flavor]->GetName() << "  "
           << h_p[i_ptbin][i_flavor]->GetBinContent(k_nbin_max/2) << " error "
           << h_p[i_ptbin][i_flavor]->GetBinError(k_nbin_max/2) << "  "
           << h_p2[i_ptbin][i_flavor]->GetBinContent(k_nbin_max/2) << "  error "
           << h_p2[i_ptbin][i_flavor]->GetBinError(k_nbin_max/2)<< " integral "  
           << h_p2[i_ptbin][i_flavor]->Integral()
           << endl;

      TString h_p_name("h_");
      h_p_name += k_flavor_name[i_flavor];
      h_p_name += "_p_pt_";
      h_p_name += int((i_ptbin+2)/2);
      if (i_ptbin%2 == 1) h_p_name += TString(".5");
      if ( i_flavor == 0 || i_flavor == 1 )
        h_p_p[i_ptbin][i_flavor] = (TH1F*)((TCanvas*)f_ppT->Get("ppT"))->GetPad(i_ptbin+1)->FindObject(h_p_name);
      else {
        h_p_p[i_ptbin][i_flavor] = new TH1F(h_p_name, h_p_name, k_n_pptbin, k_ppt_min, k_ppt_max);
        h_p_p[i_ptbin][i_flavor]->SetLineColor(5);
      }
      cout << h_p_p[i_ptbin][i_flavor]->GetName() << "  " << h_p_p[i_ptbin][i_flavor]->Integral() << endl;
    }
  }

  TH1F *h_c_percent_true = new TH1F("h_c_percent_true", "h_c_percent_true", k_n_ptbin, k_pt_min, k_pt_max);
  TH1F *h_b_percent_true = new TH1F("h_b_percent_true", "h_b_percent_true", k_n_ptbin, k_pt_min, k_pt_max);
  TH1F *h_m_percent_true = new TH1F("h_m_percent_true", "h_m_percent_true", k_n_ptbin, k_pt_min, k_pt_max);

  TH1F *h_c_spectrum_true = new TH1F("h_c_spectrum_true", "h_c_spectrum_true", k_n_ptbin, k_pt_min, k_pt_max);
  TH1F *h_b_spectrum_true = new TH1F("h_b_spectrum_true", "h_b_spectrum_true", k_n_ptbin, k_pt_min, k_pt_max);
  TH1F *h_m_spectrum_true = new TH1F("h_m_spectrum_true", "h_m_spectrum_true", k_n_ptbin, k_pt_min, k_pt_max);

  for (int i_ptbin = 0; i_ptbin < k_n_ptbin; i_ptbin++)
  {

    double n_c_tot = h_p[i_ptbin][0]->Integral();
    double n_b_tot = h_p[i_ptbin][1]->Integral();
    double n_rl_tot = h_p[i_ptbin][2]->Integral();
    double n_mb_tot = h_p[i_ptbin][3]->Integral();
    cout << n_c_tot << " " << n_b_tot << " " << n_rl_tot << " " << n_mb_tot << endl;
   
    double n_c_tot2 = h_p2[i_ptbin][0]->Integral();
    double n_b_tot2 = h_p2[i_ptbin][1]->Integral();
    double n_rl_tot2 = h_p2[i_ptbin][2]->Integral();
    double n_mb_tot2 = h_p2[i_ptbin][3]->Integral();
    cout << n_c_tot2 << " " << n_b_tot2 << " " << n_rl_tot2 << " " << n_mb_tot2 << endl;

    for (int j = 0; j < 4; j++)
    {
      h_p[i_ptbin][j]->Smooth(k_n_smooth);
      h_p2[i_ptbin][j]->Smooth(k_n_smooth);
    }

    if (0) {    
      h_p[i_ptbin][0]->Scale(1.0/n_c_tot);
      h_p[i_ptbin][1]->Scale(1.0/n_b_tot);
      h_p[i_ptbin][2]->Scale(1.0/n_rl_tot);
      h_p[i_ptbin][3]->Scale(1.0/n_mb_tot);

      h_p2[i_ptbin][0]->Scale(1.0/n_c_tot2);
      h_p2[i_ptbin][1]->Scale(1.0/n_b_tot2);
      h_p2[i_ptbin][2]->Scale(1.0/n_rl_tot2);
      h_p2[i_ptbin][3]->Scale(1.0/n_mb_tot2);
    }

    if (n_c_tot2+n_b_tot2+n_mb_tot2 != 0) {
      h_c_percent_true->SetBinContent(i_ptbin+1, double(n_c_tot2)/(n_c_tot2+n_b_tot2+n_mb_tot2));
      h_b_percent_true->SetBinContent(i_ptbin+1, double(n_b_tot2)/(n_c_tot2+n_b_tot2+n_mb_tot2));
      h_m_percent_true->SetBinContent(i_ptbin+1, double(n_mb_tot2)/(n_c_tot2+n_b_tot2+n_mb_tot2));
    }
    else {
      h_c_percent_true->SetBinContent(i_ptbin+1, 0);
      h_b_percent_true->SetBinContent(i_ptbin+1, 0);
      h_m_percent_true->SetBinContent(i_ptbin+1, 0);
    }
    h_c_spectrum_true->SetBinContent(i_ptbin+1, n_c_tot2);
    h_b_spectrum_true->SetBinContent(i_ptbin+1, n_b_tot2);
    h_m_spectrum_true->SetBinContent(i_ptbin+1, n_mb_tot2);

    double xmin = -0.0;
    double xmax = 0.0;

    int bin_start = k_bin_start;
    int bin_end = k_bin_end;
    for (int i_bin = k_bin_start; i_bin <= k_bin_end; i_bin++)
    {
      double c_tmp = h_p[i_ptbin][0]->GetBinContent(i_bin);
      double b_tmp = h_p[i_ptbin][1]->GetBinContent(i_bin);
      double m_tmp = h_p[i_ptbin][3]->GetBinContent(i_bin);
      double rl_tmp = h_p2[i_ptbin][2]->GetBinContent(i_bin);

      if( c_tmp >= k_bin_count_min && b_tmp >= k_bin_count_min && m_tmp >= k_bin_count_min && bin_start == k_bin_start ) bin_start = i_bin; 
      if( !(c_tmp >= k_bin_count_min && b_tmp >= k_bin_count_min && m_tmp >= k_bin_count_min) && i_bin > (k_bin_end-k_bin_start)/2 && bin_end == k_bin_end ) bin_end = i_bin;
    }
    xmin = -0.5+1.0/k_nbin_max*(bin_start-k_bin_start);
    xmax = -0.5+1.0/k_nbin_max*(bin_end-k_bin_start);

    xmin = k_xmin[i_ptbin];
    xmax = k_xmax[i_ptbin];
    cout << "xmin " << xmin << " xmax " << xmax << endl;

    RooRealVar ratio_signal("ratio_signal", "ratio_signal", 0.5, 0.0, 1.0);
    RooRealVar ratio_charm("ratio_charm", "ratio_charm", 0.5, 0.0, 1.0);

    RooRealVar x("x", "x", xmin, xmax);
    RooDataHist hist_c("hist_c", "hist_c", x, h_p[i_ptbin][0]);
    RooDataHist hist_b("hist_b", "hist_b", x, h_p[i_ptbin][1]);
    RooDataHist hist_m("hist_m", "hist_m", x, h_p[i_ptbin][3]);
    RooDataHist hist_rl("hist_rl", "hist_rl", x, h_p2[i_ptbin][2]);

    RooHistPdf pdf_c("pdf_c", "pdf_c", x, hist_c, 0);
    RooHistPdf pdf_b("pdf_b", "pdf_b", x, hist_b, 0);
    RooHistPdf pdf_m("pdf_m", "pdf_m", x, hist_m, 0);

    RooAddPdf pdf_signal("pdf_signal", "charm+beauty", RooArgList(pdf_c, pdf_b), ratio_charm);
    RooAddPdf pdf_all("pdf_all", "charm+beauty+background", RooArgList(pdf_signal, pdf_m), ratio_signal);

    pdf_all.fitTo( hist_rl );

    //ratio_signal.Print();
    //ratio_charm.Print();

    double fit_ratio_signal = ratio_signal.getVal();
    double fit_error_signal = ratio_signal.getError();
    double fit_ratio_charm  = ratio_charm.getVal();
    double fit_error_charm  = ratio_charm.getError();

    fit_ratio[i_ptbin][0] = fit_ratio_signal * fit_ratio_charm;
    fit_ratio[i_ptbin][1] = fit_ratio_signal * (1.0-fit_ratio_charm);
    fit_ratio[i_ptbin][2]  = 1.0-fit_ratio_signal;    

    fit_error[i_ptbin][0] = fit_ratio[i_ptbin][0] * sqrt( (fit_error_signal/fit_ratio_signal)**2 + (fit_error_charm/fit_ratio_charm)**2 );
    fit_error[i_ptbin][1] = fit_ratio[i_ptbin][1] * sqrt( (fit_error_signal/fit_ratio_signal)**2 + (fit_error_charm/(1-fit_ratio_charm))**2 );
    fit_error[i_ptbin][2] = fit_error_signal; 

    cout << "pT " << i_ptbin << endl;
    cout << "ratio_D   = " << fit_ratio[i_ptbin][0] << " +- " << fit_error[i_ptbin][0] << " true " << h_c_percent_true->GetBinContent(i_ptbin+1) << endl;
    cout << "ratio_B   = " << fit_ratio[i_ptbin][1] << " +- " << fit_error[i_ptbin][1] << " true " << h_b_percent_true->GetBinContent(i_ptbin+1) << endl;
    cout << "ratio_Mb  = " << fit_ratio[i_ptbin][2] << " +- " << fit_error[i_ptbin][2] << " true " << h_m_percent_true->GetBinContent(i_ptbin+1) << endl;
  }

  TCanvas *c_c_percent = new TCanvas( "c_c_percent", "c_c_percent", 1200, 900);
  TH1F *h_c_percent = new TH1F("h_c_percent", "h_c_percent", k_n_ptbin, k_pt_min, k_pt_max);
  TH1F *h_b_percent = new TH1F("h_b_percent", "h_b_percent", k_n_ptbin, k_pt_min, k_pt_max);
  TH1F *h_m_percent = new TH1F("h_m_percent", "h_m_percent", k_n_ptbin, k_pt_min, k_pt_max);

  TH1F *h_c_spectrum = new TH1F("h_c_spectrum", "h_c_spectrum", k_n_ptbin, k_pt_min, k_pt_max);
  TH1F *h_b_spectrum = new TH1F("h_b_spectrum", "h_b_spectrum", k_n_ptbin, k_pt_min, k_pt_max);
  TH1F *h_m_spectrum = new TH1F("h_m_spectrum", "h_m_spectrum", k_n_ptbin, k_pt_min, k_pt_max);

  for (int i_ptbin = 0; i_ptbin < k_n_ptbin; i_ptbin++)
  {
    //if (i_ptbin == 0) h_c_percent->SetBinContent(1, 0.9853);
    //else if (i_ptbin == 1) h_c_percent->SetBinContent(2, 0.9675);
    //else 
    h_c_percent->SetBinContent(i_ptbin+1, fit_ratio[i_ptbin][0]);
    h_b_percent->SetBinContent(i_ptbin+1, fit_ratio[i_ptbin][1]);
    h_m_percent->SetBinContent(i_ptbin+1, fit_ratio[i_ptbin][2]);

    h_c_percent->SetBinError(i_ptbin+1, fit_error[i_ptbin][0]);
    h_b_percent->SetBinError(i_ptbin+1, fit_error[i_ptbin][1]);
    h_m_percent->SetBinError(i_ptbin+1, fit_error[i_ptbin][2]);
  
    h_c_spectrum->SetBinContent(i_ptbin+1, fit_ratio[i_ptbin][0] * h_p2[i_ptbin][2]->Integral() );
    h_b_spectrum->SetBinContent(i_ptbin+1, fit_ratio[i_ptbin][1] * h_p2[i_ptbin][2]->Integral() );
    h_m_spectrum->SetBinContent(i_ptbin+1, fit_ratio[i_ptbin][2] * h_p2[i_ptbin][2]->Integral() );

    h_c_spectrum->SetBinError(i_ptbin+1, fit_error[i_ptbin][0] * h_p2[i_ptbin][2]->Integral() );
    h_b_spectrum->SetBinError(i_ptbin+1, fit_error[i_ptbin][1] * h_p2[i_ptbin][2]->Integral() );
    h_m_spectrum->SetBinError(i_ptbin+1, fit_error[i_ptbin][2] * h_p2[i_ptbin][2]->Integral() );

}

  TGraphErrors *tg_c_percent = histo_to_tgraph( h_c_percent, false );
  TGraphErrors *tg_b_percent = histo_to_tgraph( h_b_percent, false );
  TGraphErrors *tg_m_percent = histo_to_tgraph( h_m_percent, false );


  tg_c_percent->SetTitle("Extracted fraction #mu from D / B / Bkgnd");
  tg_c_percent->GetXaxis()->SetTitle("#mu p_{T} (GeV/c)");
  tg_c_percent->GetYaxis()->SetTitle("N_{i} / N_{total}");

  tg_c_percent->SetLineColor(4);
  tg_c_percent->SetLineWidth(3);
  tg_c_percent->SetMarkerStyle(20);
  tg_c_percent->SetMarkerSize(2);
  tg_c_percent->SetMarkerColor(4);
  tg_c_percent->SetMinimum(-0.05);
  tg_c_percent->SetMaximum(1.0);
  tg_c_percent->Draw("AP");
  tg_b_percent->SetLineColor(2);
  tg_b_percent->SetLineWidth(3);
  tg_b_percent->SetMarkerStyle(21);
  tg_b_percent->SetMarkerSize(2);
  tg_b_percent->SetMarkerColor(2);
  tg_b_percent->Draw("Psame");
  tg_m_percent->SetLineColor(8);
  tg_m_percent->SetLineWidth(3);
  tg_m_percent->SetMarkerStyle(22);
  tg_m_percent->SetMarkerSize(2);
  tg_m_percent->SetMarkerColor(8);
  tg_m_percent->Draw("Psame");

  h_c_percent_true->SetLineColor(4);
  h_b_percent_true->SetLineColor(2);
  h_m_percent_true->SetLineColor(8);
  h_c_percent_true->SetLineStyle(2);
  h_b_percent_true->SetLineStyle(2);
  h_m_percent_true->SetLineStyle(2);

  h_c_percent_true->Draw("plsame");
  h_b_percent_true->Draw("plsame");
  h_m_percent_true->Draw("plsame");

  TLatex ll[4];
  double lable_x = 2;
  //ll[0].SetTextSize(0.045);
  //ll[0].DrawLatex(lable_x, 1e5,"Merged events");
  ll[1].SetTextColor(2);
  ll[1].DrawLatex(lable_x, 0.9, "N_{B} / N_{total}");
  ll[2].SetTextColor(4);
  ll[2].DrawLatex(lable_x, 0.8, "N_{D} / N_{total}");
  ll[3].SetTextColor(8);
  ll[3].DrawLatex(lable_x, 0.7, "N_{BG} / N_{total}");
   
  
  TFile *f_out = new TFile("cbsep.root", "RECREATE");
//  c_cbsep->Write();
  h_c_percent->Write();
  h_b_percent->Write();
  h_m_percent->Write();
  c_c_percent->Write();
  
  h_c_percent_true->Write();
  h_b_percent_true->Write();
  h_m_percent_true->Write();

  h_c_spectrum->Write();
  h_b_spectrum->Write();
  h_m_spectrum->Write();

  h_c_spectrum_true->Write();
  h_b_spectrum_true->Write();
  h_m_spectrum_true->Write();

  c_c_percent->SaveAs("cbsep.gif");
  
  TFile *f_cut_ratio = new TFile("../make_pt_spectrum/make_pt_spectrum.root");
  TH1F *h_reco_vs_gen_ratio_c = (TH1F*)f_cut_ratio->Get("cut_ratio_charm");
  TH1F *h_reco_vs_gen_ratio_b = (TH1F*)f_cut_ratio->Get("cut_ratio_beauty");
  
  TCanvas *c_bratio = new TCanvas( "c_bratio", "c_bratio", 800, 600);
  TH1F *h_bratio = new TH1F( "h_bratio", "h_bratio", k_n_ptbin, k_pt_min, k_pt_max);
  for (int i_ptbin = 0; i_ptbin < k_n_ptbin; i_ptbin++)
  {
    double mu_before_reco_c = h_c_percent->GetBinContent(i_ptbin+1) / h_reco_vs_gen_ratio_c->GetBinContent(i_ptbin+1);
    double mu_before_reco_b = h_b_percent->GetBinContent(i_ptbin+1) / h_reco_vs_gen_ratio_b->GetBinContent(i_ptbin+1);

    cout << "pT " << i_ptbin << endl;
    cout << "mu_after_reco_c " << h_c_percent->GetBinContent(i_ptbin+1) << " mu_after_reco_b " << h_b_percent->GetBinContent(i_ptbin+1) << endl;
    cout << "mu_reco_ratio_c " << h_reco_vs_gen_ratio_c->GetBinContent(i_ptbin+1) << " mu_reco_ratio_b " << h_reco_vs_gen_ratio_b->GetBinContent(i_ptbin+1) << endl;
    cout << "mu_before_reco_c " << mu_before_reco_c << " mu_before_reco_b " << mu_before_reco_b << endl;
    double bratio_content = mu_before_reco_b / (mu_before_reco_b+mu_before_reco_c);
    h_bratio->SetBinContent(i_ptbin+1, bratio_content );
    //cout << "bratio = " << bratio_content << endl;

    double bratio_after_reco = h_b_percent->GetBinContent(i_ptbin+1) / (h_c_percent->GetBinContent(i_ptbin+1)+h_b_percent->GetBinContent(i_ptbin+1));
    double bratio_after_reco_error = sqrt( (h_c_percent->GetBinError(i_ptbin+1)/h_c_percent->GetBinContent(i_ptbin+1))**2 + (h_b_percent->GetBinError(i_ptbin+1)/h_b_percent->GetBinContent(i_ptbin+1))**2 ) * h_bratio->GetBinContent(i_ptbin+1);
    double ratio_reco_vs_gen = h_reco_vs_gen_ratio_b->GetBinContent(i_ptbin+1) / (h_reco_vs_gen_ratio_c->GetBinContent(i_ptbin+1)+h_reco_vs_gen_ratio_b->GetBinContent(i_ptbin+1));
    double ratio_reco_vs_gen_error = ratio_reco_vs_gen * sqrt( (h_reco_vs_gen_ratio_c->GetBinError(i_ptbin+1)/h_reco_vs_gen_ratio_c->GetBinContent(i_ptbin+1))**2 + (h_reco_vs_gen_ratio_b->GetBinError(i_ptbin+1)/h_reco_vs_gen_ratio_b->GetBinContent(i_ptbin+1))**2 );
    double bratio_error = bratio_content * sqrt( (bratio_after_reco_error/bratio_after_reco)**2 + (ratio_reco_vs_gen_error/ratio_reco_vs_gen)**2 );
    h_bratio->SetBinError(i_ptbin+1, bratio_error);
  }

  TFile *f_pt_spectrum = new TFile("../make_pt_spectrum/make_pt_spectrum.root");
  TH1F *h_pT_true_bratio = new TH1F( "h_pT_true_bratio", "h_pT_true_bratio", k_n_ptbin, k_pt_min, k_pt_max);
  TH1F *h_pT_true_c = (TH1F*)f_pt_spectrum->Get("pythia_charm_muon_pt");
  TH1F *h_pT_true_b = (TH1F*)f_pt_spectrum->Get("pythia_beauty_muon_pt");
  *h_pT_true_bratio = *h_pT_true_b / (*h_pT_true_c + *h_pT_true_b);
  cout << "b ratio " << endl;
  for (int i_ptbin = 0; i_ptbin < k_n_ptbin; i_ptbin++)
  {
    cout << "pT " << 0.5*i_ptbin + 1 << "\t" << h_bratio->GetBinContent(i_ptbin+1) << " +- " << h_bratio->GetBinError(i_ptbin+1) << " true " << h_pT_true_bratio->GetBinContent(i_ptbin+1) << endl;
  }

  h_pT_true_bratio->SetLineColor(4);
  h_pT_true_bratio->SetLineWidth(2);
  
  h_bratio->SetMinimum(0.0);
  h_bratio->SetMaximum(1.0);
  h_bratio->SetLineColor(6);
  h_bratio->GetXaxis()->SetTitle("#mu p_{T} (GeV/c)");
  h_bratio->GetYaxis()->SetTitle("b ratio");
  h_bratio->Draw("e");
  h_pT_true_bratio->Draw("plsame");
  c_bratio->SaveAs("c_mu_pT_bratio.gif");

/*
  TCanvas *c_p_p_total = new TCanvas( "c_p_p_total", "c_p_p_total", 1200, 900);
  TH1F *h_p_p_total[2];
  for (int i_flavor = 0; i_flavor < 2; i_flavor++)
  {
    TString h_p_p_total_name("h_");
    h_p_p_total_name += k_flavor_name[i_flavor];
    h_p_p_total_name += "_p_pT_total";
    cout << h_p_p_total_name << endl;

    h_p_p_total[i_flavor] = new TH1F(h_p_p_total_name, h_p_p_total_name,  k_n_pptbin, k_ppt_min, k_ppt_max);
    h_p_p_total[i_flavor]->SetLineColor( h_p_p[0][i_flavor]->GetLineColor() );
    for (int i_ptbin = 0; i_ptbin < k_n_ptbin; i_ptbin++)
    {
      // this is the ratio of py_p_pT in [0.75,18.75] vs in [0, +inf]
      if (h_p[i_ptbin][i_flavor]->Integral() != 0) h_p_p[i_ptbin][i_flavor]->Scale( h_p[i_ptbin][i_flavor]->Integral() / h_p_p[i_ptbin][i_flavor]->Integral()) ;
      int tt = 0;
      for (int j_ptbin = 0; j_ptbin < k_n_pptbin; j_ptbin++) tt += h_p_p[i_ptbin][i_flavor]->GetBinContent(j_ptbin+1);
      cout << "py_p_pT scale " << i_ptbin << " = " << h_p[i_ptbin][i_flavor]->Integral() << " / " << h_p_p[i_ptbin][i_flavor]->Integral() << " : " << tt << endl;


      if (h_p_p[i_ptbin][i_flavor]->Integral() != 0) h_p_p[i_ptbin][i_flavor]->Scale( 1.0/h_p_p[i_ptbin][i_flavor]->Integral() );
      cout << i_ptbin << " fit_ratio " << fit_ratio[i_ptbin][i_flavor] << " true ratio c " << h_c_percent_true->GetBinContent(i_ptbin+1) << " true ratio b " << h_b_percent_true->GetBinContent(i_ptbin+1) << endl;

      // for pt>5, use true ratio
      if (i_ptbin < 18) h_p_p[i_ptbin][i_flavor]->Scale( fit_ratio[i_ptbin][i_flavor]*h_p2[i_ptbin][2]->Integral() );
      else {
        if (i_flavor == 0)      h_p_p[i_ptbin][i_flavor]->Scale( h_c_percent_true->GetBinContent(i_ptbin+1)* h_p2[i_ptbin][2]->Integral() );
        else if (i_flavor == 1) h_p_p[i_ptbin][i_flavor]->Scale( h_b_percent_true->GetBinContent(i_ptbin+1)* h_p2[i_ptbin][2]->Integral() );
      }

      //cout << "h_reco_vs_gen_ratio_c->GetBinContent(i_ptbin+1) " << h_reco_vs_gen_ratio_c->GetBinContent(i_ptbin+1) << " h_reco_vs_gen_ratio_b->GetBinContent(i_ptbin+1) " << h_reco_vs_gen_ratio_b->GetBinContent(i_ptbin+1) << endl;
      if (i_ptbin < 0) { 
        if (i_flavor == 0)      h_p_p[i_ptbin][i_flavor]->Scale( 1.0/h_reco_vs_gen_ratio_c->GetBinContent(i_ptbin+1) );
        else if (i_flavor == 1) h_p_p[i_ptbin][i_flavor]->Scale( 1.0/h_reco_vs_gen_ratio_b->GetBinContent(i_ptbin+1) );
      }

      *h_p_p_total[i_flavor] = *h_p_p_total[i_flavor] + *h_p_p[i_ptbin][i_flavor];
      for (int i_p_ptbin = 0; i_p_ptbin < k_n_pptbin; i_p_ptbin++)
      {
        //h_p_p[i_ptbin][i_flavor]->SetBinError(i_p_ptbin+1, h_p_p[i_ptbin][i_flavor]->GetBinContent(i_p_ptbin+1) * ((fit_error[i_ptbin][i_flavor]/(fit_error[i_ptbin][0]+fit_error[i_ptbin][1]))**2 + h_reco_vs_gen_ratio_c->GetBinError(i_ptbin+1)**2 + h_reco_vs_gen_ratio_b->GetBinContent(i_ptbin+1)**2) );
        //h_p_p_total[i_flavor]->SetBinError(i_p_ptbin+1, sqrt(h_p_p_total[i_flavor]->GetBinError(i_p_ptbin+1)**2 + h_p_p[i_ptbin][i_flavor]->GetBinError(i_p_ptbin+1)**2) );
      }
    }
    for (int i_p_ptbin = 0; i_p_ptbin < k_n_pptbin; i_p_ptbin++)
    {
      cout << "error " << h_p_p_total[i_flavor]->GetBinError(i_p_ptbin+1) << endl;
    }

    h_p_p_total[i_flavor]->SetMinimum(0.5);
    h_p_p_total[i_flavor]->GetXaxis()->SetTitle("Meson(D/B) p_{T} (GeV/c)");
    h_p_p_total[i_flavor]->GetYaxis()->SetTitle("Count");
    if (i_flavor == 0) h_p_p_total[i_flavor]->Draw( "e" );
    if (i_flavor == 1) h_p_p_total[i_flavor]->Draw( "esame" );
    for (int i_ptbin = 0; i_ptbin < k_n_ptbin; i_ptbin++) 
    {
      if (i_flavor == 0) continue;
      //h_p_p[i_ptbin][i_flavor]->SetLineColor(i_ptbin);
      //h_p_p[i_ptbin][i_flavor]->SetLineStyle(i_flavor);
      //h_p_p[i_ptbin][i_flavor]->Draw("plsame");
    }

    f_out->cd();
    h_p_p_total[i_flavor]->Write();
  }
  c_p_p_total->SetLogy();
  c_p_p_total->SaveAs("c_p_p_total.gif");

  TFile *f_pythia[2];
  f_pythia[0] = new TFile("../../data/merge/match_pdst_pp_ca_4.253b_2.root");
  f_pythia[1] = new TFile("../../data/merge/match_pdst_pp_ba_24m_2.root");

  TTree *t_p_pT_true[2];
  for (int i_flavor = 0; i_flavor < 2; i_flavor++)
  {
    t_p_pT_true[i_flavor] = (TTree*)f_pythia[i_flavor]->Get("mc_trk_eval");
    cout << t_p_pT_true[i_flavor]->GetEntries() << endl;
  }

  TCanvas *c_p_pT_true = new TCanvas( "c_p_pT_true", "c_p_pT_true", 1200, 900);
  TH1F *h_p_pT_true[2];

  TString cut_value("abs((x0reco-evtx0mc)*(px0reco/sqrt(px0reco**2+py0reco**2))+(y0reco-evty0mc)*(py0reco/sqrt(px0reco**2+py0reco**2)))<0.5&&x0reco!=-9999&&y0reco!=-9999");

  TString dca_phi_cut_value("&&abs((y0reco-evty0mc)*(px0reco/sqrt(px0reco**2+py0reco**2))-(x0reco-evtx0mc)*(py0reco/sqrt(px0reco**2+py0reco**2)))<1&&x0reco!=-9999&&y0reco!=-9999&&mu_px!=-9999");

  TString mu_cut_value("&&mu_chi2<5&&mu_lastGap==4&&abs(mu_eta)>1.2&&abs(mu_eta)<2.4&&mu_DG0<15&&mu_pT<30&&abs(mu_pz)>2.8&&abs(mu_pz)<=30&&mu_DDG0<10&&mu_mutr_nhits>=12");

  for (int i_flavor = 0; i_flavor < 2; i_flavor++)
  {
    TString h_p_pT_true_name("h_");
    h_p_pT_true_name += k_flavor_name[i_flavor];
    h_p_pT_true_name += "_p_pT_true";
    cout << h_p_pT_true_name << endl;
    h_p_pT_true[i_flavor] = new TH1F( h_p_pT_true_name, h_p_pT_true_name,  k_n_pptbin, k_ppt_min, k_ppt_max);
    h_p_pT_true[i_flavor]->SetLineColor( h_p_p[0][i_flavor]->GetLineColor() );

    t_p_pT_true[i_flavor]->Draw( "py_p_pT>>"+h_p_pT_true_name, cut_value+dca_phi_cut_value+mu_cut_value );
    
    for (int i_ptbin = 0; i_ptbin < k_n_ptbin; i_ptbin++) cout << h_p_pT_true[i_flavor]->GetBinContent(i_ptbin+1) << endl;
  }

  for (int i_flavor = 0; i_flavor < 2; i_flavor++)
  {
    h_p_pT_true[i_flavor]->SetMinimum(0.5);
    h_p_pT_true[i_flavor]->GetXaxis()->SetTitle("Pythia_Meson(D/B) p_{T} (GeV/c)");
    h_p_pT_true[i_flavor]->GetYaxis()->SetTitle("Count");

    TString drawOption( "" );
    if (i_flavor != 0) drawOption = TString("same");
    h_p_pT_true[i_flavor]->Draw( drawOption);
  }
  c_p_pT_true->SetLogy();
  c_p_pT_true->SaveAs("c_p_pT_true.gif");

  TCanvas *c_p_p_bratio = new TCanvas( "c_p_p_bratio", "c_p_p_bratio", 1200, 900);
  TH1F *h_p_p_bratio = new TH1F( "h_p_pT_bratio", "h_p_pT_bratio",  k_n_pptbin, k_ppt_min, k_ppt_max);
  *h_p_p_bratio = *h_p_p_total[1] / (*h_p_p_total[0] + *h_p_p_total[1]);
  for (int i_p_ptbin = 0; i_p_ptbin < k_n_pptbin; i_p_ptbin++)
  {
    double error = 0;
    if ( h_p_p_total[0]->GetBinContent(i_p_ptbin+1) != 0 ) error = h_p_p_total[0]->GetBinError(i_p_ptbin+1)/h_p_p_total[0]->GetBinContent(i_p_ptbin+1);
    if ( h_p_p_total[1]->GetBinContent(i_p_ptbin+1) != 0 ) error = sqrt( error**2 + (h_p_p_total[1]->GetBinError(i_p_ptbin+1)/h_p_p_total[1]->GetBinContent(i_p_ptbin+1))**2 );
    h_p_p_bratio->SetBinError(i_p_ptbin+1, error*h_p_p_bratio->GetBinContent(i_p_ptbin+1) );
  }

  TH1F *h_p_p_true_bratio = new TH1F( "h_p_pT_true_bratio", "h_p_pT_true_bratio", k_n_pptbin, k_ppt_min, k_ppt_max);
  // *h_p_p_true_bratio = *h_p_pT_true[1] / (*h_p_pT_true[0] + *h_p_pT_true[1]);
  for (int i_p_ptbin = 0; i_p_ptbin < k_n_pptbin; i_p_ptbin++)
  {
    h_p_p_true_bratio->SetBinContent(i_p_ptbin+1, h_p_pT_true[1]->GetBinContent(i_p_ptbin+1)/(h_p_pT_true[0]->GetBinContent(i_p_ptbin+1)+h_p_pT_true[1]->GetBinContent(i_p_ptbin+1)));
  }
  h_p_p_true_bratio->SetLineColor(4);
  h_p_p_true_bratio->SetLineWidth(2);

  h_p_p_bratio->SetMinimum(0.0);
  h_p_p_bratio->SetMaximum(1.0);
  h_p_p_bratio->SetLineColor(6);
  h_p_p_bratio->GetXaxis()->SetTitle("Meson(D/B) p_{T} (GeV/c)");
  h_p_p_bratio->GetYaxis()->SetTitle("b ratio");
  h_p_p_bratio->Draw("e");
  h_p_p_true_bratio->Draw("plsame");
  c_p_p_bratio->SaveAs("c_p_p_bratio.gif");
*/
  f_out->Close();
}
