############################################################################################
# This project contain files for stacking image from static camera for HDR data
# with some filtering and histogram manipulations implemented in c/c++
#
#  target platfrom GNU c/c++  (gcc 4.4.7 | linux 2.6.32.x86_64 tested) 
#
#  The project may compile also with M$ Visual Studio (provided example project contains a
#  version of dirent.h that allows compiling but no libtiff, etc... no guarantee!)
#
#############################################################################################
# Please use reference:
#
#  Varjo, Sami; Hannuksela, Jari "Image Based Visibility Estimation During Day and Night",
#  in C.V. Jawahar and S.Shann(Eds.): ACCV 2014 Workshops, Part III, LNCS 9010, pp. 277-289, 2015 
#
#  Sami Varjo 2014, University of Oulu, Finland
#  sami.varjo@ee.oulu.fi
#
#############################################################################################
#
# #contents 
# bin	    the linked result from Makefile
# data      some example test data and example results
# include   c/c++ include files
# obj       object files from Makefile
# src       c/c++ project files 
# Makefile  invoke make to build the project 
#
########################################################
# The main program has several options to use with data
#       invoke> procesHDR
# to see the options / help (or give only -h parameter)
#
# depends on libtiff  (make sure you have correct paths in Makefile)
#
# implemented is:
#   linear exposuretime hdr stacking from given folder of data
#       -p select file prefix in folder def "-p img" (if folder contain more than one type file na$
#       -s select file suffix in folder def "-s .tif"
#       -e file containing exposuretimes -e expTimes.txt (build ins from 25 50 100... 52428800) 22$
#
#   save output image (optional) -o  <file name>
#       -8b for writing 8bit output instead of 32 bit (def)
#
#   -c apply contrast limited adaptive histogram equalization (CLAHE)
#
#   -r apply retinex style filtering
#
#
###############################################################################################
# The example results in data folder were obtained invoking following commands :
#
# (You can infact invoke this README.txt to reproduce the results if you have the data)

./bin/x86_64bit/processHDR data/2014-08-15_00h00m01s/ -o data/res1_clache.tif -v -c -8b
./bin/x86_64bit/processHDR data/2014-08-15_12h00m29s/ -o data/res2_clahe.tif -e data/expTimes.txt -v -c -8b
./bin/x86_64bit/processHDR data/2014-08-15_00h00m01s/ -o data/res1.tif -r -c
./bin/x86_64bit/processHDR data/2014-08-15_12h00m29s/ -o data/res2.tif -e data/expTimes.txt -c -r



