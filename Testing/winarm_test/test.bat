cd "C:\a\CMSIS-DSP\CMSIS-DSP\Testing\winarm_test"
mkdir build
cd build
cmake -G "Ninja" ..
ninja
test.exe
