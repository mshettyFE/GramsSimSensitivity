Creates an executable named 'Extract' that takes in the output of GramsG4 and GramsDetSim and extract the Compton scatter series.

Saves a root file containing a list of scatters series. Each scatter series consists of the parent gamma ray and a number of compton scatters/photoabsorbtions

# Parameters

Running ./Extract -h yields
```
    [ --Binnings <Size of a cell in each direction> ]
    [ --Dimensions <Dimensions of LArTPC (x,y,z)> ]
    [ --EnergyUnit <energy unit for program output> ]
    [ -o | --ExtractOutput <Output of Extract> ]
    [ -f | --Filter <How to filter each event> ]
    [ --GramsDetSimName <Name of input GramsDetSim file that was generated from GramsG4 file> ]
    [ --GramsG4Name <Name of Input GramsG4 File> ]
    [ --LengthUnit <length unit for program output> ]
    [ --Seperation <Seperation between adjacent scatters. Used in Sphere option of Filter> ]
    [ --TimeUnit <time unit for program output> ]
    [ --comment <document purpose of run> ]
    [ -d | --debug ]                    
    [ -h | --help ]                     # show help then exit
    [ --options <XML file of options> ] 
    [ -s | --rngseed <random number seed> ]
    [ -v | --verbose ]                  # display details
```

A lot of these are unused in Extract since they aren't relavent to this program (they are there because they are global parameters used in GramsSim, and I didn't want to have multiple options.xml files).

## Actually Useful Options
* help: prints the output displayed above
* verbose: for when you want more information on what's happening while the program is running
* GramsG4Name: Name of input file from gramsg4
* GramsDetSimName: Name of input file from gramsdetsim
* ExtractOutput: Name out final output file
* LengthUnit: specify units of either cm or mm. Defaults to cm.
* Filter: dictates what selection criteria to apply. Defaults to Standard
    * Standard: for "all in events" (all Compton scatters inside LAr), we need at least 2 compton scatters. For "Escape events" (the photon escapes the detector), we need at least 3 compton scatters. For either scatter series type, we require that all the Compton scatters to originate from the primary gamma ray (ie. no showers)
    * Sphere: Same as Standard, but with the additional criteria that each compton scatter/photoabsorbtion is at least some distance away from each other
        * Seperation: denotes how far apart each scatter must be if using 'Sphere' filter. units same as LengthUnit. Defaults to 10 (cm)
    * UniqueCells: same as Standard, but also demands that each temporally adjacent scatter series occurs in it's own optically isolated cell
        * Dimensions: a 3D vector that specifies the x,y,z lengths of the LArTPC of GRAMS. Same units as LengthUnit. Defaults to <70,70,80>
        * Binnings: a 3D vector containing the number of optically isolated cells along each axis (rounded down to integers.) Defaults to <7,7,8>