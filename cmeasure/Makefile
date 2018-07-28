# ----------------------------------------------------------------------
# Visual C++ / GNU MAKE MakeFile for CMeasure module
# ----------------------------------------------------------------------
#
# Instructions on how to create the executable:
#
# Linux (openSUSE, Red Hat, CentOS): 
#       make linux
#
# MS Windows 10:
#       nmake windows
#

#
# Win32 build commands (must use VC++ 2.0 [32-bit] or later)
#


#
# Linux specific location for include headers and libs
#
NIINCLUDE = /usr/local/natinst/nidaqmxbase/include
NILIBPATH = /usr/local/natinst/nidaqmxbase/lib64
NILIBNAME = nidaqmxbase

#
# Windows specific locations for include headers and daqmxbase lib
#
WIN_NIDAQMXBASE_INCLUDE = C:\Program Files (x86)\National Instruments 20\NI-DAQmx Base\Include
WIN_NILIB = C:\Program Files (x86)\National Instruments 20\NI-DAQmx Base\Lib\nidaqmxbase.lib
#
# Setup flags for compiling and linking
#
WIN_CL_FLAGS=/EHsc /c /DWIN32 /I"$(WIN_NIDAQMXBASE_INCLUDE)"
WIN_LINK_FLAGS=/MACHINE:I386

all:
	echo "You must specify linux or windows build targets (make linux or nmake windows)"


linux:
	mkdir -p bin obj
	g++ -std=c++11 -I$(NIINCLUDE) -c measurements.cpp -o obj/measurements.o
	g++ -std=c++11  obj/measurements.o  -L$(NILIBPATH) -l$(NILIBNAME) -o bin/measurements.exe


windows:
	mkdir -p bin obj
	cl $(WIN_CL_FLAGS) src\stdafx.cpp /Foobj\stdafx.obj
	cl $(WIN_CL_FLAGS) src\measurements.cpp /Foobj\measurements.obj
	cl $(WIN_CL_FLAGS) src\cmeasure.cpp /Foobj\cmeasure.obj
	link /OUT:bin\cmeasure.exe obj\cmeasure.obj obj\measurements.obj obj\stdafx.obj "$(WIN_NILIB)"


clean:
	rm bin/* obj/*
	del bin\* obj\*



