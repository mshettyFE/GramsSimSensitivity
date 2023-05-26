#!/bin/bash
process=$1
n_batches=10000
total_events=200000000
event_per_batch=20000
mac_path=mac/Source1MeV.mac
export PATH=/sbin:/usr/sbin:/bin:/usr/bin
export G4NEUTRONHPDATA=/usr/local/share/Geant4-10.5.1/data/G4NDL4.5
source /usr/nevis/adm/nevis-init.sh
module load cmake root geant4 hepmc3 healpix
tar -xzf SenseJobSource1MeV.tar.gz
cd SenseJob
./gramssky -o SourceEvents.hepmc3  --RadiusSphere 300 --PositionGeneration Point --PointSource "1,0,0" --EnergyMin 0.1 --EnergyMax 10.0 -n ${event_per_batch} -s ${process} -r ${process}  --OriginSphere "(0,0,-40.0 )"  --EnergyGeneration Fixed --FixedEnergy 1
./gramsg4 -g ThinGrams.gdml -i SourceEvents.hepmc3  -s ${process} -r ${process} -o Source_${process} -m ${mac_path}
./gramsdetsim -i Source_${process}.root -s ${process} -o Source_Det_${process}.root
./Extract --GramsG4Name Source_${process}.root --GramsDetSimName Source_Det_${process}.root -o Source_Extracted_${process}.root
./Reconstruct -i Source_Extracted_${process}.root -o Source_Reco_${process}.root --SourceType Iso 
cp Source_Reco_${process}.root ..
cp Source_Extracted_${process}.root ..
cd ..
