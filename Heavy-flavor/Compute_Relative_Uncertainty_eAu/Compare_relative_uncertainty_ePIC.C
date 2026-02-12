// Code to draw S/B and Significance as a function of BDT Threshold
// Shyam Kumar; INFN Bari; shyam055119@gmail.com; shyam.kumar@ba.infn.it 
#include <TCanvas.h>
#include <TH2F.h>
#include "ePIC_style.C"
void Compare_relative_uncertainty_ePIC(){
	gROOT->ProcessLine("set_ePIC_style()");
	gStyle->SetEndErrorSize(10); // length in pixels
	gStyle->SetTitleOffset(1.30,"XY"); // length in pixels
  gStyle->SetTitleSize(0.045,"XY");
	
   TFile *fout = new TFile("cmp_ep_eAu.root","recreate"); 	

   TCanvas *c = new TCanvas("c","c",2000,700);
   c->Divide(3,1,0,0);

   float pt[5] = {0.0, 1.0, 2.0, 5.0, 10.0};
   float pterror[4] = {0.5,0.5, 1.5, 2.5};

   TMultiGraph *mg = new TMultiGraph("mg",";#it{p}_{T} (GeV/c);Relative Uncertainty (R_{eAu})");
   // Graph1 with rectangular cuts 
   TGraphErrors *gr_y1 = new TGraphErrors("uncertainties_-2.0_-1.0.txt","%*lg %lg %lg");
   TGraphErrors *gr_y1_ML = new TGraphErrors("uncertainties_-2.0_-1.0_ML.txt","%*lg %lg %lg");
   // Graph 2,3 are with ML cuts    
   TGraphErrors *gr_y2 = new TGraphErrors("uncertainties_-1.0_1.0.txt","%*lg %lg %lg");
   TGraphErrors *gr_y2_ML = new TGraphErrors("uncertainties_-1.0_1.0_ML.txt","%*lg %lg %lg");
   
   TGraphErrors *gr_y3 = new TGraphErrors("uncertainties_1.0_3.0.txt","%*lg %lg %lg");
   TGraphErrors *gr_y3_ML = new TGraphErrors("uncertainties_1.0_3.0_ML.txt","%*lg %lg %lg");
   
  
   // Canvas 1
   for (int i = 0; i<gr_y1->GetN(); ++i){
   double x, y;
   gr_y1->GetPoint(i,x,y);
   gr_y1->SetPoint(i,0.5*(pt[i]+pt[i+1]),1.0);	
   gr_y1->SetPointError(i,pterror[i],y);	   	
   } 
   
   for (int i = 0; i<gr_y1_ML->GetN(); ++i){
   double x, y;
   gr_y1_ML->GetPoint(i,x,y);
   gr_y1_ML->SetPoint(i,0.5*(pt[i]+pt[i+1]),1.0);	
   gr_y1_ML->SetPointError(i,pterror[i],y);	   	
   } 
   gr_y1_ML->SetLineColor(kBlue);
   gr_y1_ML->SetMarkerColor(kBlue);
   gr_y1_ML->SetMarkerStyle(20); 
   
   c->cd(1);
   gPad->SetTopMargin(0.05);

   gr_y1->SetLineColor(kBlack);
  // mg->Add(gr_y1);
   mg->Add(gr_y1_ML);
   mg->GetXaxis()->SetRangeUser(-0.1,5.9);
   mg->GetYaxis()->SetRangeUser(0.95, 1.07);  
   mg->Draw("AP");
   TLegend *legend = new TLegend(0.72, 0.80, 0.90, 0.90);
   legend->SetTextSize(0.041); 
   legend ->SetHeader("-2 < y < -1","C");
//   legend -> AddEntry(gr_y1,"Std Cuts", "p");
   legend -> AddEntry(gr_y1_ML,"PID + Topo", "p");   
   legend ->Draw();
   TLatex Text_com;
   Text_com.SetTextAlign(13);  //align at top
   Text_com.SetTextSize(0.035);
   Text_com.DrawLatexNDC(.16,.86,"e+p, e+Au, #sqrt{s} = 63 GeV");
   Text_com.DrawLatexNDC(.16,.81,"L_{proj}^{ep} = 10 fb^{-1}, L_{proj}^{eAu} = 10 fb^{-1}");

   // Add dates: needed for performance plots
   TLatex Text_date;
   Text_date.SetTextSize(0.035);
   Text_date.SetTextFont(52);
   Text_date.DrawLatexNDC(.65,.97,"Simu campaign: 10/2025");  // performance plot

   TLatex Text_ePIC;
   Text_ePIC.SetTextSize(0.05);
   Text_ePIC.SetTextFont(62);
   Text_ePIC.DrawLatexNDC(.16,.88,"ePIC Performance");  // performance plot
   
  // Canvas 2
   mg = new TMultiGraph("mg",";#it{p}_{T} (GeV/c);Relative Uncertainty (R_{eAu})");
   for (int i = 0; i<gr_y2->GetN(); ++i){
   double x, y;
   gr_y2->GetPoint(i,x,y);
   gr_y2->SetPoint(i,0.5*(pt[i]+pt[i+1]),1.0);	
   gr_y2->SetPointError(i,pterror[i],y);	   	
   } 
   c->cd(2);
   gPad->SetTopMargin(0.05);
   gr_y2->SetLineColor(kRed);
   gr_y2->SetMarkerColor(kRed);

   for (int i = 0; i<gr_y2_ML->GetN(); ++i){
   double x, y;
   gr_y2_ML->GetPoint(i,x,y);
   gr_y2_ML->SetPoint(i,0.5*(pt[i]+pt[i+1]),1.0);	
   gr_y2_ML->SetPointError(i,pterror[i],y);	   	
   } 
   gr_y2_ML->SetLineColor(kBlue);
   gr_y2_ML->SetMarkerColor(kBlue);
   gr_y2_ML->SetMarkerStyle(20); 
  // mg->Add(gr_y2);
   mg->Add(gr_y2_ML); 
   mg->GetXaxis()->SetRangeUser(-0.1,10.5);
   mg->GetYaxis()->SetRangeUser(0.95, 1.07);  
   mg->Draw("AP");
   legend = new TLegend(0.70, 0.80, 0.90, 0.90);
   legend->SetTextSize(0.043); 
   legend ->SetHeader("-1 < y < 1","C");
  // legend -> AddEntry(gr_y2,"Wo ML", "p");
   legend -> AddEntry(gr_y2_ML,"PID + Topo", "p");
   legend ->Draw();

    // Canvas 3
   mg = new TMultiGraph("mg",";#it{p}_{T} (GeV/c);Relative Uncertainty (R_{eAu})");
   for (int i = 0; i<gr_y3->GetN(); ++i){
   double x, y;
   gr_y3->GetPoint(i,x,y);
   gr_y3->SetPoint(i,0.5*(pt[i]+pt[i+1]),1.0);	
   gr_y3->SetPointError(i,pterror[i],y);	   	
   } 
   c->cd(3);
   gPad->SetTopMargin(0.05);
   gPad->SetRightMargin(0.01);
   gr_y3->SetLineColor(kBlue);
   gr_y3->SetMarkerColor(kBlue);
   
   for (int i = 0; i<gr_y3_ML->GetN(); ++i){
   double x, y;
   gr_y3_ML->GetPoint(i,x,y);
   gr_y3_ML->SetPoint(i,0.5*(pt[i]+pt[i+1]),1.0);	
   gr_y3_ML->SetPointError(i,pterror[i],y);	   	
   } 
   gr_y3_ML->SetLineColor(kBlue);
   gr_y3_ML->SetMarkerColor(kBlue);
   gr_y3_ML->SetMarkerStyle(20); 
  // mg->Add(gr_y3);
   mg->Add(gr_y3_ML); 
   mg->GetXaxis()->SetRangeUser(-0.1,10.5);
   mg->GetYaxis()->SetRangeUser(0.95, 1.07);  
   mg->Draw("AP");
   legend = new TLegend(0.70, 0.80, 0.90, 0.90);
   legend->SetTextSize(0.043); 
   legend ->SetHeader("1 < y < 3","C");
 //  legend -> AddEntry(gr_y3,"Wo ML", "p");
   legend -> AddEntry(gr_y3_ML,"PID + Topo", "p");
   legend ->Draw(); 
   gr_y1_ML->Print(); 
   gr_y2_ML->Print();
   gr_y3_ML->Print();
    c->SaveAs("JetsHF_Statistcial_Projection_ReAu.png");
    c->SaveAs("JetsHF_Statistcial_Projection_ReAu.pdf");
    fout->cd();
    mg->Write();
    fout->Close();

}
