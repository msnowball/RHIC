const Int_t knpart = 5;
const Int_t knseg_max  = 3;
const Int_t knseg[] = {1, 1, 1, 1, 1}; //{2, 3, 3, 1, 3};
const char *kpart_name[knpart] = {"B", "D", "p", "k", "Rl"};
const char  kreadin[knpart] = {0,0,1,0,1};
const Int_t knpbin = 9; //18
const Float_t kpmin = 0.75;
const Float_t kpmax = 5.25;
const Float_t kpstep = (kpmax-kpmin)/knpbin;
const Int_t knppbin = 40;
const Float_t kppmin = 0.0;
const Float_t kppmax = 20.0;
const Int_t knpadx = 3;
const Int_t knpady = 3;
const Int_t khbin = 500;
const Float_t khxmin = -0.5;
const Float_t khxmax = 0.5;
const Float_t khymin = 1e-1;
const Float_t khymax = 1e6;
const Int_t khcolor[] = { 2, 4, 8, 3, 1  };
const Int_t khwidth[] = { 3, 3, 3, 3, 3, };
const Float_t klt = 1e10; // 10*pb^-1 = 1e10*mb^-1
const Float_t ksigma[knpart] = { 5.869e-4, 1.727e-2, 5.155e1, 5.155e1, 1.727e-2 };
const Float_t knevent[knpart][knseg_max] = { {2e8, 0}, {2e8, 1e9}, {1e9, 1e10}, {1e9, 1e10}, {2e8, 0} };
const Float_t kreal[] = {30, 30, 1, 1, 30}; 

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

