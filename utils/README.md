This folder contains all the code to generate the libraries and executables used across the rest of the code. When you run cmake and make, these libraries and executables get copied to the other directories as needed.

# Folders and Files
* Extraction creates the executable that extracts the Compton scatter data from the outputs of GramsG4 and GramsDetSim and pushes them into a single TTree in a ROOT file.
* Reconstruction creates the executable that takes in the output of Extraction, groups all the Compton scatters that belong to the same series, and constructs the associated Compton cone for each series
* GenSkyMap takes in the output of Reconstruction and turns the Compton cones into all-sky maps in right ascension and declination (Need to implement energy dependent reweighing and flux dependent reweighting)
* [include](include/README.md) contains the header files for all the libraries
* src contains the associted source files for the libraries
* GenEnergySpectrum.py can generate a root file with logarithmically event energy bins that approximate a flux distribution (used in GenSkyMap for reweighting)
* GenCondorScripts.py sets up the skeleton of the requirements to run a condor_job on the nevis batch cluster