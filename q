commit 423064e246b77eb8ea14b51671723c66d2050b0b
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat Nov 11 23:38:20 2023 -0500

    Got ReadRootFile compiling. Everything on top of ReadRoot is broken though... Need to work at that.

commit b9b99f6f818b47a0a46bdd9cf7ba4011ab9327ad
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat Nov 11 03:37:13 2023 -0500

    Added print functionality to all Entry classes instead of using a generic on in base class

commit aa5d0360622a5cf139136f23034f1a5df8f4a60e
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat Nov 11 02:16:05 2023 -0500

    Fixed types in some class methods

commit 0dd49a1a3f8d744475140d12628485b0a1eb9355
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat Nov 11 02:07:48 2023 -0500

    Finished RecoEntry class. Fixed ExtractEntry Class

commit fc53d1fe8c22ce890c018f8468a051c75e4aff3f
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat Nov 11 01:35:10 2023 -0500

    Now compile with all warnings on. Finished tentative verion of ExtractEntry

commit 24e01031da08294cfd876777176d04492aff0c72
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat Nov 11 01:02:16 2023 -0500

    Got DetSimEntry class tentatively done

commit 1238ae27b93fe05352ae16558a7b8f065c0d26b6
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat Nov 11 00:46:04 2023 -0500

    Finished GramsG4 class tentatively

commit edd0b0654eb1be3cd7fdff852c39336e0d78a6bb
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat Nov 11 00:30:21 2023 -0500

    Started writing family of Entry classes

commit 125020fe029db171b49ea2ccbfc6d96e38aa304c
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Nov 10 18:01:52 2023 -0500

    Added reminder

commit d5d58a363de157f9f02f88480be5cb7cd49612c9
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Nov 10 18:00:41 2023 -0500

    Started refactoring Entry to enable Switching between GramsSim and GramsDetSim

commit eeeb66c441179e4e1a731ac0532123a55c5e5a0a
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Nov 10 13:55:04 2023 -0500

    Accidentally deleted GenSkyMap. Restored those files

commit eb77bbbd229d46f03e90c4554c794de9e549ea86
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Nov 10 13:23:43 2023 -0500

    Added linear and log option for binnings in GenCondorJobs

commit c6bb1a6ab907add32a84c0a2f0358e86fc02e301
Merge: 558f9c0 54fe595
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Nov 10 13:03:50 2023 -0500

    Merge branch 'IncorporateDetSimOutput' of https://github.com/mshettyFE/GramsSimSensitivity into IncorporateDetSimOutput

commit 558f9c0a3f202769cdfe254fbaee3910aaf6aa02
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Nov 10 13:00:23 2023 -0500

    Nevermind

commit 54fe59597350234c2846226f1467ae712937d7ff
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Nov 10 11:08:37 2023 -0500

    Deleted old sensitivity aggregator code

commit ca561a2f82017ea9a885071357ae6dfb7f733716
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Nov 10 10:52:49 2023 -0500

    Added parameter check in GenCondorJobs.py

commit d80c94358e78e5ace2a10c241c7c22e28d4e0ab6
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Nov 10 10:51:40 2023 -0500

    ConfigFiles/Configs.toml

commit d0ec7aec67c83c8220433436af7ed0e7fcae3f03
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Wed Nov 8 12:31:12 2023 -0500

    Update README.md
    
    Forgot to add "make" command in setup

commit 7c4e0e0c8e963d005705d6243038fa6d0c442e77
Merge: 9f642e4 d966754
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Wed Nov 8 11:54:50 2023 -0500

    Merge pull request #15 from mshettyFE/Refactor

commit d9667548672a0a07893c86745edeaba5ca9875b5
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Nov 3 16:30:37 2023 -0400

    Fixed error in main README.md

commit c0ac5e6d42404b26088a30c7b4370cda4085e6e5
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Nov 3 16:29:03 2023 -0400

    Fixed bugs in GenMasks, GenCondorJobs and CalcEffArea

commit f34d2c42f2b41775f2f8ce565435cdd83056916c
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Nov 3 15:15:32 2023 -0400

    Modified GenMask.py and CalcEffArea.py to copy their output to relavent directories

