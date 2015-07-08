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

# Strictness level
fullcuts = True

# Save file name
outfile = "OUTPUT.root"

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Primary finding algo
primary_algo = ertool.AlgoPrimaryFinder()

# Pi0 Algo strict
pi0_algo_strict = ertool.ERAlgoPi0_NdkModeZero("strict_pi0")
pi0_algo_strict.setVerbose(False)
pi0_algo_strict.setMinShrEnergy(0)
pi0_algo_strict.setMaxShrEnergy(500)
pi0_algo_strict.setIPMax(5)
pi0_algo_strict.setMinFitMass(120)
pi0_algo_strict.setMaxFitMass(150)
pi0_algo_strict.setAngleMax(3.14)

# Pi0 Algo loose
pi0_algo_loose = ertool.ERAlgoPi0_NdkModeZero("loose_pi0")
pi0_algo_loose.setVerbose(False)
pi0_algo_loose.setMinShrEnergy(0)
pi0_algo_loose.setMaxShrEnergy(1000)
pi0_algo_loose.setIPMax(10)
pi0_algo_loose.setMinFitMass(0)
pi0_algo_loose.setMaxFitMass(300)
pi0_algo_loose.setAngleMax(3.14)

# Single e Algo Strict
singlee_algo_strict = ertool.ERAlgoSingleE_NdkModeZero("strict_e")
singlee_algo_strict.setEThreshold(200)
singlee_algo_strict.setVtxToShrStartDist(10)
singlee_algo_strict.setVtxToTrkStartDist(10)
singlee_algo_strict.setMaxIP(10)

# Single e Algo Loose
singlee_algo_loose = ertool.ERAlgoSingleE_NdkModeZero("loose_e")
singlee_algo_loose.setEThreshold(0)
singlee_algo_loose.setVtxToShrStartDist(0)
singlee_algo_loose.setVtxToTrkStartDist(0)
singlee_algo_loose.setMaxIP(0)

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file(outfile)

#This cut is applied in helper... ertool showers are not made if the energy of mcshower or reco shower
#is below this threshold. This has to be above 0 or else the code may segfault. This is not a "physics cut".
#Do not change this value unless you know what you are doing.
Ecut = 20 # in MeV

my_ana = ertool.ERAnaModeZero("ana")
my_ana.SetDebug(False)
my_ana.SetECut(Ecut)

# Add analyzer to the unit
my_anaunit = fmwk.ExampleERSelection()

my_anaunit._mgr.AddAlgo(primary_algo)
# Apply strict cuts and then loose cuts to avoid over-IDing showers as
# either pions or es.
my_anaunit._mgr.AddAlgo(pi0_algo_strict)
my_anaunit._mgr.AddAlgo(singlee_algo_strict)
if fullcuts:
	my_anaunit._mgr.AddAlgo(pi0_algo_loose)
	my_anaunit._mgr.AddAlgo(singlee_algo_loose)
my_anaunit._mgr.AddAna(my_ana)


my_anaunit._mgr._mc_for_ana = True
my_anaunit.SetShowerProducer(True,"mcreco")
my_anaunit.SetTrackProducer(True,"mcreco")


# Add unit to the entire process
my_proc.add_process(my_anaunit)

# RUN THE JEWELS
my_proc.run()

# done!
print "\nFinished running ana_processor event loop."
if not fullcuts: print "NOTE: This was performed only with strict cuts."

sys.exit(0)