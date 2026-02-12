// Code to evaluate the relative statistical uncertainty on Lc+/D0 ratio
// Shyam Kumar; INFN Bari, shyam.kumar@ba.infn.it 

void RelUncertainty_D0_ML(float ymin = -1.0, float ymax = 1.0, float ptmin = 1.0, float ptmax = 2.0)
{
  ofstream myfile;
  myfile.open (Form("uncertainties_%1.1f_%1.1f_ML.txt",ymin,ymax),ios_base::app); // ,ios_base::app Edit mode 
  
   ofstream myfile1;  
   myfile1.open("RelUncertainty_summary.txt",ios_base::app); // ,ios_base::app Edit mode 		
  
  TFile *fD0_eAu = TFile::Open(Form("../D0_Jan25_ML_Q2_1_eAu/ML_model_ePIC_Shyam_Final/Differential_ML_YPt/ML_Output_Optuna_%1.1f_%1.1f_%1.1f_%1.1f/final_results_sb_significance.root",ymin,ymax,ptmin,ptmax));
  TCanvas *c_eAu =  (TCanvas*) fD0_eAu->Get("c");
  TMultiGraph *mg_eAu =  (TMultiGraph*)c_eAu->FindObject("mg");
  TList *graphs_eAu = mg_eAu->GetListOfGraphs();
  TGraph *g_eAu = (TGraph*)graphs_eAu->At(3);
  double y_max_eAu = TMath::MaxElement(g_eAu->GetN(),g_eAu->GetY());
  int idx_max_eAu = TMath::LocMax(g_eAu->GetN(),g_eAu->GetY());
  double bdt_eAu, Signif_eAu; 
  g_eAu->GetPoint(idx_max_eAu,bdt_eAu,Signif_eAu);
  
  TFile *fD0_ep = TFile::Open(Form("../D0_Jan25_ML_Q2_1_ep/ML_model_ePIC_Shyam_Final/Differential_ML_YPt/ML_Output_Optuna_%1.1f_%1.1f_%1.1f_%1.1f/final_results_sb_significance.root",ymin,ymax,ptmin,ptmax));
  TCanvas *c_ep =  (TCanvas*) fD0_ep->Get("c");
  TMultiGraph *mg_ep =  (TMultiGraph*)c_ep->FindObject("mg");
  TList *graphs_ep = mg_ep->GetListOfGraphs();
  TGraph *g_ep = (TGraph*)graphs_ep->At(3);
  double y_max_ep = TMath::MaxElement(g_ep->GetN(),g_ep->GetY());
  int idx_max_ep = TMath::LocMax(g_ep->GetN(),g_ep->GetY());
  double bdt_ep, Signif_ep; 
  g_ep->GetPoint(idx_max_ep,bdt_ep,Signif_ep);
  
  // --- Relative uncertainties   σ(S)/S = sqrt(S+B)/S ---
  double relSignif_eAu = 1./Signif_eAu;
  double relSignif_ep = 1./Signif_ep;


  // Standard error propagation
  double ratio = 0.0, rel_ratio = 0.0, e_ratio = 0.0;
  if (Signif_ep > 0) {
    rel_ratio = sqrt(relSignif_eAu*relSignif_eAu + relSignif_ep*relSignif_ep);
  }

  // --- Print results ---
  myfile1<<"-->(ymin, ymax) = ("<<ymin<<","<<ymax<<")"<<"-->ptmin, ptmax) = ("<<ptmin<<","<<ptmax<<")"<<"-------"<<endl;
  myfile1 << fixed << setprecision(4);
  myfile1 << "eAu (ML):\t S/sqrt(S+B) =" << Signif_eAu << " Inverse Significance = "<<1./Signif_eAu<< "  rel sqrt(S+B)/S =" << relSignif_eAu
       << " \n";
  myfile1 << "ep (ML):\t S/sqrt(S+B) =" << Signif_ep << " Inverse Significance = "<<1./Signif_ep<< "  rel sqrt(S+B)/S=" << relSignif_ep
       << "\n";

  myfile1 << "(relative uncertainty  after adding in quadrature = " << rel_ratio
       << " = " << rel_ratio*100.0 << "%)\n";
   myfile<<ptmin<<"\t"<<ptmax<<"\t"<<rel_ratio<<"\t"<<bdt_ep<<"\t"<<bdt_eAu<<endl; 
   myfile1<<endl;
   myfile.close(); 
   myfile1.close();   
}

