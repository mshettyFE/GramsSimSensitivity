This repository contains the various scripts and tools used to calculate the sensitivity of the GRAMS detector to MeV gamma rays. Based on the Geant4 simulation [GramsSim](https://github.com/wgseligman/GramsSim).

# Folders
Each folder contains a different aspect of calculating the sensitivity. For more information on a particular aspect of the simulation, please read the associated README.md in each folder.
* [BackgroundRecon](BackgroundRecon/README.md) contains the scripts necessary to set up a condor jobs for two purposes:
    1. Calculating the Compton cones generated from an isotropic MeV gamma ray source.
    2. Generating AllSkyMaps from said Compton Cones
* [cmake](cmake/README.md) contains the .cmake modules needed to build the project (taken directly from [here](https://github.com/wgseligman/GramsSim/tree/master/cmake))
* [EffArea](EffArea/README.md) contains the scripts necessary to calculate the effective area of the detector
* [SenseJob](SenseJob/README.md) contains the GramsSim executables/files upon which all of these simulations are based off of.
* [SourceRecon](SourceRecon/README.md) is similar to BackgroundRecon in that it calculates the Compton Cones generated from a monoenergetic point source. It also, given the reconstructed background skymaps, background flux, effective area and exposure time, can calculate the sensitivity of GRAMS to a point source at a given energy.
* [utils](utils/README.md) contains a bunch of static libraries and programs that are used elsewhere

# Building the Project
You will need to be able to install cmake, ROOT, and XercesC.

Go to the directory containing the github repo, but not the repo itself. Run the following commands:

```
module load cmake root
mkdir GramsSimSensitivityWork
cd GramsSimSensitivityWork
cmake ../GramsSimSensitivity
make
```

NOTE:
```
module load cmake root
```
is unique to the Nevis cluster. You need to substitute that with whatever commands will load in cmake and root on your machine.

# TODO
There are two very important caveats to keep in mind when interpreting your results from the above simulations.

The first is that experimental uncertainties are not taken into account (for instance, electronics noise, the discretization error associated with a pixel readout system, energy uncertainties etc).

While GramsSim does have tools to model these uncertainties like [GramsElecSim](https://github.com/wgseligman/GramsSim/tree/master/GramsElecSim) and [GramsReadoutSim](https://github.com/wgseligman/GramsSim/tree/master/GramsReadoutSim), one need to fiddle with a bunch of parameters in order to get reasonable results. In order to do this fiddling, experimental data is needed, which ostenstiably would come from the results of the testing rigs in Japan and Northeastern. Until that data comes through, GramsElecSim is not included in the pipeline.

The second is that the optical photon simulation of GramsSim has yet to be completed. Whether sucessive scatters are optically isolated has a great impact on the effective area of the detector, as well as the reconstruction of the z cooridnate of each scatter. Hence, this code will eventually need to be reworked to include the optical simulation.