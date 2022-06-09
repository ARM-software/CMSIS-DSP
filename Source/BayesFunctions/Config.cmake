cmake_minimum_required (VERSION 3.14)


target_sources(CMSISDSP PRIVATE BayesFunctions/arm_gaussian_naive_bayes_predict_f32.c)

if ((NOT ARMAC5) AND (NOT DISABLEFLOAT16))
target_sources(CMSISDSP PRIVATE BayesFunctions/arm_gaussian_naive_bayes_predict_f16.c)
endif()

