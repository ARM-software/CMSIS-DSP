cmake -DHOST=YES \
  -DCMAKE_BUILD_TYPE=Release \
  -DLOOPUNROLL=ON \
  -DWRAPPER=YES \
  -DCMAKE_POSITION_INDEPENDENT_CODE=YES \
  -DCMSISDSP=".." \
  -DCMAKE_C_FLAGS_RELEASE="-std=c11 -O3 -ffast-math -DNDEBUG -Wall -Wextra" \
  -DCMAKE_CXX_FLAGS_RELEASE="-fno-rtti -std=c++11 -O3 -ffast-math -DNDEBUG -Wall -Wextra -Wno-unused-parameter" \
  -DNEON=YES \
  -G "Unix Makefiles" ..

# For Mac universal lib
# -arch x86_64 -arch arm64 -mmacosx-version-min=11.0
