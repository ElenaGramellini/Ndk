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

void SignalPlotsForTechNote( )
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
  inFileMCP = new TFile("Whole13MC.root");
  inFileMCP->cd();
  TTree* _algoMCP_tree = (TTree*)(inFileMCP->Get("_ProtonSel_tree"));

  TCanvas *C1     = new TCanvas("C1","C1",600,600);
  C1->cd();  
  auto cut1 = new TH1D("Proton","Total deposited Energy;  E_{#mu} [MeV];Normalized Count",30,0,1200);
  _algoMCP_tree->Draw("_proton_En>>Proton",CPdg,"norm");
  cut1->SetLineWidth(3);
  cut1->SetLineColor(kBlue-1);
  cut1->GetYaxis()->SetTitleOffset(1.5);
  C1->SaveAs("ProtonMCEnStat.png");  
  gStyle->SetOptStat(0);  
  C1->SaveAs("ProtonMCEn.png"); 

 
  TCut CPdgPReco    = ""; 
  TFile   *inFileRecoP;
  inFileRecoP = new TFile("Whole13Reco.root");
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
  C1R->SaveAs("ProtonRecoEnNoCutStat.png");
  gStyle->SetOptStat(0);  
  C1R->SaveAs("ProtonRecoEnNoCut.png");
  C11SameStat->cd();  
  gStyle->SetOptStat(1111);
  cut1->Draw("");
  cut1R->Draw("sames");
  C11SameStat->SaveAs("ProtonSameNoCutStat.png");
  gStyle->SetOptStat(0);
  cut1->Draw("");
  cut1R->Draw("sames");
  TLegend *leg1 = new TLegend(0.15,0.65,0.45,0.85);
  leg1->SetFillColor(0);
  leg1->SetHeader("Normalized Distributions");
  leg1->AddEntry(cut1  ,"MC Energy Dep"  ,"l");
  leg1->AddEntry(cut1R,"3D Reco" ,"l");
  leg1->Draw("same");
  C11SameStat->SaveAs("ProtonSameNoCut.png");
  
  C1R->cd();  
  CPdgPReco    = cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin&&cdisEnMin;
  gStyle->SetOptStat(1111); 
  _algoPReco_tree->Draw("_proton_En>>ProtonReco",CPdgPReco,"norm");
  C1R->SaveAs("ProtonRecoEnGeoStat.png"); 
  gStyle->SetOptStat(0);  
  _algoPReco_tree->Draw("_proton_En>>ProtonReco",CPdgPReco,"norm");
  C1R->SaveAs("ProtonRecoEnGeo.png");
  C11SameStat->cd();  
  gStyle->SetOptStat(1111);
  cut1->Draw("");
  cut1R->Draw("sames");
  C11SameStat->SaveAs("ProtonSameGeoStat.png");
  gStyle->SetOptStat(0);
  cut1->Draw("");
  cut1R->Draw("sames");
  leg1->Draw("same");
  C11SameStat->SaveAs("ProtonSameGeo.png");


  C1R->cd();  
  CPdgPReco    = cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin&&cdisEnMin&&cProtonP&&cPdgSSS&&cPdgSh&&cShowerEn;
  gStyle->SetOptStat(1111);  
  _algoPReco_tree->Draw("_proton_En>>ProtonReco",CPdgPReco,"norm");
  C1R->SaveAs("ProtonRecoEnGeoAndEnStat.png");  
  gStyle->SetOptStat(0);  
  _algoPReco_tree->Draw("_proton_En>>ProtonReco",CPdgPReco,"norm");
  C1R->SaveAs("ProtonRecoEnGeoAndEn.png");  
  C11SameStat->cd();  
  gStyle->SetOptStat(1111);
  cut1->Draw("");
  cut1R->Draw("sames");
  C11SameStat->SaveAs("ProtonSameGeoAndEnStat.png");
  gStyle->SetOptStat(0);
  cut1->Draw("");
  cut1R->Draw("same");
  leg1->Draw("same");
  C11SameStat->SaveAs("ProtonSameGeoAndEn.png");
  /*
  CPdgPReco = "";
  gStyle->SetOptStat(0);  
  TCanvas *C11R     = new TCanvas("C11R","C11R",600,600);
  C11R->cd();  
  auto cut11R = new TH1D("cut11R","Total deposited Energy;  E_{#mu} [MeV];Count",30,0,1200);
  _algoPReco_tree->Draw("_proton_En>>cut11R",CPdgPReco,"");
  cut11R->SetLineWidth(3);
  cut11R->SetLineColor(kRed+2);
  cut11R->GetYaxis()->SetTitleOffset(1.5);
  //C11R->SaveAs("ProtonRecoEnNoCut.png");  
  */

  

  //  auto  ps = (TPaveStats *)cut1R->GetListOfFunctions()->FindObject("stats");
  //  ps->SetX1NDC(0.15);
  //  ps->SetX2NDC(0.55);


  /*
  gStyle->SetOptStat(0); 
  TCanvas *C11Same     = new TCanvas("C11Same","C11Same",600,600);
  C11Same->cd();
  cut11->Draw("");
  cut11R->Draw("same");
  C11Same->SaveAs("ProtonSame.png");  
  */

  //########## Proton End ##############
  gStyle->SetOptStat(1111);
  //########## Gamma Begin ##############
  TCut CPdg3S    = "_shower_Pdg==22"; 
  TFile   *inFileMCG;
  gStyle->SetOptStat(1111);
  inFileMCG = new TFile("Whole13MC.root");
  inFileMCG->cd();
  TTree* _algoMCG_tree = (TTree*)(inFileMCG->Get("_ProtonSel_tree"));

  TCanvas *C2     = new TCanvas("C2","C2",600,600);
  C2->cd();
  auto cut2 = new TH1D("Gamma","Gamma deposited Energy;  E_{#mu} [MeV];Normalized Count",20,0,800);
  _algoMCG_tree->Draw("_shower_DepEn>>Gamma",CPdg3S,"norm");
  cut2->SetLineWidth(3);
  cut2->SetLineColor(kBlue-1);
  gStyle->SetOptStat(1111);
  cut2->GetYaxis()->SetTitleOffset(1.5);
  C2->SaveAs("GammaMCEnStat.png");  
  gStyle->SetOptStat(0);  
  _algoMCG_tree->Draw("_shower_DepEn>>Gamma",CPdg3S,"norm");
  C2->SaveAs("GammaMCEn.png");  

  
  TFile   *inFileRecoGamma;
  inFileRecoGamma = new TFile("Whole13Reco.root");
  inFileRecoGamma->cd();
  TTree* _algoGammaReco_tree = (TTree*)(inFileRecoGamma->Get("_ProtonSel_tree"));


  gStyle->SetOptStat(1111); 
  TCanvas *C2R     = new TCanvas("C2R","C2R",600,600);
  C2R->cd();  
  auto cut2R = new TH1D("GammaReco","Gamma deposited Energy;  E_{#mu} [MeV];Normalized Count",20,0,800); 
  C2R->cd(); 
  _algoGammaReco_tree->Draw("_shower_DepEn>>GammaReco",cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin&&cdisEnMin&&cProtonP&&cPdgSSS&&cPdgSh&&cShowerEn,"norm");
  cut2R->SetLineWidth(3);
  cut2R->SetLineColor(kRed+2);
  cut2R->GetYaxis()->SetTitleOffset(1.5);
  C2R->SaveAs("GammaRecoEnStat.png");  
  gStyle->SetOptStat(0);  
  _algoGammaReco_tree->Draw("_shower_DepEn>>GammaReco",cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin&&cdisEnMin&&cProtonP&&cPdgSSS&&cPdgSh&&cShowerEn,"norm");
  C2R->SaveAs("GammaRecoEn.png");  
  

  TCanvas *C22SameStat     = new TCanvas("C22SameStat","C22SameStat",600,600);
  gStyle->SetOptStat(1111);
  C22SameStat->cd();  
  cut2->Draw("");
  cut2R->Draw("sames");
  //  auto  ps = (TPaveStats *)cut3R->GetListOfFunctions()->FindObject("stats");
  //  ps->SetX1NDC(0.15);
  //  ps->SetX2NDC(0.55);
  C22SameStat->SaveAs("GammaSameStat.png");

  gStyle->SetOptStat(0); 
  TCanvas *C22Same     = new TCanvas("C22Same","C22Same",600,600);
  C22Same->cd();
  cut2->Draw("");
  cut2R->Draw("same");
  TLegend *leg2 = new TLegend(0.15,0.65,0.45,0.85);
  leg2->SetFillColor(0);
  leg2->SetHeader("Normalized Distributions");
  leg2->AddEntry(cut2  ,"MC Energy Dep"  ,"l");
  leg2->AddEntry(cut2R,"3D Reco" ,"l");
  leg2->Draw("same");
  C22Same->SaveAs("GammaSame.png"); 

  //########## Gamma End ##############



  //########## Muon Begin ##############
  gStyle->SetOptStat(1111);
  TCut CPdgT    = "_tracks_Pdg==13"; 

  TFile   *inFileMCM;
  inFileMCM = new TFile("ERAnaOneToOneMuMC.root");
  inFileMCM->cd();
  TTree* _algoMCM_tree = (TTree*)(inFileMCM->Get("_ana1to1_tree"));

  TCanvas *C3     = new TCanvas("C3","C3",600,600);
  C3->cd();
  auto cut3 = new TH1D("MuMC","Muon deposited Energy;  E_{#mu} [MeV];Normalized Count",20,0,800);
  _algoMCM_tree->Draw("_tracks_DepEnFromLength>>MuMC",CPdgT,"norm");
  cut3->SetLineWidth(3);
  cut3->SetLineColor(kBlue-1);
  cut3->GetYaxis()->SetTitleOffset(1.5);
  C3->SaveAs("MuonMCEnStat.png");  

  gStyle->SetOptStat(0);  
  TCanvas *C33     = new TCanvas("C33","C33",600,600);
  C33->cd();  
  auto cut33 = new TH1D("cut33","Muon deposited Energy;  E_{#mu} [MeV];Normalized Count",20,0,800);
  _algoMCM_tree->Draw("_tracks_DepEnFromLength>>cut33",CPdgT,"norm");
  cut33->SetLineWidth(3);
  cut33->SetLineColor(kBlue-1);
  cut33->GetYaxis()->SetTitleOffset(1.5);
  C33->SaveAs("MuonMCEn.png");  


  TCut CPdgTReco    = "_tracks_Pdg==13||_tracks_Pdg==211"; 
  TFile   *inFileRecoMu;
  gStyle->SetOptStat(1111);  
  inFileRecoMu = new TFile("ERAnaOneToOneMuReco.root");
  inFileRecoMu->cd();
  TTree* _algoMuReco_tree = (TTree*)(inFileRecoMu->Get("_ana1to1_tree"));

  TCanvas *C3R     = new TCanvas("C3R","C3R",600,600);
  C3R->cd();  
  auto cut3R = new TH1D("MuReco","Muon deposited Energy;  E_{#mu} [MeV];Normalized Count",20,0,800);
  _algoMuReco_tree->Draw("_tracks_DepEnFromLength>>MuReco",CPdgTReco,"norm");
  cut3R->SetLineWidth(3);
  cut3R->SetLineColor(kRed+2);
  cut3R->GetYaxis()->SetTitleOffset(1.5);
  C3R->SaveAs("MuRecoEnStat.png");  

  gStyle->SetOptStat(0);  
  TCanvas *C33R     = new TCanvas("C33R","C33R",600,600);
  C33R->cd();  
  auto cut33R = new TH1D("cut33R","Muon deposited Energy;  E_{#mu} [MeV];Normalized Count",20,0,800);
  _algoMuReco_tree->Draw("_tracks_DepEnFromLength>>cut33R",CPdgTReco,"norm");
  cut33R->SetLineWidth(3);
  cut33R->SetLineColor(kRed+2);
  cut33R->GetYaxis()->SetTitleOffset(1.5);
  C33R->SaveAs("MuRecoEn.png");  

  TCanvas *C33SameStat     = new TCanvas("C33SameStat","C33SameStat",600,600);
  gStyle->SetOptStat(1111);
  C33SameStat->cd();  
  cut3->Draw("");
  cut3R->Draw("sames");
  //  auto  ps = (TPaveStats *)cut3R->GetListOfFunctions()->FindObject("stats");
  //  ps->SetX1NDC(0.15);
  //  ps->SetX2NDC(0.55);
  C33SameStat->SaveAs("MuonSameMuStat.png");

  gStyle->SetOptStat(0); 
  TCanvas *C33Same     = new TCanvas("C33Same","C33Same",600,600);
  C33Same->cd();
  cut33->Draw("");
  cut33R->Draw("same");
  TLegend *leg3 = new TLegend(0.50,0.65,0.80,0.85);
  leg3->SetFillColor(0);
  leg3->SetHeader("Normalized Distributions");
  leg3->AddEntry(cut3  ,"MC Energy Dep"  ,"l");
  leg3->AddEntry(cut3R,"3D Reco" ,"l");
  leg3->Draw("same");
  C33Same->SaveAs("MuonSame.png");  
  
}


