"C:\Program Files\ARM\Development Studio 2022.0\bin\FVP_VE_Cortex-A5x1.exe" ^
    -f configs/ARMCA5neon_config.txt ^
    Objects\test.Release+FVP_A5Neon.axf > results_a5.txt

"C:\Program Files\ARM\Development Studio 2022.0\bin\FVP_VE_Cortex-A7x1.exe" ^
    -f configs/ARMCA7neon_config.txt ^
    Objects\test.Release+FVP_A7Neon.axf > results_a7.txt

"C:\Program Files\ARM\Development Studio 2022.0\bin\FVP_VE_Cortex-A9x1.exe" ^
    -f configs/ARMCA9neon_config.txt ^
    Objects\test.Release+FVP_A9Neon.axf > results_a9.txt