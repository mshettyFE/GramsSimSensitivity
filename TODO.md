Seperate Area, Source, Background and have GenCondor.py script choose what to do
Refactor GenEffArea, GenCondorScripts, GenCondorCountsHists into 1 script
    Have script take in either Area, Source, Background. Then generate associate files
Refactor folders to take into account new condor job structure
Refactor CalcSensitivity to python script to replace current CalcSensitvity.cpp in order to be compatible with .toml file
Rewrite documentation
