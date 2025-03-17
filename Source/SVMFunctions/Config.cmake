cmake_minimum_required (VERSION 3.14)


if (FASTBUILD)
  target_sources(CMSISDSP PRIVATE SVMFunctions/SVMFunctions.c)

  if ((NOT ARMAC5) AND (NOT DISABLEFLOAT16))
    target_sources(CMSISDSP PRIVATE SVMFunctions/SVMFunctionsF16.c)
  endif()

else()

target_sources(CMSISDSP PRIVATE SVMFunctions/arm_svm_linear_init_f32.c) 
target_sources(CMSISDSP PRIVATE SVMFunctions/arm_svm_rbf_init_f32.c)
target_sources(CMSISDSP PRIVATE SVMFunctions/arm_svm_linear_predict_f32.c)
target_sources(CMSISDSP PRIVATE SVMFunctions/arm_svm_rbf_predict_f32.c)
target_sources(CMSISDSP PRIVATE SVMFunctions/arm_svm_polynomial_init_f32.c)
target_sources(CMSISDSP PRIVATE SVMFunctions/arm_svm_sigmoid_init_f32.c)
target_sources(CMSISDSP PRIVATE SVMFunctions/arm_svm_polynomial_predict_f32.c)  
target_sources(CMSISDSP PRIVATE SVMFunctions/arm_svm_sigmoid_predict_f32.c)





if ((NOT ARMAC5) AND (NOT DISABLEFLOAT16))
target_sources(CMSISDSP PRIVATE SVMFunctions/arm_svm_linear_init_f16.c) 
target_sources(CMSISDSP PRIVATE SVMFunctions/arm_svm_rbf_init_f16.c)
target_sources(CMSISDSP PRIVATE SVMFunctions/arm_svm_linear_predict_f16.c)
target_sources(CMSISDSP PRIVATE SVMFunctions/arm_svm_rbf_predict_f16.c)
target_sources(CMSISDSP PRIVATE SVMFunctions/arm_svm_polynomial_init_f16.c)
target_sources(CMSISDSP PRIVATE SVMFunctions/arm_svm_sigmoid_init_f16.c)
target_sources(CMSISDSP PRIVATE SVMFunctions/arm_svm_polynomial_predict_f16.c)  
target_sources(CMSISDSP PRIVATE SVMFunctions/arm_svm_sigmoid_predict_f16.c)
endif()



endif()