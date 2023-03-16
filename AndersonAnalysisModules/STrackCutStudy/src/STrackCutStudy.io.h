// ----------------------------------------------------------------------------
// 'STrackCutStudy.io.h'
// Derek Anderson
// 03.14.2022
//
// Reads in the 'ntp_track' Ntuple
// generated by the SVtxEvaluator
// class and studies the impact
// of cutting on various quantities.
// ----------------------------------------------------------------------------

#pragma once

using namespace std;



// i/o methods ----------------------------------------------------------------

void STrackCutStudy::SetInputOutputFiles(const TString sEmbedOnlyInput, const TString sPileupInput, const TString sOutput) {

  sInFileEO = sEmbedOnlyInput;
  sInFilePU = sPileupInput;
  sOutfile  = sOutput;
  cout << "    Set i/o files:\n"
       << "      Embed Only  = " << sInFileEO.Data() << "\n"
       << "      With Pileup = " << sInFilePU.Data() << "\n"
       << "      Output      = " << sOutput.Data()
       << endl;
  return;

}  // end 'SetInputOutputFiles(TString, TString, TString)'



void STrackCutStudy::SetInputTuples(const TString sEmbedOnlyTuple, const TString sPileupTuple, const TString sEmbedOnlyClusterTuple) {

  sInTupleEO = sEmbedOnlyTuple;
  sInTuplePU = sPileupTuple;
  sInClustEO = sEmbedOnlyClusterTuple;
  cout << "    Set input track tuples:\n"
       << "      Embed Only  = " << sInTupleEO.Data() << "\n"
       << "      With Pileup = " << sInTuplePU.Data()
       << endl;

  if (sEmbedOnlyClusterTuple != "") {
    cout << "    Set input cluster tuples:\n"
         << "      Embed-only cluster tuple = " << sInClustEO.Data()
         << endl;
  }
  return;

}  // end 'SetInputTuples(TString, TString)'



void STrackCutStudy::SetStudyParameters(const Bool_t intNorm, const Bool_t avgClustCalc, const Double_t normalFracMin, const Double_t normalFracMax) {

  doIntNorm       = intNorm;
  doAvgClustCalc  = avgClustCalc;
  normalPtFracMin = normalFracMin;
  normalPtFracMax = normalFracMax;
  cout << "    Set normal pT fraction:\n"
       << "      Normal Pt Fraction = (" << normalPtFracMin << ", " << normalPtFracMax << ")"
       << endl;

  if (doIntNorm) {
    cout << "    Normalizing by integral." << endl;
  } else {
    cout << "    No normalization." << endl;
  }

  if (doAvgClustCalc) {
    cout << "    Will calculate average cluster size." << endl;
  } else {
    cout << "    Will not calculate average cluster size." << endl;
  }
  return;

}  // end 'SetStudyParameters(bool, bool, double, double)'



void STrackCutStudy::SetCutFlags(const Bool_t doPrimary, const Bool_t doMVtx, const Bool_t doTpc, const Bool_t doVz, const Bool_t doDcaXY, const Bool_t doDcaZ, const Bool_t doQuality) {

  doPrimaryCut = doPrimary;
  doMVtxCut    = doMVtx;
  doTpcCut     = doTpc;
  doVzCut      = doVz;
  doDcaXyCut   = doDcaXY;
  doDcaZCut    = doDcaZ;
  doQualityCut = doQuality;
  cout << "    Set cut flags:\n"
       << "      doPrimaryCut = " << doPrimaryCut << "\n"
       << "      doMVtxCut    = " << doMVtxCut    << "\n"
       << "      doTpcCut     = " << doTpcCut     << "\n"
       << "      doVzCut      = " << doVzCut      << "\n"
       << "      doDcaXyCut   = " << doDcaXyCut   << "\n"
       << "      doDCaZCut    = " << doDcaZCut    << "\n"
       << "      doQualityCut = " << doQualityCut
       << endl;
  return;


}  // end 'SetCutFlags(Bool_t, Bool_t, Bool_t, Bool_t, Bool_t, Bool_t, Bool_t)'