commit 47a833b59c9a99b6863096d0718df1d809d231e9
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Nov 3 13:51:42 2023 -0400

    Added main README.md

commit 9f6095e8ae846c1c26a026f97317fa4f2ae40a0c
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Nov 3 13:37:59 2023 -0400

    Wrote main page README.md

commit e9be4ae7811056e521e79d4fcfb72d6a6fc2ae6d
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Nov 3 10:59:48 2023 -0400

    Changed config.sh to reflect refactoring

commit 0891adfeb80ee95a86da795865f26850fa472270
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Nov 3 10:58:52 2023 -0400

    Changed logspace to linspace in EffArea Calculation

commit 15a898937dd977ef97eb1433deaa84948a807fde
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed Nov 1 16:44:41 2023 -0400

    Purged old documentation, removed outdated CalcSenstivity script, and rearranged files

commit 1fc81babab063cddb453520d42d4c9b895f9cc62
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed Nov 1 16:43:12 2023 -0400

    Purged old documentation, removed outdated CalcSenstivity script, and rearranged files

commit 4779cfd13ed21ff1fd650c65153756080d2cdcbc
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Thu Oct 26 12:14:20 2023 -0400

    Converted CalcSensitivity to a python script that uses .toml config files. Also added overwrite warning to GenCondorJobs.py

commit 3d60e7c9e11e9021b0cd3acc7d3283e200fe63b7
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed Oct 25 17:20:39 2023 -0400

    Changed Config files and updated associated scripts

commit 34663bc46819ff4a80826d8ea91d14908f67f4f1
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Mon Oct 16 19:21:22 2023 -0400

    Cleaned up extraneous files and edited CMakeLists.txt

commit 9d7da4a20887c897971ab6731f0f1f439b053e34
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Mon Oct 16 19:07:42 2023 -0400

    Finished refactoring structure of code. Need to rewrite CalcSensitivity into python script for .toml compatilibity

commit c61419f4e41c03d1f3d64688e0234f1fb294ae25
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Mon Oct 16 16:19:54 2023 -0400

    Have GenMask.py clean up after itself

commit 4de0d064b7e9a5a82922121e0232bc11d8273785
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Mon Oct 16 16:04:06 2023 -0400

    Fixed EffectiveArea Calculation (extract factor of 4)

commit edfaa23d198fa02918d26e2183f3115584c55dce
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Tue Oct 10 17:20:58 2023 -0400

    Tracked down bug. Wasn't properly tokenizing arguments for subprocess. Need to apply to rest of scripts

commit 124dfad1fe7a40606d7c96d971a526536ad8c20b
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed Oct 4 14:51:19 2023 -0400

    Finished rough draft of refactor. need to test everything out to weed out bugs

commit f435e2d20c98e5dcb3088c1e0d925cec53cf80e6
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Tue Oct 3 15:56:37 2023 -0400

    Started debugging of GenCondorJobs.py

commit c6412efe41a6e47ae438a47bb4af57a7f35c973d
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Mon Oct 2 19:33:54 2023 -0400

    Added Source and Background cone generation to GenCondorJobs.py. Need to test and deal with bugs

commit 1509478c9ae27b63c01011c2f970e50226d71168
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Mon Oct 2 02:44:49 2023 -0400

    Accidentally ran cmake in source directory. Fixed that

commit 0020946631f687ee3d261a0a7f1d7b40e48e7121
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Mon Oct 2 02:29:03 2023 -0400

    Continuinig to refector

commit e7e115894eba63ec5b725a957717ca2d525d0779
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed Sep 27 15:52:09 2023 -0400

    Changed GramsSim to master branch

commit a83a700b32dea5fa29bd7f77394c0ce04c9a3159
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Thu Sep 21 01:15:54 2023 -0400

    Fixed CMake files and added Config.toml. Need to refactor rest of scripts around .toml file

commit afd42d4b86f4e9fdedc16c5c15b059d5722d9239
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed Sep 20 09:35:34 2023 -0400

    Added conda dependencies. Removed git submodule to start anew

commit 7bedcff59e3839aa51728041fbd105dce6f46da9
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Sep 15 19:38:22 2023 -0400

    Edited config.sh to copy over SensitivityOptions. Added TODO list

