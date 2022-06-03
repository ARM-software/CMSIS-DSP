function(compilerVersion)
  execute_process(COMMAND "${CMAKE_C_COMPILER}" -dumpversion
     OUTPUT_VARIABLE CVERSION
     ERROR_VARIABLE CVERSION
    )
  SET(COMPILERVERSION ${CVERSION} PARENT_SCOPE)
  #cmake_print_variables(CVERSION)
  #cmake_print_variables(CMAKE_C_COMPILER)
  #MESSAGE( STATUS "CMD_OUTPUT:" ${CVERSION})
endfunction()

function(compilerSpecificCompileOptions PROJECTNAME)

  # Add support for the type __fp16 even if there is no HW
  # support for it.
  if (FLOAT16)
  target_compile_options(${PROJECTNAME} PUBLIC "-mfp16-format=ieee")
  endif()

  if ((OPTIMIZED) AND (NOT (ISAPP)))
    target_compile_options(${PROJECTNAME} PUBLIC "-Ofast")
  endif()
  
  if (FASTMATHCOMPUTATIONS)
      target_compile_options(${PROJECTNAME} PUBLIC "-ffast-math")
  endif()
  
  if (HARDFP)
    target_compile_options(${PROJECTNAME} PUBLIC "-mfloat-abi=hard")
    target_link_options(${PROJECTNAME} PUBLIC "-mfloat-abi=hard")
  endif()
  
  if (LITTLEENDIAN)
    target_compile_options(${PROJECTNAME} PUBLIC "-mlittle-endian")
  endif()

  if (CORTEXM OR CORTEXR)
    target_compile_options(${PROJECTNAME} PUBLIC "-mthumb")
  endif()

  target_link_options(${PROJECTNAME} PUBLIC "-mcpu=${ARM_CPU}")

  # Need to add other gcc config for other cortex-m cores
  if (ARM_CPU STREQUAL "cortex-m55" )
     target_compile_options(${PROJECTNAME} PUBLIC "-march=armv8.1-m.main+mve.fp+fp.dp")
     target_compile_options(${PROJECTNAME} PUBLIC "-mfpu=fpv5-d16")
     target_link_options(${PROJECTNAME} PUBLIC "-mfpu=fpv5-d16")
  endif()

  if (ARM_CPU STREQUAL "cortex-m55+nomve.fp+nofp" )
     target_compile_options(${PROJECTNAME} PUBLIC "-march=armv8.1-m.main+dsp+fp.dp")
     target_compile_options(${PROJECTNAME} PUBLIC "-mfpu=fpv5-d16")
     target_link_options(${PROJECTNAME} PUBLIC "-mfpu=fpv5-d16")
  endif()
  

  if (ARM_CPU STREQUAL "cortex-m33" )
     target_compile_options(${PROJECTNAME} PUBLIC "-mfpu=fpv5-sp-d16")
     target_link_options(${PROJECTNAME} PUBLIC "-mfpu=fpv5-sp-d16")
  endif()

  if (ARM_CPU STREQUAL "cortex-m7" )
     target_compile_options(${PROJECTNAME} PUBLIC "-mfpu=fpv5-d16")
     target_link_options(${PROJECTNAME} PUBLIC "-mfpu=fpv5-d16")
  endif()

  if (ARM_CPU STREQUAL "cortex-m4" )
     target_compile_options(${PROJECTNAME} PUBLIC "-mfpu=fpv4-sp-d16")
     target_link_options(${PROJECTNAME} PUBLIC "-mfpu=fpv4-sp-d16")
  endif()

  #if (ARM_CPU STREQUAL "cortex-m0" )
  #   target_compile_options(${PROJECTNAME} PUBLIC "")
  #   target_link_options(${PROJECTNAME} PUBLIC "")
  #endif()
  
  if (ARM_CPU STREQUAL "cortex-a32" )
      if (NOT (NEON OR NEONEXPERIMENTAL))
        target_compile_options(${PROJECTNAME} PUBLIC "-march=armv8-a;-mfpu=vfpv3-d16")
        target_link_options(${PROJECTNAME} PUBLIC "-march=armv8-a;-mfpu=vfpv3-d16")
      endif()
  endif()
  
  if (ARM_CPU STREQUAL "cortex-a9" )
      if (NOT (NEON OR NEONEXPERIMENTAL))
        target_compile_options(${PROJECTNAME} PUBLIC "-march=armv7-a;-mfpu=vfpv3-d16")
        target_link_options(${PROJECTNAME} PUBLIC "-march=armv7-a;-mfpu=vfpv3-d16")
      endif()
  endif()
  
  if (ARM_CPU STREQUAL "cortex-a7" )
      if (NOT (NEON OR NEONEXPERIMENTAL))
          target_compile_options(${PROJECTNAME} PUBLIC "-march=armv7-a;-mfpu=vfpv3-d16")
          target_link_options(${PROJECTNAME} PUBLIC "-march=armv7-a;-mfpu=vfpv3-d16")
      endif()
  endif()
  
  if (ARM_CPU STREQUAL "cortex-a5" )
      if ((NEON OR NEONEXPERIMENTAL))
        target_compile_options(${PROJECTNAME} PUBLIC "-march=armv7-a;-mfpu=neon-vfpv4")
        target_link_options(${PROJECTNAME} PUBLIC "-march=armv7-a;-mfpu=neon-vfpv4")
      else()
        target_compile_options(${PROJECTNAME} PUBLIC "-march=armv7-a;-mfpu=vfpv3-d16")
        target_link_options(${PROJECTNAME} PUBLIC "-march=armv7-a;-mfpu=vfpv3-d16")
      endif()
  endif()
  
