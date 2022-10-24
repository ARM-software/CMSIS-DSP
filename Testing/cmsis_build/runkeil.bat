ECHO OFF
C:\Keil_v5\ARM\VHT\VHT_MPS2_Cortex-M0plus.exe ^
   -f configs/ARM_VHT_MPS2_M0plus_config.txt ^
   out\test\VHT_M0P\Release\test.Release+VHT_M0P.axf > results_m0p.txt

C:\Keil_v5\ARM\VHT\VHT_MPS2_Cortex-M23.exe ^
   -f configs/ARM_VHT_MPS2_M23_config.txt ^
   out\test\VHT_M23\Release\test.Release+VHT_M23.axf > results_m23.txt

C:\Keil_v5\ARM\VHT\VHT_MPS2_Cortex-M3.exe ^
   -f configs/ARM_VHT_MPS2_M3_config.txt ^
   out\test\VHT_M3\Release\test.Release+VHT_M3.axf > results_m3.txt

C:\Keil_v5\ARM\VHT\VHT_MPS2_Cortex-M4.exe ^
   -f configs/ARM_VHT_MPS2_M4FP_config.txt ^
   out\test\VHT_M4\Release\test.Release+VHT_M4.axf > results_m4.txt

C:\Keil_v5\ARM\VHT\VHT_MPS2_Cortex-M7.exe ^
   -f configs/ARM_VHT_MPS2_M7DP_config.txt ^
   out\test\VHT_M7\Release\test.Release+VHT_M7.axf > results_m7.txt

C:\Keil_v5\ARM\VHT\VHT_MPS2_Cortex-M7.exe ^
   -f configs/ARM_VHT_MPS2_M7DP_config.txt ^
   out\test\VHT_M7_UNROLLED\Release\test.Release+VHT_M7_UNROLLED.axf > results_m7_unrolled.txt


C:\Keil_v5\ARM\VHT\VHT_MPS2_Cortex-M33.exe ^
   -f configs/ARM_VHT_MPS2_M33_DSP_FP_config.txt ^
   -a cpu0="out\test\VHT_M33\Release\test.Release+VHT_M33.axf" > results_m33.txt

C:\Keil_v5\ARM\VHT\VHT_MPS3_Corstone_SSE-300.exe ^
  -f configs/ARM_VHT_Corstone_300_config.txt ^
  -a cpu0="out\test\VHT-Corstone-300\Release\test.Release+VHT-Corstone-300.axf" > results_cs300.txt


C:\Keil_v5\ARM\VHT\VHT_Corstone_SSE-310.exe ^
  -f configs/ARM_VHT_Corstone_310_config.txt ^
  -a cpu0="out\test\VHT-Corstone-310\Release\test.Release+VHT-Corstone-310.axf" > results_cs310.txt

C:\Keil_v5\ARM\VHT\VHT_Corstone_SSE-300_Ethos-U55.exe ^
  -f configs/ARM_VHT_Corstone_300_config.txt ^
  -a cpu0="out\test\VHT-Corstone-300\Release\test.Release+VHT-Corstone-300.axf" > results_cs300_u55.txt
