############################################################
## This file is generated automatically by Vivado HLS.
## Please DO NOT edit it.
## Copyright (C) 1986-2016 Xilinx, Inc. All Rights Reserved.
############################################################
open_project PipeNet
set_top net
add_files PipeNet/src/include/lib/conv.h
add_files PipeNet/src/include/helper.h
add_files PipeNet/src/cpp/net.cpp
add_files PipeNet/src/include/pipenet.h
add_files -tb PipeNet/src/cpp/tb.cpp
open_solution "solution1"
set_part {xc7z020clg484-1} -tool vivado
create_clock -period 10 -name default
#source "./PipeNet/solution1/directives.tcl"
csim_design
csynth_design
cosim_design
export_design -format ip_catalog