void STrackCutStudy::SetTrackCuts(const pair<UInt_t, UInt_t> nMVtxRange, const pair<UInt_t, UInt_t> nTpcRange, const pair<Double_t, Double_t> vzRange, const pair<Double_t, Double_t> dcaXyRange, const pair <Double_t, Double_t> dcaZRange, const pair<Double_t, Double_t> qualityRange) {

  nMVtxCut   = nMVtxRange;
  nTpcCut    = nTpcRange;
  vzCut      = vzRange;
  dcaXyCut   = dcaXyRange;
  dcaZCut    = dcaZRange;
  qualityCut = qualityRange;
  cout << "    Set track cuts:\n"
       << "      mvtx hits = (" << nMVtxCut.first   << ", " << nMVtxCut.second   << ")\n"
       << "      tpc hits  = (" << nTpcCut.first    << ", " << nTpcCut.second    << ")\n"
       << "      z-vertex  = (" << vzCut.first      << ", " << vzCut.second      << ")\n"
       << "      dca (xy)  = (" << dcaXyCut.first   << ", " << dcaXyCut.second   << ")\n"
       << "      dca (z)   = (" << dcaZCut.first    << ", " << dcaZCut.second    << ")\n"
       << "      quality   = (" << qualityCut.first << ", " << qualityCut.second << ")"
       << endl;
  return;

}  // end 'SetTrackCuts(pair<UInt_t, UInt_t>, pair<UInt_t, UInt_t>, pair<Double_t, Double_t>, pair<Double_t, Double_t>, pair<Double_t, Double_t>, pair<Double_t, Double_t>)'



void STrackCutStudy::SetPlotText(const Ssiz_t nTxtE, const Ssiz_t nTxtP, const TString sTxtE[], const TString sTxtP[]) {

  nTxtEO = nTxtE;
  nTxtPU = nTxtP;
  for (Ssiz_t iTxtEO = 0; iTxtEO < nTxtEO; iTxtEO++) {
    sTxtEO.push_back(sTxtE[iTxtEO]);
  }
  for (Ssiz_t iTxtPU = 0; iTxtPU < nTxtPU; iTxtPU++) {
    sTxtPU.push_back(sTxtP[iTxtPU]);
  }
  cout << "    Set plot text." << endl;
  return;

}  // end 'SetPlotText(Ssiz_t, Ssiz_t, TString[], TString[])'



void STrackCutStudy::InitFiles() {

  // open files
  fOut  = new TFile(sOutfile.Data(),  "recreate");
  fInEO = new TFile(sInFileEO.Data(), "read");
  fInPU = new TFile(sInFilePU.Data(), "read");
  if (!fOut || !fInEO || !fInPU) {
    cerr << "PANIC: couldn't open a file!\n"
         << "       fOut = " << fOut << ", fInEO = " << fInEO << ", fInPU = " << fInPU << "\n"
         << endl;
    assert(fOut && fInEO && fInPU);
  }
  cout << "      Initialized files." << endl;
  return;

}  // end 'InitFiles()'



