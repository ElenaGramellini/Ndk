#ifndef ERTOOL_ERALGOMU_CXX
#define ERTOOL_ERALGOMU_CXX

#include "ERAlgoMu.h"

namespace ertool {
  int crazy = 0;
  int zero  = 0;

  ERAlgoMu::ERAlgoMu(const std::string& name) 
  : AlgoBase(name)
  , _algoMu_tree(nullptr)
  {
    // histogram to hold the energy of each reconstructed michel electron
    
    // set verbosity to be off by default
    _verbose = false;
  }

  void ERAlgoMu::Reset()
  {   }

  void ERAlgoMu::AcceptPSet(const ::fcllite::PSet& cfg)
  {
    // Load singleE params
    //_alg_emp.AcceptPSet(cfg);

    return;
  }

  void ERAlgoMu::ProcessBegin()
  {

    if (_algoMu_tree) { delete _algoMu_tree; }
    _algoMu_tree = new TTree("_algoMu_tree","algoMu Tree");
    _algoMu_tree->Branch("_mu_En"    ,&_mu_En    ,"_mu_En/D     ");
    _algoMu_tree->Branch("_mu_DepEn" ,&_mu_DepEn ,"_mu_DepEn/D  ");
    _algoMu_tree->Branch("_mu_Mom"   ,&_mu_Mom   ,"_mu_Mom/D    ");
    _algoMu_tree->Branch("_mu_px"    ,&_mu_px    ,"_mu_px/D     ");
    _algoMu_tree->Branch("_mu_py"    ,&_mu_py    ,"_mu_py/D     ");
    _algoMu_tree->Branch("_mu_pz"    ,&_mu_pz    ,"_mu_pz/D     ");

    _algoMu_tree->Branch("_mu_x"     ,&_mu_x     ,"_mu_x/D      ");
    _algoMu_tree->Branch("_mu_y"     ,&_mu_y     ,"_mu_y/D      ");
    _algoMu_tree->Branch("_mu_z"     ,&_mu_z     ,"_mu_z/D      ");

    _algoMu_tree->Branch("_mu_xEnd"  ,&_mu_xEnd  ,"_mu_xEnd/D   ");
    _algoMu_tree->Branch("_mu_yEnd"  ,&_mu_yEnd  ,"_mu_yEnd/D   ");
    _algoMu_tree->Branch("_mu_zEnd"  ,&_mu_zEnd  ,"_mu_zEnd/D   ");

    _algoMu_tree->Branch("_mu_EnMPS" ,&_mu_EnMPS ,"_mu_EnMPS/D  ");
    _algoMu_tree->Branch("_mu_MomMPS",&_mu_MomMPS,"_mu_MomMPS/D ");
    _algoMu_tree->Branch("_mu_pxMPS" ,&_mu_pxMPS ,"_mu_pxMPS/D  ");
    _algoMu_tree->Branch("_mu_pyMPS" ,&_mu_pyMPS ,"_mu_pyMPS/D  ");
    _algoMu_tree->Branch("_mu_pzMPS" ,&_mu_pzMPS ,"_mu_pzMPS/D  ");
    _algoMu_tree->Branch("_mu_leng"  ,&_mu_leng  ,"_mu_leng/D  ");
    
    return;
  }

