import sys
from ROOT import gSystem
gSystem.Load("libNdk_git_Mode0")
from ROOT import sample

try:

    print "PyROOT recognized your class %s" % str(sample)

except NameError:

    print "Failed importing Mode0..."

sys.exit(0)

