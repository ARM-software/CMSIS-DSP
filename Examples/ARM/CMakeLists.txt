cmake_minimum_required (VERSION 3.14)
project (examples VERSION 0.1)



# Add Testing folder to module path
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../../Testing)

################################### 
#
# LIBRARIES
#
###################################

########### 
#
# CMSIS DSP
#

# Add and configure CMSIS-DSP using the functions from the
# test framework.
#
# It is not required to use the configLib function. 
# You can just include the Source sub-directory
# and define your own compilation options.
# But in that case, you also need to define CMSISCORE to
# point to the CMSIS Core folder from the CMSIS
# repository
# configLib is using the CMSIS variable to find the
# CMSISCORE.
include(config)

add_subdirectory(../../Source bin_dsp)
configLib(CMSISDSP)


################################### 
#
# EXAMPLES
#
###################################

function(configureExample EXAMPLE)
  add_executable(${EXAMPLE})
  configApp(${EXAMPLE})

  # To print some results in stdout when running
  # in a fast model
  target_compile_definitions(${EXAMPLE} PRIVATE SEMIHOSTING)


  ### Sources and libs
  target_link_libraries(${EXAMPLE} PRIVATE CMSISDSP)

  target_include_directories(${EXAMPLE} PRIVATE ${EXAMPLE})

endfunction()




configureExample(arm_bayes_example)
target_sources(arm_bayes_example PRIVATE arm_bayes_example/arm_bayes_example_f32.c)


configureExample(arm_class_marks_example)
target_sources(arm_class_marks_example PRIVATE arm_class_marks_example/arm_class_marks_example_f32.c)

configureExample(arm_convolution_example)
target_sources(arm_convolution_example PRIVATE arm_convolution_example/arm_convolution_example_f32.c 
    arm_convolution_example/math_helper.c)

configureExample(arm_dotproduct_example)
target_sources(arm_dotproduct_example PRIVATE arm_dotproduct_example/arm_dotproduct_example_f32.c)

configureExample(arm_fft_bin_example)
target_sources(arm_fft_bin_example PRIVATE arm_fft_bin_example/arm_fft_bin_data.c 
    arm_fft_bin_example/arm_fft_bin_example_f32.c)

configureExample(arm_fir_example)
target_sources(arm_fir_example PRIVATE arm_fir_example/arm_fir_data.c 
    arm_fir_example/math_helper.c 
    arm_fir_example/arm_fir_example_f32.c)

configureExample(arm_graphic_equalizer_example)
target_sources(arm_graphic_equalizer_example PRIVATE arm_graphic_equalizer_example/math_helper.c 
    arm_graphic_equalizer_example/arm_graphic_equalizer_data.c 
    arm_graphic_equalizer_example/arm_graphic_equalizer_example_q31.c)

configureExample(arm_linear_interp_example)
target_sources(arm_linear_interp_example PRIVATE arm_linear_interp_example/math_helper.c 
    arm_linear_interp_example/arm_linear_interp_data.c 
    arm_linear_interp_example/arm_linear_interp_example_f32.c)

configureExample(arm_matrix_example)
target_sources(arm_matrix_example PRIVATE arm_matrix_example/math_helper.c 
    arm_matrix_example/arm_matrix_example_f32.c)

configureExample(arm_signal_convergence_example)
target_sources(arm_signal_convergence_example PRIVATE arm_signal_converge_example/math_helper.c 
    arm_signal_converge_example/arm_signal_converge_data.c 
    arm_signal_converge_example/arm_signal_converge_example_f32.c)

configureExample(arm_sin_cos_example)
target_sources(arm_sin_cos_example PRIVATE arm_sin_cos_example/arm_sin_cos_example_f32.c)

configureExample(arm_svm_example)
target_sources(arm_svm_example PRIVATE arm_svm_example/arm_svm_example_f32.c)

configureExample(arm_variance_example)
target_sources(arm_variance_example PRIVATE arm_variance_example/arm_variance_example_f32.c)
