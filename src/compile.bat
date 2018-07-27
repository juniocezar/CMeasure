set INCLUDE=%INCLUDE%C:\Program Files (x86)\National Instruments 18\Shared\ExternalCompilerSupport\C\include;

del *.exe
cl -c stdafx.cpp
cl -c measurements.cpp
cl measurements.obj stdafx.obj "C:\Program Files (x86)\National Instruments 18\Shared\ExternalCompilerSupport\C\lib32\msvc\NIDAQmx.lib"
del *obj
