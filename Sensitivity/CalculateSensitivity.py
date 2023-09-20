import configparser
import os
import argparse

parser = argparse.ArgumentParser(
                    prog='GenCondorJobs',
                    description='Generates .sh and .cmd files for condor jobs to send to Nevis cluster')
parser.add_argument("config",help="Path to .ini config file")
args = parser.parse_args()

config = configparser.ConfigParser()
config.read(args.config)
print(config.sections())