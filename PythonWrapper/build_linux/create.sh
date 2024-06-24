cmake -DHOST=YES \
  -DLOOPUNROLL=ON \
  -DWRAPPER=YES \
  -DCMAKE_POSITION_INDEPENDENT_CODE=YES \
  -DCMSISDSP=".." \
  -DCMAKE_C_FLAGS_RELEASE="-std=c11 -Ofast -ffast-math -DNDEBUG -Wall -Wextra" \
  -DCMAKE_CXX_FLAGS_RELEASE="-fno-rtti -std=c++11 -Ofast -ffast-math -DNDEBUG -Wall -Wextra -Wno-unused-parameter" \
  -G "Unix Makefiles" ..

# For Mac universal lib
# -arch x86_64 -arch arm64 -mmacosx-version-min=11.0
