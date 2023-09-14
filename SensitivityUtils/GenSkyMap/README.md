Given a dataset of Compton cones, an effective area histogram, and a binary mask, calculates the energy dependent count distribution and sky map reconstruction within the mask.

# Parameters
Running ./GenCountsHists -h yields
```
  ./GenCountsHists
    [ --EffAreaFile <Effective Area histogram generated from CalcEffArea.py> ]
    [ --EnergyUnit <energy unit for program output> ]
    [ -t | --ExposureTime <The exposure time that the input events correspond to> ]
    [ -i | --Input <Input root file from Reconstruct.cpp program> ]
    [ --LengthUnit <length unit for program output> ]
    [ --MaskFile <Binary mask file. From GenMask.py> ]
    [ -N | --NPts <Number of Points generated for each cone> ]
    [ -o | --Output <Output root file containing Counts TH1D histogram and TH2D Sky Map> ]
    [ --PhysicalFluxFile <Physical energy flux that follows a user defined distribution. From GenEnergySpectrum.py> ]
    [ --ReferenceFluxFile <Reference energy flux that follows a Log Uniform distribution. From GenEnergySpectrum.py> ]
    [ --TimeUnit <time unit for program output> ]
    [ --TotalEvents <The total number of events assigned to the batch job> ]
    [ --comment <document purpose of run> ]
    [ -d | --debug ]                    
    [ -h | --help ]                     # show help then exit
    [ --options <XML file of options> ] 
    [ -s | --rngseed <random number seed> ]
    [ -v | --verbose ]                  # display details
    [ -w | --weighted ]                 # Specify if we are implementing reweighting scheme
```

Just like [Extract](../Extraction/), some of these options do nothing (see Extract for the why this is the case). The implemented options are as follows:
* help: print the above
* verbose: provide more output
* Input (REQUIRED): Input root file name containing Compton cone data from Reconstruct
* Output (REQUIRED): Output root file name containing Counts histogram and SkyMap reconstruction
* MaskFile (REQUIRED): the name of the binary mask to apply. Created from [GenMask.py](../GenMask.py).
* EffAreaFile (REQUIRED): the name of the effective area root histogram. Created from [CalcEffArea.py](../../EffArea/CalcEffArea.py)
* NPts: The number of points to generate per Compton cone. Defaults to 1000
* weighted: Flag to state wheather the reweighting scheme is being used. NOTE: If set, requires ReferenceFluxFile, PhysicalFluxFile, ExposureTime and TotalEvents to be set
* ReferenceFluxFile: A root histogram plotting a log-uniform distribution. Generated from [GenEnergySpectrum.py](../../Background/GenEnergySpectrum.py)
* PhysicalFluxFile: a root histogram plotting the physical background flux. Generated from [GenEnergySpectrum.py](../../Background/GenEnergySpectrum.py). Currently set to spectrum found on pg. 7 from [this paper](https://arxiv.org/abs/2212.05713). Feel free to change if needed.
* ExposureTime: How long the generated background gamma rays correspond to in seconds
* TotalEvents: The total number of gamma rays generated to produce the background dataset (the set that I make has  10,000 batches * 20,000 photons per batch)