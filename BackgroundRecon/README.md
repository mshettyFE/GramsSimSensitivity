This folder contains the files needed to generate the background dataset.

# Files and Folders
Once you run cmake in your build directory, you get the following in this folder:
* /BackgroundCountHist: folder to be zipped when running condor jobs generated from GenCondorCountsHistsScripts.py
* Commands.txt
* GenCondorCountsHistsScripts.py
* GenCondorScripts.py 
* GenEnergySpectrum.py
* GenMask.py
* README.md
* /SenseJob: folder to be zipped when running condor jobs generated from GenCondorScripts.py

* GenEnergySpectrum.py can generate a root file with logarithmically even energy bins that approximate a flux distribution (used in GenCountsHists for reweighting of background)
