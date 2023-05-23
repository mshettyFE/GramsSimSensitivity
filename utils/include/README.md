Contains the header files used in a variety of other programs.

* ExtractUtils.h contains functions used for extracting Compton scatter data from GramsSim
* GenSkyMapUtils.h contains Compton Cone reconstruction functions
* Options.h is copied wholesale from [here](https://github.com/wgseligman/GramsSim/tree/master/util). This utility reads in an xml file where you can specify various parameters to be used in a program. Each parameter can be overridden by arguments in the command line. See the link for a more comprehensive explaination of the system
* ReadRootFiles.h contains functions that read in the ROOT file outputs of Extraction, Reconstruction, GramsG4 and GramsDetSim
* ReconstructUtils.h contains functions used for generating Compton Cones from the output of Extract