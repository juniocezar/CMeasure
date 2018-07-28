
cd C:\Users\windows10\Documents\repositorios\CMeasure\src
set INCLUDE=%INCLUDE%C:\Program Files (x86)\National Instruments 20\NI-DAQmx Base\Include;
set NILIB=C:\Program Files (x86)\National Instruments 20\NI-DAQmx Base\Lib\nidaqmxbase.lib

cl /EHsc -c stdafx.cpp /Foobj\stdafx.obj
cl /EHsc -c measurements.cpp /Foobj\measurements.obj
cl /EHsc -c cmeasure.cpp /Foobj\cmeasure.obj
cl /EHsc /Fobin\cmeasure.exe obj\cmeasure.obj obj\measurements.obj obj\stdafx.obj "%NILIB%"
