#ifndef LARLITE_SHOWERFILTER_NDKMODEZERO_H
#define LARLITE_SHOWERFILTER_NDKMODEZERO_H

#include "Analysis/ana_base.h"
#include "DataFormat/shower.h"
namespace larlite {
  /**
     \class ShowerFilter_NdkModeZero
     User custom analysis class made by wderocco
   */
  class ShowerFilter_NdkModeZero : public ana_base{
  
  public:

    /// Default constructor
    ShowerFilter_NdkModeZero(){ _name="ShowerFilter_NdkModeZero"; _fout=0; _flip=false;};

    /// Default destructor
    virtual ~ShowerFilter_NdkModeZero(){};

    /** IMPLEMENT in ShowerFilter_NdkModeZero.cc!
        Initialization method to be called before the analysis event loop.
    */ 
    virtual bool initialize();

    /** IMPLEMENT in ShowerFilter_NdkModeZero.cc! 
        Analyze a data event-by-event  
    */
    virtual bool analyze(storage_manager* storage);

    /** IMPLEMENT in ShowerFilter_NdkModeZero.cc! 
        Finalize method to be called after all events processed.
    */
    virtual bool finalize();

    void flip(bool on) { _flip = on; }
    void setShowerMin(int min){ _sh_min=min; }
    void setShowerMax(int max){ _sh_max=max; }

    protected:

    size_t total_events;
    size_t kept_events;

    // boolean to flip logical operation of algorithm
    bool _flip;

    // shower cuts
    int _sh_min;
    int _sh_max;
    
  };
}
#endif
