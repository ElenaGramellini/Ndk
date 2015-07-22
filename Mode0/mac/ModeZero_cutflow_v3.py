import sys, math, os
from ROOT import *

##################################################################################
# FUNCTION DEFINITIONS
#




##################################################################################################
# SETTING PARAMETERS
#
doTruth = False
purely_topology = False

vtx_cut = 50				# Distance from pi0 start to e+ start
angle_cut = 2.0 			# Minimum opening angle between pi0 and e+
E_min_eplus = 200			# Minimum e+ energy
E_max_eplus = 100000		# Maximum e+ energy
E_min_pi0 = 250				# Minimum pi0 energy
E_max_pi0 = 100000			# Maximum pi0 energy
# mom_min_pi0 = 350
# mom_max_pi0 = 600
# m_min_pi0 = 100
# m_max_pi0 = 200
E_min_proton = 800			# Minimum proton energy
E_max_proton = 1000			# Maximum proton energy
# mom_min_proton = 0
# mom_max_proton = 1000
# m_min_proton = 0
# m_max_proton = 1000
# alpha_min = 0.5
pcut = 300 					# Components of proton momentum must all be below this

n_events=0
cut1=0
cut2=0
cut3=0
cut4=0
cut5=0
cut6=0
cut7=0
cut8=0
cut9=0

dalitz=0

# failures=0
# successes=0

h_proton_p = TH1D("h_proton_p", "Reconstructed proton momentum; Momentum (MeV); Count", 15, 0, 500)
h_proton_e = TH1D("h_proton_e", "Reconstructed proton energy; Energy (MeV); Count", 15, 700, 1300)
h_proton_m = TH1D("h_proton_m", "Reconstructed proton mass; Mass (MeV); Count", 15, 700, 1300)
h_pion_p = TH1D("h_pion_p", "Reconstructed #pi^{0} momentum; Momentum (MeV); Count", 15, 200, 700)
h_pion_e = TH1D("h_pion_e", "Reconstructed #pi^{0} energy; Energy (MeV); Count", 15, 200, 700)
h_pion_m = TH1D("h_pion_m", "Reconstructed #pi^{0} mass; Mass (MeV); Count", 15, 50, 350)
h_eplus_p = TH1D("h_eplus_p", "Reconstructed e^{+} momentum; Momentum (MeV); Count", 15, 200, 700)
h_eplus_e = TH1D("h_eplus_e", "Reconstructed e^{+} energy; Energy (MeV); Count", 15, 200, 700)
h_eplus_m = TH1D("h_eplus_m", "Reconstructed e^{+} mass; Mass (MeV); Count", 15, 0, 1.2)

