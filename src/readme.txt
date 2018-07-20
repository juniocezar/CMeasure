
How to compile from command line:


>>>  %comspec% /k "C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\Common7\Tools\VsDevCmd.bat"

it will open another prompt shell inside the current one



>>>  cd SRC_DIR
>>>> set INCLUDE=%INCLUDE%C:\Program Files (x86)\National Instruments 18\Shared\ExternalCompilerSupport\C\include;
>>>> cl /EHsc measurements.cpp "C:\Program Files (x86)\National Instruments 18\Shared\ExternalCompilerSupport\C\lib32\msvc\NIDAQmx.lib"
