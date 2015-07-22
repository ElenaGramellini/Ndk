import sys, os
import ROOT
from ROOT import ertool

data_maker = ertool.ToyDataMaker()

# Make showers

my_shower1=data_maker.MakeShower(125,0,500, 135.0/2.0,0,0,  22)
my_shower2=data_maker.MakeShower(125,0,500, -135.0/2.0,0,0, 22)
# Fake ID
myid=ROOT.std.pair("size_t","string")()

#
# Configure ERTool
#

algopi0 = ertool.ERAlgoPi0_NdkModeZero()
# algopi0 = ertool.AlgoPi0()
algopi0.setVerbose(True)
algopi0.setAngleMax(3.142)
algopi0.setMinFitMass(0)
algopi0.setMaxFitMass(1000)


# My ERTool manager
mgr=ertool.Manager()
# Add algorithm
mgr.AddAlgo(algopi0)
# Add ana
mgr.AddAna(ertool.ERAnaPi0Test())

#
# Run it
#
mgr.Initialize()

# Fake "event" 1
mgr.ClearData()
mgr.Add(my_shower1,myid)
mgr.Add(my_shower2,myid)
mgr.Process()

mgr.Finalize()
