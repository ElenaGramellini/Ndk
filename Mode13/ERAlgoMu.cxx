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

    _algoMu_tree->Branch("n_tracks"  , &n_tracks, "n_tracks/I");

    _algoMu_tree->Branch("_mu_begEndLength", "vector<double>" , &_mu_begEndLength   );
    _algoMu_tree->Branch("_mu_lengthRatio" , "vector<double>" , &_mu_lengthRatio    );
    _algoMu_tree->Branch("_mu_En"    , "vector<double>" , &_mu_En    );
    _algoMu_tree->Branch("_mu_DepEn" , "vector<double>" , &_mu_DepEn );
    _algoMu_tree->Branch("_mu_Mom"   , "vector<double>" , &_mu_Mom   );
    _algoMu_tree->Branch("_mu_px"    , "vector<double>" , &_mu_px    );
    _algoMu_tree->Branch("_mu_py"    , "vector<double>" , &_mu_py    );
    _algoMu_tree->Branch("_mu_pz"    , "vector<double>" , &_mu_pz    );

    _algoMu_tree->Branch("_mu_Pdg"   , "vector<int>"    , &_mu_Pdg   );						          
    _algoMu_tree->Branch("_mu_x"     , "vector<double>" , &_mu_x     );
    _algoMu_tree->Branch("_mu_y"     , "vector<double>" , &_mu_y     );
    _algoMu_tree->Branch("_mu_z"     , "vector<double>" , &_mu_z     );
						          
    _algoMu_tree->Branch("_mu_xEnd"  , "vector<double>" , &_mu_xEnd  );
    _algoMu_tree->Branch("_mu_yEnd"  , "vector<double>" , &_mu_yEnd  );
    _algoMu_tree->Branch("_mu_zEnd"  , "vector<double>" , &_mu_zEnd  );
						          
    _algoMu_tree->Branch("_mu_EnMPS" , "vector<double>" , &_mu_EnMPS );
    _algoMu_tree->Branch("_mu_MomMPS", "vector<double>" , &_mu_MomMPS);
    _algoMu_tree->Branch("_mu_pxMPS" , "vector<double>" , &_mu_pxMPS );
    _algoMu_tree->Branch("_mu_pyMPS" , "vector<double>" , &_mu_pyMPS );
    _algoMu_tree->Branch("_mu_pzMPS" , "vector<double>" , &_mu_pzMPS );
    _algoMu_tree->Branch("_mu_leng"  , "vector<double>" , &_mu_leng  );
    
    return;
  }

  bool ERAlgoMu::Reconstruct(const EventData &data, ParticleGraph& graph)
  {
    ClearTree();
    auto datacpy = data;
    int Pdg = -1; 
    double maxlength = 0;
    int relevant_ID = 0;
    // Loop through Particles associated with a track
    n_tracks = graph.GetParticleNodes(RecoType_t::kTrack).size();

    for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){
      auto const& particleFromDataP = graph.GetParticle(t);
      auto const& track = datacpy.Track(particleFromDataP.RecoID());
      auto begEnd = track.front() - track.back();
      double begEndLength = begEnd.Length();
      if (maxlength < begEndLength) 
	{
	  maxlength = begEndLength ;
	  relevant_ID = particleFromDataP.RecoID();
	}
    }

    //for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){
      
      // get track object
    // auto const& particleFromDataP = graph.GetParticle(t);
    if(relevant_ID){
      auto const& track = datacpy.Track(relevant_ID);
      
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
      //if (Edep < 0    ) continue;
      //if (Edep > 20000) continue;
      double length = track.Length();
      auto begEnd = track.front() - track.back();
      double begEndLength = begEnd.Length();
      
      double lengthRatio = begEndLength/length;
      


      // track direction
      geoalgo::Vector_t Dir = (track[1]-track[0]);
      Dir.Normalize();
      
      double mass = ParticleMass(Pdg);
      double Energy  = Edep + mass;
      double Mom_Mag = sqrt( Energy*Energy - mass*mass );

      geoalgo::Vector_t Mom = Dir *  Mom_Mag; 

      _mu_Pdg.push_back(Pdg);
      _mu_En.push_back( Energy  );
      _mu_DepEn.push_back( Edep    );
      _mu_Mom.push_back( Mom_Mag );
      _mu_px.push_back( Mom[0]  );
      _mu_py.push_back( Mom[1]  );
      _mu_pz.push_back( Mom[2]  );

      _mu_x.push_back( (track.front())[0]   );
      _mu_y.push_back( (track.front())[1]   );
      _mu_z.push_back( (track.front())[2]   );

      _mu_xEnd.push_back( (track.back())[0]   );
      _mu_yEnd.push_back( (track.back())[1]   );
      _mu_zEnd.push_back( (track.back())[2]   );
      _mu_leng.push_back( length);
      _mu_begEndLength.push_back(begEndLength);
      _mu_lengthRatio.push_back(lengthRatio);
    

    //std::cout<<"Number of crazy found is "<<crazy<<std::endl;
    //std::cout<<"Number of < 0   found is "<<zero<<std::endl;

    _algoMu_tree->Fill();
    }//End loop over tracks
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

    n_tracks = 0;

    _mu_begEndLength.clear();
    _mu_lengthRatio.clear();

    _mu_En.clear()   ;  _mu_EnMPS .clear();
    _mu_DepEn.clear();	_mu_MomMPS.clear();
    _mu_Mom.clear()  ;	_mu_leng  .clear();

    _mu_px.clear()   ;  _mu_pxMPS.clear();
    _mu_py.clear()   ;  _mu_pyMPS.clear();
    _mu_pz.clear()   ;  _mu_pzMPS.clear();
    
    _mu_Pdg.clear()  ;
    _mu_x.clear()    ;  _mu_xEnd.clear();       
    _mu_y.clear()    ;  _mu_yEnd.clear();	    
    _mu_z.clear()    ;  _mu_zEnd.clear();	    

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
      
