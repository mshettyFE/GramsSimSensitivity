Write up TOML config file
    Have a shared header where shared arguments reside
    Each program has its own header, with arguments unique to it underneath
    Have the option to add and remove programs to shell script and cmd at will
Refactor GenEffArea, GenCondorScripts, GenCondorCountsHists into 1 script
    Have script take in either Area, Source, Background. Then generate associate files
    Have Reco, Extracted, and SkyMap all be generated in one condor job and have the option to skip over generating a certain output file
            Still need to make sure that each program has the necessary inputs
                IDEA: Preappend Reco,Extract, and SkyMap to output of each program and use filename to check on a per job basis
                Extend this idea to GramsSim programs
Refactor folders to take into account new condor job structure
Rewrite documentation