Initial sensitivity calculations form GRAMS.
Importantly, does not take into account experimental errors.

# Building Project
Go to the directory containing the github repo, but not the repo itself. Run the following commands.

NOTE:
```
the module load cmake root
```

loads in root and cmake on the Nevis cluster. Substitute this for whatever command will load in these programs into the enviornment.

```
module load cmake root
mkdir BuildSense
cd BuildSense
cmake ../BuildSense
make
```
