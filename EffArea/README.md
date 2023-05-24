# Files and Folders Generated
Contains the code needed to calculate the effective area of the detector.

Once you run cmake and make, you end up with the following in this directory (ignoring all the cmake and make leftovers):

* AggregateEffArea
* CommandsSphere.txt
* Commands.txt
* CommandsUnique.txt
* GenCondorScripts.py
* libGramsSensitivityCalcEffAreaFuncs.so
* options.xml
* SenseJob/

# Running Condor Jobs
To calculate the effective area, you first need to run GenCondorScripts.py. This script takes in a output directory, condor job name, and .txt file and does the following:
* creates the specified output directory
* Generates the necessary .cmd, .sh, and .mac files
    * Optionally, creates a unique .mac file for each job in SenseJob/mac/batch/
* Merges the .txt file into the shell script
* Creates a tarball from SenseJob

 You can run
```
python GenCondorScripts.py -h
```
to get a feel for what options you can tweak.

As an example, the following
* Creates a directory at ${output_dir}
* Creates a .cmd file that has 10 batches that generate 1000000 events in total. 
* Appends the contents of CommandsSphere.txt to a partially generated shell script
* Assigns each batch a unique incident particle energy via the optional macro files
* Zips SenseJob with a name consistent with .cmd file

```
python GenCondorScripts.py ${output_dir}  EffAreaSphere CommandsSphere.txt -m -s log --BatchMacroName Mono -nb 10 -ne 1000000
```

Once this is done running, you should see EffAreaSphere.cmd and EffAreaSphere.sh in the folder. To run your job on condor, run the following command:
```
condor_submit EffAreaSphere.cmd -batch-name EffAreaSphere
```

You can monitor your jobs with 
```
condor_q
```