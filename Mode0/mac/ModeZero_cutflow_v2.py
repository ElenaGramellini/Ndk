import sys, math, os
from ROOT import *

##################################################################################
# FUNCTION DEFINITIONS
#
# This function performs matching to associate IDed es and pions into
# proton decay events.
#
def doMatching( T, truth=False, vtx_cut=20, angle_cut=2.4, IP_cut=5 ):

	returnList = []

	# Correctly count number of pions depending on mode.

	if truth:
		n_pions = len(T._e_pion_truth)
		n_es = len(T._e_eplus_truth)
	else:
		n_pions = len(T._e_pion)
		n_es = len(T._e_eplus)

	# Loop through all pion, eplus combinations and assess if they
	# have the correct topologies.

	for i in xrange( n_pions ):

		if truth:
			pion_x = T._x_pion_truth[i]
			pion_y = T._y_pion_truth[i]
			pion_z = T._z_pion_truth[i]
			pion_px = T._px_pion_truth[i]
			pion_py = T._py_pion_truth[i]
			pion_pz = T._pz_pion_truth[i]
		else:	
			pion_x = T._x_pion[i]
			pion_y = T._y_pion[i]
			pion_z = T._z_pion[i]
			pion_px = T._px_pion[i]
			pion_py = T._py_pion[i]
			pion_pz = T._pz_pion[i]

		pion_vertex = geoalgo.Vector(pion_x, pion_y, pion_z)
		pion_mom = geoalgo.Vector(pion_px, pion_py, pion_pz)

		for j in xrange( n_es ):

			if truth:
				eplus_x = T._x_eplus_truth[j]
				eplus_y = T._y_eplus_truth[j]
				eplus_z = T._z_eplus_truth[j]
				eplus_px = T._px_eplus_truth[j]
				eplus_py = T._py_eplus_truth[j]
				eplus_pz = T._pz_eplus_truth[j]
			else:
				eplus_x = T._x_eplus[j]
				eplus_y = T._y_eplus[j]
				eplus_z = T._z_eplus[j]
				eplus_px = T._px_eplus[j]
				eplus_py = T._py_eplus[j]
				eplus_pz = T._pz_eplus[j]

			eplus_vertex = geoalgo.Vector(eplus_x, eplus_y, eplus_z)
			eplus_mom = geoalgo.Vector(eplus_px, eplus_py, eplus_pz)

			# Check to see if vertices are nearby

			if eplus_vertex.Dist(pion_vertex)>vtx_cut: continue

			# Check to see if they are oriented in opposite directions

			if eplus_mom.Angle(pion_mom)<angle_cut: continue

			# Check that their impact parameter is small

			# pion_traj = geoalgo.HalfLine(pion_vertex, pion_mom)
			# eplus_traj = geoalgo.HalfLine(eplus_vertex, eplus_mom)
			# findRel = ertool.AlgoFindRelationship()
			# IPvtx = geoalgo.Vector(0,0,0)
			# IP = findRel.FindClosestApproach(pion_traj, eplus_traj, IPvtx)
			# if IP>100:
			# 	print IP
			# 	print "Pion dir after:", pion_traj.Dir()[0], pion_traj.Dir()[1], pion_traj.Dir()[2]
			# 	print "Pion dir before:", pion_mom[0], pion_mom[1], pion_mom[2]
			# 	print "Eplus dir after:", eplus_traj.Dir()[0], eplus_traj.Dir()[1], eplus_traj.Dir()[2]
			# 	print "Eplus dir before:", eplus_mom[0], eplus_mom[1], eplus_mom[2]
			# 	print "pion vtx after:", pion_traj.Start()[0], pion_traj.Start()[1], pion_traj.Start()[2]
			# 	print "pion vtx before:", pion_vertex[0], pion_vertex[1], pion_vertex[2]
			# 	print "Eplus vtx after:", eplus_traj.Start()[0], eplus_traj.Start()[1], eplus_traj.Start()[2]
			# 	print "Eplus vtx before:", eplus_vertex[0], eplus_vertex[1], eplus_vertex[2]
			# # IP = sqrt(geoalgo.SqDist(pion_mom, eplus_mom, pion_vertex, eplus_vertex))
			# # print IP
			# if IP>IP_cut: continue

			# If these three cuts have been passed, we can add the index pair to 
			# the list of good combinations.

			returnList.append((i, j))

			# print eplus_vertex.Dist(pion_vertex)
			# print eplus_mom.Angle(pion_mom)

	return returnList




##################################################################################################
# SETTING PARAMETERS
#
doTruth = True

vtx_cut = 20e-6
E_min_eplus = 300
E_max_eplus = 500
E_min_pi0 = 350
E_max_pi0 = 600
mom_min_pi0 = 350
mom_max_pi0 = 600
m_min_pi0 = 100
m_max_pi0 = 200
E_min_proton = 929
E_max_proton = 935
mom_min_proton = 0
mom_max_proton = 1000
m_min_proton = 0
m_max_proton = 1000
alpha_min = 0.5

