#ifndef ERTOOL_ERALGOMU_CXX
#define ERTOOL_ERALGOMU_CXX

#include "ERAlgoMu.h"

namespace ertool {
  
  ERAlgoMu::ERAlgoMu(const std::string& name) : AlgoBase(name)
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

    InitializeHistos();

    return;
  }

  bool ERAlgoMu::Reconstruct(const EventData &data, ParticleGraph& graph)
  {
    /*
    // Loop through Particles associated with a track
    for (auto const& t : graph.GetParticleNodes(RecoType_t::kTrack)){
      
      // get track object
      auto const& track = data.Track(graph.GetParticle(t).RecoID());
      
      // calculate distance between track end point and shower start point
      auto const& trackEnd = track.back();
      
      n_mu++;
    */
      /*
      // edit the particle's information
      graph.GetParticle(p).SetParticleInfo(11,
					   ParticleMass(11),
					   trackEnd,
					   shower.Dir()*shower._energy);
      */

      // also since we know it, add info on relationship
      
      // finally, if in verbose mode, cout some info
    /*  
    if (_verbose){
	std::cout << "We found a muon!" << std::endl
		  << "Track end point: " << trackEnd << std::endl
		  << "Track energy: " << track._energy << " [MeV]" << std::endl
		  << std::endl;
      }
      
      // fill histogram with muon electron energy
      mu_energy->Fill(track._energy);
      
    }//End loop over tracks

    */
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


}

#endif