void STrackCutStudy::InitTuples() {

  // grab tuples
  ntTrkEO = (TNtuple*) fInEO -> Get(sInTupleEO.Data());
  ntTrkPU = (TNtuple*) fInPU -> Get(sInTuplePU.Data());
  if (!ntTrkEO || !ntTrkPU) {
    cerr << "PANIC: couldn't grab an input Ntuple!\n"
         << "       ntTrkEO = " << ntTrkEO << ", ntTrkPU = " << ntTrkPU << "\n"
         << endl;
    assert(ntTrkEO && ntTrkPU);
  }

  if (doAvgClustCalc) {
    ntClustEO = (TNtuple*) fInEO -> Get(sInClustEO.Data());
    if (!ntClustEO) {
      cerr << "PANIC: couldn't grab an input cluster NTuple!\n"
           << "       ntClustEO = " << ntClustEO << "\n"
           << endl;
      assert(ntClustEO);
    }
  }
  cout << "      Initialized input ntuples." << endl;

  // set embed-only branch addresses
  ntTrkEO -> SetBranchAddress("event",           &event);
  ntTrkEO -> SetBranchAddress("seed",            &seed);
  ntTrkEO -> SetBranchAddress("trackID",         &trackID);
  ntTrkEO -> SetBranchAddress("crossing",        &crossing);
  ntTrkEO -> SetBranchAddress("px",              &px);
  ntTrkEO -> SetBranchAddress("py",              &py);
  ntTrkEO -> SetBranchAddress("pz",              &pz);
  ntTrkEO -> SetBranchAddress("pt",              &pt);
  ntTrkEO -> SetBranchAddress("eta",             &eta);
  ntTrkEO -> SetBranchAddress("phi",             &phi);
  ntTrkEO -> SetBranchAddress("deltapt",         &deltapt);
  ntTrkEO -> SetBranchAddress("deltaeta",        &deltaeta);
  ntTrkEO -> SetBranchAddress("deltaphi",        &deltaphi);
  ntTrkEO -> SetBranchAddress("charge",          &charge);
  ntTrkEO -> SetBranchAddress("quality",         &quality);
  ntTrkEO -> SetBranchAddress("chisq",           &chisq);
  ntTrkEO -> SetBranchAddress("ndf",             &ndf);
  ntTrkEO -> SetBranchAddress("nhits",           &nhits);
  ntTrkEO -> SetBranchAddress("nmaps",           &nmaps);
  ntTrkEO -> SetBranchAddress("nintt",           &nintt);
  ntTrkEO -> SetBranchAddress("ntpc",            &ntpc);
  ntTrkEO -> SetBranchAddress("nmms",            &nmms);
  ntTrkEO -> SetBranchAddress("ntpc1",           &ntpc1);
  ntTrkEO -> SetBranchAddress("ntpc11",          &ntpc11);
  ntTrkEO -> SetBranchAddress("ntpc2",           &ntpc2);
  ntTrkEO -> SetBranchAddress("ntpc3",           &ntpc3);
  ntTrkEO -> SetBranchAddress("nlmaps",          &nlmaps);
  ntTrkEO -> SetBranchAddress("nlintt",          &nlintt);
  ntTrkEO -> SetBranchAddress("nltpc",           &nltpc);
  ntTrkEO -> SetBranchAddress("nlmms",           &nlmms);
  ntTrkEO -> SetBranchAddress("layers",          &layers);
  ntTrkEO -> SetBranchAddress("vertexID",        &vertexID);
  ntTrkEO -> SetBranchAddress("vx",              &vx);
  ntTrkEO -> SetBranchAddress("vy",              &vy);
  ntTrkEO -> SetBranchAddress("vz",              &vz);
  ntTrkEO -> SetBranchAddress("dca2d",           &dca2d);
  ntTrkEO -> SetBranchAddress("dca2dsigma",      &dca2dsigma);
  ntTrkEO -> SetBranchAddress("dca3dxy",         &dca3dxy);
  ntTrkEO -> SetBranchAddress("dca3dxysigma",    &dca3dxysigma);
  ntTrkEO -> SetBranchAddress("dca3dz",          &dca3dz);
  ntTrkEO -> SetBranchAddress("dca3dzsigma",     &dca3dzsigma);
  ntTrkEO -> SetBranchAddress("pcax",            &pcax);
  ntTrkEO -> SetBranchAddress("pcay",            &pcay);
  ntTrkEO -> SetBranchAddress("pcaz",            &pcaz);
  ntTrkEO -> SetBranchAddress("gtrackID",        &gtrackID);
  ntTrkEO -> SetBranchAddress("gflavor",         &gflavor);
  ntTrkEO -> SetBranchAddress("gnhits",          &gnhits);
  ntTrkEO -> SetBranchAddress("gnmaps",          &gnmaps);
  ntTrkEO -> SetBranchAddress("gnintt",          &gnintt);
  ntTrkEO -> SetBranchAddress("gntpc",           &gntpc);
  ntTrkEO -> SetBranchAddress("gnmms",           &gnmms);
  ntTrkEO -> SetBranchAddress("gnlmaps",         &gnlmaps);
  ntTrkEO -> SetBranchAddress("gnlintt",         &gnlintt);
  ntTrkEO -> SetBranchAddress("gnltpc",          &gnltpc);
  ntTrkEO -> SetBranchAddress("gnlmms",          &gnlmms);
  ntTrkEO -> SetBranchAddress("gpx",             &gpx);
  ntTrkEO -> SetBranchAddress("gpy",             &gpy);
  ntTrkEO -> SetBranchAddress("gpz",             &gpz);
  ntTrkEO -> SetBranchAddress("gpt",             &gpt);
  ntTrkEO -> SetBranchAddress("geta",            &geta);
  ntTrkEO -> SetBranchAddress("gphi",            &gphi);
  ntTrkEO -> SetBranchAddress("gvx",             &gvx);
  ntTrkEO -> SetBranchAddress("gvy",             &gvy);
  ntTrkEO -> SetBranchAddress("gvz",             &gvz);
  ntTrkEO -> SetBranchAddress("gvt",             &gvt);
  ntTrkEO -> SetBranchAddress("gfpx",            &gfpx);
  ntTrkEO -> SetBranchAddress("gfpy",            &gfpy);
  ntTrkEO -> SetBranchAddress("gfpz",            &gfpz);
  ntTrkEO -> SetBranchAddress("gfx",             &gfx);
  ntTrkEO -> SetBranchAddress("gfy",             &gfy);
  ntTrkEO -> SetBranchAddress("gfz",             &gfz);
  ntTrkEO -> SetBranchAddress("gembed",          &gembed);
  ntTrkEO -> SetBranchAddress("gprimary",        &gprimary);
  ntTrkEO -> SetBranchAddress("nfromtruth",      &nfromtruth);
  ntTrkEO -> SetBranchAddress("nwrong",          &nwrong);
  ntTrkEO -> SetBranchAddress("ntrumaps",        &ntrumaps);
  ntTrkEO -> SetBranchAddress("ntruintt",        &ntruintt);
  ntTrkEO -> SetBranchAddress("ntrutpc",         &ntrutpc);
  ntTrkEO -> SetBranchAddress("ntrumms",         &ntrumms);
  ntTrkEO -> SetBranchAddress("ntrutpc1",        &ntrutpc1);
  ntTrkEO -> SetBranchAddress("ntrutpc11",       &ntrutpc11);
  ntTrkEO -> SetBranchAddress("ntrutpc2",        &ntrutpc2);
  ntTrkEO -> SetBranchAddress("ntrutpc3",        &ntrutpc3);
  ntTrkEO -> SetBranchAddress("layersfromtruth", &layersfromtruth);
  ntTrkEO -> SetBranchAddress("nhittpcall",      &nhittpcall);
  ntTrkEO -> SetBranchAddress("nhittpcin",       &nhittpcin);
  ntTrkEO -> SetBranchAddress("nhittpcmid",      &nhittpcmid);
  ntTrkEO -> SetBranchAddress("nhittpcout",      &nhittpcout);
  ntTrkEO -> SetBranchAddress("nclusall",        &nclusall);
  ntTrkEO -> SetBranchAddress("nclustpc",        &nclustpc);
  ntTrkEO -> SetBranchAddress("nclusintt",       &nclusintt);
  ntTrkEO -> SetBranchAddress("nclusmaps",       &nclusmaps);
  ntTrkEO -> SetBranchAddress("nclusmms",        &nclusmms);

  // set with-pileup branch
  ntTrkPU -> SetBranchAddress("event",           &pu_event);
  ntTrkPU -> SetBranchAddress("seed",            &pu_seed);
  ntTrkPU -> SetBranchAddress("gntracks",        &pu_gntracks);
  ntTrkPU -> SetBranchAddress("gtrackID",        &pu_gtrackID);
  ntTrkPU -> SetBranchAddress("gflavor",         &pu_gflavor);
  ntTrkPU -> SetBranchAddress("gnhits",          &pu_gnhits);
  ntTrkPU -> SetBranchAddress("gnmaps",          &pu_gnmaps);
  ntTrkPU -> SetBranchAddress("gnintt",          &pu_gnintt);
  ntTrkPU -> SetBranchAddress("gnmms",           &pu_gnmms);
  ntTrkPU -> SetBranchAddress("gnintt1",         &pu_gnintt1);
  ntTrkPU -> SetBranchAddress("gnintt2",         &pu_gnintt2);
  ntTrkPU -> SetBranchAddress("gnintt3",         &pu_gnintt3);
  ntTrkPU -> SetBranchAddress("gnintt4",         &pu_gnintt4);
  ntTrkPU -> SetBranchAddress("gnintt5",         &pu_gnintt5);
  ntTrkPU -> SetBranchAddress("gnintt6",         &pu_gnintt6);
  ntTrkPU -> SetBranchAddress("gnintt7",         &pu_gnintt7);
  ntTrkPU -> SetBranchAddress("gnintt8",         &pu_gnintt8);
  ntTrkPU -> SetBranchAddress("gntpc",           &pu_gntpc);
  ntTrkPU -> SetBranchAddress("gnlmaps",         &pu_gnlmaps);
  ntTrkPU -> SetBranchAddress("gnlintt",         &pu_gnlintt);
  ntTrkPU -> SetBranchAddress("gnltpc",          &pu_gnltpc);
  ntTrkPU -> SetBranchAddress("gnlmms",          &pu_gnlmms);
  ntTrkPU -> SetBranchAddress("gpx",             &pu_gpx);
  ntTrkPU -> SetBranchAddress("gpy",             &pu_gpy);
  ntTrkPU -> SetBranchAddress("gpz",             &pu_gpz);
  ntTrkPU -> SetBranchAddress("gpt",             &pu_gpt);
  ntTrkPU -> SetBranchAddress("geta",            &pu_geta);
  ntTrkPU -> SetBranchAddress("gphi",            &pu_gphi);
  ntTrkPU -> SetBranchAddress("gvx",             &pu_gvx);
  ntTrkPU -> SetBranchAddress("gvy",             &pu_gvy);
  ntTrkPU -> SetBranchAddress("gvz",             &pu_gvz);
  ntTrkPU -> SetBranchAddress("gvt",             &pu_gvt);
  ntTrkPU -> SetBranchAddress("gfpx",            &pu_gfpx);
  ntTrkPU -> SetBranchAddress("gfpy",            &pu_gfpy);
  ntTrkPU -> SetBranchAddress("gfpz",            &pu_gfpz);
  ntTrkPU -> SetBranchAddress("gfx",             &pu_gfx);
  ntTrkPU -> SetBranchAddress("gfy",             &pu_gfy);
  ntTrkPU -> SetBranchAddress("gfz",             &pu_gfz);
  ntTrkPU -> SetBranchAddress("gembed",          &pu_gembed);
  ntTrkPU -> SetBranchAddress("gprimary",        &pu_gprimary);
  ntTrkPU -> SetBranchAddress("trackID",         &pu_trackID);
  ntTrkPU -> SetBranchAddress("px",              &pu_px);
  ntTrkPU -> SetBranchAddress("py",              &pu_py);
  ntTrkPU -> SetBranchAddress("pz",              &pu_pz);
  ntTrkPU -> SetBranchAddress("pt",              &pu_pt);
  ntTrkPU -> SetBranchAddress("eta",             &pu_eta);
  ntTrkPU -> SetBranchAddress("phi",             &pu_phi);
  ntTrkPU -> SetBranchAddress("deltapt",         &pu_deltapt);
  ntTrkPU -> SetBranchAddress("deltaeta",        &pu_deltaeta);
  ntTrkPU -> SetBranchAddress("deltaphi",        &pu_deltaphi);
  ntTrkPU -> SetBranchAddress("charge",          &pu_charge);
  ntTrkPU -> SetBranchAddress("quality",         &pu_quality);
  ntTrkPU -> SetBranchAddress("chisq",           &pu_chisq);
  ntTrkPU -> SetBranchAddress("ndf",             &pu_ndf);
  ntTrkPU -> SetBranchAddress("nhits",           &pu_nhits);
  ntTrkPU -> SetBranchAddress("layers",          &pu_layers);
  ntTrkPU -> SetBranchAddress("nmaps",           &pu_nmaps);
  ntTrkPU -> SetBranchAddress("nintt",           &pu_nintt);
  ntTrkPU -> SetBranchAddress("ntpc",            &pu_ntpc);
  ntTrkPU -> SetBranchAddress("nmms",            &pu_nmms);
  ntTrkPU -> SetBranchAddress("ntpc1",           &pu_ntpc1);
  ntTrkPU -> SetBranchAddress("ntpc11",          &pu_ntpc11);
  ntTrkPU -> SetBranchAddress("ntpc2",           &pu_ntpc2);
  ntTrkPU -> SetBranchAddress("ntpc3",           &pu_ntpc3);
  ntTrkPU -> SetBranchAddress("nlmaps",          &pu_nlmaps);
  ntTrkPU -> SetBranchAddress("nlintt",          &pu_nlintt);
  ntTrkPU -> SetBranchAddress("nltpc",           &pu_nltpc);
  ntTrkPU -> SetBranchAddress("nlmms",           &pu_nlmms);
  ntTrkPU -> SetBranchAddress("vertexID",        &pu_vertexID);
  ntTrkPU -> SetBranchAddress("vx",              &pu_vx);
  ntTrkPU -> SetBranchAddress("vy",              &pu_vy);
  ntTrkPU -> SetBranchAddress("vz",              &pu_vz);
  ntTrkPU -> SetBranchAddress("dca2d",           &pu_dca2d);
  ntTrkPU -> SetBranchAddress("dca2dsigma",      &pu_dca2dsigma);
  ntTrkPU -> SetBranchAddress("dca3dxy",         &pu_dca3dxy);
  ntTrkPU -> SetBranchAddress("dca3dxysigma",    &pu_dca3dxysigma);
  ntTrkPU -> SetBranchAddress("dca3dz",          &pu_dca3dz);
  ntTrkPU -> SetBranchAddress("dca3dzsigma",     &pu_dca3dzsigma);
  ntTrkPU -> SetBranchAddress("pcax",            &pu_pcax);
  ntTrkPU -> SetBranchAddress("pcay",            &pu_pcay);
  ntTrkPU -> SetBranchAddress("pcaz",            &pu_pcaz);
  ntTrkPU -> SetBranchAddress("nfromtruth",      &pu_nfromtruth);
  ntTrkPU -> SetBranchAddress("nwrong",          &pu_nwrong);
  ntTrkPU -> SetBranchAddress("ntrumaps",        &pu_ntrumaps);
  ntTrkPU -> SetBranchAddress("ntruintt",        &pu_ntruintt);
  ntTrkPU -> SetBranchAddress("ntrutpc",         &pu_ntrutpc);
  ntTrkPU -> SetBranchAddress("ntrumms",         &pu_ntrumms);
  ntTrkPU -> SetBranchAddress("ntrutpc1",        &pu_ntrutpc1);
  ntTrkPU -> SetBranchAddress("ntrutpc11",       &pu_ntrutpc11);
  ntTrkPU -> SetBranchAddress("ntrutpc2",        &pu_ntrutpc2);
  ntTrkPU -> SetBranchAddress("ntrutpc3",        &pu_ntrutpc3);
  ntTrkPU -> SetBranchAddress("layersfromtruth", &pu_layersfromtruth);
  ntTrkPU -> SetBranchAddress("nhittpcall",      &pu_nhittpcall);
  ntTrkPU -> SetBranchAddress("nhittpcin",       &pu_nhittpcin);
  ntTrkPU -> SetBranchAddress("nhittpcmid",      &pu_nhittpcmid);
  ntTrkPU -> SetBranchAddress("nhittpcout",      &pu_nhittpcout);
  ntTrkPU -> SetBranchAddress("nclusall",        &pu_nclusall);
  ntTrkPU -> SetBranchAddress("nclustpc",        &pu_nclustpc);
  ntTrkPU -> SetBranchAddress("nclusintt",       &pu_nclusintt);
  ntTrkPU -> SetBranchAddress("nclusmaps",       &pu_nclusmaps);
  ntTrkPU -> SetBranchAddress("nclusmms",        &pu_nclusmms);
  cout << "      Set branch addresses." << endl;
  return;

}  // end 'InitTuples()'