endfunction()

function(preprocessScatter CORE PLATFORMFOLDER SCATTERFILE)

    
    file(REMOVE ${SCATTERFILE})

    # Copy the mem file to the build directory 
    # so that it can be find when preprocessing the scatter file
    # since we cannot pass an include path to armlink
    add_custom_command(
      OUTPUT
       ${SCATTERFILE}
      COMMAND
        ${CMAKE_C_COMPILER} -E -x c -I${PLATFORMFOLDER}/${CORE}/LinkScripts/GCC -o ${SCATTERFILE} ${PLATFORMFOLDER}/${CORE}/LinkScripts/GCC/lnk.ld 
      COMMAND 
        python ${CMSIS}/CMSIS/DSP/filterLinkScript.py ${SCATTERFILE} 
      DEPENDS  
       "${PLATFORMFOLDER}/${CORE}/LinkScripts/GCC/lnk.ld;${PLATFORMFOLDER}/${CORE}/LinkScripts/GCC/mem_${CORE}.h"
      )
    
    add_custom_target(
      scatter ALL
      DEPENDS "${SCATTERFILE};${PLATFORMFOLDER}/${CORE}/LinkScripts/GCC/mem_${CORE}.h"
    )

    add_dependencies(${PROJECTNAME} scatter)
endfunction()

function(toolchainSpecificLinkForCortexM  PROJECTNAME CORE PLATFORMFOLDER HASCSTARTUP)
    if (HASCSTARTUP)
      target_sources(${PROJECTNAME} PRIVATE ${PLATFORMFOLDER}/${CORE}/Startup/GCC/startup_${CORE}.c)
    else()
      target_sources(${PROJECTNAME} PRIVATE ${PLATFORMFOLDER}/${CORE}/Startup/GCC/startup_${CORE}.S)
    endif() 
    target_sources(${PROJECTNAME} PRIVATE  ${PLATFORMFOLDER}/${CORE}/Startup/GCC/support.c)

    target_include_directories(${PROJECTNAME} PRIVATE ${PLATFORMFOLDER}/${CORE}/LinkScripts/GCC)

    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tempLink)
    set(SCATTERFILE ${CMAKE_CURRENT_BINARY_DIR}/tempLink/lnk.ld)
    preprocessScatter(${CORE} ${PLATFORMFOLDER} ${SCATTERFILE})

    set_target_properties(${PROJECTNAME} PROPERTIES LINK_DEPENDS "${SCATTERFILE}")

    target_link_options(${PROJECTNAME} PRIVATE "--entry=Reset_Handler;-T${SCATTERFILE}")
endfunction()

function(toolchainSpecificLinkForCortexA  PROJECTNAME CORE PLATFORMFOLDER)
    target_sources(${PROJECTNAME} PRIVATE ${PLATFORMFOLDER}/${CORE}/Startup/GCC/startup_${CORE}.c)
    target_sources(${PROJECTNAME} PRIVATE  ${PLATFORMFOLDER}/${CORE}/Startup/GCC/support.c)

    # RTE Components
    target_include_directories(${PROJECTNAME} PRIVATE ${CMSIS}/CMSIS/DSP/Testing)
    target_include_directories(${PROJECTNAME} PRIVATE ${PLATFORMFOLDER}/${CORE}/LinkScripts/GCC)

    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tempLink)
    set(SCATTERFILE ${CMAKE_CURRENT_BINARY_DIR}/tempLink/lnk.ld)
    preprocessScatter(${CORE} ${PLATFORMFOLDER} ${SCATTERFILE})


    set_target_properties(${PROJECTNAME} PROPERTIES LINK_DEPENDS "${SCATTERFILE}")

    target_link_options(${PROJECTNAME} PRIVATE "--entry=Reset_Handler;-T${SCATTERFILE}")