# h_vtx_dist = TH1D("h_vtx_dist", "Distance between pion position and e^{+} start; Distance (cm); Count", 40, 0, 200)


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
for entry in xrange(nentries):
	T.GetEntry(entry)
	if not entry%1000: print 'Processing entry', entry, 'of', nentries

	n_events+=1

	# Correctly count number of pions depending on mode.

	if doTruth:
		n_pions = len(T._e_pion_truth)
		n_es = len(T._e_eplus_truth)
		# Keep track of truth non-yy pion decays
		if len(T._x_ph2_truth)==0 or len(T._x_ph1_truth)==0:
			dalitz+=1
			continue
	else:
		n_pions = len(T._e_pion)
		n_es = len(T._e_eplus)

	# CUT 1: AT LEAST 1 PION

	if n_pions<1: continue
	cut1+=1

	# CUT 2: AT LEAST 1 ELECTRON

	if n_es<1: continue
	cut2+=1

	# Loop through all pion, eplus combinations and assess if they
	# have the correct topologies.

	for i in xrange( n_pions ):

		if doTruth:
			pion_x = T._x_pion_truth[i]
			pion_y = T._y_pion_truth[i]
			pion_z = T._z_pion_truth[i]
			pion_px = T._px_pion_truth[i]
			pion_py = T._py_pion_truth[i]
			pion_pz = T._pz_pion_truth[i]
			pion_e = T._e_pion_truth[i]
		else:	
			pion_x = T._x_pion[i]
			pion_y = T._y_pion[i]
			pion_z = T._z_pion[i]
			pion_px = T._px_pion[i]
			pion_py = T._py_pion[i]
			pion_pz = T._pz_pion[i]
			pion_e = T._e_pion[i]

		pion_vertex = geoalgo.Vector(pion_x, pion_y, pion_z)
		pion_mom = geoalgo.Vector(pion_px, pion_py, pion_pz)

		for j in xrange( n_es ):

			if doTruth:
				eplus_x = T._x_eplus_truth[j]
				eplus_y = T._y_eplus_truth[j]
				eplus_z = T._z_eplus_truth[j]
				eplus_px = T._px_eplus_truth[j]
				eplus_py = T._py_eplus_truth[j]
				eplus_pz = T._pz_eplus_truth[j]
				eplus_e = T._e_eplus_truth[j]
			else:
				eplus_x = T._x_eplus[j]
				eplus_y = T._y_eplus[j]
				eplus_z = T._z_eplus[j]
				eplus_px = T._px_eplus[j]
				eplus_py = T._py_eplus[j]
				eplus_pz = T._pz_eplus[j]
				eplus_e = T._e_eplus[j]

			eplus_vertex = geoalgo.Vector(eplus_x, eplus_y, eplus_z)
			eplus_mom = geoalgo.Vector(eplus_px, eplus_py, eplus_pz)

			# Start cutting.

			# CUT 3: DISTANCE BETWEEN SHOWER STARTS

			# Create sphere that encloses first energy depositions and find radius.
			if doTruth:
				ph1_vertex = geoalgo.Vector(T._x_ph1_truth[i], T._y_ph1_truth[i], T._z_ph1_truth[i])
				ph2_vertex = geoalgo.Vector(T._x_ph2_truth[i], T._y_ph2_truth[i], T._z_ph2_truth[i])
			else:
				ph1_vertex = geoalgo.Vector(T._x_ph1[i], T._y_ph1[i], T._z_ph1[i])
				ph2_vertex = geoalgo.Vector(T._x_ph2[i], T._y_ph2[i], T._z_ph2[i])
			sphere = geoalgo.Sphere(eplus_vertex, ph1_vertex, ph2_vertex)

			if sphere.Radius()>vtx_cut: continue
			cut3+=1

			# CUT 4: OPENING ANGLE OF PI0 AND EPLUS

			if eplus_mom.Angle(pion_mom)<angle_cut: continue
			cut4+=1

			# CUT 5: IMPACT PARAMETER

			# pion_traj = geoalgo.HalfLine(pion_vertex, pion_mom)
			# eplus_traj = geoalgo.HalfLine(eplus_vertex, eplus_mom)
			# findRel = ertool.AlgoFindRelationship()
			# IPvtx = geoalgo.Vector(0,0,0)
			# IP = findRel.FindClosestApproach(pion_traj, eplus_traj, IPvtx)
			# if IP>IP_cut:
				# cut5+=1
				# continue

			# This concludes the topological cutflow. Now we impose the
			# calorimetry cuts.	If we are looking purely at topologies, this
			# is as far as we go.

			if purely_topology: continue

			# Calorimetry cuts begin.

			pion_4mom = TLorentzVector(pion_px, pion_py, pion_pz, pion_e)
			eplus_4mom = TLorentzVector(eplus_px, eplus_py, eplus_pz, eplus_e)

			# CUT 6: EPLUS ENERGY

			if eplus_e<E_min_eplus or eplus_e>E_max_eplus: continue
			cut6+=1

			# CUT 7: PION ENERGY

			if pion_e<E_min_pi0 or pion_e>E_max_pi0: continue
			cut7+=1

			# Now we can start cutting on proton-related quantities.

			proton_4mom = pion_4mom+eplus_4mom

			proton_p = proton_4mom.P()
			proton_e = proton_4mom.E()
			proton_m = proton_4mom.M()

			# CUT 8: PROTON ENERGY

			if proton_e<E_min_proton or proton_e>E_max_proton: continue
			cut8+=1

			# CUT 9: PROTON MOMENTA

			if not (abs(proton_4mom.Px())<pcut and abs(proton_4mom.Py())<pcut and abs(proton_4mom.Pz())<pcut): continue
			cut9+=1

			h_proton_p.Fill(proton_p)
			h_proton_e.Fill(proton_e)
			h_proton_m.Fill(proton_m)
			h_pion_p.Fill(pion_4mom.P())
			h_pion_e.Fill(pion_4mom.E())
			h_pion_m.Fill(pion_4mom.M())
			h_eplus_p.Fill(eplus_4mom.P())
			h_eplus_e.Fill(eplus_4mom.E())
			h_eplus_m.Fill(eplus_4mom.M())




# Now we print results...

print "\n\nRESULTS:"
print "Number of events:", n_events
if doTruth: print "Dalitz and other bad pion decays:", dalitz
print "---------- TOPOLOGICAL CUTS -------------"
print "Passed cut 1:", cut1
print "Passed cut 2:", cut2
print "Passed cut 3:", cut3
print "Passed cut 4:", cut4
# print "Passed cut 5:", cut5
if not purely_topology:
	print "--------- CALORIMETRIC CUTS -------------"
	print "Passed cut 6:", cut6
	print "Passed cut 7:", cut7
	print "Passed cut 8:", cut8
	print "Passed cut 9:", cut9


# Write plots out.

write_file = TFile("FullCutsDistributions_PerfectRecoSignal.root", "RECREATE")

h_proton_p.Write()
h_proton_e.Write()
h_proton_m.Write()
h_pion_p.Write()
h_pion_e.Write()
h_pion_m.Write()
h_eplus_p.Write()
h_eplus_e.Write()
h_eplus_m.Write()
# h_vtx_dist.Write()

write_file.Close()

killin_it = raw_input("Hit enter to kill me.")
exit()
