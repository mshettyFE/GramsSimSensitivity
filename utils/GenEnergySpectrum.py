import ROOT
import math
import sys
import numpy as np
import argparse
from array import array

def BrokenPowerLaw(E,**kwargs):
## Broken power law spectrum. Flux function must have the form func(E,**kwargs)
## Please make sure that you sanatize your inputs like below when you write your own
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
        return np.power((1/Eref)*E,-Gamma1)
    else:
        return np.power((1/Eref)*E,-Gamma2)


if __name__ == "__main__":
## Read in output file name, min and max energies, and bin width
    parser = argparse.ArgumentParser(
                    prog='GenEnergySpectrum.py',
                    description='Generate energy spectrum of background. Log scale on x')
    parser.add_argument('-o', '--OutputFileName', default="EnergySpec.root",help="Name of output file")
    parser.add_argument('-p','--Plot',action='store_true')
    parser.add_argument("--minE",type=float, default = -1, help="Minimum energy of range. Note this is log scale (ie 0.1 corresponds to -1)")
    parser.add_argument("--maxE",type=float, default=1, help="Maximum energy of range.  Note this is log scale (ie 0.1 corresponds to -1)")
    parser.add_argument("-n","--NumDiv",default=100, help="Number of divisions")
    args = parser.parse_args()

## Generate logarithmic binnings
    new_bins = [0]*(args.NumDiv+1)
    width = (args.maxE-args.minE)/(args.NumDiv)
    for i in range(0,args.NumDiv+1):
        pwr = args.minE+i*width
        new_bins[i] = 10**pwr
## Create histogram with log binnings
    EnergyHist = ROOT.TH1D("BackgroundSpectrum", 'Background Flux', args.NumDiv, array('d',new_bins) )
## Get centers of log bins
    EnergyCenters = [EnergyHist.GetBinCenter(i) for i in range(args.NumDiv)]
## Need to modify the following line if you have a different function
    output = [BrokenPowerLaw(energy,Eref=3,Gamma1=3.3,Gamma2=2) for energy in EnergyCenters]
## Fills output histogram with flux function by filling each bin weighted by the function
    for i in range(len(output)):
        EnergyHist.Fill(EnergyCenters[i],output[i])
## Save to disk
    inFile = ROOT.TFile.Open(args.OutputFileName ,"recreate")
## Draw if needed
    if(args.Plot):
        c1=ROOT.TCanvas()
        EnergyHist.SetMarkerStyle(3)
        EnergyHist.Draw("hist P")
        ROOT.gPad.SetLogx()
        c1.Print("EnergySpec.png")
## Clean up
    EnergyHist.Write()
    inFile.Close()