#include(CMakePrintHelpers)



#get_filename_component(PROJECT_NAME ${CMAKE_CURRENT_SOURCE_DIR} NAME)

#cmake_print_variables(PROJECT_NAME)


function(cortexm CORE PROJECT_NAME PLATFORMFOLDER CSTARTUP)
   
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMSIS}/CMSIS/Core/Include)
    
    target_sources(${PROJECT_NAME} PRIVATE ${PLATFORMFOLDER}/${CORE}/system_${CORE}.c)
    

    toolchainSpecificLinkForCortexM(${PROJECT_NAME} ${CORE} ${PLATFORMFOLDER} ${CSTARTUP})

    configplatformForApp(${PROJECT_NAME} ${CORE} ${PLATFORMFOLDER})
    SET(PLATFORMID ${PLATFORMID} PARENT_SCOPE)

endfunction()

function(cortexa CORE PROJECT_NAME PLATFORMFOLDER)
    target_include_directories(${PROJECT_NAME} PRIVATE ${CMSIS}/CMSIS/Core_A/Include)

    target_sources(${PROJECT_NAME} PRIVATE ${PLATFORMFOLDER}/${CORE}/irq_ctrl_gic.c)
    target_sources(${PROJECT_NAME} PRIVATE ${PLATFORMFOLDER}/${CORE}/mmu_${CORE}.c)
    target_sources(${PROJECT_NAME} PRIVATE ${PLATFORMFOLDER}/${CORE}/system_${CORE}.c)

    
    target_compile_definitions(${PROJECT_NAME} PRIVATE -DCMSIS_device_header="${CORE}.h")

    toolchainSpecificLinkForCortexA(${PROJECT_NAME} ${CORE} ${PLATFORMFOLDER})

    configplatformForApp(${PROJECT_NAME} ${CORE} ${PLATFORMFOLDER})
    SET(PLATFORMID ${PLATFORMID} PARENT_SCOPE)
endfunction()

function(cortexr CORE PROJECT_NAME  PLATFORMFOLDER)
    target_include_directories(${PROJECT_NAME} PRIVATE ${CORER}/Include)

    #target_sources(${PROJECT_NAME} PRIVATE ${PLATFORMFOLDER}/${CORE}/irq_ctrl_gic.c)
    target_sources(${PROJECT_NAME} PRIVATE ${PLATFORMFOLDER}/${CORE}/mmu_${CORE}.c)
    target_sources(${PROJECT_NAME} PRIVATE ${PLATFORMFOLDER}/${CORE}/system_${CORE}.c)

    
    target_compile_definitions(${PROJECT_NAME} PRIVATE -DCMSIS_device_header="${CORE}.h")

    toolchainSpecificLinkForCortexR(${PROJECT_NAME} ${CORE} ${PLATFORMFOLDER})

    configplatformForApp(${PROJECT_NAME} ${CORE} ${PLATFORMFOLDER})
    SET(PLATFORMID ${PLATFORMID} PARENT_SCOPE)
endfunction()

function(configboot PROJECT_NAME)

  target_include_directories(${PROJECT_NAME} PRIVATE ${CMSIS}/CMSIS/DSP/Include)
  set_platform_core()

  ###################
  #
  # Cortex M
  #
  # C startup for M55 boot code
  if (ARM_CPU MATCHES "^[cC]ortex-[Mm].*")
  if (ARMAC5)
    # ASM startup
    cortexm(${CORE} ${PROJECT_NAME} ${PLATFORMFOLDER} OFF)  
  else()
    # C startup
    cortexm(${CORE} ${PROJECT_NAME} ${PLATFORMFOLDER} ON)  
  endif()  
  endif()
  
  ###################
  #
  # Cortex cortex-a
  #
  if (ARM_CPU MATCHES "^[cC]ortex-[Aa].*")
    cortexa(${CORE} ${PROJECT_NAME} ${PLATFORMFOLDER})
    
  endif()

  ###################
  #
  # Cortex cortex-r
  #
  if (ARM_CPU MATCHES "^[cC]ortex-[rR].*")
    cortexr(${CORE} ${PROJECT_NAME} ${PLATFORMFOLDER})
    
  endif()

  SET(PLATFORMID ${PLATFORMID} PARENT_SCOPE)
  
endfunction()

