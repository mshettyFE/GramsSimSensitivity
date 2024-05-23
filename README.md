# Introduction
GramsSimSensitivity is for gamma-ray sensitivity calculations for the upcoming GRAMS (Gamma Ray and Anti-Matter Survey) experiment. The actual physics simulations of the gamma-rays propagating through the detector is performed by [GramsSim](https://github.com/wgseligman/GramsSim). This repository is a wrapper around GramsSim which provides syncrhonization between all of the various condor jobs and scripts of the sensitivity calculation.

While this code is designed to run on the Nevis condor batch farm, it should be possible with some tweaking to run on similar condor farms (the preamble written to the various .cmd files needs to be modified to accomplish this; see the [condor script generator](./GenCondorJobs.py) for details)
# Building
## Conda Enviornment
After cloning the repositiory with the command:
```
git clone https://github.com/mshettyFE/GramsSimSensitivity.git
```
cd into GramsSimSensitivity
* If you are at Nevis:
    * You can set up your enviornment by running:
        *  ```conda activate /nevis/riverside/share/ms6556/conda/envs/GramsDev```
* If you are not at Nevis:
    * You first need to install conda (see [here](https://conda.io/projects/conda/en/latest/user-guide/install/index.html))
    * Run the following commands to download the requisite packages for GramsSim and GramsSimSensitivity
```
env_name="grams-dev"
conda $env create -f environment.yml
```
Once everything is done, run the following to activate the enviornment:
* ```conda $env activate```
## Set up
* In order to build GramsSim from source, as well as prepare the  ```GramsWork``` directory (which is the folder that gets transferred to the condor batch farms), run ```config.sh```:
```
./config.sh # In GramSimSensitivity directory
```

* Run ```./compile.sh``` in root directory of repository. What this does create a ```build``` directory, cd into it, run cmake+make, set up a directory structure, transfer over the files, and then finally, clean up temporary files.
* (Side note: this process of building a project in a different directory that the source is called "out of source" building, and is a common paradigm for building with cmake. GramsSim also does this)
* Your final directory structure should look like the following, with GramsSimSensitivity and $WorkDirName on the same level:
```
.
├── GramSimSensitivity
    ├── ...
└── $WorkDirName
    ├── ...
```

# _IMPORTANT!_
* If you make any changes to the GramsSimSensitivity files, you need to re-run ```compile.sh``` so that the changes get mirrored in the build directory!!!
* All of the scripts mentioned in [Workflow](#Workflow) should be run in the $WorkDirName directory.
# Workflow
The overall workflow of the sensitivity calculation is as follows:
* Calculate the effective area of the detector for a given energy and geometry
* Calculate the detector response to a point source of a given energy
    * As part of this step, generate a "local" background to the source via the ARM (Angular Resolution Measure) at the given energy
* Calculate the detector response to some isotropic background
    * The background follows some power law flux distribution, which is compared to some reference distribution for scaling purposes
* Calculate the differential point source sensitivity at that particular energy
## Creating Condor Jobs for Each Subtask
In order to coordinate between the condor job scripts across each subtask, the following [script](./GenCondorJobs.py) is utilized along with a [.toml](./ConfigFiles/Configs.toml) file. 
### Job Generation
The script generates .cmd, .sh and .tar.gz files which are needed to specify a condor job for a particular subtasks. The script also ensures that arguments between jobs are compatible with each other (assuming that .toml file doesn't change in the interim time). Arguments are as follows:
* config: The path to the .toml config file
* --Job: What Sub task you want to run: EffectiveArea, Source and Background
* --JobType: What output data you want. "Cones" corresponds to the MC-truth level information for each scatter series, as well as Compton Cone data. "SkyMap" corresponds to the 2D skymap associated with all the Compton cones, as well as the number of cones which fall within the "local background"
* -b: Is used to run the script without any safety prompts. Normally, the script will check if you have the potential to overwrite previously generated root files upon running your condor job and warn you about this possibility. The -b flag just assumes you are aware of this risk and will generate condor jobs anyways.
### Config file
* The default Config file can be found [here](./ConfigFiles/Configs.toml).
    * See the  [the full spec](https://toml.io/en/v1.0.0) for more information on the .toml format
* Feel free to modify as needed. A TOML validator can be found [here](https://www.toml-lint.com/).
### NOTE
* After running a sucessful job, you can copy [the clean up script](./RemoveClutter.sh) to the condor output directory and run it to clean up the temporary files.

## Subtasks
* For all of these, make sure that you are in the working directory and NOT GramsSimSensitivity. Also, make sure to modify the variables in Configs.toml to your liking PRIOR to running any tasks (changing Configs.toml mid-analysis might create inconsistencies).
    * NOTE: Of particular importance is getting the ```output``` variable under ```[General]``` to point to the correct location. At Nevis, this should point to some ```data``` partition. In general though, this should point to some directory OTHER than the GramsSimSensitivity or Work directory. 
### Effective Area
Running an effective area calculation consists of the following:
1. cd-ing into the ```EffArea``` directory
2. Run the following command: ``` python3 GenCondorJobs.py Configs.toml --Job EffectiveArea --JobType Cones```
3. Run ```condor_submit``` on the .cmd files that gets generated
    *   You can check the status of the job with ```condor_q```
4. Once the batch job is done, run ```python3 CalcEffArea.py Configs.toml```
### Source
To generate the source, do the following:
1. cd-ing into the ```Source``` directory
2. Run ```python3 GenMask.py Configs.toml```
3. Run ```python3 GenCondorJobs.py Configs.toml --Job Source --JobType Cones```
4. Run ```condor_submit``` on the .cmd files that gets generated
5. Once that job is done, run ```python3 GenCondorJobs.py Configs.toml --Job Source --JobType SkyMap```
6. Run ```condor_submit``` on the .cmd files that gets generated

### Background
To generate the background, do the following:
1. cd-ing into the ```Background``` directory
2. Run ```python3 GenCondorJobs.py Configs.toml --Job Background --JobType Cones```
3. Run ```condor_submit``` on the .cmd file that gets generated
4. Once that job is done, run ```python3 GenCondorJobs.py Configs.toml --Job Background --JobType SkyMap```
5. Run ```condor_submit``` on the .cmd files that gets generated

### Sensitivity Calculation
1. Assuming that you ran all the other scripts, just run the following in the ```Sensitivity``` folder to calculate the sensitivity:
```
python CalculateSensitivity.py Configs.toml
```