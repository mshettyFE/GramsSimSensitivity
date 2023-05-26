import numpy as np
import ROOT
import os,sys
import argparse
import glob
from matplotlib import pyplot as plt

def lorentzian(x,params):
## ARM follows Lorentzian distribution, hence we pass this function to ROOT for fitting
## A is normalization, \gamma is the FWHM (ie. ARM) and x0 is the center of the distribution (ie. 0)
    A = params[0]
    xo = params[1]
    gamma = params[2]
    return (A*(gamma/2.0)**2)/((gamma/2.0)**2+(x[0]-xo)**2)

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
                        prog='ProgramName',
                        description='What the program does')
    parser.add_argument("SourcePath", help="Location where all the reconstruction files of the source are at")
    parser.add_argument("ReconstructionBase", help="The base name of the source files (ie. drop the ${num}.root part)")
    parser.add_argument("HistogramPath", help="A sample histogram we want to create a mask for (for instance, a background histogram)")
    parser.add_argument("RA_loc", type=float, help="Right ascension Location of source")
    parser.add_argument("ALT_loc", type=float, help="Altitude location of source")
    parser.add_argument("-d","--draw",action="store_true", help="Wheather you want to save the ARM distribution and final map to jpgs")
    parser.add_argument("--nbins",type=int, default=50000, help="Number of binnings to create in ARM distribution")
    args = parser.parse_args()
    ## Set batch mode on so that canvas doesn't keep opening
    ROOT.gROOT.SetBatch(1)
    ## Read in the example histogram and create a blank copy that preserves the binnings
    TemplateHistogramFile = ROOT.TFile(args.HistogramPath,"READ")
    blankHist = TemplateHistogramFile.Get("SkyMap").Clone()
    blankHist.Reset()
    ## The regex? you pass to glob to identify the reconstruction files
    ## https://docs.python.org/3/library/glob.html
    query = os.path.join(args.SourcePath,args.ReconstructionBase)+"*root"
    ## Get files and sanity check
    files = glob.glob(query)
    if len(files)==0:
        print("Couldn't find files. Check that the path and basename are correct")
        sys.exit()
    ## Attach all files to TChain
    Cones = ROOT.TChain("Cones")
    for file in files:
        Cones.Add(file)
    ## Set up drawing canvas if needed
    if(args.draw):
        c1 = ROOT.TCanvas()
        pic_name = "ARM_Dist.jpg"
        ROOT.gStyle.SetOptFit(111)
    ## Create histogram of ARM
    Cones.Draw("ARM>>hist("+str(args.nbins)+",-5,5)")
    ARM_Dist = ROOT.gDirectory.Get("hist")
    ## Define Lorentzian to fit to ARM.
    func = ROOT.TF1("Lorentzian",lorentzian,-90,90,3)
    ## Set up fitting code by setting default parameters
    func.SetParameters(ARM_Dist.GetBinContent(ARM_Dist.GetMaximumBin()),ARM_Dist.GetMean(),ARM_Dist.GetRMS())
    func.SetParNames ("Constant","Mean_value","Gamma")
    ## Fit function and extract ARM
    ARM_Dist.Fit("Lorentzian")
    ARM = abs(func.GetParameter(2))
    ## Draw ARM distribution
    if(args.draw):
        ARM_Dist.Draw()
        c1.SaveAs(pic_name)
    ## Calculate mask and draw if desired
    mask = create_mask(blankHist,0,0,ARM)
    if(args.draw):
        c2 = ROOT.TCanvas()
        pic_name = "Mask.jpg"
        mask.SetStats(0)
        mask.Draw("colz")
        c2.SaveAs(pic_name)
