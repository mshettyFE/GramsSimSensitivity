Initial sensitivity calculations form GRAMS.
Importantly, does not take into account experimental errors.

# Building Project
Go to the directory containing the github repo, but not the repo itself. Create a directory
<code>
# Set up the tools as appropriate for your system
# For example, at Nevis type
module load cmake root

# Create a separate build/work directory. This directory should
# not be the Sensitivity directory or a sub-directory of it. This
# only has to be done once. 
mkdir BuildSense

# Build
cd BuildSense
cmake ../BuildSense
make
<code>