commit 1cb687ca25b89ebb33360e7512574d6a3793582c
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Sep 15 17:45:06 2023 -0400

    Added SensitivityOptions.xml, which hold the arguments for all the cpp executables

commit 6c32b85ed4613bc98dee7efa3257624a6ca984a4
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Sep 15 17:44:34 2023 -0400

    Decoupled Source and Background batches from each other

commit d14985205e01cb9fa31072e8deef3c7b6c6ef066
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Thu Sep 14 12:17:37 2023 -0400

    Renamed some files

commit ccf890013edac0eb8c3cf17df133b501a535e01b
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Thu Sep 14 11:46:26 2023 -0400

    Added GramsSim submodule and config.sh to build folder for condor jobs

commit a96c1b8403ec426e00745beac3d371016d2bc527
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Thu Sep 14 11:17:44 2023 -0400

    First

commit 9f642e4d5bed8082cfc6b0f68b81c89f9ecadc4d
Merge: 9491c56 4355397
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Mon Jun 5 07:24:38 2023 -0400

    Merge pull request #14 from mshettyFE/develop
    
    Proofread documentation. Hopefully no errors

commit 435539794b5638edcfe0fa2a913d217a5243b641
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Mon Jun 5 07:23:48 2023 -0400

    Proofread documentation. Hopefully no errors

commit 9491c5628c39a84b407b89f96f0209d066b6e4cf
Merge: 05e7cbb a312d85
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Mon Jun 5 06:30:06 2023 -0400

    Merge pull request #13 from mshettyFE/develop
    
    Added last bit of documentation and removed extraneous options from C…

commit a312d8591ff7ea2537b7e950467d7240862f1be4
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sun Jun 4 22:48:57 2023 -0400

    Added last bit of documentation and removed extraneous options from CalcSensi

commit 05e7cbb6611b644a4c763644592ffab2156a9479
Merge: a43adf0 646b860
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Sat Jun 3 17:00:05 2023 -0400

    Merge pull request #12 from mshettyFE/develop
    
    Develop

commit 646b8603f8fd0c92fc6393acbad8a036c39ad3b7
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat Jun 3 15:52:18 2023 -0400

    More Documentation

commit eaa6a177877827ffa928f8195060b54d68766329
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat Jun 3 14:00:04 2023 -0400

    Added more documentation

commit 436e562a50749475dfcbb12de237474c7c53491c
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat Jun 3 12:12:46 2023 -0400

    Added RemoveClutter.sh and worked on documentation

commit ba9d0a3a87eeaccdc70125834b6f4c761bfa346a
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Jun 2 21:02:18 2023 -0400

    Tested that I didn't break anything.

commit ba12ab7a9153f7822972406b34eb7da9c0f08cd7
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Jun 2 19:57:11 2023 -0400

    Added typedefs, file existence checking, null histogram checking, and comments throughout repo

commit b86c14cefe1854d41892c176bb9bd547c65fa3e2
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Jun 2 10:45:46 2023 -0400

    Finalized code changes for now. Need to add in more comments and flesh out README.md files

commit 739f550f159c613d7002ae686c97bc5bae29cc09
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Jun 2 10:11:12 2023 -0400

    Fixed CalculateSensitivity.cpp fully (I think...)

commit 90cce429c56d29ea9a5c7b7162d9bd6c979f01e1
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Jun 2 09:25:04 2023 -0400

    Partially fixed CalculateSensitivity.cpp. Need to fix reading in Source Sky Maps

commit b531fdad769132c268cbbcae1842c96cc66d8b3d
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri Jun 2 00:28:21 2023 -0400

    Tweaked GenCondorHistsScripts.py to actually work

commit a43adf038270bfccd9e7cb03672b839d9158906b
Merge: 3437cda 055719f
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Wed May 31 19:30:36 2023 -0400

    Merge pull request #11 from mshettyFE/develop
    
    Develop

commit 055719fc4f47206fce429a2db23e74679b5e69cd
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed May 31 19:29:56 2023 -0400

    Changed module load statement in top-level README.md

commit bd7a9ba4902383518bf3f1fc037a76ea17e9a32c
Merge: 6b2db7d 6f484a9
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed May 31 18:47:58 2023 -0400

    Merge branch 'develop' of github.com:mshettyFE/GramsSimSensitivity into develop