n_events=0
n_triplets=0
showercut_yield=0
vtxcut_yield=0
etwogammacut_yield=0
eplusenergycut_yield=0
pizeroenergycut_yield=0
protonenergycut_yield=0
gammaanglecut_yield=0

failures=0
successes=0

h_proton_p = TH1D("h_proton_p", "Reconstructed proton momentum; Momentum (MeV); Count", 150, 0, 400)
h_proton_e = TH1D("h_proton_e", "Reconstructed proton energy; Energy (MeV); Count", 150, 500, 1000)
h_proton_m = TH1D("h_proton_m", "Reconstructed proton mass; Mass (MeV); Count", 150, 500, 1000)

##################################################################################################
# EXECUTION OF CODE
#
#
# Read in argv to get path to file
if len(sys.argv) < 2:
	print "Failed to provide ROOT file. Exiting..."
	exit()
else:
	file_path = sys.argv[1]

# Check to make sure it's a real file
if not os.path.isfile(file_path):
	print "Not a valid path:", file_path
	exit()

# Create TFile and extract tree
T = TChain("_result_tree")
T.Add(file_path)

# Now we loop through tree entries and apply the cutflow
nentries = T.GetEntries()
for i in xrange(nentries):
	T.GetEntry(i)
	if not i%1000: print 'Processing entry', i, 'of', nentries

	n_events+=1

	associatedEvents = doMatching(T, doTruth)

	if len(associatedEvents)==0:
		failures+=1
		continue
	else:
		successes+=1

	for pair in associatedEvents:

		if doTruth:
			pion_mom = TLorentzVector(T._px_pion_truth[pair[0]], T._py_pion_truth[pair[0]], T._pz_pion_truth[pair[0]], T._e_pion_truth[pair[0]])
			eplus_mom = TLorentzVector(T._px_eplus_truth[pair[1]], T._py_eplus_truth[pair[1]], T._pz_eplus_truth[pair[1]], T._e_eplus_truth[pair[1]])
		else:
			pion_mom = TLorentzVector(T._px_pion[pair[0]], T._py_pion[pair[0]], T._pz_pion[pair[0]], T._e_pion[pair[0]])
			eplus_mom = TLorentzVector(T._px_eplus[pair[1]], T._py_eplus[pair[1]], T._pz_eplus[pair[1]], T._e_eplus[pair[1]])

		proton_mom = pion_mom+eplus_mom

		proton_p = proton_mom.P()
		proton_e = proton_mom.E()
		proton_m = proton_mom.M()

		if proton_p > 200: exit()

		h_proton_p.Fill(proton_p)
		h_proton_e.Fill(proton_e)
		h_proton_m.Fill(proton_m)



	# if threeShowersCut(T):
	# 	showercut_yield+=1
	# else:
	# 	continue

	# # In case there are more than one triplet in an event, we must loop through all of them
	# for triplet_id in xrange(T.n_triplets):
	# 	n_triplets+=1

	# 	if vertexRadiusCut(T, vtx_cut, triplet_id):
	# 		vtxcut_yield+=1
	# 	else:
	# 		continue

	# 	if ePlusTwoGammaCut(T, triplet_id):
	# 		etwogammacut_yield+=1
	# 	else:
	# 		continue

	# 	eplus_ind = makeShowerList("pdg").index(-11)

	# 	if ePlusEnergyCut(T, E_min_eplus, E_max_eplus, triplet_id, eplus_ind):
	# 		eplusenergycut_yield+=1
	# 	else:
	# 		continue

	# 	if piZeroEnergyCut(T, E_min_pi0, E_max_pi0, mom_min_pi0, mom_max_pi0, m_min_pi0, m_max_pi0, triplet_id, eplus_ind):
	# 		pizeroenergycut_yield+=1
	# 	else:
	# 		continue

	# 	if protonEnergyCut(T, E_min_proton, E_max_proton, mom_min_proton, mom_max_proton, m_min_proton, m_max_proton, triplet_id):
	# 		protonenergycut_yield+=1
	# 	else:
	# 		continue

	# 	if gammaAngleCut(T, alpha_min, triplet_id, eplus_ind):
	# 		gammaanglecut_yield+=1
	# 	else:
	# 		continue



# Now we print results...

print "RESULTS:"
print "Number of events:", n_events
print "Number of successful IDs:", successes
print "Number of failed IDs:", failures
# print "Number of events with a triplet:", showercut_yield
# print "---------------------------------"
# print "Number of triplets in", n_events, "events:", n_triplets
# print "Triplets that passed vertex cut:", vtxcut_yield
# print "Triplets that passed eplus, two gamma cut:", etwogammacut_yield
# print "Triplets taht passed eplus energy cut:", eplusenergycut_yield
# print "Triplets that passed pion energy cut:", pizeroenergycut_yield
# print "Triplets that passed proton energy cut:", protonenergycut_yield
# print "Triplets that passed angular gamma cut:", gammaanglecut_yield