executable = /nevis/milne/files/ms6556/Spring2023/SensitivityRepo/BackgroundRecon/Source1MeV.sh
arguments = $(Process)
transfer_input_files = /nevis/milne/files/ms6556/Spring2023/SensitivityRepo/BackgroundRecon/SenseJobSource1MeV.tar.gz
initialdir = /nevis/riverside/data/ms6556/Source/1MeV/
universe       = vanilla
should_transfer_files = YES
when_to_transfer_output = ON_EXIT
requirements = ( Arch == "X86_64" )
request_memory = 300
request_disk = 3000000
output = Source1MeV_$(Process).out
error = Source1MeV_$(Process).err
log = Source1MeV_$(Process).log
notification   = Never
queue 10000