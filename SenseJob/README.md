Contains the GramSim executables and files used ubiquitously in other folders.
The ones that aren't from GramsSim are
* ThinFlatGrams.gdml
* ThinGrams.gdml
* Modified options.xml

ThinFlatGrams.gdml and ThinGrams.gdml modify the seperator sheets to be negligably thin. Both of these are primarily based on the gdml file found [here](https://github.com/wgseligman/GramsSim/blob/master/grams.gdml).

The "Flat" refers to a pancake geometry of the LArTPC with dimensions of 140x140x20 cm. The default is a cube of dimensions 70x70x80 cm. Note that these both have the same volume.

The idea behind the cube is that you will have a more uniform response from all possible source locations, while the pancake is more receptive to sources from the zenith. The pancake also should have less of a problem maintaining a uniform electric field between the anode and cathode.

For MeV gamma ray detection, we default to a cubic structure.

The options.xml contains all the parameters used by the cpp executables (since C++ doesn't come with an equivalent to argparse natively). Look inside for details. If you want a through explaination for how to use/modify the .xml file, see [here](https://github.com/wgseligman/GramsSim/tree/master/util).