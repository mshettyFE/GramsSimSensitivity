This folder contains all the code to generate the libraries and executables used across the rest of the code. When you run cmake and make, these libraries and executables get copied to the other directories as needed.

# Folders and Files
* [Extraction](./Extraction/) creates the executable that extracts the Compton scatter data from the outputs of GramsG4 and GramsDetSim and pushes them into a single TTree in a ROOT file.
* [Reconstruction](./Reconstruction/) creates the executable that takes in the output of Extraction, groups all the Compton scatters that belong to the same series, and constructs the associated Compton cone for each series
* [GenCountsHists](./GenCountsHists/) takes in the output of Reconstruction (ie. Compton Cones) as well as a binary mask root file (which defines the neighborhood of the source) and generates all-sky maps in right ascension (RA) and altitude (ALT) with the mask applied. It also tabulates the energy distribution of the Compton cones within the neighborhood defined by the binary mask file.
* [include](./include/) contains the header files for all the libraries
* [src](./src) contains the associted source files for the libraries
* [GenCondorScripts.py](#running-condor-jobs-to-generate-cones) sets up the skeleton of the requirements to run a condor job on the nevis batch cluster for the purpose of generating a data set
* [GenCondorCountsHistsScripts.py](#example-usage-of-gencondorcountshistsscriptspy) takes in the output directory folder from the condor job created by GenCondorScripts.py and then calculates the count histogram and sky map in the locality around the true source location
* [GenMask.py](#defining-neighborhood-to-source-ie-genmask) takes in the output directory folder where the source cone files are stored and create a binary mask which defines the neighborhood around the source that is the "neighborhood" (ie. only events within this region are considered). 

GenCondorScripts.py and GenCondorCountsHistsScripts.py are seperate since I though that for other GRAMS work, you don't necessarily want the Counts and SkyMap; you just want the scatter series data and cone data. Feel free to combine them if you want to.

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

This writes series data to Background_Extracted_${process}.root in \${Output} and cone data to Background_Reco_${process}.root in \${Output}, where ${process} ranges from 0 to number of Batches -1. Each job simulates 20,000 gamma rays.

A similar command works with the Source reconstruction.

As another example, say that you want to run the effective area calculation. Suppose that you want to save the root files in ${Output}. 

You want energies ranging from 0.1 to 10 MeV, with 1000 data points. Utilizing no additional filters, you use the [Commands.txt](../EffArea/Commands.txt) in EffArea. cd to the build EffArea directory and run:

```
python GenCondorScripts.py ${Output}  EffArea Commands.txt  -nb 1000 -ne 20000000 -m --minE 0.1 --maxE 10
```

# Defining Neighborhood to Source (ie. GenMask)
The way that GenMask works is that it read in the ARM value from each Cone and puts them all into a histogram bounded between -0.5 and 0.5 radians (these are arbitrary values, but I found that there are very few values outside these points, you get a better $\chi^{2}/dof$, and you get around the same ARM as the full $-\pi$ to $\pi$). 

It then fits a Lorentzian, defines as
$A\frac{\frac{\Gamma}{2}}{(\frac{\Gamma}{2})^{2}+(x-x_{0})^{2}}$ where A, $\Gamma$ and $ x_{0}$ are the fit parameters. 

A is a normalization constant. $x_{0}$ is the center of the distribution, and $\Gamma$ is the FWHM, or in the content of Compton cameras, the ARM. Once the fitting is done, $\Gamma$ is taken as the ARM of the source.

After calculating the ARM, GenSky then takes the center of each bin the output skymap and computes the angle w.r.t. the true source location. If this angle is smaller than the ARM, then you set that bin to 1 (ie. we should include this bin in the final map). After interating through all the bins, it output the mask to a root file as a TH2D.

GenMask has the following required arguments:
* SourcePath: the output folder  where the cone data for your source lies at
* ReconstructionBase: the file name format of the source Cone data root files
* RA_loc: The true RA location where your source was generate
* ALT_loc: the true ALT location where your source was generate
* Output: the name of the output root file

Optional arguments:
* RABins: Number of bins along RA axis. Defaults to 500
* ALTBins: Number of bins along ALT axis. Defaults to 500
* --draw: Flag to indicate that you want to draw the ARM distribution and the final masked sky map as .jpegs
    * NOTE: ROOT doesn't properly display the sky map if you set counts to 1. You can see the actual map better if you set the counts to 1000 instead of 1 (although for the rest of the pipeline, it should be set to 1 to be properly interpreted as a binary mask)
* --nbins: the number of bins used when fitting the ARM distribution. Defaults to 50,000

As an example usage:
```
python GenMask.py \${Input} Source_Reco_ 0 0 Mask.root -d
```

takes in cone data from ${Input} with the base name of "Source_Reco_". The true source location is at RA=0  and ALT=0. The output file is called Mask.root. We draw the intermediate fit and final sky map as .jpgs.

# Generating Sky Maps and Counts
In an analogous manner to GenCondorScripts.py, GenCondorCountsHistsScripts.py generates the counts histogram and skymaps fron a series of cones.

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

If we didn't do this, then we would need to first calculate the number of photons given our exposure time, effective area, and physical flux, and only use that number of photons. If we had to few phhotons, we would need to generate more. There is also the problem where you might not be able to evenly divide the projected number of photons amoungst all the batches. Reweighting solves both of these issues.

To deal with a variable background flux, we now utilize the RefFlux and PhysicalFlux root files. We normalize both distributions so that they becomes probability distributions (Denoted as $P_{ref}(E)$ and $P_{phys}(E)$ ).

Suppose that we have a compton cone that came from a gamma ray of energy E. We assign the weight $\frac{P_{phys}(E)}{P_{ref}(E)}$.

If we didn't do this, we would need to re-simulate the background whenever we wanted to test out a different flux.

To combine both effects, we simply multiply the weight together on a per cone basis.

## Example Usage of GenCondorCountsHistsScripts.py

First, you create a directory where you want to store the output counts and skymaps of the background. Call this directory \${Output}. Let's say that you want the output files to have the preappended string "LocalBack".

Suppose that your background reconstruction files are located in the folder \${Input} and all share the preappended name "Background_Reco_". You generated 10,000 batches with a total number of events of 200000000.

Suppose that you have a folder in the same directory and GenCondorCountsHistsScripts.py with the name \${Folder}. Inside the folder is the effective are file \${EffA}, the mask file \${Mask}, the reference flux file \${Ref}, the physical flux file \${Phys}, the GenCountsHists executable and the associated options.xml file.

Suppose that you assigned an exposure time of \${ExtTime}, you would run the following command in the directory containing \${Folder} and GenCondorCountsHistsScripts.py:
```
python GenCondorCountsHistsScripts.py ${Input} Background_Reco_ ${Output} LocalBack 10000 ${Folder} ${EffA} ${Mask} -w --RefFlux ${Ref}  --PhysicalFlux ${Phys}--ExposureTime 86400 --TotalEvents 200000000
```

NOTE: you might get an error message along the lines of a potential memory leak from adding histograms. This is OK. There is no memory leak (at least, I'm pretty sure there isn't). What is happening the aggregation process to the final sky map.

Similarly, if you wanted to do source counts and skymaps, you would only need the effective area file and mask file in ${Folder}. You also wouldn't need TotalEvents or ExposureTime:
```
python GenCondorCountsHistsScripts.py ${Input} Source_Reco_ ${Output} LocalSource 10000 ${Folder} ${EffA} ${Mask}
```