commit 3437cda2aca13c8cb1e688411e9caf832ac5dafb
Merge: 7fca077 6f484a9
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Wed May 31 18:45:49 2023 -0400

    Merge pull request #10 from mshettyFE/develop
    
    Develop

commit 6b2db7d3bff733dfc5886c1468b1b3be10a7869b
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed May 31 18:44:21 2023 -0400

    Modified GenCondorCountsHistsScripts.py and associated files. Need to test

commit 6f484a9bff76b720c6cfc399c8b66213f936700f
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed May 31 15:32:04 2023 -0400

    Test

commit d4a8dae38d054996d01cd5807c49de17bd69f0fd
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed May 31 11:56:15 2023 -0400

    Restructured CMake files.

commit 640263e0f4b74da24d4e3605316ecca1e76b049c
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed May 31 10:49:56 2023 -0400

    Finished CalculateSensitivity.py. It runs really slow. Need to use condor on source cones

commit 7fca0777f169333b5ba10e660255819de7fc3c15
Merge: 62006fe acdc05f
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Wed May 31 02:11:54 2023 -0400

    Merge pull request #9 from mshettyFE/develop
    
    Develop

commit acdc05f2e0f1a282d0fdd2885c0d8276b6876df0
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed May 31 02:10:15 2023 -0400

    Modified CalculateSensitivity.cpp. It builds, but I need to test it

commit fb984d34bc5db224f0ddc4b7941725210d60c58f
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed May 31 01:14:40 2023 -0400

    Verified that GenCondorScriptsHist.py now works

commit 62006fedd6edeaec7cd61855b05ea58878283a70
Merge: 46ef350 0db4b0a
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Wed May 31 01:03:18 2023 -0400

    Merge pull request #8 from mshettyFE/develop
    
    Develop

commit 0db4b0ad419242e87af4cf9f7f73bab0f945b791
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed May 31 01:01:33 2023 -0400

    GenBackCounts.cpp works. Modified GenCondorScriptsHist.py. Need to check that it works.

commit c1c7351d8d3d24024dc9f8b105dd7ec94ae0001a
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed May 31 00:28:42 2023 -0400

    Modified inputs of GenBackCounts.cpp

commit 46ef350e2553c6d40ea20c9a4624996a005d8c24
Merge: 2d9c92a 7e8500a
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Wed May 31 00:14:54 2023 -0400

    Merge pull request #7 from mshettyFE/develop
    
    Change CMake structure

commit 7e8500a048309e43f80c647cd588fdfe22632196
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Tue May 30 23:57:20 2023 -0400

    Change CMake structure

commit 2d9c92a79a4fd33f076846cca244fec011586f7b
Merge: a361dea aaa24f3
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Tue May 30 15:30:56 2023 -0400

    Merge pull request #6 from mshettyFE/develop
    
    Develop

commit aaa24f36e46d7f8b8552f3be2415d75c6b29d8c8
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Tue May 30 15:29:27 2023 -0400

    Added CalcSensitivity.py. Need to change GenSkyMap to GenBackCounts and only apply to Background

commit cee94c8c6bbe609e354f7a5d5451897a27b460f4
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Tue May 30 01:08:00 2023 -0400

    Added Masking of Background

commit e148bb8821bc035a330ebe1db7156744b6d8641c
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Mon May 29 19:57:25 2023 -0400

    Added CalculateSensitivity.cpp and associated libraries. work in progress

commit 7ef3294b87f2e651b79c5e7b9b770fe26b807bfc
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sun May 28 12:11:07 2023 -0400

    Created GenCondorScriptsHist.py for skymap condor script generation

commit ceb0433d93189db7e0a41fe7b22b37c1e13cbc5d
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sun May 28 11:25:13 2023 -0400

    Test

commit 7411fa00eb7f3c8595f888ec454663a2393592bd
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat May 27 12:24:21 2023 -0400

    Fixed GenMask.py and CmakeList

commit 3b2bb32785f11e8ade01016c222a0ee1a7060f88
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri May 26 20:50:32 2023 -0400

    Changed libaries to be static since I had issues with .so files not being found on some condor batch jobs for some reason

