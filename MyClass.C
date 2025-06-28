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

  Long64_t evnt_tot = 0;
  Long64_t evnt_vtx1 = 0;
  

  // fiducial parameters
  cout << "Zvmax = " << Zvmax << ", Rhovmax = " << Rhovmax << endl;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {// Main loop
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    evnt_tot ++;
    
    // Select events with only 1 vertex within the fiducial volume
    int nvip = 0; // number of vertices at IP (vip) within the fiducial volume 
    int kvip[3]; // index of vip
    int kvip_nvip1 = -1; // index of nvip equal 1
    
    for (int kv = 0; kv < nv; kv++) {// loop on vertices
      
      if (TMath::Abs(zv[kv] - bz) < Zvmax && TMath::Sqrt((xv[kv] - bx) * (xv[kv] - bx) + (yv[kv] - by) * (yv[kv] - by)) < Rhovmax) { // fiducial volume
	
	kvip[nvip] = kv;
	nvip ++;

	h_nvip_kvip -> Fill(nvip, kvip[nvip - 1]);
	
	//cout << "nvip = " << nvip << ", kv = " << kv << ", kvip[" << nvip - 1  << "] = " << kvip[nvip - 1] << endl;
	
      }
      
    }

    if (nvip != 1) continue;
    kvip_nvip1 = kvip[nvip - 1];
      
    evnt_vtx1 ++;
      
    cout << "nvip = " << nvip << ", kvip_nvip1 = " << kvip_nvip1 << endl; 
    h_nvip1_kvip -> Fill(nvip, kvip_nvip1);
	
    
    
  }// end the main loop

  cout << "total # events = " << evnt_tot << "\n"
       << "evnt_vtx1 = " << evnt_vtx1 << "\n";
      

}

int MyClass::vtx_selection() {// returns selected vertex id (if only one is found)

  int nvip = 0; // number of vertices at IP (vip) within the fiducial volume 
  
  return nvip;
  
}
