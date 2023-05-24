import argparse
import os,sys,math
import array
import ROOT

if __name__=="__main__":
    parser = argparse.ArgumentParser(prog=sys.argv[0], description='Concatenate effective area results')
    parser.add_argument("start_folder", help="Folder where effective area calculations are stored")
    parser.add_argument("base_name",help="base_name of root files in folder. Ex. Ext_0.root as a base_name of Ext_")
    parser.add_argument("Output", help="Outputs EffArea data in specified format. Available formats are either .txt or .root.")
    parser.add_argument("-n","--TotalEvents",type=int,default=10000,help="Number of events assigned to each batch file")
    parser.add_argument("-b", "--TotalBatches", type=int,default=10000,help="Number of batches")
    parser.add_argument("--tree_name", default='Cones',help="Name of TTree in EffArea root files")
    parser.add_argument("--sphere_rad", default=300,type=float,help="Radius of generating sphere")
    parser.add_argument("--seperator", default='\t',help="Seperator of .txt file output")
    parser.add_argument("-v","--verbose",action="store_true")
    args = parser.parse_args()
    mapping = []
    if not (args.Output.endswith(".txt") or args.Output.endswith(".root")):
        print("Invalid output format")
        sys.exit()
    for batch in range(args.TotalBatches):
        if(args.start_folder.endswith('/')):
            args.start_folder = args.start_folder[:-1]
        path = os.path.join(args.start_folder,args.base_name+str(batch)+".root")
        try:
            file = ROOT.TFile(path,"READ")
            tree = file.Get(args.tree_name)
        except:
            print("Problem with opening ROOT file:", path)
            sys.exit()
        counts = tree.GetEntries()
        if(counts==0):
            print("Root file empty")
            sys.exit()
        for entry in tree:
            TruthEnergy = entry.TruthEnergy
            break
        EffA = 4*math.pi*args.sphere_rad*args.sphere_rad*(counts/(args.TotalEvents))
        mapping.append( (TruthEnergy,EffA) )
    if(args.Output.endswith(".txt")):
        with open(args.Output,'w') as f:
            f.write("Energy\tEffArea\n")
            for item in mapping:
                line = str(item[0])+str(args.seperator)+str(item[1])+'\n'
                if args.verbose:
                    print(line)
                f.write(line)
    if(args.Output.endswith(".root")):
        root_file= ROOT.TFile(args.Output, "RECREATE")
        binnings=[]
        for item in mapping:
            binnings.append(item[0])
        binnings.append(binnings[-1])
        EffAreaHist = ROOT.TH1D("EffArea", "Effective Area", len(mapping),  array.array('d',binnings))
        for bin in range(0,EffAreaHist.GetNbinsX()):
            EffAreaHist.SetBinContent(bin,mapping[bin][1])
            if args.verbose:
                print(binnings[bin],EffAreaHist.GetBinContent(bin))
        root_file.Write()
