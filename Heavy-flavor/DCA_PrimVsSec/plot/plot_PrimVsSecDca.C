#include "ePIC_style.C"


//=====================
void plot_PrimVsSecDca(bool save_plot = false, bool add_epic_logo = false)
{
  // ===== Set the style =====
  gROOT->ProcessLine("set_ePIC_style()");
  gStyle->SetTitleYOffset(1.4);

  TString coll_sys = "ep";

  TFile *fin = TFile::Open(Form("ep_10x100_minQ2_1_D0_25.10.3.root"), "read");

  const char* part_name = "Pi";
  const char* part_title = "#pi";
  const char* dca_name = "xy";
  const char* vtx_name = "MC";
  const char* trk_name[2] = {"Prim", "Sec"};
  const char* trk_title[2] = {"Primary", "Secondary"};
  
  const int nPtBins = 2;
  const double ptBinsLow[nPtBins] =  {0.5, 1.0};
  const double ptBinsHigh[nPtBins] = {1.0, 1.5};
  const int nEtaBins = 4;
  const double etaBinsLow[nEtaBins] =  {-2.0, -1.0, 1.0, 2.0};
  const double etaBinsHigh[nEtaBins] = {-1.0, 1.0, 2.0, 3.0};
  TH3F *h3RcPartDca[2];
  TH1F *h1RcPartDca[2][nPtBins][nEtaBins];
  for(int i=0; i<2; i++)
    {
      THnSparseF *hn = (THnSparseF*)fin->Get(Form("h%s%sDCA%sTo%sVtx", trk_name[i], part_name, dca_name, vtx_name));
      h3RcPartDca[i] = (TH3F*)hn->Projection(0,1,2);
      for(int p=0; p<nPtBins; p++)
	{
	  for(int e=0; e<nEtaBins; e++)
	    {
	      int xbin1 = h3RcPartDca[i]->GetXaxis()->FindBin(ptBinsLow[p]+1e-4);
	      int xbin2 = h3RcPartDca[i]->GetXaxis()->FindBin(ptBinsHigh[p]-1e-4);
	      int ybin1 = h3RcPartDca[i]->GetYaxis()->FindBin(etaBinsLow[e]+1e-4);
	      int ybin2 = h3RcPartDca[i]->GetYaxis()->FindBin(etaBinsHigh[e]-1e-4);
	      h1RcPartDca[i][p][e] = (TH1F*)h3RcPartDca[i]->ProjectionZ(Form("%s_pt%d_eta%d",h3RcPartDca[i]->GetName(),p,e),xbin1,xbin2,ybin1,ybin2);
	    }
	}
    }

  // compare primary vs. secondary
  TCanvas *c = new TCanvas(Form("c%sDCA%s_SecVsPrim",part_name,dca_name), Form("c%sDCA%s_SecVsPrim",part_name,dca_name), 1200, 600);
  c->Divide(nEtaBins, 2);
  
  TLegend *leg = new TLegend(0.17, 0.75, 0.37, 0.9);
  leg->SetTextSize(0.045);
  leg->SetHeader("Pion");
  for(int p=0; p<nPtBins; p++)
    {
      for(int e=0; e<nEtaBins; e++)
	{
	  c->cd(p*nEtaBins+e+1);
	  //gPad->SetBottomMargin(0.15);
	  for(int i=0; i<2; i++)
	    {
	      TH1F *h1plot = (TH1F*)h1RcPartDca[i][p][e]->Clone(Form("%s_plot_SecVsPrim",h1RcPartDca[i][p][e]->GetName()));
	      if(h1plot->GetEntries()<2) continue;
	      h1plot->Scale(1./h1plot->GetEntries());
	      h1plot->SetMaximum(1.25*h1plot->GetMaximum());
	      h1plot->GetXaxis()->SetRangeUser(-0.5, 0.5);

	      h1plot->SetTitle(Form(";DCA_{%s} (mm);Probability density", dca_name));
	      h1plot->SetLineColor(i+1);
	      h1plot->SetLineStyle(2-i);
	      h1plot->GetYaxis()->SetNdivisions(505);
	      if(i==0) h1plot->Draw("hist");
	      else     h1plot->Draw("sames hist");
	      if(p==0 && e==0)
		{
		  leg->AddEntry(h1plot, trk_title[i], "L");
		}
	    }
	  TLatex text;
	  text.SetTextSize(0.045);
	  text.SetTextAlign(13);  //align at top
	  text.DrawLatexNDC(.54,.9,Form("%1.1f < p_{T} < %1.1f GeV/c", ptBinsLow[p], ptBinsHigh[p])); 
	  text.DrawLatexNDC(.54,.84,Form("%1.1f < #eta < %1.1f", etaBinsLow[e], etaBinsHigh[e]));
	}
    }
  c->cd(2);
  leg->Draw();

  c->cd(1);
  TLatex Text_com;
  Text_com.SetTextAlign(13);  //align at top
  Text_com.DrawLatexNDC(.15,.85,"e+p, #sqrt{s} = 63 GeV"); // 18x275: 141 GeV; 10x100: 63 GeV; 5x41: 29 GeV
  //Text_com.DrawLatexNDC(.15,.8,"L_{proj} = 10 fb^{-1}");
  // Text_com.DrawLatexNDC(.15,.99,"MM/YYYY");  // performance plot

  TLatex Text_date;
   Text_date.SetTextSize(0.04);
   Text_date.SetTextFont(52);
   Text_date.DrawLatexNDC(.5,.96,"Simu campaign: 10/2025");  // performance plot

   TLatex Text_ePIC;
   Text_ePIC.SetTextSize(0.045);
   Text_ePIC.SetTextFont(62);
   Text_ePIC.DrawLatexNDC(.15,.88,"ePIC Performance");  // performance plot
   //Text_ePIC.DrawLatexNDC(.15,.88,"ePIC Internal");  // for internal use only
   //Text_ePIC.DrawLatexNDC(.15,.88,"ePIC Preliminary"); // preliminary released version 
   //Text_ePIC.DrawLatexNDC(.15,.88,"ePIC Work in Progress"); // work in progress to be shown outside
   //Text_ePIC.DrawLatexNDC(.15,.88,"ePIC"); // final published version



   if(add_epic_logo)
     {
       // ===== Add ePIC logo to the figure if desired ======
       TImage *logo = TImage::Open("EPIC-logo_black_small.png");
       TPad *pad2 = new TPad("pad2", "Pad 2", 0.8, 0.8, 0.93, 0.93); // Create a new pad and then draw the image in it
       pad2->Draw();
       pad2->cd(); // Enter the new pad
       logo->Draw();
     }

   if(save_plot)
     {
       c->SaveAs(Form("figs/JetsHF_%sDCA%sToMCVtx.pdf",part_name,dca_name));
     }
}
