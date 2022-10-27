ECHO OFF
ECHO "Gen Test"
cd .. 
python processTests.py -f Output.pickle -e TransformRF32
cd cmsis_build 
ECHO "Build"
cbuild "test.Release+VHT-Corstone-300.cprj" 
ECHO "Run"
C:\Keil_v5\ARM\VHT\VHT_Corstone_SSE-300_Ethos-U55.exe -f configs/ARM_VHT_Corstone_300_config.txt -a cpu0="out\test\VHT-Corstone-300\Release\test.Release+VHT-Corstone-300.axf" > results_cs300_u55.txt
python ..\processResult.py  -f ..\Output.pickle -e -r results_cs300_u55.txt
