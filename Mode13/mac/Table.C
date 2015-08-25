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

void Table( )
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
  auto cut1 = new TH1D("Proton","Total deposited Energy;  E_{#mu} [MeV];Count",30,0,1200);
  _algoMCP_tree->Draw("_proton_En>>Proton","","norm");
  std::cout<<"Cominatorics total........................ "<<cut1->GetEntries()<<"\n";
  _algoMCP_tree->Draw("_proton_En>>Proton",cCosm1,"norm"); 
  std::cout<<"Passing track filter...................... "<<cut1->GetEntries()<<"\n";                      
  _algoMCP_tree->Draw("_proton_En>>Proton",cCosm1&&crazyEnTrack,"norm");
  std::cout<<"Track deposited energy.................... "<<cut1->GetEntries()<<"\n";                    
  _algoMCP_tree->Draw("_proton_En>>Proton",cCosm1&&crazyEnTrack&&crazyEnShower,"norm");
  std::cout<<"Shower deposited energy................... "<<cut1->GetEntries()<<"\n";                                       
  _algoMCP_tree->Draw("_proton_En>>Proton",cCosm1&&crazyEnTrack&&crazyEnShower&&cOA,"norm");
  std::cout<<"OpeningAngle.............................. "<<cut1->GetEntries()<<"\n";                    
  _algoMCP_tree->Draw("_proton_En>>Proton",cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin,"norm");
  std::cout<<"Impact parameter.......................... "<<cut1->GetEntries()<<"\n";                                              
  _algoMCP_tree->Draw("_proton_En>>Proton",cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin&&cdisEnMin,"norm");
  std::cout<<"Distance between first energy depositions. "<<cut1->GetEntries()<<"\n";                     
  _algoMCP_tree->Draw("_proton_En>>Proton",cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin&&cdisEnMin&&cPdgSSS,"norm");
  std::cout<<"Track ID.................................. "<<cut1->GetEntries()<<"\n";                                            
  _algoMCP_tree->Draw("_proton_En>>Proton",cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin&&cdisEnMin&&cPdgSSS&&cPdgSh,"norm");
  std::cout<<"Shower ID................................. "<<cut1->GetEntries()<<"\n";                                           
  _algoMCP_tree->Draw("_proton_En>>Proton",cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin&&cdisEnMin&&cPdgSSS&&cPdgSh&&cShowerEn,"norm");
  std::cout<<"Shower deposited energy................... "<<cut1->GetEntries()<<"\n";                                       
  _algoMCP_tree->Draw("_proton_En>>Proton",cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin&&cdisEnMin&&cProtonP&&cPdgSSS&&cPdgSh&&cShowerEn,"norm");
  std::cout<<"PTot...................................... "<<cut1->GetEntries()<<"\n";                     

  std::cout<<"\n\n";

  _algoMCP_tree->Draw("_proton_En>>Proton",CPdg,"norm");
  std::cout<<"Total..................................... "<<cut1->GetEntries()<<"\n";
  _algoMCP_tree->Draw("_proton_En>>Proton",CPdg&&cCosm1,"norm"); 
  std::cout<<"Passing track filter...................... "<<cut1->GetEntries()<<"\n";                      
  _algoMCP_tree->Draw("_proton_En>>Proton",CPdg&&cCosm1&&crazyEnTrack,"norm");
  std::cout<<"Track deposited energy.................... "<<cut1->GetEntries()<<"\n";                    
  _algoMCP_tree->Draw("_proton_En>>Proton",CPdg&&cCosm1&&crazyEnTrack&&crazyEnShower,"norm");
  std::cout<<"Shower deposited energy................... "<<cut1->GetEntries()<<"\n";                                       
  _algoMCP_tree->Draw("_proton_En>>Proton",CPdg&&cCosm1&&crazyEnTrack&&crazyEnShower&&cOA,"norm");
  std::cout<<"OpeningAngle.............................. "<<cut1->GetEntries()<<"\n";                    
  _algoMCP_tree->Draw("_proton_En>>Proton",CPdg&&cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin,"norm");
  std::cout<<"Impact parameter.......................... "<<cut1->GetEntries()<<"\n";                                              
  _algoMCP_tree->Draw("_proton_En>>Proton",CPdg&&cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin&&cdisEnMin,"norm");
  std::cout<<"Distance between first energy depositions. "<<cut1->GetEntries()<<"\n";                     
  _algoMCP_tree->Draw("_proton_En>>Proton",CPdg&&cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin&&cdisEnMin&&cPdgSSS,"norm");
  std::cout<<"Track ID.................................. "<<cut1->GetEntries()<<"\n";                                            
  _algoMCP_tree->Draw("_proton_En>>Proton",CPdg&&cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin&&cdisEnMin&&cPdgSSS&&cPdgSh,"norm");
  std::cout<<"Shower ID................................. "<<cut1->GetEntries()<<"\n";                                           
  _algoMCP_tree->Draw("_proton_En>>Proton",CPdg&&cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin&&cdisEnMin&&cPdgSSS&&cPdgSh&&cShowerEn,"norm");
  std::cout<<"Shower deposited energy................... "<<cut1->GetEntries()<<"\n";                                       
  _algoMCP_tree->Draw("_proton_En>>Proton",CPdg&&cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin&&cdisEnMin&&cProtonP&&cPdgSSS&&cPdgSh&&cShowerEn,"norm");
  std::cout<<"PTot...................................... "<<cut1->GetEntries()<<"\n";                     

  std::cout<<"\n\n";
  TCut CPdgPReco    = ""; 
  TFile   *inFileRecoP;
  inFileRecoP = new TFile("Whole13Reco.root");
  inFileRecoP->cd();
  TTree* _algoPReco_tree = (TTree*)(inFileRecoP->Get("_ProtonSel_tree"));

  std::cout<<"Reco\n";
  TCanvas *C1R     = new TCanvas("C1R","C1R",600,600);
  C1R->cd();  
  auto cut2 = new TH1D("Proton","Total deposited Energy;  E_{#mu} [MeV];Count",30,0,1200);
  _algoPReco_tree->Draw("_proton_En>>Proton","","norm");
  std::cout<<"Cominatorics total........................ "<<cut2->GetEntries()<<"\n";
  _algoPReco_tree->Draw("_proton_En>>Proton",cCosm1,"norm"); 
  std::cout<<"Passing track filter...................... "<<cut2->GetEntries()<<"\n";                      
  _algoPReco_tree->Draw("_proton_En>>Proton",cCosm1&&crazyEnTrack,"norm");
  std::cout<<"Track deposited energy.................... "<<cut2->GetEntries()<<"\n";                    
  _algoPReco_tree->Draw("_proton_En>>Proton",cCosm1&&crazyEnTrack&&crazyEnShower,"norm");
  std::cout<<"Shower deposited energy................... "<<cut2->GetEntries()<<"\n";                                       
  _algoPReco_tree->Draw("_proton_En>>Proton",cCosm1&&crazyEnTrack&&crazyEnShower&&cOA,"norm");
  std::cout<<"OpeningAngle.............................. "<<cut2->GetEntries()<<"\n";                    
  _algoPReco_tree->Draw("_proton_En>>Proton",cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin,"norm");
  std::cout<<"Impact parameter.......................... "<<cut2->GetEntries()<<"\n";                                              
  _algoPReco_tree->Draw("_proton_En>>Proton",cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin&&cdisEnMin,"norm");
  std::cout<<"Distance between first energy depositions. "<<cut2->GetEntries()<<"\n";                     
  _algoPReco_tree->Draw("_proton_En>>Proton",cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin&&cdisEnMin&&cPdgSSS,"norm");
  std::cout<<"Track ID.................................. "<<cut2->GetEntries()<<"\n";                                            
  _algoPReco_tree->Draw("_proton_En>>Proton",cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin&&cdisEnMin&&cPdgSSS&&cPdgSh,"norm");
  std::cout<<"Shower ID................................. "<<cut2->GetEntries()<<"\n";                                           
  _algoPReco_tree->Draw("_proton_En>>Proton",cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin&&cdisEnMin&&cPdgSSS&&cPdgSh&&cShowerEn,"norm");
  std::cout<<"Shower deposited energy................... "<<cut2->GetEntries()<<"\n";                                       
  _algoPReco_tree->Draw("_proton_En>>Proton",cCosm1&&crazyEnTrack&&crazyEnShower&&cOA&&cIPMin&&cdisEnMin&&cProtonP&&cPdgSSS&&cPdgSh&&cShowerEn,"norm");
  std::cout<<"PTot...................................... "<<cut2->GetEntries()<<"\n";        
}


