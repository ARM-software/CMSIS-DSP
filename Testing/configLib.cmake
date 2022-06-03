# This is for building a library only
# It is similar to the config.cmake but ignoring anything related to the platform
# and boot code
include(Toolchain/Tools)
option(OPTIMIZED "Compile for speed" OFF)

enable_language(CXX C ASM)


# Otherwise there is a .obj on windows and it creates problems
# with armlink. 
SET(CMAKE_C_OUTPUT_EXTENSION .o)
SET(CMAKE_CXX_OUTPUT_EXTENSION .o)
SET(CMAKE_ASM_OUTPUT_EXTENSION .o)

include(configCore)


function(configLib PROJECTNAME)
  configcore(${PROJECTNAME} NO)
  
  SET(COREID ${COREID} PARENT_SCOPE)

endfunction()

