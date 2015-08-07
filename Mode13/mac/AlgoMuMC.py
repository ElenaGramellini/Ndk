import sys

# these lines of code simply warn the user
# if he has not provided enough arguments
# an input file must be provided
if len(sys.argv) < 2:
    msg  = '\n'
    msg += "Usage 1: %s $INPUT_ROOT_FILE\n" % sys.argv[0]
    msg += '\n'
    sys.stderr.write(msg)
    sys.exit(1)

# import all the things we need
from seltool import ertool
from larlite import larlite as fmwk

# Create ana_processor instance
my_proc = fmwk.ana_processor()

# Set input root file
for x in xrange(len(sys.argv)-1):
    my_proc.add_input_file(sys.argv[x+1])

# Specify IO mode
my_proc.set_io_mode(fmwk.storage_manager.kREAD)

# Specify output root file name
my_proc.set_ana_output_file("MuStudyMC.root")


# create an instance of the ERAlgo we want to use
my_MuAlgo = ertool.ERAlgoMu()
my_MuAna  = ertool.ERAnaMu()

# create an instance of the Analysis Unit to be used
# this should work like a black box
# it is responsible for reading in the larlite file
# and giving the ERTool manager showers/tracks
# the manager will then run all the ERAlgo and ERAna
# modules assigned to this Analysis Unit
my_anaunit = fmwk.ExampleERSelection()
# Set Producers
# First Argument: True = MC, False = Reco
my_anaunit.SetTrackProducer(True,"mcreco");
#my_anaunit.SetTrackProducer(False,"trackkalmanhit");

# should we load the MCTruth information? decide here
# this is stuff that can come useful in ERAna module
my_anaunit._mgr._mc_for_ana = True

my_anaunit._mgr.AddAlgo(my_MuAlgo)
my_anaunit._mgr.AddAlgo(my_MuAna)

my_proc.add_process(my_anaunit)

my_proc.run()

# done!
print
print "Finished running ana_processor event loop!"
print

#my_algo.StoreParams()
sys.exit(0)
