// Macro to plot the canvas with no spaces
// Shyam Kumar; INFN Bari, shyam.kumar@ba.infn.it
#include "ePIC_style.C"

void Compare_SV_ePIC(TString histtitle="x_{Rec}^{SV} - x_{Mc}^{SV} (mm)"){

    gROOT->ProcessLine("set_ePIC_style()");
    gStyle->SetOptStat(0);
    gStyle->SetOptFit(0);
    gStyle->SetOptTitle(0);
    gStyle->SetTitleSize(0.05,"XY");
    gStyle->SetLabelSize(0.05,"XY");
    gStyle->SetMarkerSize(1.0);

    const Int_t nPtbins = 4;
    const Int_t nYbins = 5;
    double pt[nPtbins]={0.,1.,2.,5.};
    double y[nYbins]={-2.0,-1.0,1.0,2.0, 3.5};

    TFile *f1 = TFile::Open("test.root");
    TH3F *hSVx = (TH3F*)f1->Get("hRes_SVx_Helixfit");
    TH3F *hSVy = (TH3F*)f1->Get("hRes_SVy_Helixfit");

    TF1 *fDG = new TF1("fDG","[0]*exp(-0.5*((x-[1])/[2])^2) + [3]*exp(-0.5*((x-[1])/[4])^2)",-0.2, 0.2);
    fDG->SetParNames("A_{core}","mean","sigma_{core}","A_{tail}","sigma_{tail}");
    fDG->SetParameters(0.11,0.0,0.1,0.05,0.1);
    fDG->SetLineWidth(1);

    const int nplots = 1;
    double arr[2];
    TCanvas *c[nplots];
    for (int i=0; i<nplots; ++i){
        c[i]= new TCanvas(Form("c%d",i),Form("c%d",i),1400,1000);
        c[i]->SetGrid();
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

    pads[0]->SetLeftMargin(0.12); pads[0]->SetRightMargin(0.00); pads[0]->SetBottomMargin(0.00); pads[0]->SetTopMargin(0.05);
    pads[1]->SetLeftMargin(0.00); pads[1]->SetRightMargin(0.02); pads[1]->SetBottomMargin(0.00); pads[1]->SetTopMargin(0.05);
    pads[2]->SetLeftMargin(0.12); pads[2]->SetRightMargin(0.00); pads[2]->SetBottomMargin(0.14); pads[2]->SetTopMargin(0.00);
    pads[3]->SetLeftMargin(0.00); pads[3]->SetRightMargin(0.02); pads[3]->SetBottomMargin(0.14); pads[3]->SetTopMargin(0.00);

    for (int ip=0; ip<4; ++ip) pads[ip]->Draw();
    TLegend *legend, *legend1;

    for (int i =0; i < nYbins-1; ++i){

        legend = new TLegend(0.60, 0.68, 0.90, 0.93);
        legend->SetTextSize(0.045);
        legend->SetTextFont(42);
        legend1 = new TLegend(0.70, 0.65, 0.90, 0.90);
        legend1->SetTextSize(0.045);
        legend1->SetTextFont(42);

        int bin_eta_min = hSVx->GetYaxis()->FindBin(y[i]+0.0001);
        int bin_eta_max = hSVx->GetYaxis()->FindBin(y[i+1]-0.0001);

        int bin_pt_min = hSVx->GetXaxis()->FindBin(pt[0]+0.0001);
        int bin_pt_max = hSVx->GetXaxis()->FindBin(pt[1]-0.0001);

        TH1D *h1_SVx = (TH1D*) hSVx->ProjectionZ(Form("hSig_eta_%1.1f_%1.1f_pt_%1.1f_%1.1f_px",y[i],y[i+1],pt[0],pt[1]),
                                                  bin_pt_min, bin_pt_max, bin_eta_min, bin_eta_max,"o");
        h1_SVx->SetTitle(Form("hSig_eta_%1.1f_%1.1f_pt_%1.1f_%1.1fGeV/c",y[i],y[i+1],pt[0],pt[1]));
        h1_SVx->SetLineColor(kBlack);
        h1_SVx->SetMarkerColor(kBlack);

        TH1D *h1_SVy = (TH1D*) hSVy->ProjectionZ(Form("hSig_eta_%1.1f_%1.1f_pt_%1.1f_%1.1f_py",y[i],y[i+1],pt[0],pt[1]),
                                                  bin_pt_min, bin_pt_max, bin_eta_min, bin_eta_max,"o");
        h1_SVy->SetTitle(Form("hSig_eta_%1.1f_%1.1f_pt_%1.1f_%1.1fGeV/c",y[i],y[i+1],pt[0],pt[1]));
        h1_SVy->SetLineColor(kRed);
        h1_SVy->SetMarkerColor(kRed);

        bin_pt_min = hSVx->GetXaxis()->FindBin(pt[2]+0.0001);
        bin_pt_max = hSVx->GetXaxis()->FindBin(pt[3]-0.0001);

        TH1D *h3_SVx = (TH1D*) hSVx->ProjectionZ(Form("hSig_eta_%1.1f_%1.1f_pt_%1.1f_%1.1f_px",y[i],y[i+1],pt[2],pt[3]),
                                                  bin_pt_min, bin_pt_max, bin_eta_min, bin_eta_max,"o");
        h3_SVx->SetTitle(Form("hSig_eta_%1.1f_%1.1f_pt_%1.1f_%1.1fGeV/c",y[i],y[i+1],pt[2],pt[3]));
        h3_SVx->SetLineColor(kBlue);
        h3_SVx->SetMarkerColor(kBlue);

        TH1D *h3_SVy = (TH1D*) hSVy->ProjectionZ(Form("hSig_eta_%1.1f_%1.1f_pt_%1.1f_%1.1f_py",y[i],y[i+1],pt[2],pt[3]),
                                                  bin_pt_min, bin_pt_max, bin_eta_min, bin_eta_max,"o");
        h3_SVy->SetTitle(Form("hSig_eta_%1.1f_%1.1f_pt_%1.1f_%1.1fGeV/c",y[i],y[i+1],pt[2],pt[3]));
        h3_SVy->SetLineColor(kGreen+3);
        h3_SVy->SetMarkerColor(kGreen+3);

        pads[i]->cd();
        h1_SVx->GetXaxis()->SetRangeUser(-0.49,0.49);
        h1_SVy->GetXaxis()->SetRangeUser(-0.49,0.49);
        h3_SVx->GetXaxis()->SetRangeUser(-0.49,0.49);
        h3_SVy->GetXaxis()->SetRangeUser(-0.49,0.49);

        h1_SVx->SetTitle(Form(";%s; Probability density",histtitle.Data()));
        h1_SVx->SetLineWidth(1);
        h1_SVy->SetLineWidth(1);
        h3_SVx->SetLineWidth(1);
        h3_SVy->SetLineWidth(1);

        h1_SVx->Scale(1./h1_SVx->Integral());
        h1_SVy->Scale(1./h1_SVy->Integral());
        h3_SVx->Scale(1./h3_SVx->Integral());
        h3_SVy->Scale(1./h3_SVy->Integral());

        h1_SVx->SetMarkerStyle(20);
        h1_SVy->SetMarkerStyle(24);
        h3_SVy->SetMarkerStyle(25);

        double max = h1_SVx->GetMaximum();
        if (i==0 || i==1) h1_SVx->SetMaximum(0.17);
        else h1_SVx->SetMaximum(0.11);
        h1_SVx->SetMinimum(0.00001);

        h1_SVx->Draw("EP");
        if (i==2 || i==3){
        h1_SVx->SetTitleSize(0.05,"XY");
        h1_SVx->SetLabelSize(0.05,"XY");	
        } 

        fDG = new TF1("fDG","[0]*exp(-0.5*((x-[1])/[2])^2) + [3]*exp(-0.5*((x-[1])/[4])^2)",
                      h1_SVx->GetMean()-2.0*h1_SVx->GetStdDev(), h1_SVx->GetMean()+2.0*h1_SVx->GetStdDev());
        fDG->SetParNames("A_{core}","mean","sigma_{core}","A_{tail}","sigma_{tail}");
        fDG->SetParameters(max,h1_SVx->GetMean(),h1_SVx->GetStdDev(),0.5*max,h1_SVx->GetStdDev());
        printf("Maximum = %f\n",max);

        fDG->SetParLimits(0, 0.50*max, 1.5*max);
        fDG->SetParLimits(1, -0.01, 0.01);
        fDG->SetParLimits(2, 0.20*h1_SVx->GetStdDev(), 1.05*h1_SVx->GetStdDev());
        fDG->SetParLimits(4, 0.50*h1_SVx->GetStdDev(), 3.0*h1_SVx->GetStdDev());

        h1_SVx->Fit(fDG,"QR0");
        h1_SVx->Fit(fDG,"R0+");
        fDG->SetLineWidth(1);
        fDG->Draw("same");

        legend1->SetHeader(Form("%1.1f < y < %1.1f",y[i],y[i+1]),"C");
        double sigma_core = fDG->GetParameter(2)*1000;
        double sigma_tail = fDG->GetParameter(4)*1000;
        double w_core = fDG->GetParameter(0)/(fDG->GetParameter(0)+fDG->GetParameter(3));
        double w_tail = fDG->GetParameter(3)/(fDG->GetParameter(0)+fDG->GetParameter(3));
        double sigma_eff = sqrt(w_core*sigma_core*sigma_core+w_tail*sigma_tail*sigma_tail);
        legend1->AddEntry(fDG,Form("#sigma_{eff} (#Deltax) = %1.0f #mum",sigma_eff),"l");

        h1_SVy->Draw("EP-sames");
        h3_SVx->Draw("EP-sames");

        fDG = new TF1("fDG","[0]*exp(-0.5*((x-[1])/[2])^2) + [3]*exp(-0.5*((x-[1])/[4])^2)",
                      h3_SVx->GetMean()-2.0*h3_SVx->GetStdDev(), h3_SVx->GetMean()+2.0*h3_SVx->GetStdDev());
        fDG->SetParNames("A_{core}","mean","sigma_{core}","A_{tail}","sigma_{tail}");
        fDG->SetParameters(h3_SVx->GetMaximum(),h3_SVx->GetMean(),h3_SVx->GetStdDev(),0.5*h3_SVx->GetMaximum(),h3_SVx->GetStdDev());

        if (i==0) fDG->SetParLimits(0, 0.65*h3_SVx->GetMaximum(), 1.10*h3_SVx->GetMaximum());
        else      fDG->SetParLimits(0, 0.70*h3_SVx->GetMaximum(), 1.10*h3_SVx->GetMaximum());

        fDG->SetParLimits(1, -0.01, 0.01);
        fDG->SetParLimits(2, 0.25*h3_SVx->GetStdDev(), 1.05*h3_SVx->GetStdDev());
        fDG->SetParLimits(4, 0.50*h3_SVx->GetStdDev(), 3.0*h3_SVx->GetStdDev());

        h3_SVx->Fit(fDG,"QR0");
        h3_SVx->Fit(fDG,"R0+");
        fDG->SetLineColor(kBlue);
        fDG->SetLineWidth(1);
        fDG->Draw("same");

        sigma_core = fDG->GetParameter(2)*1000;
        sigma_tail = fDG->GetParameter(4)*1000;
        w_core = fDG->GetParameter(0)/(fDG->GetParameter(0)+fDG->GetParameter(3));
        w_tail = fDG->GetParameter(3)/(fDG->GetParameter(0)+fDG->GetParameter(3));
        sigma_eff = sqrt(w_core*sigma_core*sigma_core+w_tail*sigma_tail*sigma_tail);
        legend1->AddEntry(fDG,Form("#sigma_{eff} (#Deltax) = %1.0f #mum",sigma_eff),"l");

        h3_SVy->Draw("EP-sames");

        if (i==1){
            legend = new TLegend(0.02, 0.68, 0.30, 0.93);
            legend->AddEntry(h1_SVx, Form("#Deltax^{SV} (%1.0f < p_{T} < %1.0f GeV/c)",pt[0], pt[1]), "p");
            legend->AddEntry(h1_SVy, Form("#Deltay^{SV} (%1.0f < p_{T} < %1.0f GeV/c)",pt[0], pt[1]), "p");
            legend->AddEntry(h3_SVx, Form("#Deltax^{SV} (%1.0f < p_{T} < %1.0f GeV/c)",pt[2], pt[3]), "p");
            legend->AddEntry(h3_SVy, Form("#Deltay^{SV} (%1.0f < p_{T} < %1.0f GeV/c)",pt[2], pt[3]), "p");
        }

        legend->Draw();
        legend1->Draw();
    }

    // Your text block: draw it in the TOP-LEFT pad (same effect as your old c->cd(1))
    pads[0]->cd();

    TLatex Text_com;
    Text_com.SetTextAlign(13);
    Text_com.SetTextSize(0.055);
    Text_com.DrawLatexNDC(.17,.85,"e+p, #sqrt{s} = 63 GeV");
    Text_com.DrawLatexNDC(.20,.78,"D^{0}#rightarrowK#pi");

    TLatex Text_date;
    Text_date.SetTextSize(0.04);
    Text_date.SetTextFont(52);
    Text_date.DrawLatexNDC(.65,0.96,"Simu campaign: 10/2025");

    TLatex Text_ePIC;
    Text_ePIC.SetTextSize(0.06);
    Text_ePIC.SetTextFont(62);
    Text_ePIC.DrawLatexNDC(.17,.88,"ePIC Performance");

    c[0]->SaveAs("JetsHF_SVxy_Resolution.png");
    c[0]->SaveAs("JetsHF_SVxy_Resolution.pdf");
}

