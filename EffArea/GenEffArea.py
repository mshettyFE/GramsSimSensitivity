import numpy as np
import math

energies = np.logspace(math.log10(0.1),math.log10(10),num=1000)
n = 20000
energy_str = "energies=( "
for e in energies:
    energy_str = energy_str + ' \"'+str(e)+'\" '
energy_str += ")\n"
with open("EffAreaRedo.sh",'w') as f:
    f.write("""#!/bin/bash
process=$1
export PATH=/sbin:/usr/sbin:/bin:/usr/bin
export G4NEUTRONHPDATA=/usr/local/share/Geant4-10.5.1/data/G4NDL4.5
source /usr/nevis/adm/nevis-init.sh
module load cmake root geant4 hepmc3 healpix
tar -xzf EffAreaRedo.tar.gz
cd SenseJob\n""")
    f.write(energy_str)
    f.write("echo ${energies[$process]}\n")
    f.write("./gramssky -o Events.hepmc3  --RadiusSphere 300 --RadiusDisc 200 --PositionGeneration Iso --PhiMinMax \"(-3.14159,3.14159)\" --ThetaMinMax \"(0,3.14159265)\" -n "+str(n)+" -s ${process} -r ${process}  --OriginSphere \"(0,0,-40.0 )\" --Fixed ${energies[$process]}\n")
    f.write("./gramsg4 -g ThinGrams.gdml -i Events.hepmc3  -s ${process} -r ${process} -o EffArea_${process} -m mac/EffAreaHepmc3.mac\n")
    f.write("./gramsdetsim -i EffArea_${process}.root -s ${process} -o EffAreaDet_${process}.root\n")
    f.write("./Extract --GramsG4Name EffArea_${process}.root --GramsDetSimName EffAreaDet_${process}.root -o EffAreaExtract_${process}.root\n")
    f.write("./Reconstruct  -i EffAreaExtract_${process}.root -o EffArea_Reco${process}.root\n")
#    f.write("./EffArea EffArea_Reco${process} EffA${process}.txt "+str(n)+" 200\n")
#    f.write("cp EffA${process}.txt ..\n")
    f.write("cp EffArea_Reco${process}.root ..\n")
    f.write("cd ..")
