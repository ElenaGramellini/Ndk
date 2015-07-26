#ifndef ERTOOL_ERALGOMU_CXX
#define ERTOOL_ERALGOMU_CXX

#include "ERAlgoMu.h"

namespace ertool {
  
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
    _algoMu_tree->Branch("_mu_x"     ,&_mu_x     ,"_mu_x/D     ");
    
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

      if (_verbose){
	std::cout<<"\n";	
	std::cout<<track._pid_score[Track::kTrackPartIDMax]<<" score kTrackPartIDMax\n";
	std::cout<<track._pid_score[Track::kPIDA]<<" score kPIDA\n";
	std::cout<<track._pid_score[Track::kUnknown]<<" score kUnknown\n";
	std::cout<<track._pid_score[Track::kProton]<<" score proton\n";
	std::cout<<track._pid_score[Track::kPion]<<" score pion\n";
	std::cout<<track._pid_score[Track::kKaon]<<" score kaon\n";
	std::cout<<track._pid_score[Track::kMuon]<<" score muon\n";
	std::cout<<"\n";	
	std::cout<<"Pid    : "<<track._pid<<" \n";
	std::cout<<"Energy : "<<track._energy<<" \n";
	std::cout<<"Length : "<<track.Length()<<" \n";
	std::cout<<"\n In Particle\n";	
	std::cout<<"Energy         : "<<particleFromDataP.Energy()<<"\n";	
	std::cout<<"Kinetic Energy : "<<particleFromDataP.KineticEnergy()<<"\n";	
	std::cout<<"Momentum       : "<<particleFromDataP.Momentum()<<"\n";

      }
      n_mu++;

      
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
      double lenght = track.Length();
      double rough_dEdx = Edep/lenght;
      
      // if  (rough_dEdx < 2.4) Pdg = 13;

      // track direction
      if (track.Length() < 0.3)  continue;
      geoalgo::Vector_t Dir = (track[1]-track[0]);
      Dir.Normalize();
      double mass = ParticleMass(Pdg);
      geoalgo::Vector_t Mom = Dir * ( sqrt( Edep * (Edep+2*mass) ) );


      graph.GetParticle(t).SetParticleInfo(Pdg,
					   mass,
					   track.front(),
					   Mom);

      auto muon =graph.GetParticle(t);
      double Energy = sqrt(Mom*Mom + mass*mass);

      if (_verbose){
	std::cout<<"Edep  .............. "<<Edep      <<" \n";
	std::cout<<"lenght.............. "<<lenght    <<" \n";
	std::cout<<"dedx  .............. "<<rough_dEdx<<"\n";
	std::cout<<"mass  .............. "<<mass      <<"  \n";
	std::cout<<"Dir   .............. "<<Dir       <<"  \n";
	std::cout<<"Momentum ........... "<<Mom       <<"  \n";
	std::cout<<"Energy ............. "<<Energy    <<" \n";
      }
      
      
    }//End loop over tracks

    
    return true;
  }

  void ERAlgoMu::ProcessEnd(TFile* fout)
  {
    /*
   if(fout){
      fout->cd();
      if(mu_energy)
	mu_energy->Write();
      if(muTru_energy)
	muTru_energy->Write();
    }
    */
  }

  void ERAlgoMu::InitializeHistos(){
    
/*
    if(!mu_energy)
      mu_energy = new TH1F("mu_energy","mu_energy",100,0,2000);
    if(!muTru_energy)
      muTru_energy = new TH1F("muTru_energy","muTru_energy",100,0,2000);
*/

  }


  void ERAlgoMu::Finalize()
  {

    //    std::cout<<"Number of mu's found is "<<n_mu<<std::endl;
  }

  void ERAlgoMu::ClearTree(){

    _mu_x         = -1 ;
    
    return;
  }

}

#endif
