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

void CosmicsPlotsForTechNote( )
{
  TCut CPdg    = "_shower_Pdg==22&&_track_Pdg==13"; 


  //########## Proton Begin ##############
  //////////// Cosmi Cuts ///////////
  TCut xCont =  "((_track_x>3.    || _track_x< 250.) && (_track_xEnd>3.   ||_track_xEnd< 250.))";
  TCut yCont =  "((_track_y>-113. || _track_y< 113.) && (_track_yEnd>-113.||_track_yEnd< 113.))";
  TCut zCont =  "((_track_z>3.    || _track_z<1050.) && (_track_zEnd>3.   ||_track_zEnd<1050.))";
  TCut cCosm1        = xCont&&yCont&&zCont;
  TCut crazyEnTrack   = "_track_EnRange<2000";
  TCut crazyEnShower  = "_shower_DepEn<2000";
  //////////// Calo and Pid
  TCut cPdgSSS           = "_track_Pdg==13||_track_Pdg==211"; 
  TCut cPdgSh          = "_shower_Pdg==22"; 
  TCut cShowerEn      = "_shower_DepEn>65&&_shower_DepEn<700";
  TCut cProtonP  = "_proton_px<400&&_proton_px>-400&&_proton_py<400&&_proton_py>-400&&_proton_pz<400&&_proton_pz>-400";

  ////////////      Geom Cuts      ///////////
  TCut cOA            = "_proton_openingAngle>1.2||_proton_openingAngle<0.5";
  TCut cIPMin         = "_proton_IP<15";
  TCut cdisEnMin      = "_protonEnDepDistance<70";


  TFile   *inFileMCP;
  inFileMCP = new TFile("Cosmic13MC.root");
  inFileMCP->cd();
  TTree* _algoMCP_tree = (TTree*)(inFileMCP->Get("_ProtonSel_tree"));

  TCanvas *C1     = new TCanvas("C1","C1",600,600);
  C1->cd();  
  auto cut1 = new TH1D("Proton","Total deposited Energy;  E_{#mu} [MeV];Normalized Count",30,0,1200);
  _algoMCP_tree->Draw("_proton_En>>Proton",CPdg,"norm");
  cut1->SetLineWidth(3);
  cut1->SetLineColor(kBlue-1);
  cut1->GetYaxis()->SetTitleOffset(1.5);
  C1->SaveAs("ComsProtonMCEnStat.png");  
  gStyle->SetOptStat(0);  
  C1->SaveAs("ComsProtonMCEn.png"); 

 
  TCut CPdgPReco    = ""; 
  TFile   *inFileRecoP;
  inFileRecoP = new TFile("Cosmic13Reco.root");
  inFileRecoP->cd();
  TTree* _algoPReco_tree = (TTree*)(inFileRecoP->Get("_ProtonSel_tree"));

  TCanvas *C1R     = new TCanvas("C1R","C1R",600,600);
  TCanvas *C11SameStat     = new TCanvas("C11SameStat","C11SameStat",600,600);

  C1R->cd();  
  gStyle->SetOptStat(1111);  
  auto cut1R = new TH1D("ProtonReco","Total deposited Energy;  E_{#mu} [MeV]; Normalized Count",30,0,1200);
  gStyle->SetOptStat(1111); 
  _algoPReco_tree->Draw("_proton_En>>ProtonReco",CPdgPReco,"norm");
  cut1R->SetLineWidth(3);
  cut1R->SetLineColor(kRed+2);
  cut1R->GetYaxis()->SetTitleOffset(1.5); 
  C1R->SaveAs("ComsProtonRecoEnNoCutStat.png");
  gStyle->SetOptStat(0);  
  C1R->SaveAs("ComsProtonRecoEnNoCut.png");
  C11SameStat->cd();  
  gStyle->SetOptStat(1111);
  cut1->Draw("");
  cut1R->Draw("sames");
  C11SameStat->SaveAs("ComsProtonSameNoCutStat.png");
  gStyle->SetOptStat(0);
  cut1->Draw("");
  cut1R->Draw("sames");
  TLegend *leg1 = new TLegend(0.15,0.65,0.45,0.85);
  leg1->SetFillColor(0);
  leg1->SetHeader("Normalized Distributions");
  leg1->AddEntry(cut1  ,"MC Energy Dep"  ,"l");
  leg1->AddEntry(cut1R,"3D Reco" ,"l");
  leg1->Draw("same");
  C11SameStat->SaveAs("ComsProtonSameNoCut.png");
  
  C1R->cd();  
  CPdgPReco    = cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin&&cdisEnMin;
  gStyle->SetOptStat(1111); 
  _algoPReco_tree->Draw("_proton_En>>ProtonReco",CPdgPReco,"norm");
  C1R->SaveAs("ComsProtonRecoEnGeoStat.png"); 
  gStyle->SetOptStat(0);  
  _algoPReco_tree->Draw("_proton_En>>ProtonReco",CPdgPReco,"norm");
  C1R->SaveAs("ComsProtonRecoEnGeo.png");
  C11SameStat->cd();  
  gStyle->SetOptStat(1111);
  cut1->Draw("");
  cut1R->Draw("sames");
  C11SameStat->SaveAs("ComsProtonSameGeoStat.png");
  gStyle->SetOptStat(0);
  cut1->Draw("");
  cut1R->Draw("sames");
  leg1->Draw("same");
  C11SameStat->SaveAs("ComsProtonSameGeo.png");


  C1R->cd();  
  CPdgPReco    = cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin&&cdisEnMin&&cProtonP&&cPdgSSS&&cPdgSh&&cShowerEn;
  gStyle->SetOptStat(1111);  
  _algoPReco_tree->Draw("_proton_En>>ProtonReco",CPdgPReco,"norm");
  C1R->SaveAs("ComsProtonRecoEnGeoAndEnStat.png");  
  gStyle->SetOptStat(0);  
  _algoPReco_tree->Draw("_proton_En>>ProtonReco",CPdgPReco,"norm");
  C1R->SaveAs("ComsProtonRecoEnGeoAndEn.png");  
  C11SameStat->cd();  
  gStyle->SetOptStat(1111);
  cut1->Draw("");
  cut1R->Draw("sames");
  C11SameStat->SaveAs("ComsProtonSameGeoAndEnStat.png");
  gStyle->SetOptStat(0);
  cut1->Draw("");
  cut1R->Draw("same");
  leg1->Draw("same");
  C11SameStat->SaveAs("ComsProtonSameGeoAndEn.png");  
}