void STrackCutStudy::SaveHists() {

  // create output directories
  TDirectory *dOut[NType];
  for (UInt_t iDir = 0; iDir < NType; iDir++) {
    fOut       -> cd();
    dOut[iDir] = (TDirectory*) fOut -> mkdir(sTrkNames[iDir].Data());
  }
  cout << "      Made directories." << endl;

  // save histograms
  for (int iType = 0; iType < NType; iType++) {
    dOut[iType] -> cd();
    for (size_t iTrkVar = 0; iTrkVar < NTrkVar; iTrkVar++) {
      hTrkVar[iType][iTrkVar]         -> Write();
      hTrkVarDiff[iType][iTrkVar]     -> Write();
      hTrkVarFrac[iType][iTrkVar]     -> Write();
      hTrkVarVsNTpc[iType][iTrkVar]   -> Write();
      hTrkVarVsPtReco[iType][iTrkVar] -> Write();
      hTrkVarVsPtTrue[iType][iTrkVar] -> Write();
      hTrkVarVsPtFrac[iType][iTrkVar] -> Write();
    }
    for (size_t iPhysVar = 0; iPhysVar < NPhysVar; iPhysVar++) {
      hPhysVar[iType][iPhysVar]         -> Write();
      hPhysVarDiff[iType][iPhysVar]     -> Write();
      hPhysVarFrac[iType][iPhysVar]     -> Write();
      hPhysVarVsNTpc[iType][iPhysVar]   -> Write();
      hPhysVarVsPtReco[iType][iPhysVar] -> Write();
      hPhysVarVsPtTrue[iType][iPhysVar] -> Write();
      hPhysVarVsPtFrac[iType][iPhysVar] -> Write();
    }
  }  // end type loop

  cout << "      Saved histograms." << endl;
  return;

}  // end 'SaveHists()'

// end ------------------------------------------------------------------------
