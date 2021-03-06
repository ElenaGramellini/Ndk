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
my_proc.set_ana_output_file("Whole13Reco.root")

# create an instance of the ERAlgo we want to use
# primary finder algorithm                                                                                                                       
primary_algo = ertool.AlgoPrimaryFinder()
primary_algo.setVtxToTrkStartDist(1)
primary_algo.setVtxToTrkDist(1)
primary_algo.setVtxToShrStartDist(50)
primary_algo.setMaxIP(1)

my_MyAna = ertool.ERAnaMode13()


# create an instance of the Analysis Unit to be used
# this should work like a black box
# it is responsible for reading in the larlite file
# and giving the ERTool manager showers/tracks
# the manager will then run all the ERAlgo and ERAna
# modules assigned to this Analysis Unit
my_anaunit = fmwk.ExampleERSelection()
# Set Producers
my_anaunit.SetTrackProducer(False,"trackkalmanhit");
my_anaunit.SetShowerProducer(False,"showerrecopandora");

# cctrack            
# pandoraCosmicKHit 
# pandoraNuKHit     
# stitchkalmanhit   
# stitchkalmanhitcc 
# trackkalmanhit    
# trackkalmanhitcc  


# should we load the MCTruth information? decide here
# this is stuff that can come useful in ERAna module
my_anaunit._mgr._mc_for_ana = True
#my_anaunit._mgr.AddAlgo(primary_algo)
my_anaunit._mgr.AddAna(my_MyAna)

my_proc.add_process(my_anaunit)

my_proc.run()


# done!
print
print "Finished running ana_processor event loop!"
print

#my_algo.StoreParams()
sys.exit(0)