commit 1f9d72aa9c06b6284826fc4e302c5cb075df3afd
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri May 26 17:00:01 2023 -0400

    Fixed ARM bug in reconstruction. Added source mask generation script

commit 0f234b4cfd9b890c0ba2323f0d40102747908c51
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri May 26 12:49:08 2023 -0400

    Added Condor Histogram script generator. work in progress

commit c75647df0ca2908c361e4f5dd183a4fe42b2d846
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri May 26 11:26:40 2023 -0400

    Reworked CMake files

commit a361dea263f0890e28277481d110741d4adeeb79
Merge: 4a0a4c6 c9bd1b1
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Thu May 25 18:14:12 2023 -0400

    Merge pull request #5 from mshettyFE/develop
    
    Develop

commit c9bd1b1d3a83957245d77ce04a009860d4d8a5be
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Thu May 25 18:12:45 2023 -0400

    Fixed Reweighting and Source Command files

commit c0d540f85ba6246eef9b960b741b5eac0c3371e0
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Thu May 25 16:28:44 2023 -0400

    Added Reweighting Scheme to GenSkyMap. Still need to debug

commit 6782f68833188a59461a63d95fdcb2511b615624
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed May 24 18:40:25 2023 -0400

    Gutted CalcEffArea.cpp and turn it into a .py file instead

commit 3b2f91f677b8de94ada0a1fd4a779c4ba5fa675a
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed May 24 15:22:03 2023 -0400

    Added command files for source reconstruction

commit a66a5ca0a60bf71922e76102bc8520cf52721efe
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed May 24 15:18:06 2023 -0400

    Added Command files for SourceRecon

commit 7339c39159bd2a9395631ea1b20230206018a86d
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed May 24 14:41:29 2023 -0400

    Fixed GenCondorScripts.py to read in mac address correctly. Edited Commands.txt for Background and Source

commit 4a0a4c6d6f3af42cea5348286c37bc0b6e01fa8c
Merge: 3d5a139 7296e17
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Wed May 24 13:29:40 2023 -0400

    Merge pull request #4 from mshettyFE/develop
    
    Develop

commit 7296e173378012403267ef61c4a4a29b02ce693d
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed May 24 13:29:05 2023 -0400

    Finagled CMake files for EffArea

commit 0b06e94165fb4bc21e40e01e745a693163d584af
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed May 24 10:19:07 2023 -0400

    Changed default disk request on GenCondorScripts.py

commit 3d5a139d44809c4f08113e49517c3e8659016e59
Merge: 5bff798 a962b6c
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Tue May 23 21:27:13 2023 -0400

    Merge pull request #3 from mshettyFE/develop
    
    Edited README.md for EffArea

commit a962b6c5884936d541fd5b0e40b34ca53672e648
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Tue May 23 21:26:08 2023 -0400

    Edited README.md for EffArea

commit 5bff798422af0d4135d605d735edcfd5b16ef314
Merge: 79cfcab a10f7fb
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Tue May 23 19:09:39 2023 -0400

    Merge pull request #2 from mshettyFE/develop
    
    Develop

commit a10f7fb364d1aa83d317ecb838c4671df7eb0ae6
Merge: 15f2a15 79cfcab
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Tue May 23 19:09:07 2023 -0400

    Merge branch 'main' into develop

commit 15f2a15ffc392e9f04fbcdeba6adecdf39c31258
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Tue May 23 19:07:06 2023 -0400

    Added README.md files

commit 79cfcab4dde527ae9b2a089d849c74ef3606663a
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Tue May 23 14:24:49 2023 -0400

    Update README.md

commit 652c4276804bd9b0d4f48e1f4ec84268d955e219
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Tue May 23 14:02:19 2023 -0400

    Edited README

commit 572958a9f1ebb0811740aafa4841db10c6dd2014
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Tue May 23 13:59:45 2023 -0400

    Update README.md

commit fc69a350a21f6bbddb089577aade6ab5aadb9415
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Tue May 23 13:56:11 2023 -0400

    Update README.md

commit 025a052c3a2a7281aa749f224b7cde52236e2f71
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Tue May 23 13:54:31 2023 -0400

    Update README.md

