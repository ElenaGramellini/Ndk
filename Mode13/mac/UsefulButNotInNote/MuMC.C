#include "TInterpreter.h"
#include "TCanvas.h"
#include "TSystem.h"
#include "TFile.h"
#include "TLegend.h"
#include "TH2.h"
#include "TProfile.h"
#include "TH1F.h"
#include "TH1.h"
#include "TNtuple.h"
#include "TPaveLabel.h"
#include "TPaveText.h"
#include "TFrame.h"
#include "TSystem.h"
#include <iostream>

void MuMC( )
{
  TCut CPdg    = "_tracks_Pdg==13"; 

  TFile   *inFileMC;
  //gStyle->SetOptStat(0);  
  inFileMC = new TFile("ERAnaOneToOneMuMC.root");
  inFileMC->cd();
  TTree* _algoMuMC_tree = (TTree*)(inFileMC->Get("_ana1to1_tree"));


  TCanvas *C1     = new TCanvas("C1","C1",600,600);
  C1->cd();
  
  auto cut2 = new TH1D("cut2","Muon deposited Energy;  E_{#mu} [MeV];Count",20,0,800);
  _algoMuMC_tree->Draw("_tracks_DepEn>>cut2",CPdg,"");
  //  gStyle->SetOptStat(0);
  cut2->SetLineWidth(3);
  cut2->SetLineColor(kBlue-1);
  cut2->GetYaxis()->SetTitleOffset(1.5);

  C1->SaveAs("MuMCEn.png");  
  //  TFile   *inFileReco;
  //  inFileReco = new TFile("ERAnaOneToOneMuMC.root");
  //  inFileReco->cd();
  //  TTree* _algoMuReco_tree = (TTree*)(inFileReco->Get("_ProtonSel_tree"));
}


