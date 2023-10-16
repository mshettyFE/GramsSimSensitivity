# Program to generate Mask based on source reconstruction
# Also defines binnings of sky maps

temp = []

import shlex
import numpy as np
import ROOT
import sys
import os
import random
import argparse
import tomllib
import subprocess

def write_file(OutputName,mask):
    # Write to file
    OutFile = ROOT.TFile.Open(OutputName, "RECREATE")
    OutFile.WriteObject(mask, "Mask")
    OutFile.Close()

def lorentzian(x,params):
## ARM follows a Lorentzian distribution (technically a Voigt function, but fitting to that gave we poorer results)
# We pass this function to ROOT for fitting
## A is normalization, \gamma is the FWHM (ie. ARM) and x0 is the center of the distribution (ie. 0)
    A = params[0]
    xo = params[1]
    gamma = params[2]
# https://mathworld.wolfram.com/LorentzianFunction.html
    return (A*(gamma/2.0))/((gamma/2.0)**2+(x[0]-xo)**2)

def SphereToCart(Azi,Alt):
## Takes in Azi and Alt as radians and calculates x,y,z direction
    x_truth = np.cos(Azi)*np.cos(Alt)
    y_truth = np.sin(Azi)*np.cos(Alt)
    z_truth = np.sin(Alt)
    return np.array([x_truth,y_truth,z_truth])

def create_mask(HistogramBlank,truth_RA_loc,truth_ALT_loc,ARMAngle):
    ## Calculates what area of the sky we restrict outselves to based on true RA and ALT locations and ARM value
    ## Essentially calculates the angle between the center of all the bins and the true source location and sees if this angle is less than the ARM
    ## If the above holds, we set the count to 1 (ie. include this bin when considering if a cone should be counted)

    ## Set up RA and ALT axes, and get x,y,z location of source
    RA_Axis = HistogramBlank.GetXaxis()
    ALT_Axis = HistogramBlank.GetYaxis()
    TruthLoc = SphereToCart(truth_RA_loc,truth_ALT_loc)
    ## Run through all possible RA and ALT values
    for i in range(HistogramBlank.GetNbinsX()):
        for j in range(HistogramBlank.GetNbinsY()):
            ## Get the center RA and ALT in each bin
            RA = RA_Axis.GetBinCenter(i)
            ALT = ALT_Axis.GetBinCenter(j)
            ## Convert to cartesian coordinates
            CandidateLoc = SphereToCart(RA,ALT)
            ## Calculate angle between truth and candidate
            Angle = np.arccos(np.dot(CandidateLoc,TruthLoc))
            ## Get current global bin from RA and ALT values
            current_bin = HistogramBlank.FindBin(RA,ALT)
            ## If below ARM, set flag to 1 and if above, set to 0
            if (Angle<=ARMAngle):
                HistogramBlank.SetBinContent(current_bin,1)
            else:
                HistogramBlank.SetBinContent(current_bin,0)
    ## Return TH2D histogram
    return HistogramBlank

if __name__=="__main__":
    ## Parse command line
    parser = argparse.ArgumentParser(
                        prog='GenMask',
                        description='Generates a mask to define a neighborhood on the sky')
    parser.add_argument("config", help="Path to .toml file")
    arguments = parser.parse_args()
    with open(arguments.config, "rb") as f:
        data = tomllib.load(f)
# draw
    if(data["Mask"]["draw"] == True):
        draw=True
    else:
        draw = False
# Read in relavent parameters
    RA_binning = data["General"]["RABinnings"]
    ALT_binning = data["General"]["ALTBinnings"]
    geo = data["General"]["Geometry"].lower()
    if(geo != "flat" and geo != "cube"):
        sys.exit(-1)
    SourceEnergy = data["Source"]["SourceEnergy"]
# Convert to degrees
    RA_loc = data["Source"]["RASourceLoc"]*np.pi/180
    ALT_loc = data["Source"]["ALTSourceLoc"]*np.pi/180
    n_events = data["Mask"]["nevents"]
    nbins = data["Mask"]["nbins"]
    OutputName = data["Mask"]["MaskOutput"]+".root"
    EffAreaFile = data["Mask"]["EffArea"]+".root"
    Cart_loc = SphereToCart(RA_loc,ALT_loc)

# Generate Source events

