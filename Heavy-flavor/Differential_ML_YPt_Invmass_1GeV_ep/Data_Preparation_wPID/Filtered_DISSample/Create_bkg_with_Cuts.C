// Create a filtered Bkg tree with topological cuts
void Create_bkg_with_Cuts(){

TString preselection ="(mass_D0 > 1.6 && mass_D0 < 2.5)";

TFile *f_bkg = TFile::Open("../../../../D0_Jan25_Bkg/HF_reco/helix/BkgD0.root");	
TTree *t_bkg = (TTree*)f_bkg->Get("treeMLBkg");
t_bkg->SetBranchStatus("mult", 0);
TFile* f_bkg_filt = new TFile("BkgD0.root", "RECREATE");
TTree *tree_bkg_filt = t_bkg->CopyTree(preselection.Data());
f_bkg_filt->cd();
tree_bkg_filt->Write();
f_bkg_filt->Close();
}
