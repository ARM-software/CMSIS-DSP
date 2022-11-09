ECHO OFF
ECHO "Gen Test"
cd .. 
python processTests.py -f Output.pickle -e UnaryTestsF32 
cd cmsis_build 

REM ECHO "Build"
REM cbuild "test.Release+VHT-Corstone-300.cprj" 
REM ECHO "Run"
REM C:\Keil_v5\ARM\VHT_11.18.29\VHT_Corstone_SSE-300_Ethos-U55.exe -f configs/ARM_VHT_Corstone_300_config.txt -a cpu0="out\test\VHT-Corstone-300\Release\test.Release+VHT-Corstone-300.axf" > results_cs300_u55.txt
REM set MyError=%errorlevel% 
REM python ..\processResult.py  -f ..\Output.pickle -e -r results_cs300_u55.txt
REM if %MyError% neq 0 goto :error
REM goto :EOF

REM ECHO "Build"
REM cbuild "test.Release+VHT_M7_UNROLLED.cprj" 
REM ECHO "Run"
REM C:\Keil_v5\ARM\VHT_11.18.29\VHT_MPS2_Cortex-M7.exe -f configs/ARM_VHT_MPS2_M7DP_config.txt out\test\VHT_M7_UNROLLED\Release\test.Release+VHT_M7_UNROLLED.axf > results_m7_unrolled.txt
REM set MyError=%errorlevel% 
REM python ..\processResult.py  -f ..\Output.pickle -e -r results_m7_unrolled.txt
REM if %MyError% neq 0 goto :error
REM goto :EOF

ECHO "Build"
cbuild "test.Release+VHT_M0P.cprj" 
ECHO "Run"
C:\Keil_v5\ARM\VHT_11.18.29\VHT_MPS2_Cortex-M0plus.exe -f configs/ARM_VHT_MPS2_M0plus_config.txt out\test\VHT_M0P\Release\test.Release+VHT_M0P.axf > results_m0p.txt
set MyError=%errorlevel% 
python ..\processResult.py  -f ..\Output.pickle -e -r results_m0p.txt
if %MyError% neq 0 goto :error
goto :EOF

REM ECHO "Build"
REM cbuild "test.Release+VHT_M55.cprj" 
REM ECHO "Run"
REM C:\Keil_v5\ARM\VHT_11.18.29\VHT_MPS2_Cortex-M55.exe -f configs/ARM_VHT_MPS2_M55_config.txt -a cpu0="out\test\VHT_M55\Release\test.Release+VHT_M55.elf" > results_m55.txt
REM set MyError=%errorlevel% 
REM python ..\processResult.py  -f ..\Output.pickle -e -r results_m55.txt
REM if %MyError% neq 0 goto :error
REM goto :EOF

:error
echo Failed running all tests with error %MyError%.
exit /b %errorlevel%
