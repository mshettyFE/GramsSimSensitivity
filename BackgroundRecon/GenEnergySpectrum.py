# Program to generate reference flux (log-uniform) and physical flux (ie. the background)
# These are utilized in the reweighting scheme

import ROOT
import math
import sys
import numpy as np
import argparse
from array import array

## Flux function must have the form func(E,**kwargs)
## Please make sure that you sanatize your inputs like below when you write your own
## Also, physical fluxes should have units of 1/(MeV cm^2 s sr) to be consistent with the rest of the programs

def BrokenPowerLaw(E,**kwargs):
## Broken power law spectrum. 
# Background MeV Spectra found here: https://iopscience.iop.org/article/10.3847/1538-4357/acab69
    try:
    ## Constant before flux function has units of 1/(MeV cm^2 s sr)
        A = kwargs["A"]
        if(A<=0):
            print("Invalid constant")
            sys.exit()
    except:
        print("Invalid constant")
        sys.exit()
    try:
        Eref = kwargs["Eref"]
    except:
        print("Eref not specified")
        sys.exit()
    try:
        Gamma1 = kwargs["Gamma1"]
    except:
        print("Gamma1 not specified")
        sys.exit()
    try:
        Gamma2 = kwargs["Gamma2"]
    except:
        print("Gamma2 not specified")
        sys.exit()
    if E<=Eref:
        return A*np.power((1/Eref)*E,-Gamma1)
    else:
        return A*np.power((1/Eref)*E,-Gamma2)

def LogUniform(E,**kwargs):
## Suppose that you treat this function as a probability distribution.
## If you draw enough points and set energy to a log scale, then you get a uniform distribution
## This is desirable for sensitivity studies since energies span a wide range of orders of magnitude
## This is the Reference energy-dependent flux used in GenSkyMap, hence the units don't really matter for this one
    try:
        a = kwargs["a"]
    except:
        print("a not specified")
        sys.exit()
    try:
        b = kwargs["b"]
    except:
        print("b not specified")
        sys.exit()
    scaling = np.log10(b/a)
    return np.power(E,-1)/scaling

if __name__ == "__main__":
## Read in effective area for binnings, and the output file names of the Reference and Background fluxes
    parser = argparse.ArgumentParser(
                    prog='GenEnergySpectrum.py',
                    description='Generate energy-dependent flux. Used for reweighting of background.')
    parser.add_argument("EffAreaFile", help="Effective Area file. Will use the binnings from this file for the physical and reference energy binnings")
    parser.add_argument('-pfn','--PhysicalFileName', default="BackgroundFlux.root",help="Name of physical flux root file")
    parser.add_argument('-rfn','--ReferenceFileName', default="ReferenceFlux.root",help="Name of reference flux root file")
    args = parser.parse_args()
    EffAreaFile = ROOT.TFile(args.EffAreaFile, "READ")
    EffAreaHist = EffAreaFile.EffArea
    BackgroundFile = ROOT.TFile(args.PhysicalFileName ,"RECREATE")
## NOTE: When using Clone(), the title doesn't get changed. Make sure that you are aware of this
## You can change this if you want to with SetNameTitle(), but I didnt' care enough to do so
    BackgroundEnergyHist = EffAreaHist.Clone("BackgroundFlux")
    BackgroundEnergyHist.Reset()
## Get centers of bins. Use these bin centers as the x axis values
    EnergyCenters = [BackgroundEnergyHist.GetBinCenter(i) for i in range(BackgroundEnergyHist.GetNbinsX())]
## Need to modify the following line if you have a different background flux
# Background MeV Spectra found here: https://iopscience.iop.org/article/10.3847/1538-4357/acab69
    BackOutput = [BrokenPowerLaw(energy,A=2.2E-4,Eref=3,Gamma1=3.3,Gamma2=2) for energy in EnergyCenters]
## Fills output histogram with flux function by filling each bin weighted by the function
    for bin in range(0,BackgroundEnergyHist.GetNbinsX()):
        BackgroundEnergyHist.SetBinContent(bin,BackOutput[bin])
## Save to disk
    BackgroundFile.WriteObject(BackgroundEnergyHist, "PhysicalFlux")
## Create reference file
    ReferenceFile = ROOT.TFile(args.ReferenceFileName ,"RECREATE")
    ReferenceEnergyHist = EffAreaHist.Clone("ReferenceFlux")
    ReferenceEnergyHist.Reset()
    EnergyCenters = [ReferenceEnergyHist.GetBinCenter(i) for i in range(ReferenceEnergyHist.GetNbinsX())]
# Standard LogUniform distribution (ie. equal probabilities amoungst log bins)
# Bottom and top edge are the min and max energies of EffArea (normally 0.1 to 10 MeV)
    bottom_edge = EffAreaHist.GetBinLowEdge(0)
    center_of_top =  ReferenceEnergyHist.GetBinCenter(ReferenceEnergyHist.GetNbinsX())
# Have to do some weird stuff to get the top edge since ROOT excludes the upper limit from the histogram
    top_edge = (center_of_top-ReferenceEnergyHist.GetBinLowEdge(ReferenceEnergyHist.GetNbinsX()))+center_of_top
    RefOutput = [LogUniform(energy,a=bottom_edge,b=top_edge) for energy in EnergyCenters]
## Fills output histogram with flux function by filling each bin weighted by the function
    for bin in range(0,ReferenceEnergyHist.GetNbinsX()):
        ReferenceEnergyHist.SetBinContent(bin,RefOutput[bin])
## Save to disk
    ReferenceFile.WriteObject(ReferenceEnergyHist,"ReferenceFlux")