endfunction()

function(toolchainSpecificLinkForCortexR  PROJECTNAME CORE PLATFORMFOLDER)
    target_sources(${PROJECTNAME} PRIVATE ${PLATFORMFOLDER}/${CORE}/Startup/GCC/startup_${CORE}.c)
    target_sources(${PROJECTNAME} PRIVATE  ${PLATFORMFOLDER}/${CORE}/Startup/GCC/support.c)

    # RTE Components
    target_include_directories(${PROJECTNAME} PRIVATE ${CMSIS}/CMSIS/DSP/Testing)
    target_include_directories(${PROJECTNAME} PRIVATE ${PLATFORMFOLDER}/${CORE}/LinkScripts/GCC)

    file(MAKE_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/tempLink)
    set(SCATTERFILE ${CMAKE_CURRENT_BINARY_DIR}/tempLink/lnk.ld)
    preprocessScatter(${CORE} ${PLATFORMFOLDER} ${SCATTERFILE})


    set_target_properties(${PROJECTNAME} PROPERTIES LINK_DEPENDS "${SCATTERFILE}")

    target_link_options(${PROJECTNAME} PRIVATE "--entry=Reset_Handler;-T${SCATTERFILE}")
endfunction()

function(compilerSpecificPlatformConfigLibForM PROJECTNAME)
  if (SEMIHOSTING)
    target_link_options(${PROJECTNAME} PRIVATE "--specs=rdimon.specs")
    target_compile_options(${PROJECTNAME} PRIVATE "--specs=rdimon.specs")
  else()
    target_link_options(${PROJECTNAME} PRIVATE "--specs=nosys.specs")
    target_compile_options(${PROJECTNAME} PRIVATE "--specs=nosys.specs")
  endif()
endfunction()

function(compilerSpecificPlatformConfigLibForA PROJECTNAME)
  if (SEMIHOSTING)
    target_link_options(${PROJECTNAME} PRIVATE "--specs=rdimon.specs")
    target_compile_options(${PROJECTNAME} PRIVATE "--specs=rdimon.specs")
  else()
    target_link_options(${PROJECTNAME} PRIVATE "--specs=nosys.specs")
    target_compile_options(${PROJECTNAME} PRIVATE "--specs=nosys.specs")
  endif()
endfunction()

function(compilerSpecificPlatformConfigLibForR PROJECTNAME)
  if (SEMIHOSTING)
    target_link_options(${PROJECTNAME} PRIVATE "--specs=rdimon.specs")
    target_compile_options(${PROJECTNAME} PRIVATE "--specs=rdimon.specs")
  else()
    target_link_options(${PROJECTNAME} PRIVATE "--specs=nosys.specs")
    target_compile_options(${PROJECTNAME} PRIVATE "--specs=nosys.specs")
  endif()
endfunction()

function(compilerSpecificPlatformConfigAppForM PROJECTNAME)
  if (SEMIHOSTING)
    target_link_options(${PROJECTNAME} PRIVATE "--specs=rdimon.specs")
    target_compile_options(${PROJECTNAME} PRIVATE "--specs=rdimon.specs")
  else()
    target_link_options(${PROJECTNAME} PRIVATE "--specs=nosys.specs")
    target_compile_options(${PROJECTNAME} PRIVATE "--specs=nosys.specs")
  endif()

  target_link_options(${PROJECTNAME} PUBLIC "-Wl,--gc-sections")

endfunction()

function(compilerSpecificPlatformConfigAppForA PROJECTNAME)
  if (SEMIHOSTING)
    target_link_options(${PROJECTNAME} PRIVATE "--specs=rdimon.specs")
    target_compile_options(${PROJECTNAME} PRIVATE "--specs=rdimon.specs")
  else()
    target_link_options(${PROJECTNAME} PRIVATE "--specs=nosys.specs")
    target_compile_options(${PROJECTNAME} PRIVATE "--specs=nosys.specs")
  endif()

endfunction()

function(compilerSpecificPlatformConfigAppForR PROJECTNAME)
  if (SEMIHOSTING)
    target_link_options(${PROJECTNAME} PRIVATE "--specs=rdimon.specs")
    target_compile_options(${PROJECTNAME} PRIVATE "--specs=rdimon.specs")
  else()
    target_link_options(${PROJECTNAME} PRIVATE "--specs=nosys.specs")
    target_compile_options(${PROJECTNAME} PRIVATE "--specs=nosys.specs")
  endif()

endfunction()