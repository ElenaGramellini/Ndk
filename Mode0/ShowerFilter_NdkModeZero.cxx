#ifndef LARLITE_SHOWERFILTER_NDKMODEZERO_CXX
#define LARLITE_SHOWERFILTER_NDKMODEZERO_CXX

#include "ShowerFilter_NdkModeZero.h"

namespace larlite {

  bool ShowerFilter_NdkModeZero::initialize() {
    
    total_events = 0;
    kept_events = 0;
    
    return true;

  }
  
  bool ShowerFilter_NdkModeZero::analyze(storage_manager* storage) {
  
    //Grab the showers
    auto showers = storage->get_data<event_shower>("showerrecopandora");
    if(!showers) {
      print(larlite::msg::kERROR,__FUNCTION__,Form("Did not find specified data product, showers!"));
      return false;
    }

    total_events++;

    bool ret = true;
    
    //Enforce the number of showers
    if( showers->size() < _sh_min || showers->size() > _sh_max )
      ret = false;
    
    //check the status of the ret variable
    if (ret && !_flip)
      kept_events++;
    if (!ret && _flip)
      kept_events++;
    if (_flip)
      return (!ret);
    return ret;  
  }

  bool ShowerFilter_NdkModeZero::finalize() {

    std::cout<<"ShowerFilter: Total events = "<<total_events<<std::endl;
    std::cout<<"ShowerFilter: Final kept events = "<<kept_events<<std::endl;

    return true;
  }

}
#endif
