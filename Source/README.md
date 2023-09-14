This folder contains the scripts needed to produce point-like, monoenergetic sources at various RA,ALT positions on the sky

# Files and Folders
Ignoring the cmake and make files, once you build the project with cmake, you get the following:
* CalcSensi: program that takes in count histograms from the background and source and generates the sensitivity
* CalculateSensitivity.cpp: source file of CalcSensi
* Commands100KeV.txt: shell commands that are inserted by GenCondorScripts.py which generate 0.1 MeV photons at RA=0, ALT=0 (frankly, this energy is too low for GRAMS, so any sensitivity you get here is pretty terrible)
* Commands10MeV.txt: Same as above, but 10 MeV (haven't full ran the pipeline on these)
* Commands1MeV.txt: Ditto, but 1 MeV (full ran pipeline on these gammas)
* GenCondorCountsHistsScripts.py: see [utils](../utils/README.md/#generating-sky-maps-and-counts)
* GenCondorScripts.py: see [utils](../utils/README.md/#running-condor-jobs-to-generate-cones)
* GenMask.py: see [utils](../utils/README.md/#defining-neighborhood-to-source-ie-genmask)
* options.xml: In this context, holds the parameters for CalcSensi. See [SenseJob](../SenseJob/README.md) for details
* SenseJob/: folder that gets zipped up by GenCondorScripts.py
* SourceCountHist/: folder that gets zipped up by GenCondorCountsHistsScripts.py

# Source Generation Workflow
* You should first run the code in [EffArea](../EffArea/).
* Create two directories (  \${Cones} and \${Counts}) to store the cone data and the count data for your source respectively.
* Run [GenCondorScripts](../utils/README.md/#running-condor-jobs-to-generate-cones) following the bottom example, then test and submit condor jobs
* Run [GenMask](../utils/GenMask.py) to define your local neighborhood
* Make sure the effective area histogram is copied over to SourceCountHist
* Run [GenCondorCountsHistsScripts](../utils/README.md/#generating-sky-maps-and-counts) without any reweighting
* Go to [Background](../Background/) to do a similar procedure with the background

# Sensitivity Calculation
* This is the last step of the pipeline
* Calculates the 3 and 5 sigma thresholds from the background, calculates how many source photons would be needed to hit these thresholds, and then converts this photon number to a sensitivity
* The necessary parameters are
    * BackgroundCountFolder: Folder containing background count root files from condor jobs produced by GenCondorCountsHistsScripts.py
    * BackgroundBaseName: preappended name of background counts histograms
    * SourceCountsFolder: Same as background version, but for source
    * SourceCountsBaseName: same as background version, but for source
    * Batches: The number of batches to consider for both the background and source files
    * EffectiveAreaRoot: Location of the effective area root file
    * ExposureTime: amount of time assigned to background in seconds
    * SourceEnergy: The energy of the generated gamma rays of the source
    * SourceEventsPerJob: The number of events generated per job. NOT the total number of generated gamma rays
* Optional parameters include the default help and verbose options
* Every other parameter is unused/unimplemented

As an example, look at the following:

```
./CalcSensi --BackgroundCountFolder ${BackInput} --BackgroundBaseName BackCountHists_  --ExposureTime 86400 --SourceCountsFolder ${SourceInput} --SourceCountsBaseName SourceCountHists_  --SourceEnergy 1  --Batches 10000 --SourceEventsPerJob 20000 --EffectiveAreaRoot ${EffAreaRoot}
```

What this does is reads in background count histograms from \${BackInput} of the form "BackCountHists_{\$process}.root" and source count histograms from \${SourceInput} of the form "SourceCountHists_\${process}.root". The background was assigned an exposure time of 1 day (86400 seconds), the source had an original energy of 1 MeV with 20,000 original 1 MeV gammas per job. The effective area root file is called \${EffAreaRoot}.

Hence, the program calculates the sensitivity of 1 MeV sources with an exposure time of 1 day.