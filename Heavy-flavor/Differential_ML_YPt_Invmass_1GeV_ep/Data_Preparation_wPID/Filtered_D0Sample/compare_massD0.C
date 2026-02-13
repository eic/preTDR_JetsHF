// Macro to plot Invariant mass of D0 as a function of Ponting Angle
// Shyam Kumar; INFN Bari, shyam.kumar@ba.infn.it

void compare_massD0(){
	
	gStyle->SetPalette(kRainBow);
	gStyle->SetTitleSize(0.045,"XY");	
	gStyle->SetTitleSize(0.04,"XY");	
	gStyle->SetLabelSize(0.04,"XY");	
	gStyle->SetTitleOffset(1.0,"XY");	
	gStyle->SetOptStat(1);
	gStyle->SetOptFit(1);
	gStyle->SetOptTitle(1);
	gStyle->SetGridColor(kBlack);     
	gStyle->SetGridWidth(2);        
	gStyle->SetGridStyle(2);
	
	TString preselection ="(mass_D0 > 1.6 && mass_D0 < 2.5) && (d0xy_pi>0.02 && d0xy_pi<10.) && (d0xy_k>0.02 && d0xy_k<10.) && decay_length <100.";
     TFile *f_sig = TFile::Open("../../../../D0_Jan25_Signal/HF_reco/helix/SignalD0.root");	
     TTree *t_sig = (TTree*)f_sig->Get("treeMLSig");
	t_sig->Draw("mass_D0>>h0(100,1.6,2.5)",Form("%s",preselection.Data()));
	t_sig->Draw("mass_D0>>h1(100,1.6,2.5)",Form("%s && costheta>0.95",preselection.Data()));
	t_sig->Draw("mass_D0>>h2(100,1.6,2.5)",Form("%s && costheta>0.99",preselection.Data()));
	const int nplots = 2;
 
  	TCanvas *c[nplots]; 
	for (int i=0; i<nplots; ++i){
	c[i]= new TCanvas(Form("c%d",i),Form("c%d",i),0,52,1400,1000);
	c[i]->SetGrid();
	c[i]->SetMargin(0.10, 0.03 ,0.12,0.07);
	} 
	
	 c[0]->SetLogy();
	 c[0]->cd();
	 TH1D *h0 = (TH1D*)gDirectory->Get("h0");
      h0->SetLineColor(kBlue);
      h0->SetLineStyle(1);
      h0->SetMaximum(h0->GetMaximum()*10.);
      h0->SetTitle(";Signal m_{D^{0}} (GeV/c^{2}); Entries (a.u.)");
      h0->SetLineWidth(2);
      h0->Draw("hist");
      c[0]->Modified(); c[0]->Update();
      TPaveStats *stats = (TPaveStats*)	h0->FindObject("stats"); // First need to update to make sure stats is there
      stats->SetTextColor(kBlue);
      
      TH1D *h1 = (TH1D*)gDirectory->Get("h1");
      h1->SetLineColor(kOrange+1);
      h1->SetLineStyle(1);
      h1->SetLineWidth(2);
      h1->Draw("sames");
      c[0]->Modified(); c[0]->Update();
      stats = (TPaveStats*)	h1->FindObject("stats"); // First need to update to make sure stats is there
      stats->SetTextColor(kOrange+1);
	 stats->SetY1NDC(stats->GetY1NDC()-0.23); 
	 stats->SetY2NDC(stats->GetY2NDC()-0.23);	
	 c[0]->Modified();
	 c[0]->Update();
	 
	 TH1D *h2 = (TH1D*)gDirectory->Get("h2");
	 h2->SetLineColor(kGreen+2);
      h2->SetLineWidth(2);	 
      h2->SetLineStyle(1);
      h2->Draw("sames");
      c[0]->Modified(); c[0]->Update();
      stats = (TPaveStats*)	h2->FindObject("stats"); // First need to update to make sure stats is there
      stats->SetTextColor(kGreen+2);
	 stats->SetY1NDC(stats->GetY1NDC()-0.46); 
	 stats->SetY2NDC(stats->GetY2NDC()-0.46);	
	 c[0]->Modified();
	 c[0]->Update();

      TLegend *lmass = new TLegend(0.15,0.75,0.30,0.92);
      lmass->SetHeader("Signal Invaraint mass","C");
      lmass->SetTextSize(0.025);
      lmass->SetBorderSize(0);
      lmass->AddEntry(h0,"No cos#theta cut");
      lmass->AddEntry(h1,"cos#theta>0.95");
      lmass->AddEntry(h2,"cos#theta>0.99");
      lmass->Draw();
      c[0]->SaveAs("Cmp_Signal.png");
      
      TFile *f_bkg = TFile::Open("../../../../D0_Jan25_Signal/HF_reco/helix/BkgD0.root");	
      TTree *t_bkg = (TTree*)f_bkg->Get("treeMLBkg");
      
      h0->Delete(); h1->Delete(); h2->Delete();
      
	 t_bkg->Draw("mass_D0>>h0(100,1.6,2.5)",Form("%s",preselection.Data()));
	 t_bkg->Draw("mass_D0>>h1(100,1.6,2.5)",Form("%s && costheta<0.99",preselection.Data()));
	 t_bkg->Draw("mass_D0>>h2(100,1.6,2.5)",Form("%s && costheta<0.95",preselection.Data()));
      
      c[1]->cd();
      c[1]->SetLogy();
      h0 = (TH1D*)gDirectory->Get("h0");
      h0->SetLineColor(kBlue);
      h0->SetLineStyle(1);
      h0->SetMinimum(4000.);
      h0->SetMaximum(12000.);
      h0->SetTitle(";Background m_{D^{0}} (GeV/c^{2}); Entries (a.u.)");
      h0->SetLineWidth(2);
      h0->Draw("hist");
      c[1]->Modified(); c[1]->Update();
      stats = (TPaveStats*)	h0->FindObject("stats"); // First need to update to make sure stats is there
      stats->SetTextColor(kBlue);
      
      h1 = (TH1D*)gDirectory->Get("h1");
      h1->SetLineColor(kOrange+1);
      h1->SetLineStyle(1);
      h1->SetLineWidth(2);
      h1->Draw("sames");
      c[1]->Modified(); c[1]->Update();
      stats = (TPaveStats*)	h1->FindObject("stats"); // First need to update to make sure stats is there
      stats->SetTextColor(kOrange+1);
	 stats->SetY1NDC(stats->GetY1NDC()-0.23); 
	 stats->SetY2NDC(stats->GetY2NDC()-0.23);	
	 c[1]->Modified();
	 c[1]->Update();
	 
	 h2 = (TH1D*)gDirectory->Get("h2");
	 h2->SetLineColor(kGreen+2);
      h2->SetLineWidth(2);	 
      h2->SetLineStyle(1);
      h2->Draw("sames");
      c[1]->Modified(); c[1]->Update();
      stats = (TPaveStats*)	h2->FindObject("stats"); // First need to update to make sure stats is there
      stats->SetTextColor(kGreen+2);
	 stats->SetY1NDC(stats->GetY1NDC()-0.46); 
	 stats->SetY2NDC(stats->GetY2NDC()-0.46);	
	 c[1]->Modified();
	 c[1]->Update();

      lmass = new TLegend(0.65,0.75,0.75,0.92);
      lmass->SetHeader("Bkg Invaraint mass","C");
      lmass->SetTextSize(0.025);
      lmass->SetBorderSize(0);
      lmass->AddEntry(h0,"No cos#theta cut");
      lmass->AddEntry(h1,"cos#theta<0.99");
      lmass->AddEntry(h2,"cos#theta<0.95");
      lmass->Draw();
      c[1]->SaveAs("Cmp_Bkg.png");


}   



















