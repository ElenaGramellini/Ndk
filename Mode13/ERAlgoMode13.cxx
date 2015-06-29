#ifndef ERTOOL_ERALGOMODE13_CXX
#define ERTOOL_ERALGOMODE13_CXX

#include "ERAlgoMode13.h"

namespace ertool {

  ERAlgoMode13::ERAlgoMode13(const std::string& name) : AlgoBase(name)
  {}

  void ERAlgoMode13::Reset()
  {}

  void ERAlgoMode13::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAlgoMode13::ProcessBegin()
  {}

  bool ERAlgoMode13::Reconstruct(const EventData &data, ParticleGraph& graph)
  {return true;}

  void ERAlgoMode13::ProcessEnd(TFile* fout)
  {}

}

#endif
