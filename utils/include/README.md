Contains the header files used in a variety of other programs.

* CalcSenseUtils.h holds the functions used in [CalcSensitivity.cpp](../../SourceRecon/CalculateSensitivity.cpp). Namely, functions to aggregate histograms and extract bin contents of histograms.
* ExtractUtils.h contains functions used for extracting Compton scatter data from GramsSim. Used in [Extract.cpp](../Extraction/Extract.cpp)
* GenBackCountsHists.h contains Compton Cone reconstruction functions. Used in [GenCountsHists.cpp](../GenCountsHists/GenCountsHists.cpp)
* Options.h is copied wholesale from [here](https://github.com/wgseligman/GramsSim/tree/master/util). This utility reads in an .xml file where you can specify various parameters to be used in a program. Each parameter can be overridden by arguments in the command line. See the link for a more comprehensive explaination of the system
* ReadRootFiles.h contains functions that read in the ROOT file outputs of Extraction, Reconstruction, GramsG4 and GramsDetSim
* ReconstructUtils.h contains functions used for generating Compton Cones from the output of Extract
* UsefulTypeDefs.h is what is says on the tin. Namely, putting all the typedefs in one place so that code in other files is easier to read. Importantly, it contains what each elements in a std::tuple corresponds to