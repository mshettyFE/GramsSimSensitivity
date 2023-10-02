Seperate Area, Source, Background and have GenCondor.py script choose what to do
Refactor GenEffArea, GenCondorScripts, GenCondorCountsHists into 1 script
    Have script take in either Area, Source, Background. Then generate associate files
    Have Reco, Extracted, and SkyMap all be generated in one condor job and have the option to skip over generating a certain output file
            Still need to make sure that each program has the necessary inputs
                IDEA: Preappend Reco,Extract, and SkyMap to output of each program and use filename to check on a per job basis
                Extend this idea to GramsSim programs
Refactor folders to take into account new condor job structure
Refactor CalcSensitivity to python script to replace current CalcSensitvity.cpp in order to be compatible with .toml file
Rewrite documentation
