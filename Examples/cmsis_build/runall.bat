REM Depending on the version of MDK you have installed, the path to 
REM FVP executables may need to be updated.
REM 
REM * MDK versions before 5.37:    `Keil_v5/ARM/FVP`
REM * MDK versions 5.38, 5.39:     `Keil_v5/ARM/VHT`
REM * MDK version 5.40 or later:   `Keil_v5/ARM/avh-fvp/bin/models`
REM 
REM With MDK >= 5.40, the name of FVP is starting with FVP_
REM instead of VHT_
REM 
REM Below the paths for MDK >= 5.40

ECHO "Bayes"
C:\Keil_v5\ARM\avh-fvp\bin\models\FVP_Corstone_SSE-300_Ethos-U55.exe ^
  -f ..\ARM\arm_bayes_example\ARMCM55_FP_MVE_config.txt ^
  -C cpu0.semihosting-enable=1 ^
  -C mps3_board.visualisation.disable-visualisation=1 ^
  -a cpu0="cprj\out\bayes\VHT-Corstone-300\Release\bayes.axf" 

ECHO "Class marks"
C:\Keil_v5\ARM\avh-fvp\bin\models\FVP_Corstone_SSE-300_Ethos-U55.exe ^
  -f ..\ARM\arm_class_marks_example\ARMCM55_FP_MVE_config.txt ^
  -C cpu0.semihosting-enable=1 ^
  -C mps3_board.visualisation.disable-visualisation=1 ^
  -a cpu0="cprj\out\classmarks\VHT-Corstone-300\Release\classmarks.axf" 

ECHO "Convolution"
C:\Keil_v5\ARM\avh-fvp\bin\models\FVP_Corstone_SSE-300_Ethos-U55.exe ^
  -f ..\ARM\arm_convolution_example\ARMCM55_FP_MVE_config.txt ^
  -C cpu0.semihosting-enable=1 ^
  -C mps3_board.visualisation.disable-visualisation=1 ^
  -a cpu0="cprj\out\convolution\VHT-Corstone-300\Release\convolution.axf" 

ECHO "Dot product"
C:\Keil_v5\ARM\avh-fvp\bin\models\FVP_Corstone_SSE-300_Ethos-U55.exe ^
  -f ..\ARM\arm_dotproduct_example\ARMCM55_FP_MVE_config.txt ^
  -C cpu0.semihosting-enable=1 ^
  -C mps3_board.visualisation.disable-visualisation=1 ^
  -a cpu0="cprj\out\dotproduct\VHT-Corstone-300\Release\dotproduct.axf" 

ECHO "FFT Bin"
C:\Keil_v5\ARM\avh-fvp\bin\models\FVP_Corstone_SSE-300_Ethos-U55.exe ^
  -f ..\ARM\arm_fft_bin_example\ARMCM55_FP_MVE_config.txt ^
  -C cpu0.semihosting-enable=1 ^
  -C mps3_board.visualisation.disable-visualisation=1 ^
  -a cpu0="cprj\out\fftbin\VHT-Corstone-300\Release\fftbin.axf" 

ECHO "Fir example"
C:\Keil_v5\ARM\avh-fvp\bin\models\FVP_Corstone_SSE-300_Ethos-U55.exe ^
  -f ..\ARM\arm_fir_example\ARMCM55_FP_MVE_config.txt ^
  -C cpu0.semihosting-enable=1 ^
  -C mps3_board.visualisation.disable-visualisation=1 ^
  -a cpu0="cprj\out\fir\VHT-Corstone-300\Release\fir.axf" 

ECHO "Graphic Equalizer"
C:\Keil_v5\ARM\avh-fvp\bin\models\FVP_Corstone_SSE-300_Ethos-U55.exe ^
  -f ..\ARM\arm_graphic_equalizer_example\ARMCM55_FP_MVE_config.txt ^
  -C cpu0.semihosting-enable=1 ^
  -C mps3_board.visualisation.disable-visualisation=1 ^
  -a cpu0="cprj\out\graphicequalizer\VHT-Corstone-300\Release\graphicequalizer.axf" 

ECHO "Linear interpolation"
C:\Keil_v5\ARM\avh-fvp\bin\models\FVP_Corstone_SSE-300_Ethos-U55.exe ^
  -f ..\ARM\arm_linear_interp_example\ARMCM55_FP_MVE_config.txt ^
  -C cpu0.semihosting-enable=1 ^
  -C mps3_board.visualisation.disable-visualisation=1 ^
  -a cpu0="cprj\out\linearinterp\VHT-Corstone-300\Release\linearinterp.axf" 

ECHO "Matrix"
C:\Keil_v5\ARM\avh-fvp\bin\models\FVP_Corstone_SSE-300_Ethos-U55.exe ^
  -f ..\ARM\arm_matrix_example\ARMCM55_FP_MVE_config.txt ^
  -C cpu0.semihosting-enable=1 ^
  -C mps3_board.visualisation.disable-visualisation=1 ^
  -a cpu0="cprj\out\matrix\VHT-Corstone-300\Release\matrix.axf" 

ECHO "Signal Converge"
C:\Keil_v5\ARM\avh-fvp\bin\models\FVP_Corstone_SSE-300_Ethos-U55.exe ^
  -f ..\ARM\arm_signal_converge_example\ARMCM55_FP_MVE_config.txt ^
  -C cpu0.semihosting-enable=1 ^
  -C mps3_board.visualisation.disable-visualisation=1 ^
  -a cpu0="cprj\out\signalconverge\VHT-Corstone-300\Release\signalconverge.axf" 

ECHO "Sin Cos"
C:\Keil_v5\ARM\avh-fvp\bin\models\FVP_Corstone_SSE-300_Ethos-U55.exe ^
  -f ..\ARM\arm_sin_cos_example\ARMCM55_FP_MVE_config.txt ^
  -C cpu0.semihosting-enable=1 ^
  -C mps3_board.visualisation.disable-visualisation=1 ^
  -a cpu0="cprj\out\sincos\VHT-Corstone-300\Release\sincos.axf" 

ECHO "SVM"
C:\Keil_v5\ARM\avh-fvp\bin\models\FVP_Corstone_SSE-300_Ethos-U55.exe ^
  -f ..\ARM\arm_svm_example\ARMCM55_FP_MVE_config.txt ^
  -C cpu0.semihosting-enable=1 ^
  -C mps3_board.visualisation.disable-visualisation=1 ^
  -a cpu0="cprj\out\svm\VHT-Corstone-300\Release\svm.axf" 

ECHO "Variance"
C:\Keil_v5\ARM\avh-fvp\bin\models\FVP_Corstone_SSE-300_Ethos-U55.exe ^
  -f ..\ARM\arm_variance_example\ARMCM55_FP_MVE_config.txt ^
  -C cpu0.semihosting-enable=1 ^
  -C mps3_board.visualisation.disable-visualisation=1 ^
  -a cpu0="cprj\out\variance\VHT-Corstone-300\Release\variance.axf" 

