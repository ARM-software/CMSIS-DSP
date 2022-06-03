include(CMakePrintHelpers)



include(configLib)
SET(PLATFORMFOLDER Platforms/FVP)
include(configPlatform)
include(configBoot)






# Config app
function (configApp project)
  # When the C compiler is used to process ASM files, the fp16 option 
  # is not always recognized.
  # So, FP16 option is ignored when building boot code 
  # which is containing ASM
  configcore(${project} YES)
  configboot(${project})
  set_platform_core()
  core_includes(${project})
  SET(PLATFORMID ${PLATFORMID} PARENT_SCOPE)
  SET(COREID ${COREID} PARENT_SCOPE)
endfunction()