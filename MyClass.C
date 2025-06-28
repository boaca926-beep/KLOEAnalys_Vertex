#define MyClass_cxx
#include "MyClass.h"
#include "TMatrixD.h"
#include "TVectorD.h"
#include <TH2.h>
#include <TStyle.h>
#include <TCanvas.h>
#include "TDecompSVD.h"
#include <sstream>
#include "TError.h"

void MyClass::Main()
{
  gErrorIgnoreLevel = kFatal; //kFatal;

  if (fChain == 0) return;

  Long64_t nentries = fChain->GetEntriesFast();
  
  Long64_t nbytes = 0, nb = 0;
  for (Long64_t jentry=0; jentry<nentries;jentry++) {
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    evnt_sum ++;
    
    // Select events with only 1 vertex in the fiducial volume
    getVertices();

    if (iv_ip != 1) continue; // vertex cut
    fiduial_indx = 1;
    evnt_fidual ++;
    
    cout << "ip = " << iv_ip  << ", indx = " << iv_indx << endl;
  
  }

  cout << "evnt_sum = " << evnt_sum << "\n"
       << "evnt_fidual = " << evnt_fidual << endl;
  

}

void MyClass::getVertices() {

  double Zv_tmp = 0., Rhov_tmp = 0.;
  int ip_tmp = 0, indx_tmp = -1;
  
  for (int k = 0; k < nv; k++) {// begin loop
    Zv_tmp = TMath::Abs(zv[k] - bz);
    Rhov_tmp = TMath::Sqrt((xv[k] - bx) * (xv[k] - bx) + (yv[k] - by) * (yv[k] - by));
    //cout << "Zv_tmp = " << Zv_tmp << ", Rhov_tmp = " << Rhov_tmp << endl;
    if (Zv_tmp <= Zvmax && Rhov_tmp <= Rhovmax) { // fiducial volume
      ip_tmp ++;
      indx_tmp = k;
    }
  }// end loop

  iv_ip = ip_tmp;
  iv_indx = indx_tmp;
  
}