## gramssky
    os.chdir("GramsWork")
    args = ["./gramssky"]
    args +=  ["--options","SensitivityOptions.xml"]
    args +=  ["--RadiusSphere", "300"]
    args +=  ["--RadiusDisc", "1"]
    if(geo == "cube"):
        args += [" --OriginSphere", "\"(0,0,-40.0 )\" "]
    elif(geo == "flat"):
        args += [" --OriginSphere", "\"(0,0,-10.0 )\" "]
    args +=  ["--PositionGeneration", "Point"]
    args += [" --PointSource", "\"(" + str(Cart_loc[0]) + ","+ str(Cart_loc[1]) + ","+ str(Cart_loc[2])+")\" "]
    args += [" --PhiMinMax", "\"(" + str(-np.pi) + " , "+ str(np.pi)+ ")\" " ]
    args += [" --ThetaMinMax", "\"(" + str(-np.pi/2.0) + " , "+ str(np.pi/2.0)+ ")\" " ]
    args += ["-n", str(n_events)]
    args += [" -s ", str(random.randint(1,100))]
    args +=   ["--EnergyGeneration","Fixed"]
    args += ["  --FixedEnergy " ,str(SourceEnergy)]
    command = ' '.join(args)
    args = shlex.split(command)
    temp.append(args)
    proc = subprocess.run(args)
# Debugging of subprocess output
    with open("gramssky_mac.mac",'w') as f:
        f.write("/run/initialize\n")
        f.write("/run/beamOn "+str(n_events)+'\n')
## gramsg4
    args = ["./gramsg4", "--options","SensitivityOptions.xml"]
    args += [ "-i", "gramssky.hepmc3", "-m", "gramssky_mac.mac"]
    args += [" -s ", str(random.randint(1,100))]
    if(geo=="flat"):
        args += ["-g","ThinFlatGrams.gdml"]
    if(geo=="cube"):
        args += ["-g","ThinGrams.gdml"]
    command = ' '.join(args)
    args = shlex.split(command)
    temp.append(args)
    proc = subprocess.run(args)
## gramsdetsim
    args = ["./gramsdetsim" ,"--options","SensitivityOptions.xml" ]
    args += ["-s", str(random.randint(1,100))]
    command = ' '.join(args)
    args = shlex.split(command)
    temp.append(args)
    proc = subprocess.run(args)
## Extract
    args = ["./Extract","--options","SensitivityOptions.xml"]
    args += [ "--GramsG4Name", "gramsg4.root","--GramsDetSimName", "gramsdetsim.root", "-o", "Extracted.root"]
    command = ' '.join(args)
    args = shlex.split(command)
    temp.append(args)
    proc = subprocess.run(args)
## Reconstruct
    args = ["./Reconstruct","--options","SensitivityOptions.xml"]
    args += ["-i", "Extracted.root", "-o", "Reco.root", "--SourceType", "Point", " --SourceLoc"]
    args += ["\"("+str(RA_loc) +","+str(ALT_loc)+")\""]    
    command = ' '.join(args)
    args = shlex.split(command)
    temp.append(args)
    proc = subprocess.run(args)

##############
    ## Set batch mode on so that canvas doesn't keep opening
    ROOT.gROOT.SetBatch(1)
    ## Create blank sky map
    blankHist = ROOT.TH2D("Mask","Binary Mask",RA_binning,-np.pi,np.pi,ALT_binning,-np.pi/2.0, np.pi/2.0)
    blankHist.Reset()
    ## Attach all files to TChain
    Cones = ROOT.TChain("Cones")
    Cones.Add("Reco.root")
    ## Set up drawing canvas if needed
    if(draw):
        c1 = ROOT.TCanvas()
        pic_name = "ARM_Dist.jpg"
        ROOT.gStyle.SetOptFit(111)
    ## We don't go out past 0.5 radians on either side since there are very few events past this ARM value
    Cones.Draw("ARM>>hist("+str(nbins)+",-"+str(-1)+","+str(1)+")")
    ARM_Dist = ROOT.gDirectory.Get("hist")
    ## Define Lorentzian to fit to ARM.
    func = ROOT.TF1("Lorentzian",lorentzian,-np.pi,np.pi,3)
    ## Set up fitting code by setting default parameters
    func.SetParameters(ARM_Dist.GetBinContent(ARM_Dist.GetMaximumBin()),ARM_Dist.GetMean(),ARM_Dist.GetRMS())
    func.SetParNames ("Constant","Mean_value","Gamma")
    ## Fit function and extract ARM
    ARM_Dist.Fit("Lorentzian")
    ARM = abs(func.GetParameter(2))
    print("ARM: ",ARM)
    ## Draw ARM distribution
    print(os.getcwd())
    if(draw):
        ARM_Dist.Draw()
        c1.SaveAs(pic_name)
    ## Calculate mask and draw if desired
    mask = create_mask(blankHist,RA_loc,ALT_loc,ARM)

# Temporary hack to display all the sky
    # Draw mask if needed
    if(draw):
        c2 = ROOT.TCanvas()
        pic_name = "Mask.jpg"
        mask.SetStats(0)
        mask.Draw("colz")
        c2.SaveAs(pic_name)
    write_file(OutputName,mask)