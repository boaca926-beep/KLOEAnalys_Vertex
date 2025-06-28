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

	nvip ++;
	kvip[nvip] = kv + 1; // shift needed?
	
	h_nvip_kvip -> Fill(nvip, kvip[nvip]);
	
	cout << "nvip = " << nvip << ", kv = " << kv << ", kvip[" << nvip << "] = " << kvip[nvip] << endl;
	
      }// end fiducial volume
      
    } // end loop on vertices

    if (nvip != 1) continue; // select events with only 1 vertex within the fiducial volume
    kvip_nvip1 = kvip[nvip]; // store the index of the selected vertex
    evnt_vtx1 ++; // count the number of selected events
    //cout << "nvip = " << nvip << ", kvip_nvip1 = " << kvip_nvip1 << endl; 
    h_nvip1_kvip -> Fill(nvip, kvip_nvip1); // nvip==1 vs. the corresponding index

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

    TVector2 trkv_sel; // initialize selected vertex associated tracks
    trkv_sel.SetX(-1); // X: index of pi+, with positive curvature
    trkv_sel.SetY(-1); // Y: index of pi-, wiht negative curvature

    TVector3 trkmom_plus;
    TVector3 trkmom_nega;
	
    if (trkv_charge[0]>0) {
      trkv_sel.SetX(trkv_index[0]); // pi+ track index in NTV block 
      trkv_sel.SetY(trkv_index[1]); // pi- track index in NTV block

      trkmom_plus = trkv_momenta[0];
      trkmom_nega = trkv_momenta[1];
    }
    else {
      trkv_sel.SetX(trkv_index[1]); //pi+ in the NTV track block 
      trkv_sel.SetY(trkv_index[0]); //pi- in the NTV track block

      trkmom_plus = trkv_momenta[1];
      trkmom_nega = trkv_momenta[0];
    }

    // get 4-vector using track-vertex parameters
    TLorentzVector TLVector_ppl = Gettrack4vectorkinfit(trkv_sel.X());
    TLorentzVector TLVector_pmi = Gettrack4vectorkinfit(trkv_sel.Y());

    cout << "ntv_vtxid = " << ntv_vtxid << "\n"
	 << "\ttrkv[0](charge, index, px, py, pz) = (" << trkv_charge[0] << ", " << trkv_index[0] << ", " << trkv_momenta[0].X() << ", " << trkv_momenta[0].Y() << ", " << trkv_momenta[0].Z() << ")" << "\n "
	 << "\ttrkv[1](charge, index, px, py, pz) = (" << trkv_charge[1] << ", " << trkv_index[1] << ", " << trkv_momenta[1].X() << ", " << trkv_momenta[1].Y() << ", " << trkv_momenta[1].Z() << ")" << "\n"
	 << "\ttrk_plus: index = " << trkv_sel.X() << ", (px, py, pz) = (" << trkmom_plus.X() << ", " << trkmom_plus.Y() << ", " << trkmom_plus.Z() << "), checked by 4-vector (px, py, pz, E) = (" << TLVector_ppl.X() << ", " << TLVector_ppl.Y() << ", " << TLVector_ppl.Z() << ", " << TLVector_ppl.E() << ")\n"
	 << "\ttrk_nega: index = " << trkv_sel.Y() << ", (px, py, pz) = (" << trkmom_nega.X() << ", " << trkmom_nega.Y() << ", " << trkmom_nega.Z() << "), checked by 4-vector (px, py, pz, E) = (" << TLVector_pmi.X() << ", " << TLVector_pmi.Y() << ", " << TLVector_pmi.Z() << ", " << TLVector_pmi.E() << ")\n";
    
    
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

TLorentzVector MyClass::Gettrack4vectorkinfit(int index) {

  TLorentzVector tvector_smeared(0.,0.,0.,0.);
  TLorentzVector tvector1(0.,0.,0.,0.);
  double E=0., psq=0., ptran=0., px=0., py=0., pz=0.;
  Double_t smearing_factor = 0.1501;
  Double_t scale_factor = 0.000367;
  double curv_tmp = curv[index];
  double phi_tmp = phiv[index];
  double cotan_tmp = cotv[index];
  
  ptran = 1000. * 1. / TMath::Abs(curv_tmp);
  //Double_t ptransvSmeared = ptran*(1.+scale_factor);
  //ptransvSmeared = generator->Gaus(ptransvSmeared,(TMath::Sqrt(sigcurv[index]))*smearing_factor*ptransvSmeared);

  tvector1.SetX(ptran * TMath::Cos(phi_tmp));
  tvector1.SetY(ptran * TMath::Sin(phi_tmp));
  tvector1.SetZ(ptran * cotan_tmp);
  tvector1.SetE(sqrt(masschpion*masschpion + tvector1.Vect().Mag2()));

  /*
  tvector_smeared.SetX(ptransvSmeared * TMath::Cos(phi));
  tvector_smeared.SetY(ptransvSmeared * TMath::Sin(phi));
  tvector_smeared.SetZ(ptransvSmeared * cotan);
  tvector_smeared.SetE(sqrt(masschpion*masschpion + tvector_smeared.Vect().Mag2()));
  */
  
  return tvector1;
  //return tvector_smeared;
}
