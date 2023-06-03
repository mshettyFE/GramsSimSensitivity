Creates Reconstruct Executable that takes in the scatter series data from Extract and generates the Compton cone for each series.

# Parameters
You can run ```./Reconstruct -h``` to view the options.

```
  ./Reconstruct
    [ --EnergyUnit <energy unit for program output> ]
    [ -i | --Input <Input root file from Extract.cpp program> ]
    [ --LengthUnit <length unit for program output> ]
    [ -o | --Output <Output Root file containing cone data and ARM. Defaults to 2*pi for isotropic> ]
    [ --SourceLoc <Right Ascension and Altitude coordinates of source. See options.xml for details> ]
    [ --SourceType <Type of input source. Either 'Iso' or 'Point'> ]
    [ --TimeUnit <time unit for program output> ]
    [ --comment <document purpose of run> ]
    [ -d | --debug ]                    
    [ -h | --help ]                     # show help then exit
    [ --options <XML file of options> ] 
    [ -s | --rngseed <random number seed> ]
    [ -v | --verbose ]                  # display details
```

Just like [Extract](../Extraction/), some of these options do nothing (see Extract for the why this is the case). The implemented options are as follows:
* help: print the above
* verbose: print more output while running
* Input: the output file from Extract used as input for Reconstruct
* Output: the output file name from Reconstruct
* SourceType: what type of source you have. Options are Point or Iso
    * Point: Assumes a point like source. Calculates physical ARM values by subtracting reconstructed angle from truth angle
        * SourceLoc: Denotes where the point source is. a 2D vector specifying RA and ALT values in radians.
    * Iso: Assumes an isotropic source. write 2pi to ARM variable (ie. non-sense value). This is so that the format of the background and source outputs are the same