commit af524761f8ac78363e9f13be38b73d842473f78e
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Tue May 23 13:53:55 2023 -0400

    Update README.md

commit a0355d073aaab95319e79c5503bb8d7c13ae4586
Merge: 612fd2c 173e70a
Author: mshettyFE <107198079+mshettyFE@users.noreply.github.com>
Date:   Tue May 23 13:52:44 2023 -0400

    Merge pull request #1 from mshettyFE/develop
    
    Develop

commit 173e70a928b80a34a6b49ebd431b0cdca3de4cad
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Tue May 23 13:25:59 2023 -0400

    Modified command files in EffArea

commit f7aeb460fea4e994ca7c654a129bd2e213fa3ce4
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Tue May 23 13:03:22 2023 -0400

    Added disk and memory constraints to GenCondor

commit 732f3b40563509ebf5b67410f5611721fd1065f8
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Mon May 22 20:38:55 2023 -0400

    Added Skeleton of Commands.txt for Background and Source Gen. Need to test.

commit 737780de7c3949da2af1ae998b97eb78095e1371
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Mon May 22 17:04:56 2023 -0400

    Fixed bug in GenCondorScripts.py

commit 37af374cbaa190b4bf685a8ecb3d422c1511b900
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Mon May 22 16:20:25 2023 -0400

    Modified GenCondorScripts.py to include Batch variables in shell script output

commit f82e97b17173e72ef49a32fc774501a60810ced2
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Mon May 22 16:03:27 2023 -0400

    Finished CalcArea executable as well as created ThinFlatGrams.gmdl for flat geometry

commit 378f97a8c254dba24acea3396acc6f2e74641c0e
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sun May 21 11:47:38 2023 -0400

    Added Reconstruct to EffArea pipeline. CalcArea.cpp unfinished

commit 2bff536a0b3047b63240df61d33caa36d255040f
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sun May 21 01:00:22 2023 -0400

    Temporarily added copying Command files to top level CMakeLists

commit c1a3eebb594dff10780b7885d2f065bcfb4cd25d
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sun May 21 00:48:29 2023 -0400

    Changed delete method to find

commit 3d6c26e9bb0ad2e4836d07c43db2970dad750b5a
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sun May 21 00:35:33 2023 -0400

    Changed purging batch directory to use rsync magic

commit e2c6d8f3196302ff1ec29e70777fa1d944ca1f6d
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sun May 21 00:15:27 2023 -0400

    Change purging of batch directory to instead delete batch folder and then recreate it

commit 362fd20d241a7764c8f4b0409ad84145defff809
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat May 20 23:11:12 2023 -0400

    Updated GenCondorScripts.py. Cleans out SenseJob/mac/batch prior to writing .mac files

commit 2cf9043193802b21c8366001fc3ed48b16e9d711
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat May 20 22:56:10 2023 -0400

    Actually fixed Commands.txt in EffArea

commit 3bdddc4e5a203a237df0289fe66b8eaa25d66f6d
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat May 20 22:53:49 2023 -0400

    Fixed Commands.txt in EffArea

commit c3473141bbd994f7c076f82f1070108fdbe224da
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat May 20 22:48:00 2023 -0400

    Fixed GenCondorScripts.py. Didn't update unzip command in .sh file generation

commit 4c885aa81bd6783188458322de9cc187c011d8bc
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat May 20 22:38:26 2023 -0400

    The tar.gz file of SenseJob needs to be unique for each series of batches. Otherwise, you get conflicts

commit 18203394ec6c1b032e24a4c27d4d5523cc1113b5
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat May 20 22:18:06 2023 -0400

    Fixed mac/batch not being created in SenseJob on make command

commit 0f68de7740638f3d51493b0ad036e68efe08b4da
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat May 20 22:00:04 2023 -0400

    Finished GenCondorScripts.py. Started on EffArea setup

commit d58a776269af0cf46c849eb217dc899aebb3d29f
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat May 20 19:06:24 2023 -0400

    Refactored GenHist to use Eigen more. Created GenCondorScripts.py to create outline of condor files

commit 335586cce0de623760bc14d89824f3b184dea3e5
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri May 19 17:49:02 2023 -0400

    Making all libraries link ROOT quotes instead of brackets.

