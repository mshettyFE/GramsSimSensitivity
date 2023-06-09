This folder contains the files needed to generate the background dataset.

# Files and Folders
Once you run cmake in your build directory, you get the following in this folder (excluding cmake and make files):
* /BackgroundCountHist: folder to be zipped when running condor jobs generated from GenCondorCountsHistsScripts.py
* Commands.txt: Example shell commands that get inserted into condor shell script by GenCondorCountsHistsScripts.py to be run on condor.
* GenCondorCountsHistsScripts.py: (see [utils](../utils/README.md/#generating-sky-maps-and-counts) for details)
* GenCondorScripts.py: (see [utils](../utils/README.md/#running-condor-jobs-to-generate-cones) for details)
* GenEnergySpectrum.py: Generate the reference log-uniform energy distribution for the background, as well as the physical flux to reweigh to
* GenMask.py: see [utils](../utils/README.md/#defining-neighborhood-to-source-ie-genmask)
* README.md
* /SenseJob: folder to be zipped when running condor jobs generated from GenCondorScripts.py

# Workflow
* Prior to running this, you first need to run the scripts in [EffArea](../EffArea/) and [SourceRecon](../SourceRecon/) (barring CalcSensitivity of course).
* Once that is done, you need to create two folders on a /data partition (let's call them \${Cones} and \${Counts}) which stores the output of the condor batch jobs generated by GenCondorScripts.py and GenCondorCountsHistsScripts.py respectively.
* Run GenCondorScripts.py (see [utils](../utils/README.md/#generating-sky-maps-and-counts) for details). For illustrative purposes, suppose that you generated a .cmd file named BackCones.cmd
* use condor_submit to send your jobs to the farm
    * NOTE: it is useful to try out your jobs for a small number of batches, where each batch has the same number of events that the full run would assign to each job. This allows you to catch typos and resource issues before swinging for the fences and submitting the production run.
    * the syntax looks like ```condor_submit BackCones.cmd```
        * You can add an additional ```-batch-name``` argument to the command to give your job a more descriptive name
* You can monitor progress with ```condor_q```
* Once the jobs are done running, make sure that there are no significant errors/ everything went OK. Then run [RemoveClutter.sh](../RemoveClutter.sh) in the ${Counts} directory to remove the condor output files
* You should copy over the mask file produced in [SourceRecon](../SourceRecon/) and the effective area root file from [EffArea](../EffArea/) to your BackgroundCountHist folder to be consistent
* Run GenEnergySpectrum.py to produce the reference and physical flux files (see below). Then move the flux files to BackgroundCountHists
* Using the Reco files containing the cone data in \${Cones}, run GenCondorCountsHistsScripts.py with your output directory set to ${Counts} (see [utils](../utils/README.md/#generating-sky-maps-and-counts) for details) with the tar folder BackgroundCountHist

* Test your jobs on a small scale, then run ```condor_submit``` on the production run
# Next Steps
Once your jobs are done, move over to [here](../SourceRecon/README.md/#sensitivity-calculation) to finally calculate the sensitivity for your source.

# GenEnergySpectrum.py
* Creates a log-uniform reference flux file that background is generated from, as well as a physical flux file that is used for reweighting
* The only required parameter is the effective area root file. This is so that we have the same energy binnings across all files
* The optional parameters only serve to rename the outputs. The default reference flux file is "ReferenceFlux.root" and the default physical flux file is "BackgroundFlux.root"