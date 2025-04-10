cmake -DHOST=YES \
  -DLOOPUNROLL=ON \
  -DWRAPPER=YES \
  -DCMAKE_POSITION_INDEPENDENT_CODE=YES \
  -DCMAKE_OSX_ARCHITECTURES="arm64" \
  -DCMAKE_OSX_DEPLOYMENT_TARGET="10.9" \
  -DCMSISDSP="path to CMSIS-DSP folder" \
  -DCMAKE_C_FLAGS_RELEASE="-std=c11 -Ofast -ffast-math -DNDEBUG -Wall -Wextra" \
  -DCMAKE_CXX_FLAGS_RELEASE="-fno-rtti -std=c++11 -Ofast -ffast-math -DNDEBUG -Wall -Wextra -Wno-unused-parameter" \
  -DNEON=YES \
  -G "Unix Makefiles" ..

# For Mac universal lib
# -arch x86_64 -arch arm64 -mmacosx-version-min=11.0

# Since the extension is now built with Neon,
# arm64;x86_64 is changed to arm64 only in the OSX
# architectures
# -DCMAKE_OSX_ARCHITECTURES="arm64"
