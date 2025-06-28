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
  Long64_t evnt_trk = 0;
  
  
  // fiducial parameters
  cout << "Zvmax = " << Zvmax << ", Rhovmax = " << Rhovmax << endl;

  for (Long64_t jentry=0; jentry<nentries;jentry++) {// Main loop
    Long64_t ientry = LoadTree(jentry);
    if (ientry < 0) break;
    nb = fChain->GetEntry(jentry);   nbytes += nb;
    // if (Cut(ientry) < 0) continue;
    evnt_tot ++; // count total number of events
    
    int nvip = 0; // number of vertices at IP (vip) within the fiducial volume 
    int kvip[3]; // index of vip
    int kvip_nvip1 = -1; // index of vertex with nvip equal 1
    
    for (int kv = 0; kv < nv; kv++) {// loop on vertices
      
      if (TMath::Abs(zv[kv] - bz) < Zvmax && TMath::Sqrt((xv[kv] - bx) * (xv[kv] - bx) + (yv[kv] - by) * (yv[kv] - by)) < Rhovmax) { // fiducial volume
	
	kvip[nvip] = kv;
	nvip ++;

	h_nvip_kvip -> Fill(nvip, kvip[nvip - 1]);
	
	//cout << "nvip = " << nvip << ", kv = " << kv << ", kvip[" << nvip - 1  << "] = " << kvip[nvip - 1] << endl;
	
      }// end fiducial volume
      
    } // end loop on vertices

    if (nvip != 1) continue; // select events with only 1 vertex within the fiducial volume
    kvip_nvip1 = kvip[nvip - 1] + 1; // store the index of the selected vertex, shift needed?
    evnt_vtx1 ++; // count the number of selected events
    //cout << "nvip = " << nvip << ", kvip_nvip1 = " << kvip_nvip1 << endl; 
    h_nvip1_kvip -> Fill(nvip, kvip_nvip1); // nvip==1 vs. the corresponding index

    TVector2 trkv_sel; // initialize selected vertex associated tracks
    trkv_sel.SetX(-1); // X: index of pi+, with positive curvature
    trkv_sel.SetY(-1); // Y: index of pi-, wiht negative curvature

    int ntv_vtxid = 0; // number of tracks (nvt) accociated with given index of vertex (vtxid)

    std::vector<TVector3> trkv_momenta; // track 3-momentum
    std::vector<Int_t> trkv_charge; // track charge
    std::vector<Int_t> trkv_index; // track index
  
    for (int ktv = 0; ktv < ntv; ktv ++) { // loop over tracks connected to vertices

      h_ktv_kvip -> Fill(iv[ktv], kvip_nvip1);
      //cout << "vertex id = " << iv[ktv] << ", track id = " << kvip_nvip1 << endl;

      if (iv[ktv] == kvip_nvip1) { // these tracks are the one connected to my vertex
	ntv_vtxid ++;          // just to check that two tracks only have been found

	if (curv[ktv] > 0) {
	  trkv_charge.push_back(1);
	  //cout << "find a positive charged track," << " curv = " << curv[ktv] << endl;
	}
	else {
	  //cout << "find a negative charged track," << " curv = " << curv[ktv] << endl;
	  trkv_charge.push_back(-1);
	}
	
	TVector3 momentum(pxtv[ktv],pytv[ktv],pztv[ktv]);
	trkv_momenta.push_back(momentum);
	trkv_index.push_back(ktv);

      }

    }// end loop over tracks connected to vertices

    if (ntv_vtxid != 2 || trkv_charge[0] * trkv_charge[1] >= 0) continue; // select 2 tracks
    evnt_trk ++;
    cout << "ntv_vtxid = " << ntv_vtxid << "\n"
	 << "\ttrkv[0](charge, index, momenta.X, momenta.Y, momenta.Z) = (" << trkv_charge[0] << ", " << trkv_index[0] << ", " << trkv_momenta[0].X() << ", " << trkv_momenta[0].Y() << ", " << trkv_momenta[0].Z() << ")" << "\n "
	 << "\ttrkv[1](charge, index, momenta.X, momenta.Y, momenta.Z) = (" << trkv_charge[1] << ", " << trkv_index[1] << ", " << trkv_momenta[1].X() << ", " << trkv_momenta[1].Y() << ", " << trkv_momenta[1].Z() << ")" << endl;
    
    
  }// end the main loop

  cout << "Summary" << "\n"
       << "total # events = " << evnt_tot << "\n"
       << "evnt_vtx1 = " << evnt_vtx1 << "\n"
       << "evnt_trk = " << evnt_trk << "\n";
      

}

int MyClass::vtx_selection() {// returns selected vertex id (if only one is found)

  int nvip = 0; // number of vertices at IP (vip) within the fiducial volume 
  
  return nvip;
  
}
