cmake -DHOST=YES ^
  -DLOOPUNROLL=ON ^
  -DWRAPPER=YES ^
  -DCMSISDSP="path to CMSIS-DSP folder" ^
  -DCMAKE_C_FLAGS_RELEASE="-std=c11 -Ofast -ffast-math -DNDEBUG -Wall -Wextra" ^
  -DCMAKE_CXX_FLAGS_RELEASE="-fno-rtti -std=c++11 -Ofast -ffast-math -DNDEBUG -Wall -Wextra -Wno-unused-parameter" ^
  -G "Unix Makefiles" ..