void draw_dca_distrib()
{
  gROOT->SetStyle("Plain");
  gStyle->SetOptStat("plain");
  //gStyle->SetOptStat(000000);

  Float_t contrib[knpart][knpbin][knseg_max];
  for ( int ipart = 0; ipart < knpart; ipart++ ) 
    for ( int ipbin = 0; ipbin < knpbin; ipbin++ )
      for ( int iseg = 0; iseg < knseg_max; iseg++ )
        contrib[ipart][ipbin][iseg] = 1;
  /*
  contrib[1][0][1] = 0;
  contrib[1][1][1] = 0;
  contrib[2][0][1] = 0;
  contrib[2][1][1] = 0;
  contrib[2][2][1] = 0;
  contrib[2][3][1] = 0;
  contrib[3][0][1] = 0;
  contrib[3][1][1] = 0;
  contrib[3][2][1] = 0;
  contrib[3][3][1] = 0;
  */

  TFile *f[knpart][knseg_max];

  //f[0][0] = new TFile("../data/merge/match_pdst_pp_ba_24m_3.root");
  //f[0][0] = new TFile("../data/merge/match_pdst_pp_ba_flat.root");
  f[0][0] = new TFile("meson_transform/data/train_b.root");

  //f[1][0] = new TFile("../data/merge/match_pdst_pp_ca_4.253b_3.root");
  //f[1][0] = new TFile("../data/merge/match_pdst_pp_ca_flat.root");
  f[1][0] = new TFile("meson_transform/data/train_c.root");


  //f[2][0] = new TFile("../data/merge/match_pdst_pp_bg_250b_2.root"); 
  f[2][0] = new TFile("../data/merge/match_pdst_pp_bg_250b_2.root");


  f[3][0] = new TFile("../data/merge/match_pdst_pp_ba_24m_3.root");

  f[4][0] = new TFile("../data/merge/match_pdst_pp_all_250b_3.root");

  TFile *freadin = new TFile("bin500_10pb/draw_dca_distrib_3.root", "READ");
  TH1F *hpart_total_tmp[knpart][knpbin];

  TString tree_name("mc_trk_eval");
  TTree *t[knpart][knseg_max]; 
  for ( Int_t ipart = 0; ipart < knpart; ipart++ ) {
    for ( Int_t iseg = 0; iseg < knseg[ipart]; iseg++ ) {
      cout << f[ipart][iseg]->GetName() << endl;
      t[ipart][iseg] = (TTree*)f[ipart][iseg]->Get(tree_name);
      //if (ipart == 4) t[ipart][iseg] = (TTree*)f[ipart][iseg]->Get("trk_eval");
    }
  }

  
  TH1F *hpart[knpart][knpbin][knseg_max];
  TString hpart_name[knpart][knpbin][knseg_max];

  TH1F *hpart_total[knpart][knpbin];
  TString hpart_total_name[knpart][knpbin];

  TString draw_value("(x0reco-evtx0mc)*(px0reco/sqrt(px0reco**2+py0reco**2))+(y0reco-evty0mc)*(py0reco/sqrt(px0reco**2+py0reco**2))");
//  TString draw_value("(y0reco-evty0mc)*(px0reco/sqrt(px0reco**2+py0reco**2))-(x0reco-evtx0mc)*(py0reco/sqrt(px0reco**2+py0reco**2))");

  TString cut_value("Abs((X0reco-evtx0mc)*(px0reco/sqrt(px0reco**2+py0reco**2))+(y0reco-evty0mc)*(py0reco/sqrt(px0reco**2+py0reco**2)))<0.5&&x0reco!=-9999&&y0reco!=-9999");
  TString dca_phi_cut_value("&&abs((y0reco-evty0mc)*(px0reco/sqrt(px0reco**2+py0reco**2))-(x0reco-evtx0mc)*(py0reco/sqrt(px0reco**2+py0reco**2)))<1&&x0reco!=-9999&&y0reco!=-9999&&mu_px!=-9999");

  //TString draw_value("((x0mc-px0mc/pz0mc*(z0mc-evtz0mc))-evtx0mc)*(px0mc/sqrt(px0mc**2+py0mc**2))+((y0mc-py0mc/pz0mc*(z0mc-evtz0mc))-evty0mc)*(py0mc/sqrt(px0mc**2+py0mc**2))");

  //TString draw_value("((y0mc-py0mc/pz0mc*(z0mc-evtz0mc))-evty0mc)*(px0mc/sqrt(px0mc**2+py0mc**2))-((x0mc-px0mc/pz0mc*(z0mc-evtz0mc))-evtx0mc)*(py0mc/sqrt(px0mc**2+py0mc**2))");

  //TString draw_value("mu_chi2");
  //TString cut_value("mu_chi2<100&&x0reco!=-9999&&y0reco!=-9999&&mu_px!=-9999");

  TString mu_cut_value("&&mu_chi2<5&&mu_lastGap==4&&abs(mu_eta)>1.2&&abs(mu_eta)<2.4&&mu_DG0<15&&mu_pT<30&&abs(mu_pz)>2.8&&abs(mu_pz)<=30&&mu_DDG0<10&&mu_mutr_nhits>=12");
 
  TString xcor("mu_pT");

  cut_value += dca_phi_cut_value;
  cut_value += mu_cut_value;

  //cut_value += "&&Entry$<100";

  TH1F *hmu_pT[knpart];
  for ( Int_t ipart = 0; ipart < knpart; ipart++ ) {
    TString hmu_pT_name("mu_pT_");
    hmu_pT_name += kpart_name[ipart];
    hmu_pT[ipart] = new TH1F(hmu_pT_name, hmu_pT_name, 2*knpbin, kpmin, kpmax);

    if (kreadin[ipart] == 0) t[ipart][0]->Draw( xcor + ">>" + hmu_pT_name, cut_value );
  } 


  TH1F *hdca_res[knpart];
  for ( Int_t ipart = 0; ipart < knpart; ipart++ ) {
    TString dca_res_name("DCA_res_");
    dca_res_name += kpart_name[ipart];
    hdca_res[ipart] = new TH1F(dca_res_name, dca_res_name, knpbin, kpmin, kpmax);
  }

  TH1F *hppart[knpart][knpbin];
  TString hppart_name[knpart][knpbin];  
  TH1F *hppart_total[knpart];
  TString hppart_total_name[knpart];
  for ( Int_t ipart = 0 ; ipart < 2; ipart++ ) {
    hppart_total_name[ipart] = TString("h_") + kpart_name[ipart] + "_p_pt_total";
    hppart_total[ipart] = new TH1F(hppart_total_name[ipart], hppart_total_name[ipart], knppbin, kppmin, kppmax);
    if (kreadin[ipart] == 0) t[ipart][0]->Draw( "py_p_pT>>" + hppart_total_name[ipart], cut_value );
  }
  for ( Int_t ipbin = 0; ipbin < knpbin; ipbin++ ) {
    Float_t pmin = kpmin + ipbin * kpstep;
    Float_t pmax = kpmin + (ipbin+1) * kpstep;

    for ( Int_t ipart = 0; ipart < knpart; ipart++ ) {
      hpart_total_name[ipart][ipbin] = TString("h_") + kpart_name[ipart] + "_pt_" + int((ipbin+2)/2);
      if (ipbin%2 == 1) hpart_total_name[ipart][ipbin] += TString(".5");
   
      //read in hist
      if (kreadin[ipart] == 1)
      {
        hpart_total_tmp[ipart][ipbin] = (TH1F*)((TCanvas*)freadin->Get("dca"))->GetPad(ipbin+1)->FindObject( hpart_total_name[ipart][ipbin] );
        if (!hpart_total_tmp[ipart][ipbin]) cout << hpart_total_name[ipart][ipbin] << " not found " << endl;
        hpart_total_tmp[ipart][ipbin]->SetName(hpart_total_name[ipart][ipbin] + "_readin");
        //cout << "  hpart_total_tmp name " << hpart_total_tmp[ipart][ipbin]->GetName() << endl;
      }

      hpart_total[ipart][ipbin] = new TH1F( hpart_total_name[ipart][ipbin], hpart_total_name[ipart][ipbin], khbin, khxmin, khxmax );
      if (kreadin[ipart] == 1) 
      {
        for (int idcabin = 0; idcabin < khbin; idcabin++) hpart_total[ipart][ipbin]->SetBinContent(idcabin+1, hpart_total_tmp[ipart][ipbin]->GetBinContent(idcabin+1));
      }

      for ( Int_t iseg = 0; iseg < knseg[ipart]; iseg++ ) {
        hpart_name[ipart][ipbin][iseg] = TString("h_") + kpart_name[ipart] + "_pt_" + (ipbin+1) + "_seg" + iseg;
        cout << hpart_name[ipart][ipbin][iseg] << endl;
        hpart[ipart][ipbin][iseg] = new TH1F( hpart_name[ipart][ipbin][iseg], hpart_name[ipart][ipbin][iseg], khbin, khxmin, khxmax );

        TString pcut;
        stringstream pcutstr;
        //pcutstr << "py_p_pT>4&&";
        pcutstr << xcor.Data() << ">=" << pmin << "&&" << xcor.Data() << "<" << pmax;
        pcut = pcutstr.str();
        cout << pcut << endl;

        //if (ipart != 4) t[ipart][iseg]->Draw( draw_value + ">>" + hpart_name[ipart][ipbin][iseg], cut_value+"&&"+pcut+"&&Entry$%2==0");
        //else t[ipart][iseg]->Draw( draw_value + ">>" + hpart_name[ipart][ipbin][iseg], cut_value+"&&"+pcut+"&&Entry$%2==1");
        if (kreadin[ipart] == 0) t[ipart][iseg]->Draw( draw_value + ">>" + hpart_name[ipart][ipbin][iseg], cut_value+"&&"+pcut );
      }
      hppart_name[ipart][ipbin] = TString("h_") + kpart_name[ipart] + "_p_pt_" + int((ipbin+2)/2);
      if (ipbin%2 == 1) hppart_name[ipart][ipbin] += TString(".5");
      cout << hppart_name[ipart][ipbin] << endl;
      hppart[ipart][ipbin] = new TH1F( hppart_name[ipart][ipbin], hppart_name[ipart][ipbin], knppbin, kppmin, kppmax );
      TString pcut;
      stringstream pcutstr;
      pcutstr << xcor.Data() << ">=" << pmin << "&&" << xcor.Data() << "<" << pmax;
      pcut = pcutstr.str();
      cout << pcut << endl;
      if (kreadin[ipart] == 0) t[ipart][0]->Draw( "py_p_pT>>" + hppart_name[ipart][ipbin], cut_value+"&&"+pcut );

      for (int jpbin = 0; jpbin < knppbin; jpbin++)
      {
        if (ipart >= 2) continue;
        if (hppart_total[ipart] != 0 && hppart_total[ipart]->GetBinContent(jpbin+1) != 0)
          hppart[ipart][ipbin]->SetBinContent(jpbin+1, hppart[ipart][ipbin]->GetBinContent(jpbin+1)/hppart_total[ipart]->GetBinContent(jpbin+1));
        else 
          hppart[ipart][ipbin]->SetBinContent(jpbin+1, 0);
      } 
    }
  }

  TCanvas *c_dca = new TCanvas( "dca", "dca", 1200, 900 );
  c_dca->Divide(knpadx, knpady) ;

  for ( Int_t ipbin = 0; ipbin < knpbin; ipbin++ ) {
    c_dca->cd(ipbin+1);
    Float_t hymax = 0.0;

    for ( Int_t ipart = knpart-1 ; ipart >= 0; ipart-- ) {
      hdca_res[ipart]->SetBinContent(ipbin, hpart[ipart][ipbin][0]->GetRMS());
      hdca_res[ipart]->SetBinError(ipbin, hpart[ipart][ipbin][0]->GetRMSError());

      TString draw_option("");
      if (ipart != knpart-1) draw_option = TString("same");

      int nseg_contrib = 0;
      for ( Int_t iseg = 0; iseg < knseg[ipart]; iseg++ ) {
        float scale = klt*ksigma[ipart]/knevent[ipart][iseg];
        cout << "ipart " << ipart << " iseg " << iseg << " ipbin " << ipbin << " : " << scale << endl;
/*        if ( ipart != knpart-1 ) {
          if( knevent[ipart][iseg] != 0 ) hpart[ipart][ipbin][iseg]->Scale( klt*ksigma[ipart]/knevent[ipart][iseg] );
          else hpart[ipart][ipbin][iseg]->Scale(0);
        }
*/
        if ( hpart[ipart][ipbin][iseg] ) hpart[ipart][ipbin][iseg]->Scale( contrib[ipart][ipbin][iseg] );
        if ( contrib[ipart][ipbin][iseg] != 0 ) nseg_contrib += contrib[ipart][ipbin][iseg];
        
        *hpart_total[ipart][ipbin] = *hpart_total[ipart][ipbin] + *hpart[ipart][ipbin][iseg];
      }

      cout << "nseg_contrib " << nseg_contrib << endl;

      hpart_total[ipart][ipbin]->Scale( 1.0/nseg_contrib );
      //if ( ipart != knpart-1 ) 
      //hpart_total[ipart][ipbin]->Scale( kreal[ipart] );

      hpart_total[ipart][ipbin]->GetXaxis()->SetTitle("dca_r [cm]");
      hpart_total[ipart][ipbin]->SetLineColor( khcolor[ipart] );
      hpart_total[ipart][ipbin]->SetLineWidth( khwidth[ipart] );
      hpart_total[ipart][ipbin]->SetMinimum(khymin);
      hpart_total[ipart][ipbin]->SetMaximum(khymax);
      if (ipart != 3) 
      hpart_total[ipart][ipbin]->Draw( draw_option );
      
      /*
      TGraphErrors *tg = histo_to_tgraph( hpart_total[ipart][ipbin], false );
      TString tg_title = hpart_total_name[ipart][ipbin];
      tg_title.Replace(0, 8, "@pT=");
      tg_title += TString("GeV");
      tg->SetTitle( tg_title );
      tg->GetXaxis()->SetTitle("DCA_r [cm]"); 
      tg->SetMarkerColor( khcolor[ipart] );
      tg->SetLineColor( khcolor[ipart] );
      tg->SetLineWidth( khwidth[ipart] );
      tg->SetMinimum(khymin);
      tg->SetMaximum(khymax);
      if (ipart != 3) tg->Draw( draw_option );

      TLatex ll[4];
      ll[0].SetTextSize(0.045);
      ll[0].DrawLatex(1., 0.9,"Merged events");
      ll[1].SetTextColor(2);
      ll[1].DrawLatex(1., 0.8, "B events");
      ll[2].SetTextColor(4);
      ll[2].DrawLatex(1., 0.7, "D events");
      ll[3].SetTextColor(6);
      ll[3].DrawLatex(1., 0.6, "Bkgnd events");
      */

      c_dca->GetPad( ipbin+1 )->SetLogy();
      c_dca->GetPad( ipbin+1 )->SetGridx();
      c_dca->GetPad( ipbin+1 )->SetGridy();

    }
  }

  TCanvas *c_dca_res = new TCanvas("c_dca_res", "c_dca_res"); 
  for ( Int_t ipart = knpart-1 ; ipart >= 0; ipart-- ) {
    TString draw_option("pl");
    if (ipart != knpart-1) draw_option += "same";

    hdca_res[ipart]->SetMinimum(0.0);
    hdca_res[ipart]->SetMaximum(0.2);
    hdca_res[ipart]->SetLineColor( khcolor[ipart] );
    if (ipart != 3) hdca_res[ipart]->Draw( draw_option ); 
  }

  TCanvas *c_ppT = new TCanvas( "ppT", "ppT", 1200, 900 );
  c_ppT->Divide(knpadx, knpady);
  for ( Int_t ipbin = 0; ipbin < knpbin; ipbin++ ) {
    c_ppT->cd(ipbin+1);

    double hbin_max = 0;
    for ( Int_t ipart = 1 ; ipart >= 0; ipart-- ) {
      if (hppart[ipart][ipbin]->GetBinContent( hppart[ipart][ipbin]->GetMaximumBin() ) > hbin_max)
        hbin_max = hppart[ipart][ipbin]->GetBinContent( hppart[ipart][ipbin]->GetMaximumBin() );
    }

    for ( Int_t ipart = knpart-1 ; ipart >= 0; ipart-- ) {
      TString draw_option("");
      if (ipart != knpart-1) draw_option = TString("same");

      hppart[ipart][ipbin]->SetLineColor( khcolor[ipart] );
      hppart[ipart][ipbin]->SetLineWidth( khwidth[ipart] );
      hppart[ipart][ipbin]->SetMaximum( hbin_max*1.2 );

      if (ipart == 0 || ipart == 1)
        hppart[ipart][ipbin]->Draw( draw_option );
    }
  }
  c_ppT->SaveAs("c_ppT.gif");

  TCanvas *c_mu_pT = new TCanvas("c_mu_pT", "c_mu_pT", 1200, 900 );
  for ( Int_t ipart = knpart-1 ; ipart >= 0; ipart-- ) {
    TString draw_option("");
    if (ipart != knpart-1) draw_option += "same";

    hmu_pT[ipart]->SetLineColor( khcolor[ipart] );
    if (ipart != 3) hmu_pT[ipart]->Draw( draw_option );
  }

  TFile *f_out = new TFile("draw_dca_distrib.root", "RECREATE");
  c_dca->Write();
  c_dca_res->Write();
  c_ppT->Write();
  c_mu_pT->Write();
}

