#ifndef ERTOOL_ERANAMODE13_CXX
#define ERTOOL_ERANAMODE13_CXX

#include "ERAnaMode13.h"

namespace ertool {

  ERAnaMode13::ERAnaMode13(const std::string& name) : AnaBase(name)
  {}

  void ERAnaMode13::Reset()
  {}

  void ERAnaMode13::AcceptPSet(const ::fcllite::PSet& cfg)
  {}

  void ERAnaMode13::ProcessBegin()
  {}

  bool ERAnaMode13::Analyze(const EventData &data, const ParticleGraph &ps)
  { return true; }

  void ERAnaMode13::ProcessEnd(TFile* fout)
  {}

}

#endif
