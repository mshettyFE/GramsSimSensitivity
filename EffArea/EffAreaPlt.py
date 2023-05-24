import matplotlib.pyplot as plt
import numpy as np
import argparse

parser =  argparse.ArgumentParser(prog="EffAreaPlt.py", description='Plots effective area from txt file')
parser.add_argument('filename', default="EffAreaData.txt", help="Name of file")
parser.add_argument('title', default="No Seperation", help="title of plot")
args= parser.parse_args()

data=np.loadtxt(args.filename,dtype=float,skiprows=1)
plt.scatter(data[:,0],data[:,1])
plt.xlabel("Energy (MeV)")
plt.ylabel("Effective Area (cm^2)")
#plt.title("Effective Area versus Energy")
plt.title(args.title)
#plt.title("Unique Cells")
plt.yscale("log")
plt.xscale("log")
plt.show()
