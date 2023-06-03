This folder contains all the code to generate the libraries and executables used across the rest of the code. When you run cmake and make, these libraries and executables get copied to the other directories as needed.

# Folders and Files
* [Extraction](./Extraction/) creates the executable that extracts the Compton scatter data from the outputs of GramsG4 and GramsDetSim and pushes them into a single TTree in a ROOT file.
* [Reconstruction](./Reconstruction/) creates the executable that takes in the output of Extraction, groups all the Compton scatters that belong to the same series, and constructs the associated Compton cone for each series
* [GenCountsHists](./GenCountsHists/) takes in the output of Reconstruction (ie. Compton Cones) as well as a binary mask root file (which defines the neighborhood of the source) and generates all-sky maps in right ascension and declination with the mask applied. It also tabulates the energy distribution of the Compton cones within the neighborhood defined by the binary mask file.
* [include](./include/) contains the header files for all the libraries
* [src](./src) contains the associted source files for the libraries
* GenCondorScripts.py sets up the skeleton of the requirements to run a condor job on the nevis batch cluster for the purpose of generating a data set
* GenCondorCountsHistsScripts.py takes in the output directory folder from the condor job created by GenCondorScripts.py and then calculates the count histogram and sky map in the locality around the true source location
* GenMask.py takes in the output directory folder where the source cone files are stored and create a binary mask which defines the neighborhood around the source that is the "local background" (ie. only events within this region are considered)

GenCondorScripts.py and GenCondorCountsHistsScripts.py are seperate since I though that for other GRAMS work, you don't necessarily want the Counts and SkyMap; you just want the scatter series data and cone data.

# Running Condor Jobs to Generate Cones
In order to generate Compton cones, you first need to run GenCondorScripts.py. This script takes in a output directory for condor, condor job name, and .txt file (which contains shell commands) and does the following:
* creates the specified output directory
* Generates the necessary .cmd, .sh, and .mac files to be run on the batch farm at nevis
    * Optionally, creates a unique .mac file for each job in SenseJob/mac/batch/ (Used in Effective Area calculation)
* Merges the .txt file into the shell script
* Creates a tarball from SenseJob

Additional parameters include:
* N_BATCHES (nb): the number of condor jobs to run. Defaults to 1000.
* N_EVENTS (ne): the total number of gamma rays to generate across all condor jobs. Defaults to 1000000.
* -mem: the amount of memory to allocate to each job. Hopefully used to prevent condor from prematurely evicting a job. Defaults to 300 MB.
* -dsk: the amount of disk space to allocate to each job. Hopefully used to prevent condor from prematurely evicting a job. Defaults to 3000000 Kb.
* --ZCenter: denotes the center of the LArTPC. Defaults to -40 cm.
* --macros: a flag indicating that each job gets assigned a different energy, as dictated by a .mac file. The energy increases in a logarithmically even manner from minE to maxE.
    * BatchMacroName: name of macro file dictating energy of each job. Defaults to REPLACE_ME_.
    * minE: minimum energy assigned to a job. Default: 0.1 MeV
    * maxE: maximum energy assigned to a job. Default: 10 MeV

In order to run this, you first create a folder on a /data partition. Call this folder ${Output}.

Next, place GenCondorScripts.py in the same build directory as SenseJob (cmake does this automatically for you).

For illustrative purposes, suppose that you want to generate 2,000,000,000 gamma rays following a log-uniform distribution as your background. 

You want 10,000 batches so that condor doesn't keep rejecting your jobs.

 Using the provided [Commands.txt](../BackgroundRecon/Commands.txt) file, you can run the following in the build directory version of BackgroundRecon:

```
python GenCondorScripts.py ${Output}  Background Commands.txt  -nb 10000 -ne 200000000 
```

Background is the preappended name assigned to the default output of condor. 

This writes series data to \${Output} with file name of the format Background_Extracted_${process}.root and Background_Reco_${process}.root, where ${process} ranges from 0 to number of Batches -1. Each job simulates 20,000 gamma rays.

A similar command works with the Source reconstruction.

As another example, say that you want to run the effective area calculation. Suppose that you want to save the root files in ${Output}. 

