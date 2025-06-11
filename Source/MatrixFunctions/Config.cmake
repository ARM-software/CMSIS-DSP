cmake_minimum_required (VERSION 3.14)


if (FASTBUILD)
  target_sources(CMSISDSP PRIVATE MatrixFunctions/MatrixFunctions.c)

  if ((NOT ARMAC5) AND (NOT DISABLEFLOAT16))
    target_sources(CMSISDSP PRIVATE MatrixFunctions/MatrixFunctionsF16.c)
  endif()

else()

set(SRCF64 MatrixFunctions/arm_mat_cholesky_f64.c
MatrixFunctions/arm_mat_inverse_f64.c
MatrixFunctions/arm_mat_ldlt_f64.c
MatrixFunctions/arm_mat_mult_f64.c
MatrixFunctions/arm_mat_solve_lower_triangular_f64.c
MatrixFunctions/arm_mat_solve_upper_triangular_f64.c
MatrixFunctions/arm_mat_sub_f64.c
MatrixFunctions/arm_mat_trans_f64.c
MatrixFunctions/arm_mat_qr_f64.c
MatrixFunctions/arm_householder_f64.c
MatrixFunctions/arm_mat_init_f64.c
)

set(SRCF32 MatrixFunctions/arm_mat_add_f32.c
MatrixFunctions/arm_mat_cholesky_f32.c
MatrixFunctions/arm_mat_cmplx_mult_f32.c
MatrixFunctions/arm_mat_cmplx_trans_f32.c
MatrixFunctions/arm_mat_init_f32.c
MatrixFunctions/arm_mat_inverse_f32.c
MatrixFunctions/arm_mat_ldlt_f32.c
MatrixFunctions/arm_mat_mult_f32.c
MatrixFunctions/arm_mat_scale_f32.c
MatrixFunctions/arm_mat_solve_lower_triangular_f32.c
MatrixFunctions/arm_mat_solve_upper_triangular_f32.c
MatrixFunctions/arm_mat_sub_f32.c
MatrixFunctions/arm_mat_trans_f32.c
MatrixFunctions/arm_mat_vec_mult_f32.c
MatrixFunctions/arm_mat_qr_f32.c
MatrixFunctions/arm_householder_f32.c
)

set(SRCQ31 MatrixFunctions/arm_mat_add_q31.c
MatrixFunctions/arm_mat_cmplx_mult_q31.c
MatrixFunctions/arm_mat_cmplx_trans_q31.c
MatrixFunctions/arm_mat_init_q31.c
MatrixFunctions/arm_mat_mult_fast_q31.c
MatrixFunctions/arm_mat_mult_opt_q31.c
MatrixFunctions/arm_mat_mult_q31.c
MatrixFunctions/arm_mat_scale_q31.c
MatrixFunctions/arm_mat_sub_q31.c
MatrixFunctions/arm_mat_trans_q31.c
MatrixFunctions/arm_mat_vec_mult_q31.c
)

set(SRCQ15 MatrixFunctions/arm_mat_add_q15.c
MatrixFunctions/arm_mat_cmplx_mult_q15.c
MatrixFunctions/arm_mat_cmplx_trans_q15.c
MatrixFunctions/arm_mat_init_q15.c
MatrixFunctions/arm_mat_mult_fast_q15.c
MatrixFunctions/arm_mat_mult_q15.c
MatrixFunctions/arm_mat_scale_q15.c
MatrixFunctions/arm_mat_sub_q15.c
MatrixFunctions/arm_mat_trans_q15.c
MatrixFunctions/arm_mat_vec_mult_q15.c
)

set(SRCQ7  MatrixFunctions/arm_mat_mult_q7.c   
    MatrixFunctions/arm_mat_init_q7.c
    MatrixFunctions/arm_mat_vec_mult_q7.c
    MatrixFunctions/arm_mat_trans_q7.c
)


target_sources(CMSISDSP PRIVATE ${SRCF64})
target_sources(CMSISDSP PRIVATE ${SRCF32})

target_sources(CMSISDSP PRIVATE ${SRCQ31})
target_sources(CMSISDSP PRIVATE ${SRCQ15})
target_sources(CMSISDSP PRIVATE ${SRCQ7})

if (NEON OR NEONEXPERIMENTAL)
target_sources(CMSISDSP PRIVATE MatrixFunctions/_arm_mat_mult_neon_buffers.c)

endif()

if ((NOT ARMAC5) AND (NOT DISABLEFLOAT16))
target_sources(CMSISDSP PRIVATE MatrixFunctions/arm_mat_add_f16.c
MatrixFunctions/arm_mat_cholesky_f16.c
MatrixFunctions/arm_mat_cmplx_mult_f16.c
MatrixFunctions/arm_mat_cmplx_trans_f16.c
MatrixFunctions/arm_mat_init_f16.c
MatrixFunctions/arm_mat_inverse_f16.c
MatrixFunctions/arm_mat_mult_f16.c
MatrixFunctions/arm_mat_scale_f16.c
MatrixFunctions/arm_mat_solve_lower_triangular_f16.c
MatrixFunctions/arm_mat_solve_upper_triangular_f16.c
MatrixFunctions/arm_mat_sub_f16.c
MatrixFunctions/arm_mat_trans_f16.c
MatrixFunctions/arm_mat_vec_mult_f16.c
MatrixFunctions/arm_mat_qr_f16.c
MatrixFunctions/arm_householder_f16.c
)

endif()
endif()