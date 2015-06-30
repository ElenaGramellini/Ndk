import ROOT
from ROOT import ertool,TFile

#
# Make my fake data input
#
# My test data maker instance
data_maker=ertool.TestDataMaker()
# Make 2 fake tracks
my_shower1=data_maker.MakeShower(1,0,0, 5, 5, 5,  22)
# Fake ID
myid=ROOT.std.pair("size_t","string")()


#
# Configure ERTool
#
# My ERTool manager
mgr=ertool.Manager()
# Add algorithm
mgr.AddAlgo(ertool.ERAlgoTestShower())
# Add my algorithm's config file
mgr.AddCfgFile('yourcConfigFile.cfg')

# name of the output file
fout = TFile("yourDummy.root","RECREATE")



#
# Run it
#
mgr.Initialize()

# Fake "event" 1
mgr.ClearData()
mgr.Add(my_shower1,myid)
mgr.Process()

# finalize --> graphs
mgr.Finalize(fout)
