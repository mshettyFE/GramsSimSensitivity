This folder contains all the code to generate the libraries and executables used across the rest of the code. When you run cmake and make, these libraries and executables get copied to the other directories as needed.

# Folders and Files
* [Extraction](./Extraction/) creates the executable that extracts the Compton scatter data from the outputs of GramsG4 and GramsDetSim and pushes them into a single TTree in a ROOT file.
* [Reconstruction](./Reconstruction/) creates the executable that takes in the output of Extraction, groups all the Compton scatters that belong to the same series, and constructs the associated Compton cone for each series
* [GenCountsHists](./GenCountsHists/) takes in the output of Reconstruction (ie. Compton Cones) as well as a binary mask root file (which defines the neighborhood of the source) and generates all-sky maps in right ascension and declination with the mask applied. It also tabulates the energy distribution of the Compton cones within the neighborhood defined by the binary mask file.
* [include](./include/) contains the header files for all the libraries
* [src](./src) contains the associted source files for the libraries
* GenEnergySpectrum.py can generate a root file with logarithmically even energy bins that approximate a flux distribution (used in GenCountsHists for reweighting of background)
* GenCondorScripts.py sets up the skeleton of the requirements to run a condor job on the nevis batch cluster for the purpose of generating a data set
* GenCondorCountsHistsScripts.py takes in the output directory folder from the condor job created by GenCondorScripts.py and then calculates the count histogram and sky map in the locality around the true source location
* GenMask.py takes in the output directory folder where the source cone files are stored and create a binary mask which defines the neighborhood around the source that is the "local background" (ie. only events within this region are considered)

# Generating ComptonCones
# Generating Sky Maps and Counts