// Create a filtered Signal tree with topological cuts
void Create_Signal_with_Cuts(){

TString preselection ="(mass_D0 > 1.6 && mass_D0 < 2.5)";
TFile *f_sig = TFile::Open("../../../../D0_Jan25_Bkg/HF_reco/helix/SignalD0.root");	
TTree *t_sig = (TTree*)f_sig->Get("treeMLSig");
t_sig->SetBranchStatus("mult", 0);
TFile* f_sig_filt = new TFile("SignalD0.root", "RECREATE");
TTree *tree_sig_filt = t_sig->CopyTree(preselection.Data());
f_sig_filt->cd();
tree_sig_filt->Write();
f_sig_filt->Close();
}
