#ifndef ERTOOL_ERANAPI0TEST_CXX
#define ERTOOL_ERANAPI0TEST_CXX

#include "ERAnaPi0Test.h"

namespace ertool {

  ERAnaPi0Test::ERAnaPi0Test(const std::string& name) : AnaBase(name)
  {}

  void ERAnaPi0Test::Reset()
  {}

  void ERAnaPi0Test::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaPi0Test::ProcessBegin()
  {}

  bool ERAnaPi0Test::Analyze(const EventData &data, const ParticleGraph &graph)
  { 

    // Get particle set
    auto const& graph_arr = graph.GetParticleArray();

    for (auto &p : graph_arr){

      std::cout<<"New particle with PDG ID: "<<p.PdgCode()<<std::endl;

      // Find the pion and store its energy
      if (p.PdgCode() == 111){
        std::cout<<"------------ IDENTIFIED NEW PION ---------------"<<std::endl;
        std::cout<<"Energy: "<<p.Energy()<<std::endl;
        std::cout<<"Momentum: "<<sqrt(pow(p.Momentum()[0],2) + pow(p.Momentum()[1],2) + pow(p.Momentum()[2],2))<<std::endl;
        std::cout<<"Mass: "<<p.Mass()<<std::endl;
      }
    }



    return true;

  }

  void ERAnaPi0Test::ProcessEnd(TFile* fout)
  {}

}

#endif
