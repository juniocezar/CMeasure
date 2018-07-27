NIINCLUDE ?= /usr/local/natinst/nidaqmxbase/include
NILIBPATH ?= /usr/local/natinst/nidaqmxbase/lib64
NILIBNAME ?= nidaqmxbase

all:
	g++ -std=c++11 -I$(NIINCLUDE) -c measurements.cpp -o obj/measurements.o
	g++ -std=c++11  obj/measurements.o  -L$(NILIBPATH) -l$(NILIBNAME) -o bin/measurements.exe