  bool ERAlgoMu::Reconstruct(const EventData &data, ParticleGraph& graph)
  {
    auto datacpy = data;
    int Pdg = -1; 
    // Loop through Particles associated with a track
    for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){
      
      // get track object
      auto const& particleFromDataP = graph.GetParticle(t);
      auto const& track = datacpy.Track(particleFromDataP.RecoID());
      
      if ((track._pid_score[Track::kProton]<track._pid_score[Track::kPion])&&
	  (track._pid_score[Track::kProton]<track._pid_score[Track::kKaon])&&
	  (track._pid_score[Track::kProton]<track._pid_score[Track::kMuon]))      Pdg = 2212;
      
      if ((track._pid_score[Track::kPion]<track._pid_score[Track::kProton])&&
	  (track._pid_score[Track::kPion]<track._pid_score[Track::kKaon])&&
	  (track._pid_score[Track::kPion]<track._pid_score[Track::kMuon]))        Pdg = 211; 
      
      if ((track._pid_score[Track::kKaon]<track._pid_score[Track::kProton])&&
	  (track._pid_score[Track::kKaon]<track._pid_score[Track::kPion])&&
	  (track._pid_score[Track::kKaon]<track._pid_score[Track::kMuon]))        Pdg = 321;

      if ((track._pid_score[Track::kMuon]<track._pid_score[Track::kProton])&&
	  (track._pid_score[Track::kMuon]<track._pid_score[Track::kPion])&&
	  (track._pid_score[Track::kMuon]<track._pid_score[Track::kKaon]))        Pdg = 13;      
      
      
      // track deposited energy
      double Edep = track._energy;
      //      if (Edep < 0    ) continue;
      //if (Edep > 20000) continue;
      double lenght = track.Length();

      // track direction
      geoalgo::Vector_t Dir = (track[1]-track[0]);
      Dir.Normalize();
      
      double mass = ParticleMass(Pdg);
      double Energy  = Edep + mass;
      double Mom_Mag = sqrt( Energy*Energy - mass*mass );

      geoalgo::Vector_t Mom = Dir *  Mom_Mag; 

      _mu_En    = Energy  ;
      _mu_DepEn = Edep    ;
      _mu_Mom   = Mom_Mag ;
      _mu_px    = Mom[0]  ;
      _mu_py    = Mom[1]  ;
      _mu_pz    = Mom[2]  ;

      _mu_x     = (track.front())[0]   ;
      _mu_y     = (track.front())[1]   ;
      _mu_z     = (track.front())[2]   ;

      _mu_xEnd  = (track.back())[0]   ;
      _mu_yEnd  = (track.back())[1]   ;
      _mu_zEnd  = (track.back())[2]   ;
      _mu_leng  = lenght;

      if (_mu_En < 0)
        {
          zero++;
	  std::cout<<" Energy < 0   .............. \n";
	  std::cout<<"Pdg     .............. "<<Pdg   <<" \n";
	  std::cout<<"mass     .............. "<<mass   <<" \n";
	  std::cout<<"_mu_En   .............. "<<_mu_En   <<" \n";
	  std::cout<<"_mu_DepEn.............. "<<_mu_DepEn<<" \n";
	  std::cout<<"_mu_Mom  .............. "<<_mu_Mom  <<" \n";
	  std::cout<<"_mu_px   .............. "<<_mu_px   <<" \n";
	  std::cout<<"_mu_py   .............. "<<_mu_py   <<" \n";
	  std::cout<<"_mu_pz      ........... "<<_mu_pz   <<" \n";
	  std::cout<<"_mu_xEnd  ............. "<<_mu_xEnd <<" \n";
	  std::cout<<"_mu_yEnd  ............. "<<_mu_yEnd <<" \n";
	  std::cout<<"_mu_zEnd  ............. "<<_mu_zEnd <<" \n";
	  std::cout<<"_mu_leng                "<<_mu_leng <<" \n";

        }

      if (_mu_En > 2000)
        {
          crazy++;
	  std::cout<<" Energy > 20000   .............. \n";
	  std::cout<<"_mu_En   .............. "<<_mu_En   <<" \n";
	  std::cout<<"_mu_DepEn.............. "<<_mu_DepEn<<" \n";
	  std::cout<<"_mu_Mom  .............. "<<_mu_Mom  <<" \n";
	  std::cout<<"_mu_px   .............. "<<_mu_px   <<" \n";
	  std::cout<<"_mu_py   .............. "<<_mu_py   <<" \n";
	  std::cout<<"_mu_pz      ........... "<<_mu_pz   <<" \n";
	  std::cout<<"_mu_xEnd  ............. "<<_mu_xEnd <<" \n";
	  std::cout<<"_mu_yEnd  ............. "<<_mu_yEnd <<" \n";
	  std::cout<<"_mu_zEnd  ............. "<<_mu_zEnd <<" \n";
	  std::cout<<"_mu_leng                "<<_mu_leng <<" \n";

        } 

      _algoMu_tree->Fill();

      if (_verbose){
	std::cout<<"_mu_En   .............. "<<_mu_En   <<" \n";
	std::cout<<"_mu_DepEn.............. "<<_mu_DepEn<<" \n";
	std::cout<<"_mu_Mom  .............. "<<_mu_Mom  <<" \n";
	std::cout<<"_mu_px   .............. "<<_mu_px   <<" \n";
	std::cout<<"_mu_py   .............. "<<_mu_py   <<" \n";
	std::cout<<"_mu_pz      ........... "<<_mu_pz   <<" \n";
	std::cout<<"_mu_xEnd  ............. "<<_mu_xEnd <<" \n";
	std::cout<<"_mu_yEnd  ............. "<<_mu_yEnd <<" \n";
	std::cout<<"_mu_zEnd  ............. "<<_mu_zEnd <<" \n";
	std::cout<<"_mu_leng 		    "<<_mu_leng <<" \n";
    }	   	
      
     

  
    }//End loop over tracks

    //std::cout<<"Number of crazy found is "<<crazy<<std::endl;
    //std::cout<<"Number of < 0   found is "<<zero<<std::endl;
    
    return true;
  }

  void ERAlgoMu::ProcessEnd(TFile* fout)
  {
    if(fout){
      fout->cd();
      if (_algoMu_tree)
	_algoMu_tree->Write();
    }
    
  }

  void ERAlgoMu::InitializeHistos(){

  }


  void ERAlgoMu::Finalize()
  {
    std::cout<<"Number of crazy found is "<<crazy<<std::endl;
    std::cout<<"Number of < 0   found is "<<zero<<std::endl;
   
  }

  void ERAlgoMu::ClearTree(){

    _mu_En    = -1 ;
    _mu_DepEn = -1 ;
    _mu_Mom   = -1 ;
    _mu_px    = -1 ;
    _mu_py    = -1 ;
    _mu_pz    = -1 ;
    _mu_xEnd  = -1 ;
    _mu_yEnd  = -1 ;
    _mu_zEnd  = -1 ;

  return;
  }

}

#endif

      /*
      //Momentum and Energy Calculation with Multiple Coulomb Scattering
      _mu_MomMPS = 1000*momCalc.GetMomentumMultiScatterLLHD(track);
      _mu_EnMPS  = sqrt(_mu_MomMPS*_mu_MomMPS + mass*mass); 
      
      geoalgo::Vector_t MomMPS = Dir*_mu_MomMPS;
      _mu_pxMPS = MomMPS[0];
      _mu_pyMPS = MomMPS[1];
      _mu_pzMPS = MomMPS[2];
      */
      
      /*
      graph.GetParticle(t).SetParticleInfo(Pdg,
					   mass,
					   track.front(),
					   Mom);
      
      auto muon =graph.GetParticle(t);
      */
      