You want energies ranging from 0.1 to 10 MeV, with 1000 data points. Utilizing no additional filters, you use the [Commands.txt](../EffArea/Commands.txt) in EffArea. cd to build EffArea and run:

```
python GenCondorScripts.py ${Output}  EffArea Commands.txt  -nb 1000 -ne 20000000 -m --minE 0.1 --maxE 10
```

# Generating Sky Maps and Counts
In analogous manner to GenCondorScripts.py, GenCondorCountsHistsScripts.py generates the counts histogram and skymaps fron a series of cones.

For illustrative purposes, suppose that you want to calculate the local background to a source. You have already ran GenCondorScripts.py and stored the cone data in \${Input}.

 Each cone file has the format of ConeData_\${process}.root where \${process} is the run id. 
 
 You want to store the histograms in a folder named ${Output}.

Required parameters of GenCondorCountsHistsScripts.py:
* Input_Path: this is just \${Input}, or the absolute path to the folder where the cones are stored
* Input_Path_Rootname: for our example, this should be set to 'ConeData_'. In general, it is the substring before "\${process}.root"
* output_path: this is just the absolute path to ${Output}
* Process_name: the preappended name attach to the default output of condor
* NBatchs: the number of reconstruction files in ${Input} (can use wc -l and pipes to find this out)
* Folder: the name of the folder containing ./GenCountsHists (cmake creates this for you)
* EffectiveArea: the root histogram that stores the effective area function (generated from [CalcArea.py](../EffArea/CalcEffArea.py))
* Mask: the root file containing the SkyMap that isolates a portion of the sky. Created by [GenMask.py](../utils/GenMask.py)

Optional parameters include:
* --Weighted: a flag that dictates if you are using a reweighting scheme. If this is set, you need to place more files into Folder
* RefFlux: the name of the root file (not the path) containing 1/E reference flux (Log uniform). Required for reweighting. Generated from [GenEnergySpectrum.py](../BackgroundRecon/GenEnergySpectrum.py)
* PhysicalFlux: the name of the root file (not the path) containing the physical flux of the gamma ray background. Required for reweighting. Generated from [GenEnergySpectrum.py](../BackgroundRecon/GenEnergySpectrum.py)
* TotalEvents: the number of events generated across all the batch jobs. Required for reweighting.
* ExposureTime: The exposure time assigned to the dataset. Required for reweighting.
* mem: equivalent to GenCondorScripts.py version
* dsk: equivalent to GenCondorScripts.py version

## Aside: Reweighting
The reweighting scheme I keep mentioning is utilized so that you don't need to generate a new background dataset each time you change your background flux model or your exposure time.

We first generate a large background data set draw from a [log-uniform distribution](https://en.wikipedia.org/wiki/Reciprocal_distribution). This is done so that in bins of equal size on a log energy scale, we have a uniform distribution, which is desirable since we want our reference not to be biased towards any specific energy region.

To deal with variable exposure times, we take the PhysicalFlux file (I(E)) and the Effective Area file (A(E)), perform binwise multiplication between the two, and then integral across our energy range. This yields the total flux (More explicitly, I(E) has units of $(MeV cm^2 s *sr)^{-1}$ and A(E) has units of $cm^2$. Multiplying and integrating over energy yields $(s *sr)^{-1}$).

We then multiply by $4\pi$ steradians and by the exposure time to get the projected number of photons $N_{proj}$. We then assign all the Cones in the input with a weight of $\frac{N_{proj}}{N_{generated}}$.

If we didn't do this, then we would need to first calculate the number of photons given our exposure time, effective area, and physical flux, and re-simulate all of the background gammas.

To deal with a variable background flux, we now utilize the RefFlux and PhysicalFlux root files. We normalize both distributions so that they becomes probability distributions (Denoted as $P_{ref}(E)$ an $P_{phys}(E)$).

Suppose that we have a compton cone that came from a gamma ray of energy E. We assign the weight $\frac{P_{phys}(E)}{P_{ref}(E)}$.

If we didn't do this, we would need to re-simulate the background whenever we wanted to test out a different flux.

To combine both effects, we simply multiply the weight together on a per cone basis.