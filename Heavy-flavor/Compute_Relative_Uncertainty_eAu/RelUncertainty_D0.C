// Code to evaluate the relative statistical uncertainty on Lc+/D0 ratio
// Shyam Kumar; INFN Bari, shyam.kumar@ba.infn.it 

void RelUncertainty_D0(float ymin = -1.0, float ymax = 1.0, float ptmin = 1.0, float ptmax = 2.0)
{
 
  ofstream myfile;	
  
  myfile.open (Form("uncertainties_%1.1f_%1.1f.txt",ymin,ymax),ios_base::app); // ,ios_base::app Edit mode 	
  
   ofstream myfile1;	  
   myfile1.open("RelUncertainty_summary.txt",ios_base::app); // ,ios_base::app Edit mode 
  // --- Open files ---
 // TFile *fLc = TFile::Open(Form("../Lc_Plus_Reconstruction_Code/ML_model_ePIC_Shyam_Final/Data_Preparation/Merge_Data/LcpMassPlots_Student/LcpInvMassFit_pTbin_%1.1f_%1.1f_%1.1f_%1.1f.root",ymin,ymax,ptmin,ptmax));
 // TFile *fD0 = TFile::Open(Form("../D0_Jan25_ML_Q2_1/ML_model_ePIC_Shyam_Final/Differential_ML_YPt/Data_Preparation/Merge_Data/DzeroMassPlots_Student/DzeroInvMassCan_pTbin_%1.1f_%1.1f_%1.1f_%1.1f.root",ymin,ymax,ptmin,ptmax));
  TFile *fD0_eAu = TFile::Open(Form("../D0_Jan25_ML_Q2_1_eAu/ML_model_ePIC_Shyam_Final/Differential_ML_YPt/Data_Preparation/Merge_Data/DzeroMassPlots_Student/DzeroInvMassCan_pTbin_%1.1f_%1.1f_%1.1f_%1.1f.root",ymin,ymax,ptmin,ptmax));
  TFile *fD0_ep = TFile::Open(Form("../D0_Jan25_ML_Q2_1_ep/ML_model_ePIC_Shyam_Final/Differential_ML_YPt/Data_Preparation/Merge_Data/DzeroMassPlots_Student/DzeroInvMassCan_pTbin_%1.1f_%1.1f_%1.1f_%1.1f.root",ymin,ymax,ptmin,ptmax));

 
  if (!fD0_eAu || !fD0_ep) { cout << "Error: could not open one of the files!\n"; return; }

  // --- Get histograms ---
  TH1F *hD0_eAu = (TH1F*) fD0_eAu->Get("hFitResult");
  TH1F *hD0_ep = (TH1F*) fD0_ep->Get("hFitResult");
  if (!hD0_eAu || !hD0_ep) { cout << "Error: hFitResult not found!\n"; return; }

  double S_eAu = hD0_eAu->GetBinContent(1);
  double B_eAu = hD0_eAu->GetBinContent(2);
  double S_ep = hD0_ep->GetBinContent(1);
  double B_ep = hD0_ep->GetBinContent(2);

  double Signif_eAu = 0.0, Signif_ep = 0.0;
  if (S_eAu + B_eAu > 0) Signif_eAu = S_eAu / sqrt(S_eAu + B_eAu);
  if (S_ep + B_ep > 0) Signif_ep = S_ep / sqrt(S_ep + B_ep);

  // --- Relative uncertainties   σ(S)/S = sqrt(S+B)/S ---
  double relSignif_eAu = 0.0, relSignif_ep = 0.0;
  if (S_eAu > 0) relSignif_eAu = sqrt(S_eAu + B_eAu) / S_eAu;
  if (S_ep > 0) relSignif_ep = sqrt(S_ep + B_ep) / S_ep;


  // Standard error propagation
  double ratio = 0.0, rel_ratio = 0.0, e_ratio = 0.0;
  if (Signif_ep > 0) {
    rel_ratio = sqrt(relSignif_eAu*relSignif_eAu + relSignif_ep*relSignif_ep);
  }

  // --- Print results ---
  myfile1<<"-->(ymin, ymax) = ("<<ymin<<","<<ymax<<")"<<"-->(ptmin, ptmax) = ("<<ptmin<<","<<ptmax<<")"<<"-------"<<endl;
  myfile1 << fixed << setprecision(4);
  myfile1 << "eAu:\t S=" << S_eAu << "  B=" << B_eAu
       << "  S/sqrt(S+B)) =" << Signif_eAu << " Inverse Significance = "<<1./Signif_eAu<< "  rel sigma(S)/S =" << relSignif_eAu
       << " \n";
  myfile1 << "ep:\t S=" << S_ep << "  B=" << B_ep
       << "  S/Sqr(S+B) =" << Signif_ep << " Inverse Significance = "<<1./Signif_ep<<"  rel sigma(S)/S=" << relSignif_ep
       << "\n";

  myfile1 << "(relative uncertainty after adding in quadrature = " << rel_ratio
       << " = " << rel_ratio*100.0 << "%)\n";
   myfile<<ptmin<<"\t"<<ptmax<<"\t"<<rel_ratio<<endl; 
   myfile1<<endl;
   myfile.close();   
   myfile1.close();
}

