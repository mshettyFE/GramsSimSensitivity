 This repository contains the various scripts and tools used to calculate the sensitivity of the GRAMS detector to MeV gamma rays. Based on the Geant4 simulation [GramsSim](https://github.com/wgseligman/GramsSim).

 NOTE: if you aren't planning on building and running this on the nevis cluster, you need to do a good amount of leg work to download the required libraries and modify the condor generation scripts to work for your system's condor enviornment. You have been forewarned.

# Pipeline
The workflow for the analysis is as follows:
1. Calculate the effective area of GRAMS as a function of energy (GenCondorScripts.py in [EffArea](./EffArea))
2. Generate the background
    * Generate a "reference flux" and a "physical flux" of the background (ie. experimental data) to enable energy reweighting ([GenEnergySpectrum](./BackgroundRecon/GenEnergySpectrum.py))
    * Generate the background Compton Cones from the reference flux (GenCondorScripts.py in [BackgroundRecon](./BackgroundRecon))
3. Generate Compton cones from a monoenergetic, point-like source at some right ascension (RA) and altitude (ALT) location on the sky (NOTE: use radians) (GenCondorScripts.py in [SourceRecon](./SourceRecon/))
4. Define a "neighborhood" around the true source location based on the ARM (GenMask.py in [SourceRecon](./SourceRecon/))
5. For both the source and the background, calculate the number of Comptons cones that lie within the neighborhood of the source
    * For the background, we use the effective area and physical flux to reweight the data generated in step 2. This allows us to test variable exposure times and different physical background fluxes (GenCondorCountsHistsScripts.py in [utils](./utils/) which gets copied over on build)
    * For the source, we don't use any reweighting (same script as background case)
6. Assuming Poisson statistics, we calculate the number of source cones needed to have a 3/5 sigma threshold above the background at the source energy. (CalcSensitivity.cpp in [SourceRecon](./SourceRecon/))
    * Once we have the number of Compton cones from the source, we can then convert this number to to the number of photons needed to generate said Compton cones. Finally, we convert the number of photons needed to a sensitvitity, taking into account the effective area, exposure time, and energy of the photon

If you want to try and reproduce my results from scratch, start at [EffArea](./EffArea/).
# Files and Folders
Each folder contains a different aspect of calculating the sensitivity. For more information on a particular aspect of the simulation, please read the associated README.md in each folder.
* [BackgroundRecon](BackgroundRecon) contains the scripts necessary to set up a condor jobs for two purposes:
    1. Calculating the Compton cones generated from an isotropic MeV gamma ray source.
    2. Generating AllSkyMaps and Count histograms from said Compton Cones in a neighborhood around a source location
* [cmake](cmake/) contains the .cmake modules needed to build the project (taken directly from [here](https://github.com/wgseligman/GramsSim/tree/master/cmake))
* [EffArea](EffArea/) contains the scripts necessary to calculate the effective area of the detector as a function of energy
* [SenseJob](SenseJob/) contains the [GramsSim](https://github.com/wgseligman/GramsSim) executables/files with some modifications
* [SourceRecon](SourceRecon/) is similar to BackgroundRecon in that it calculates the Compton Cones generated from a monoenergetic point source. It also, given the reconstructed background counts, background flux, effective area and exposure time, can calculate the sensitivity of GRAMS to a point source at a given energy.
* [utils](utils/) contains a bunch of static libraries and programs that are used elsewhere
* RemoveClutter.sh is used to clean up .out, .err and .log files generated by condor. If you don't need these output files, then remove them by copying RemoveClutter.sh to the condor output folder and running
```
source RemoveClutter.sh
```
this makes looking at the root files in the output directory much easier (tab completion is not as glacially slow)
# Dependencies

To build the project, you will need [cmake](https://cmake.org/), [ROOT](https://root.cern/install/), [XercesC](https://xerces.apache.org/xerces-c/download.cgi) (xml parser), and [Eigen](https://eigen.tuxfamily.org/index.php?title=Main_Page).

If you ever want to rebuild the GramSim executables, you will also need [Geant4](https://geant4.web.cern.ch/) and [hepmc3](https://gitlab.cern.ch/hepmc/HepMC3) for everything and [healpix](https://healpix.sourceforge.io/) to build gramssky.

 If you are at Nevis, you don't have to worry about downloading any of these. Just run
```
module load cmake root geant4 hepmc3 healpix
```

# Building the Project
You can download the repository with the `git clone` command.

Navigate to the directory containing the github repo, but not the repo itself. Suppose that you want your build directory to be named "GramsSimSensitivityWork". Run the following commands:

```
module load cmake root geant4 hepmc3 healpix
mkdir GramsSimSensitivityWork
cd GramsSimSensitivityWork
cmake ../GramsSimSensitivity
make
```

This creates "GramsSimSensitivityWork" and generate the project in this folder.

# Making Changes
If you want to edit a .cpp file or header file, you need to change the file in the original GramsSimSensitivity directory. You then need to run the ```make``` command again.

If you want to temporarily edit any other file to test something out, you can stay in your build directory.

If you want to make a more permanent change to a file, go back to GramsSimSensitivity, make the edit there, go back to your build directory and run
```
cmake ../GramsSimSensitivity
```

# Caveats
There are couple of important things to keep in mind when interpreting your results from the above simulations.

The first is that experimental uncertainties are not taken into account (for instance, electronics noise, the discretization error associated with a pixel readout system, diffusion, recombination etc).

While GramsSim does have tools to model these uncertainties like [GramsElecSim](https://github.com/wgseligman/GramsSim/tree/master/GramsElecSim), [GramsReadoutSim](https://github.com/wgseligman/GramsSim/tree/master/GramsReadoutSim) and [GramsDetSim](https://github.com/wgseligman/GramsSim/tree/master/GramsDetSim), one needs to fiddle with a bunch of parameters in order to get reasonable results. In order to do this fiddling, experimental data is needed, which ostenstiably would come from either the results from the balloon test flights in Japan or the experimental setup in Northeastern. Until that data comes through, the output from these uncertainty programs are currently not included in the pipeline.

The second is that the optical photon simulation of GramsSim has yet to be completed. Whether sucessive scatters are optically isolated has a great impact on the effective area of the detector, as well as the reconstruction of the z coordinate of each scatter. Hence, this code will eventually need to be reworked to include the optical simulation results.

# Dataset
* On the nevis neutrino cluster:
    *   The background dataset I generated can be found at ```/nevis/riverside/data/ms6556/Background```. 10000 batches each with 20000 events generated for each batch
    *   1 MeV source photons generated along the x-ais ($<RA,ALT> = <0,0>$) can be found at ```/nevis/riverside/data/ms6556/Background```. 10000 batches with 20000 events generated for each batch (this was gross overkill, which I didn't know at the time)
    * Feel free to use and modify as you like, or generate your own data set (it takes anywhere between 12-24 hours to generate 2 billion events via condor)

# TODO
* Add in uncertainty programs to pipeline (electronics noise, detsim, pixel readout, optical simulation)
* Add minimum signal to noise background ratio and minimum count thresholds like [CTAO](https://www.cta-observatory.org/science/ctao-performance/#1472563157332-1ef9e83d-426c)
* Run analysis for more than a single energy (a good starting point would be to mimic the COMPTEL energy ranges and energy binnings)