commit bdf07e6725a2330cf57e0a3597575a78b2a4bae1
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri May 19 16:58:02 2023 -0400

    Refactored GenSkyMapUtils to utilize Eigen more

commit 22b9715c1a4f39880237df9f9814ef1915f2c429
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Fri May 19 16:17:16 2023 -0400

    Finished HistogramUtils. Need to refactor a bit though...

commit e7f3817d86f9520df68156f12356e5e808bf6038
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Thu May 18 20:42:05 2023 -0400

    Finished Reconstruction executable. Created skeleton of GenSKyMap for Background and Source Reconstruction

commit 3b19d749c843e5564b5b621ab01f469420cf9dff
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Thu May 18 17:52:47 2023 -0400

    Fixed CMake problem. Use POST_BUILD to copy over executables after building

commit 231fcae00c4e2e50231d26e95d9b9e754429b0a9
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Thu May 18 12:05:02 2023 -0400

    Finished Reconstruct. Need to Test. Fix CMake to put .exe in multiple locations

commit f95803c3d98e3b39e03f8c8784605de5f9d58dac
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed May 17 21:27:32 2023 -0400

    Fixed GenEnergySpectrum.py to only generate energies uniform in log-scale (consistent with sensitivity plots

commit c9af02b0ec0ba0921ccc52c28a2867dd8f705ba7
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed May 17 18:08:55 2023 -0400

    Added GenEnergySpectrum.py as convinience to generate Background Flux histogram

commit 7f9905c4d244bf23264325b7ad63d20a052227aa
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed May 17 09:13:41 2023 -0400

    Added Reading ExtractOutput Utility

commit 85f8d6cff4a0c9da8c83e1be228327ccb230ab76
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Wed May 17 08:01:19 2023 -0400

    Moves Reading ROOT files to seperate header file

commit 34021ac913129cfb0e51c18c45187f3ec179768b
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Tue May 16 18:21:28 2023 -0400

    Added Skeleton of files to be programmed

commit 18279f4465c96f556034f4d69a9b40c5f8674723
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Tue May 16 17:19:44 2023 -0400

    Reordered CMake structure. Put Extract.cpp in utils and copy over to 3 seperate locations

commit c668f4f0664ae2d78fcdb47e6f91ab6613f81f9f
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Tue May 16 11:52:20 2023 -0400

    Added Filter function to Extract program

commit 26be438c61ba502088c6c593f9df1f69b5c92f33
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Mon May 15 10:24:14 2023 -0400

    Put all options into options.xml and refactored appropriately

commit 6c29de58bb610c6c0f57fdde1a87257ab965b91c
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Mon May 15 09:50:45 2023 -0400

    Updated CMakeList.txt to place SenseJob in both EffArea and GenerateCondor on compilation

commit 7f3d7dd648a5105d59a9c67193393f97e3a1a7db
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Mon May 15 09:37:31 2023 -0400

    Finished Extract.cpp

commit a114c367ae733e5ec1b75ac740edcf74a90cf993
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sun May 14 20:08:26 2023 -0400

    Added Skeleton of Options to Extract.cpp in GenerateCondor

commit dc691c23880e873811e0d48347ebd2988fff27d8
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sun May 14 20:03:59 2023 -0400

    Added Skeleton of Options to Extract.cpp in GenerateCondor

commit 1336c6c114fb17dac0161363c8da55b89ced6815
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sun May 14 11:49:09 2023 -0400

    Figured out basic CMake usage

commit 1a1d3e2491a9fb499f5961708e4614ce139774de
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat May 13 13:51:09 2023 -0400

    Added Skeleton to develop

commit 96f55bd00e477c052da4d381bc5d5192e2155205
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat May 13 13:15:39 2023 -0400

    Testing develop branch independence

commit 612fd2cfdfbf32e731fff01b6be7feefa7de892a
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat May 13 13:08:19 2023 -0400

    Created Skeleton of framework

commit f087969a6d47633a2dfaabdcdc0f2740254a4a2d
Author: Mihir Shetty <ms6556@columbia.edu>
Date:   Sat May 13 12:42:28 2023 -0400

    Setting up Git Repo
