import sys, os

if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

from ROOT import gSystem
from ROOT import larlite as fmwk
from ROOT import ertool


# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Primary finding algo

primary_algo = ertool.AlgoPrimaryFinder()

# Pi0 Algo
pi0_algo = ertool.ERAlgoPi0_NdkModeZero()
pi0_algo.setVerbose(False)
pi0_algo.setMinShrEnergy(0)
pi0_algo.setMaxShrEnergy(1000)
pi0_algo.setIPMax(10)
pi0_algo.setMinFitMass(50)
pi0_algo.setMaxFitMass(200)
pi0_algo.setAngleMax(3.14)

# Single e Algo
singlee_algo = ertool.ERAlgoSingleE_NdkModeZero()

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("ModeZero_output.root")

#This cut is applied in helper... ertool showers are not made if the energy of mcshower or reco shower
#is below this threshold. This has to be above 0 or else the code may segfault. This is not a "physics cut".
#Do not change this value unless you know what you are doing.
Ecut = 20 # in MeV

my_ana = ertool.ERAnaModeZero()
my_ana.SetDebug(False)
my_ana.SetECut(Ecut)

# Add analyzer to the unit
my_anaunit = fmwk.ExampleERSelection()
my_anaunit._mgr.AddAlgo(primary_algo)
my_anaunit._mgr.AddAlgo(singlee_algo)
my_anaunit._mgr.AddAlgo(pi0_algo)
my_anaunit._mgr.AddAna(my_ana)

my_anaunit._mgr._mc_for_ana = True
my_anaunit.SetShowerProducer(True,"mcreco")
my_anaunit.SetTrackProducer(True,"mcreco")

# Add unit to the entire process
my_proc.add_process(my_anaunit)

# RUN THE JEWELS
my_proc.run()

# done!
print "Finished running ana_processor event loop!"

sys.exit(0)