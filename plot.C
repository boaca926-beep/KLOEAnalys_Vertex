#include "plot.h"

int plot(){

  gErrorIgnoreLevel = kError;
  TGaxis::SetMaxDigits(4);
  gStyle->SetOptStat(0);
  gStyle->SetOptTitle(0);
  gStyle->SetFitFormat("6.4g");

  const TString mc_type = "sig"; // sig ksl

  TString file_path = mc_type + ".root";
  
  TFile* file_input = new TFile(file_path);

  TIter next_tree(file_input -> GetListOfKeys());
  TString objnm_tree, classnm_tree;
  
  //int k = 0;
  TKey *key;

  while ( (key = (TKey *) next_tree() ) ) {// start cutlist_tree while loop
    
    objnm_tree   = key -> GetName();
    classnm_tree = key -> GetClassName();
    key -> GetSeekKey();
	
    cout << "classnm = " << classnm_tree << ", objnm = " << objnm_tree << endl;
    
  }

  TH2D *h_nvip_kvip = (TH2D *)file_input->Get("h_nvip_kvip");
  TH2D *h_nvip1_kvip = (TH2D *)file_input->Get("h_nvip1_kvip");
  TH2D *h_ktv_kvip = (TH2D *)file_input->Get("h_ktv_kvip");
  
  // Plot
  TPaveText *pt_1 = new TPaveText(0.25, 0.82, 0.7, 0.86, "NDC");
  TString pt_1_text = mc_type + " (before nvip=1 cut)"; 
  SetPte(pt_1, pt_1_text);

  TPaveText *pt_2 = new TPaveText(0.25, 0.82, 0.7, 0.86, "NDC");
  TString pt_2_text = mc_type + " (after nvip=1 cut)"; 
  SetPte(pt_2, pt_2_text);

  TCanvas *cv_2d = new TCanvas("cv_2d", "nvip vs. kvip", 0, 0, 1600, 800);
  cv_2d -> Divide(2, 1);

  cv_2d -> cd(1);
  gPad -> SetBottomMargin(0.15);//0.007
  gPad -> SetLeftMargin(0.15);
  gPad -> SetRightMargin(0.17);
  
  //h_nvip_kvip -> GetXaxis() -> SetNdivisions(5);
  h_nvip_kvip -> GetXaxis() -> SetTitle("Number of vertices (nvip)");
  h_nvip_kvip -> GetXaxis() -> SetTitleOffset(1.2);
  h_nvip_kvip -> GetXaxis() -> SetTitleSize(0.06);
  h_nvip_kvip -> GetXaxis() -> CenterTitle();
  h_nvip_kvip -> GetXaxis() -> SetLabelSize(0.06);
  h_nvip_kvip -> GetXaxis() -> SetLabelOffset(0.01);
  //h_nvip_kvip -> GetXaxis() -> SetRangeUser(0.2, 0.6);
  
  h_nvip_kvip -> GetYaxis() -> SetTitle("Index of vertex (kvip)");
  h_nvip_kvip -> GetYaxis() -> SetLabelOffset(0.01);
  h_nvip_kvip -> GetYaxis() -> SetTitleOffset(1.2);
  h_nvip_kvip -> GetYaxis() -> SetLabelSize(0.06);
  h_nvip_kvip -> GetYaxis() -> SetTitleSize(0.06);
  h_nvip_kvip -> GetYaxis() -> CenterTitle();

  h_nvip_kvip -> GetZaxis() -> SetLabelSize(0.06);

  h_nvip_kvip -> Draw("TEXT0COLZ");
  pt_1 -> Draw("Same");
  gPad -> SetLogz(1);

  cv_2d -> cd(2);
  gPad -> SetBottomMargin(0.15);//0.007
  gPad -> SetLeftMargin(0.15);
  gPad -> SetRightMargin(0.17);

  //h_nvip1_kvip -> GetXaxis() -> SetNdivisions(5);
  h_nvip1_kvip -> GetXaxis() -> SetTitle("Number of vertices (nvip)");
  h_nvip1_kvip -> GetXaxis() -> SetTitleOffset(1.2);
  h_nvip1_kvip -> GetXaxis() -> SetTitleSize(0.06);
  h_nvip1_kvip -> GetXaxis() -> CenterTitle();
  h_nvip1_kvip -> GetXaxis() -> SetLabelSize(0.06);
  h_nvip1_kvip -> GetXaxis() -> SetLabelOffset(0.01);
  //h_nvip1_kvip -> GetXaxis() -> SetRangeUser(0.2, 0.6);
  
  h_nvip1_kvip -> GetYaxis() -> SetTitle("Index of vertex (kvip)");
  h_nvip1_kvip -> GetYaxis() -> SetLabelOffset(0.01);
  h_nvip1_kvip -> GetYaxis() -> SetTitleOffset(1.2);
  h_nvip1_kvip -> GetYaxis() -> SetLabelSize(0.06);
  h_nvip1_kvip -> GetYaxis() -> SetTitleSize(0.06);
  h_nvip1_kvip -> GetYaxis() -> CenterTitle();

  h_nvip1_kvip -> GetZaxis() -> SetLabelSize(0.06);

  h_nvip1_kvip -> Draw("TEXT0COLZ");
  pt_2 -> Draw("Same");
  gPad->SetLogz(1);

  //
  double norm = h_ktv_kvip->Integral();
  cout << norm << endl;
  
  TPaveText *pt_30 = new TPaveText(0.25, 0.82, 0.7, 0.86, "NDC");
  TString pt_30_text = mc_type + " (nvip=1)";
  SetPte(pt_30, pt_30_text);

  TPaveText *pt_31 = new TPaveText(0.25, 0.77, 0.7, 0.81, "NDC");
  TString pt_31_text = "kvip_nvip1 shifted by +1";
  SetPte(pt_31, pt_31_text);

  TCanvas *cv_2d_1 = new TCanvas("cv_2d_1", "iv[ktv] vs. kvip_nvip1", 0, 0, 800, 800);
  cv_2d_1 -> Divide(1, 1);

  cv_2d_1 -> cd(1);
  gPad -> SetBottomMargin(0.15);//0.007
  gPad -> SetLeftMargin(0.15);
  gPad -> SetRightMargin(0.17);
  
  h_ktv_kvip -> GetXaxis() -> SetTitleOffset(1.2);
  h_ktv_kvip -> GetXaxis() -> SetTitleSize(0.06);
  h_ktv_kvip -> GetXaxis() -> CenterTitle();
  h_ktv_kvip -> GetXaxis() -> SetLabelSize(0.06);
  h_ktv_kvip -> GetXaxis() -> SetLabelOffset(0.01);
  //h_ktv_kvip -> GetXaxis() -> SetRangeUser(0.2, 0.6);
  
  h_ktv_kvip -> GetYaxis() -> SetLabelOffset(0.01);
  h_ktv_kvip -> GetYaxis() -> SetTitleOffset(1.2);
  h_ktv_kvip -> GetYaxis() -> SetLabelSize(0.06);
  h_ktv_kvip -> GetYaxis() -> SetTitleSize(0.06);
  h_ktv_kvip -> GetYaxis() -> CenterTitle();

  h_ktv_kvip -> GetZaxis() -> SetLabelSize(0.06);

  h_ktv_kvip -> Draw("TEXT0COLZ");
  pt_30 -> Draw("Same");
  pt_31 -> Draw("Same");
  gPad -> SetLogz(1);

  cv_2d -> SaveAs("vertex2D_" + mc_type + ".pdf");
  cv_2d_1 -> SaveAs("scatter_shfit_" + mc_type + ".pdf");
  
  return 0;
  
}
