# Barebones plotting of .txt files from CalcEffArea.py

import matplotlib.pyplot as plt
import numpy as np
import argparse


if __name__=="__main__":
    parser =  argparse.ArgumentParser(prog="EffAreaPlt.py", description='Plots effective area from txt file')
    parser.add_argument('filename', default="EffAreaData.txt", help="Name of file")
    parser.add_argument('title', default="No Seperation", help="title of plot")
    args= parser.parse_args()

# load in data to np array, skipping header line
    data=np.loadtxt(args.filename,dtype=float,skiprows=1)
# Standard matplotlib stuff
    plt.scatter(data[:,0],data[:,1])
    plt.xlabel("Energy (MeV)")
    plt.ylabel("Effective Area (cm^2)")
    plt.title(args.title)
    plt.yscale("log")
    plt.xscale("log")
    plt.show()
