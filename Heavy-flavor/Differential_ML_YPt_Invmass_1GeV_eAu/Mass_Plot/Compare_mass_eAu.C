// Macro to plot the canvas with no spaces
// Shyam Kumar; INFN Bari, shyam.kumar@ba.infn.it
#include "ePIC_style.C"
void Compare_mass_eAu(){

  gROOT->ProcessLine("set_ePIC_style()");
  gStyle->SetOptTitle(0);
  gStyle->SetOptStat(0);
  gStyle->SetTitleSize(0.05,"XY");
  gStyle->SetLabelSize(0.05,"XY");
  gStyle->SetTitleOffset(1.10,"XY");

  double nEvents = 341.3e+7; // 10fb-1
  bool norm_selfentries = false;
  bool norm_nEvents = true;

  const Int_t nBins = 4;
  Double_t y_arr[nBins+1]  = {-2.0, -1.0,  1.0,  2.0, 3.0};
  Double_t pt_arr[nBins-1] = { 1.0, 20.0};

  int fact_rebin = 5;
  const int nplots = 1;
  TCanvas *c[nplots];
  for (int i=0; i<nplots; ++i){
    c[i]= new TCanvas(Form("c%d",i),Form("c%d",i),0,52,1400,1000);
  }

  c[0]->SetLeftMargin(0);
  c[0]->SetRightMargin(0);
  c[0]->SetTopMargin(0);
  c[0]->SetBottomMargin(0);

  const double eps = 1e-4; 

  TPad *pads[4];
  pads[0] = new TPad("p1","", 0.0,     0.5-eps, 0.5+eps, 1.0,     0,0,0); 
  pads[1] = new TPad("p2","", 0.5-eps, 0.5-eps, 1.0,     1.0,     0,0,0); 
  pads[2] = new TPad("p3","", 0.0,     0.0,     0.5+eps, 0.5+eps, 0,0,0); 
  pads[3] = new TPad("p4","", 0.5-eps, 0.0,     1.0,     0.5+eps, 0,0,0); 

  pads[0]->SetLeftMargin(0.13); pads[0]->SetRightMargin(0.00);  pads[0]->SetBottomMargin(0.00); pads[0]->SetTopMargin(0.05);
  pads[1]->SetLeftMargin(0.00); pads[1]->SetRightMargin(0.02); pads[1]->SetBottomMargin(0.00); pads[1]->SetTopMargin(0.05);
  pads[2]->SetLeftMargin(0.13); pads[2]->SetRightMargin(0.00);  pads[2]->SetBottomMargin(0.12); pads[2]->SetTopMargin(0.00);
  pads[3]->SetLeftMargin(0.00); pads[3]->SetRightMargin(0.02); pads[3]->SetBottomMargin(0.12); pads[3]->SetTopMargin(0.00);

  for (int ip=0; ip<4; ++ip) pads[ip]->Draw();

  TFile *file[nBins][nBins-2];
  TFile *fSignal, *fBkg;
  TLegend *legend;

  for (Int_t iy=0; iy<nBins; ++iy) {
    for (Int_t ipt=0; ipt<nBins-3; ++ipt) {

      file[iy][ipt] = TFile::Open(Form("../Data_Preparation_wPID/Merge_Data/DzeroMassPlots_Student/DzeroInvMassCan_pTbin_%1.1f_%1.1f_%1.1f_%1.1f.root",
                                       y_arr[iy], y_arr[iy+1], pt_arr[ipt], pt_arr[ipt+1]));
      fSignal = TFile::Open(Form("../Data_Preparation_wPID/Merge_Data/merged_signal_y_D0_%1.1f_%1.1f_pt_D0_%1.1f_%1.1f.root",
                                 y_arr[iy], y_arr[iy+1], pt_arr[ipt], pt_arr[ipt+1]));
      fBkg = TFile::Open(Form("../Data_Preparation_wPID/Merge_Data/merged_bkg_y_D0_%1.1f_%1.1f_pt_D0_%1.1f_%1.1f.root",
                              y_arr[iy], y_arr[iy+1], pt_arr[ipt], pt_arr[ipt+1]));

      TH1F *hMass = (TH1F*) file[iy][ipt]->Get(Form("hMass_%1.1f_%1.1f_%1.1f_%1.1f",
                                                    y_arr[iy], y_arr[iy+1], pt_arr[ipt], pt_arr[ipt+1]));
      TH1D* hSampled_Sig = (TH1D*) fSignal->Get("hData_D0");
      TH1D* hSampled_Bkg = (TH1D*) fBkg->Get("hData_D0");

      TGraph *greff_signal = new TGraph(Form("../ML_Output_Optuna_%1.1f_%1.1f_%1.1f_%1.1f/bdt_efficiency_signal_vs_threshold.txt",
                                             y_arr[iy],y_arr[iy+1],pt_arr[ipt],pt_arr[ipt+1]),"%lg %lg");
      TGraph *greff_bkg = new TGraph(Form("../ML_Output_Optuna_%1.1f_%1.1f_%1.1f_%1.1f/bdt_efficiency_bkg_vs_threshold.txt",
                                          y_arr[iy],y_arr[iy+1],pt_arr[ipt],pt_arr[ipt+1]),"%lg %lg");

      int iloc = 0;
      for (int i =0; i<greff_signal->GetN(); ++i){
        double x,y;
        greff_signal->GetPoint(i,x,y);
        iloc = i;
        if (x>0.5999999) break;
      }

      double thr, sig_eff, bkg_eff;
      greff_signal->GetPoint(iloc,thr,sig_eff);
      greff_bkg->GetPoint(iloc,thr,bkg_eff);

      TF1 *fSig = (TF1*) file[iy][ipt]->Get("sigFunc");
      TF1 *fBkg = (TF1*) file[iy][ipt]->Get("bkgFunc");
      fSig->SetRange(1.6,2.5);
      fBkg->SetRange(1.6,2.5);

      TH1F *hMass_ML = (TH1F*)hMass->Clone("hSignal");
      hMass_ML->Reset();

      double signal_with_ML = sig_eff*hSampled_Sig->GetEntries();
      double bkg_with_ML    = bkg_eff*hSampled_Bkg->GetEntries();

      printf("-----------------Bin = %d --------------- \n",iy);
      printf("(Signal, Bkg)= (%f, %f) \n",hSampled_Sig->GetEntries(),hSampled_Bkg->GetEntries());
      printf("(sig_eff, bkg_eff)= (%f, %f) \n",sig_eff,bkg_eff);
      printf("(Signal with ML, Bkg with ML)= (%f, %f) \n",signal_with_ML,bkg_with_ML);

      for (int i =0; i<(int)signal_with_ML; ++i) hMass_ML->Fill(fSig->GetRandom(1.6,2.5));
      for (int i =0; i<(int)bkg_with_ML; ++i)    hMass_ML->Fill(fBkg->GetRandom(1.6,2.5));

      pads[iy]->cd();

      gPad->SetFillStyle(4000); 

      hMass->GetListOfFunctions()->Clear();
      hMass->SetMarkerColor(kBlack);
      hMass->SetLineColor(kBlack);
      hMass->SetMinimum(1.0e+4);
      hMass->SetMaximum(1.5e+7);
      hMass_ML->SetMinimum(1.0e+4);
      hMass->SetMarkerStyle(24);

      hMass->GetYaxis()->SetTitleOffset(1.60);
      hMass->SetTitle(";m_{#piK} (GeV/c^{2}); dN_{pair}/N_{events}");

      hMass_ML->SetLineColor(kRed);
      hMass_ML->SetMarkerColor(kRed);

      hMass->GetXaxis()->SetRangeUser(1.74,1.99);
      hMass_ML->GetXaxis()->SetRangeUser(1.74,1.99);

      gPad->SetLogy();

      if(norm_selfentries){
        hMass->Scale(1./hMass->Integral());
        hMass_ML->Scale(1./hMass_ML->Integral());
        hMass->SetMaximum(hMass->GetMaximum()*2.0);
        hMass->SetMinimum(hMass->GetMinimum()*0.5);
      }
      else if(norm_nEvents){
        hMass->Scale(1./nEvents);
        hMass_ML->Scale(1./nEvents);
        hMass->SetMaximum(hMass->GetMaximum()*5.0);
        hMass->SetMinimum(hMass->GetMinimum()*0.07);
      }

      hMass->SetTitleOffset(1.10,"XY");
      hMass_ML->SetTitleOffset(1.10,"XY");

      if (iy==0 ||iy==1) hMass->GetYaxis()->SetRangeUser(0.5e-6,4.0e-3);
      else hMass->GetYaxis()->SetRangeUser(0.2e-5,2.0e-3);
      
      if(iy==2 || iy==3){
        hMass->GetXaxis()->SetLabelSize(0.05);
        hMass->GetXaxis()->SetTitleSize(0.05);
        hMass->GetXaxis()->SetTitleOffset(1.10);
      }

      hMass->Draw("EP");
      hMass_ML->Draw("EP sames");

      int bin = hMass_ML->GetXaxis()->FindBin(1.86);
      printf("Entries = %f \t Bin Content = %1.6f \t Bin Error = %1.6f \n",
             hMass_ML->GetEntries(),hMass_ML->GetBinContent(bin),hMass_ML->GetBinError(bin));

      legend = new TLegend(0.65, 0.75, 0.90, 0.92);
      legend->SetTextSize(0.045);
      legend->SetTextFont(42);
      legend->SetHeader(Form("%1.1f < y < %1.1f",y_arr[iy],y_arr[iy+1]),"C");
      legend->AddEntry(hMass,"PID only", "p");
      legend->AddEntry(hMass_ML,"PID + Topo", "p");
      legend->Draw();
    }
  }

  pads[0]->cd();

  TLatex Text_ePIC;
  Text_ePIC.SetTextSize(0.055);
  Text_ePIC.SetTextFont(62);
  Text_ePIC.DrawLatexNDC(.18,.88,"ePIC Performance");

  TLatex Text_date;
  Text_date.SetTextSize(0.035);
  Text_date.SetTextFont(52);
  Text_date.DrawLatexNDC(.65,.96,"Simu campaign: 10/2025");

  TLatex Text_com;
  Text_com.SetTextSize(0.05);
  Text_com.SetTextAlign(13);
  Text_com.DrawLatexNDC(.18,.85,"e+Au, #sqrt{s} = 63 GeV");
  Text_com.DrawLatexNDC(.18,.78,"D^{0} #it{p}_{T} > 1 GeV/c");

  c[0]->SaveAs("JetsHF_Inv_mass_wo_wML_eAu.png");
  c[0]->SaveAs("JetsHF_Inv_mass_wo_wML_eAu.pdf");
  c[0]->SaveAs("JetsHF_Inv_mass_wo_wML_eAu.root");
}

