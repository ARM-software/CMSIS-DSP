#include "DspOpTests.h"
#include "dsp/none.h"
#include "dsp/utils.h"
#include "arm_common_tables.h"
#include "Error.h"

#define DEFINE_UNARY_Q31_TEST(METHOD, OPERATION)                         \
void DspOpTests::METHOD()                                               \
{                                                                       \
    const q31_t *a = inputAQ31.ptr();                                   \
    q31_t *out = outputAQ31.ptr();                                      \
    for (unsigned long i = 0; i < refAQ31.nbSamples(); ++i)             \
    {                                                                   \
        out[i] = (q31_t)OPERATION((uint32_t)a[i]);                       \
    }                                                                   \
    ASSERT_EQ(outputAQ31, refAQ31);                                     \
}

#define DEFINE_BINARY_Q31_TEST(METHOD, OPERATION)                        \
void DspOpTests::METHOD()                                               \
{                                                                       \
    const q31_t *a = inputAQ31.ptr();                                   \
    const q31_t *b = inputBQ31.ptr();                                   \
    q31_t *out = outputAQ31.ptr();                                      \
    for (unsigned long i = 0; i < refAQ31.nbSamples(); ++i)             \
    {                                                                   \
        out[i] = (q31_t)OPERATION(a[i], b[i]);                           \
    }                                                                   \
    ASSERT_EQ(outputAQ31, refAQ31);                                     \
}

#define DEFINE_TERNARY_Q31_TEST(METHOD, OPERATION)                       \
void DspOpTests::METHOD()                                               \
{                                                                       \
    const q31_t *a = inputAQ31.ptr();                                   \
    const q31_t *b = inputBQ31.ptr();                                   \
    const q31_t *c = inputCQ31.ptr();                                   \
    q31_t *out = outputAQ31.ptr();                                      \
    for (unsigned long i = 0; i < refAQ31.nbSamples(); ++i)             \
    {                                                                   \
        out[i] = (q31_t)OPERATION(a[i], b[i], c[i]);                     \
    }                                                                   \
    ASSERT_EQ(outputAQ31, refAQ31);                                     \
}

DEFINE_UNARY_Q31_TEST(test_clz, __CLZ)
DEFINE_UNARY_Q31_TEST(test_sxtb16, __SXTB16)
DEFINE_BINARY_Q31_TEST(test_ror, __ROR)
DEFINE_BINARY_Q31_TEST(test_qadd8, __QADD8)
DEFINE_BINARY_Q31_TEST(test_qsub8, __QSUB8)
DEFINE_BINARY_Q31_TEST(test_qadd16, __QADD16)
DEFINE_BINARY_Q31_TEST(test_shadd16, __SHADD16)
DEFINE_BINARY_Q31_TEST(test_qsub16, __QSUB16)
DEFINE_BINARY_Q31_TEST(test_shsub16, __SHSUB16)
DEFINE_BINARY_Q31_TEST(test_qasx, __QASX)
DEFINE_BINARY_Q31_TEST(test_shasx, __SHASX)
DEFINE_BINARY_Q31_TEST(test_qsax, __QSAX)
DEFINE_BINARY_Q31_TEST(test_shsax, __SHSAX)
DEFINE_BINARY_Q31_TEST(test_smusdx, __SMUSDX)
DEFINE_BINARY_Q31_TEST(test_smuadx, __SMUADX)
DEFINE_BINARY_Q31_TEST(test_qadd, __QADD)
DEFINE_BINARY_Q31_TEST(test_qsub, __QSUB)
DEFINE_BINARY_Q31_TEST(test_smuad, __SMUAD)
DEFINE_BINARY_Q31_TEST(test_smusd, __SMUSD)
DEFINE_TERNARY_Q31_TEST(test_smlad, __SMLAD)
DEFINE_TERNARY_Q31_TEST(test_smladx, __SMLADX)
DEFINE_TERNARY_Q31_TEST(test_smlsdx, __SMLSDX)
DEFINE_TERNARY_Q31_TEST(test_smmla, __SMMLA)

#undef DEFINE_UNARY_Q31_TEST
#undef DEFINE_BINARY_Q31_TEST
#undef DEFINE_TERNARY_Q31_TEST

static q31_t call_ssat(q31_t value, uint32_t sat)
{
    switch (sat)
    {
        case 1: return __SSAT(value, 1);
        case 5: return __SSAT(value, 5);
        case 6: return __SSAT(value, 6);
        case 8: return __SSAT(value, 8);
        case 9: return __SSAT(value, 9);
        case 10: return __SSAT(value, 10);
        case 13: return __SSAT(value, 13);
        case 16: return __SSAT(value, 16);
        case 17: return __SSAT(value, 17);
        case 21: return __SSAT(value, 21);
        case 25: return __SSAT(value, 25);
        case 29: return __SSAT(value, 29);
        case 31: return __SSAT(value, 31);
        case 32: return __SSAT(value, 32);
        default: return value;
    }
}

static q31_t call_usat(q31_t value, uint32_t sat)
{
    switch (sat)
    {
        case 1: return (q31_t)__USAT(value, 1);
        case 5: return (q31_t)__USAT(value, 5);
        case 6: return (q31_t)__USAT(value, 6);
        case 8: return (q31_t)__USAT(value, 8);
        case 9: return (q31_t)__USAT(value, 9);
        case 10: return (q31_t)__USAT(value, 10);
        case 13: return (q31_t)__USAT(value, 13);
        case 16: return (q31_t)__USAT(value, 16);
        case 17: return (q31_t)__USAT(value, 17);
        case 21: return (q31_t)__USAT(value, 21);
        case 25: return (q31_t)__USAT(value, 25);
        case 29: return (q31_t)__USAT(value, 29);
        case 31: return (q31_t)__USAT(value, 31);
        default: return value;
    }
}

void DspOpTests::test_ssat()
{
    const q31_t *a = inputAQ31.ptr();
    const q31_t *sat = inputCQ31.ptr();
    q31_t *out = outputAQ31.ptr();
    for (unsigned long i = 0; i < refAQ31.nbSamples(); ++i)
    {
        out[i] = call_ssat(a[i], (uint32_t)sat[i]);
    }
    ASSERT_EQ(outputAQ31, refAQ31);
}

void DspOpTests::test_usat()
{
    const q31_t *a = inputAQ31.ptr();
    const q31_t *sat = inputCQ31.ptr();
    q31_t *out = outputAQ31.ptr();
    for (unsigned long i = 0; i < refAQ31.nbSamples(); ++i)
    {
        out[i] = call_usat(a[i], (uint32_t)sat[i]);
    }
    ASSERT_EQ(outputAQ31, refAQ31);
}

void DspOpTests::test_smlad_sat()
{
    const q31_t *a = inputAQ31.ptr();
    const q31_t *b = inputBQ31.ptr();
    const q63_t *acc = inputAQ63.ptr();
    q63_t *out = outputQ63.ptr();
    for (unsigned long i = 0; i < refQ63.nbSamples(); ++i)
    {
        out[i] = (q63_t)__SMLALD((uint32_t)a[i], (uint32_t)b[i],
                                 (uint64_t)acc[i]);
    }
    ASSERT_EQ(outputQ63, refQ63);
}

void DspOpTests::test_smlaldx_sat()
{
    const q31_t *a = inputAQ31.ptr();
    const q31_t *b = inputBQ31.ptr();
    const q63_t *acc = inputAQ63.ptr();
    q63_t *out = outputQ63.ptr();
    for (unsigned long i = 0; i < refQ63.nbSamples(); ++i)
    {
        out[i] = (q63_t)__SMLALDX((uint32_t)a[i], (uint32_t)b[i],
                                  (uint64_t)acc[i]);
    }
    ASSERT_EQ(outputQ63, refQ63);
}

void DspOpTests::test_clip_q63_to_q31()
{
    const q63_t *input = inputAQ63.ptr();
    q31_t *out = outputAQ31.ptr();
    for (unsigned long i = 0; i < refAQ31.nbSamples(); ++i)
    {
        out[i] = clip_q63_to_q31(input[i]);
    }
    ASSERT_EQ(outputAQ31, refAQ31);
}

void DspOpTests::test_clip_q31_to_q15()
{
    const q31_t *input = inputAQ31.ptr();
    q15_t *out = outputAQ15.ptr();
    for (unsigned long i = 0; i < refAQ15.nbSamples(); ++i)
    {
        out[i] = clip_q31_to_q15(input[i]);
    }
    ASSERT_EQ(outputAQ15, refAQ15);
}

void DspOpTests::test_mult32x64()
{
    const q63_t *a = inputAQ63.ptr();
    const q31_t *b = inputAQ31.ptr();
    q63_t *out = outputQ63.ptr();
    for (unsigned long i = 0; i < refQ63.nbSamples(); ++i)
    {
        out[i] = mult32x64(a[i], b[i]);
    }
    ASSERT_EQ(outputQ63, refQ63);
}

void DspOpTests::test_recip_q31()
{
    const q31_t *input = inputAQ31.ptr();
    q31_t *out = outputAQ31.ptr();
    q15_t *shift = outputAQ15.ptr();
    for (unsigned long i = 0; i < refAQ31.nbSamples(); ++i)
    {
        shift[i] = (q15_t)arm_recip_q31(input[i], &out[i], armRecipTableQ31);
    }
    ASSERT_NEAR_EQ(refAQ31, outputAQ31, (q31_t)10);
    ASSERT_EQ(refAQ15, outputAQ15);
}

void DspOpTests::test_recip_q15()
{
    const q15_t *input = inputAQ15.ptr();
    q15_t *out = outputAQ15.ptr();
    q15_t *shift = outputBQ15.ptr();
    for (unsigned long i = 0; i < refAQ15.nbSamples(); ++i)
    {
        shift[i] = (q15_t)arm_recip_q15(input[i], &out[i], armRecipTableQ15);
    }
    ASSERT_NEAR_EQ(refAQ15, outputAQ15, (q15_t)2);
    ASSERT_EQ(refBQ15, outputBQ15);
}

void DspOpTests::test_norm_64_to_32u()
{
    const q63_t *input = inputAQ63.ptr();
    q31_t *value = outputAQ31.ptr();
    q31_t *norm = outputBQ31.ptr();
    for (unsigned long i = 0; i < refAQ31.nbSamples(); ++i)
    {
        arm_norm_64_to_32u((uint64_t)input[i], &value[i], &norm[i]);
    }
    ASSERT_EQ(refAQ31, outputAQ31);
    ASSERT_EQ(refBQ31, outputBQ31);
}

void DspOpTests::test_div_int64_to_int32()
{
    const q63_t *num = inputAQ63.ptr();
    const q31_t *den = inputBQ31.ptr();
    q31_t *out = outputAQ31.ptr();
    for (unsigned long i = 0; i < refAQ31.nbSamples(); ++i)
    {
        out[i] = arm_div_int64_to_int32(num[i], den[i]);
    }
    ASSERT_EQ(refAQ31, outputAQ31);
}

#define DEFINE_MULT_MACRO_TEST(METHOD, OPERATION)                        \
void DspOpTests::METHOD()                                               \
{                                                                       \
    const q31_t *a = inputAQ31.ptr();                                   \
    const q31_t *b = inputBQ31.ptr();                                   \
    const q31_t *c = inputCQ31.ptr();                                   \
    q31_t *out = outputAQ31.ptr();                                      \
    for (unsigned long i = 0; i < refAQ31.nbSamples(); ++i)             \
    {                                                                   \
        q31_t value = c[i];                                             \
        OPERATION(value, a[i], b[i]);                                   \
        out[i] = value;                                                 \
    }                                                                   \
    ASSERT_EQ(outputAQ31, refAQ31);                                     \
}

DEFINE_MULT_MACRO_TEST(test_multacc_32x32_keep32_r, multAcc_32x32_keep32_R)
DEFINE_MULT_MACRO_TEST(test_multsub_32x32_keep32_r, multSub_32x32_keep32_R)
DEFINE_MULT_MACRO_TEST(test_mult_32x32_keep32_r, mult_32x32_keep32_R)
DEFINE_MULT_MACRO_TEST(test_multacc_32x32_keep32, multAcc_32x32_keep32)
DEFINE_MULT_MACRO_TEST(test_multsub_32x32_keep32, multSub_32x32_keep32)
DEFINE_MULT_MACRO_TEST(test_mult_32x32_keep32, mult_32x32_keep32)

#undef DEFINE_MULT_MACRO_TEST

#define SETUP_UNARY_Q31(TEST_ID, REF_ID)                                 \
    case DspOpTests::TEST_ID:                                           \
        inputAQ31.reload(DspOpTests::INPUTA_Q31_ID, mgr);                \
        refAQ31.reload(DspOpTests::REF_ID, mgr);                         \
        outputAQ31.create(refAQ31.nbSamples(), DspOpTests::OUTA_Q31_ID, mgr); \
        break

#define SETUP_BINARY_Q31(TEST_ID, REF_ID)                                \
    case DspOpTests::TEST_ID:                                           \
        inputAQ31.reload(DspOpTests::INPUTA_Q31_ID, mgr);                \
        inputBQ31.reload(DspOpTests::INPUTB_Q31_ID, mgr);                \
        refAQ31.reload(DspOpTests::REF_ID, mgr);                         \
        outputAQ31.create(refAQ31.nbSamples(), DspOpTests::OUTA_Q31_ID, mgr); \
        break

#define SETUP_TERNARY_Q31(TEST_ID, REF_ID)                               \
    case DspOpTests::TEST_ID:                                           \
        inputAQ31.reload(DspOpTests::INPUTA_Q31_ID, mgr);                \
        inputBQ31.reload(DspOpTests::INPUTB_Q31_ID, mgr);                \
        inputCQ31.reload(DspOpTests::INPUTC_Q31_ID, mgr);                \
        refAQ31.reload(DspOpTests::REF_ID, mgr);                         \
        outputAQ31.create(refAQ31.nbSamples(), DspOpTests::OUTA_Q31_ID, mgr); \
        break

void DspOpTests::setUp(Testing::testID_t id,
                       std::vector<Testing::param_t>& params,
                       Client::PatternMgr *mgr)
{
    (void)params;
    switch (id)
    {
        case DspOpTests::TEST_SMLAD_SAT_1:
            inputAQ31.reload(DspOpTests::INPUTA_Q31_ID, mgr);
            inputBQ31.reload(DspOpTests::INPUTB_Q31_ID, mgr);
            inputAQ63.reload(DspOpTests::INPUTA_Q63_ID, mgr);
            refQ63.reload(DspOpTests::REF_SMLALD_ID, mgr);
            outputQ63.create(refQ63.nbSamples(), DspOpTests::OUT_Q63_ID, mgr);
            break;

        case DspOpTests::TEST_SMLALDX_SAT_2:
            inputAQ31.reload(DspOpTests::INPUTA_Q31_ID, mgr);
            inputBQ31.reload(DspOpTests::INPUTB_Q31_ID, mgr);
            inputAQ63.reload(DspOpTests::INPUTA_Q63_ID, mgr);
            refQ63.reload(DspOpTests::REF_SMLALDX_ID, mgr);
            outputQ63.create(refQ63.nbSamples(), DspOpTests::OUT_Q63_ID, mgr);
            break;

        case DspOpTests::TEST_CLIP_Q63_TO_Q31_3:
            inputAQ63.reload(DspOpTests::INPUTA_Q63_ID, mgr);
            refAQ31.reload(DspOpTests::REF_CLIP_Q31_ID, mgr);
            outputAQ31.create(refAQ31.nbSamples(), DspOpTests::OUTA_Q31_ID, mgr);
            break;

        case DspOpTests::TEST_CLIP_Q31_TO_Q15_4:
            inputAQ31.reload(DspOpTests::INPUTA_Q31_ID, mgr);
            refAQ15.reload(DspOpTests::REF_CLIP_Q15_ID, mgr);
            outputAQ15.create(refAQ15.nbSamples(), DspOpTests::OUTA_Q15_ID, mgr);
            break;

        SETUP_UNARY_Q31(TEST_CLZ_5, REF_CLZ_ID);
        SETUP_TERNARY_Q31(TEST_SSAT_6, REF_SSAT_ID);
        SETUP_TERNARY_Q31(TEST_USAT_7, REF_USAT_ID);
        SETUP_BINARY_Q31(TEST_ROR_8, REF_ROR_ID);

        case DspOpTests::TEST_MULT32X64_9:
            inputAQ63.reload(DspOpTests::INPUTA_Q63_ID, mgr);
            inputAQ31.reload(DspOpTests::INPUTA_Q31_ID, mgr);
            refQ63.reload(DspOpTests::REF_MULT32X64_ID, mgr);
            outputQ63.create(refQ63.nbSamples(), DspOpTests::OUT_Q63_ID, mgr);
            break;

        SETUP_BINARY_Q31(TEST_QADD8_10, REF_QADD8_ID);
        SETUP_BINARY_Q31(TEST_QSUB8_11, REF_QSUB8_ID);
        SETUP_BINARY_Q31(TEST_QADD16_12, REF_QADD16_ID);
        SETUP_BINARY_Q31(TEST_SHADD16_13, REF_SHADD16_ID);
        SETUP_BINARY_Q31(TEST_QSUB16_14, REF_QSUB16_ID);
        SETUP_BINARY_Q31(TEST_SHSUB16_15, REF_SHSUB16_ID);
        SETUP_BINARY_Q31(TEST_QASX_16, REF_QASX_ID);
        SETUP_BINARY_Q31(TEST_SHASX_17, REF_SHASX_ID);
        SETUP_BINARY_Q31(TEST_QSAX_18, REF_QSAX_ID);
        SETUP_BINARY_Q31(TEST_SHSAX_19, REF_SHSAX_ID);
        SETUP_BINARY_Q31(TEST_SMUSDX_20, REF_SMUSDX_ID);
        SETUP_BINARY_Q31(TEST_SMUADX_21, REF_SMUADX_ID);
        SETUP_BINARY_Q31(TEST_QADD_22, REF_QADD_ID);
        SETUP_BINARY_Q31(TEST_QSUB_23, REF_QSUB_ID);
        SETUP_TERNARY_Q31(TEST_SMLAD_24, REF_SMLAD_ID);
        SETUP_TERNARY_Q31(TEST_SMLADX_25, REF_SMLADX_ID);
        SETUP_TERNARY_Q31(TEST_SMLSDX_26, REF_SMLSDX_ID);
        SETUP_BINARY_Q31(TEST_SMUAD_27, REF_SMUAD_ID);
        SETUP_BINARY_Q31(TEST_SMUSD_28, REF_SMUSD_ID);
        SETUP_UNARY_Q31(TEST_SXTB16_29, REF_SXTB16_ID);
        SETUP_TERNARY_Q31(TEST_SMMLA_30, REF_SMMLA_ID);

        case DspOpTests::TEST_RECIP_Q31_31:
            inputAQ31.reload(DspOpTests::INPUTA_Q31_ID, mgr);
            refAQ31.reload(DspOpTests::REF_RECIP_Q31_ID, mgr);
            refAQ15.reload(DspOpTests::REF_RECIP_Q31_SHIFT_ID, mgr);
            outputAQ31.create(refAQ31.nbSamples(), DspOpTests::OUTA_Q31_ID, mgr);
            outputAQ15.create(refAQ15.nbSamples(), DspOpTests::OUTA_Q15_ID, mgr);
            break;

        case DspOpTests::TEST_RECIP_Q15_32:
            inputAQ15.reload(DspOpTests::INPUTA_Q15_ID, mgr);
            refAQ15.reload(DspOpTests::REF_RECIP_Q15_ID, mgr);
            refBQ15.reload(DspOpTests::REF_RECIP_Q15_SHIFT_ID, mgr);
            outputAQ15.create(refAQ15.nbSamples(), DspOpTests::OUTA_Q15_ID, mgr);
            outputBQ15.create(refBQ15.nbSamples(), DspOpTests::OUTB_Q15_ID, mgr);
            break;

        case DspOpTests::TEST_NORM_64_TO_32U_33:
            inputAQ63.reload(DspOpTests::INPUTB_Q63_ID, mgr);
            refAQ31.reload(DspOpTests::REF_NORM_VALUE_ID, mgr);
            refBQ31.reload(DspOpTests::REF_NORM_SHIFT_ID, mgr);
            outputAQ31.create(refAQ31.nbSamples(), DspOpTests::OUTA_Q31_ID, mgr);
            outputBQ31.create(refBQ31.nbSamples(), DspOpTests::OUTB_Q31_ID, mgr);
            break;

        case DspOpTests::TEST_DIV_INT64_TO_INT32_34:
            inputAQ63.reload(DspOpTests::INPUTA_Q63_ID, mgr);
            inputBQ31.reload(DspOpTests::INPUTB_Q31_ID, mgr);
            refAQ31.reload(DspOpTests::REF_DIV_ID, mgr);
            outputAQ31.create(refAQ31.nbSamples(), DspOpTests::OUTA_Q31_ID, mgr);
            break;

        SETUP_TERNARY_Q31(TEST_MULTACC_32X32_KEEP32_R_35,
                          REF_MULTACC_32X32_KEEP32_R_ID);
        SETUP_TERNARY_Q31(TEST_MULTSUB_32X32_KEEP32_R_36,
                          REF_MULTSUB_32X32_KEEP32_R_ID);
        SETUP_TERNARY_Q31(TEST_MULT_32X32_KEEP32_R_37,
                          REF_MULT_32X32_KEEP32_R_ID);
        SETUP_TERNARY_Q31(TEST_MULTACC_32X32_KEEP32_38,
                          REF_MULTACC_32X32_KEEP32_ID);
        SETUP_TERNARY_Q31(TEST_MULTSUB_32X32_KEEP32_39,
                          REF_MULTSUB_32X32_KEEP32_ID);
        SETUP_TERNARY_Q31(TEST_MULT_32X32_KEEP32_40,
                          REF_MULT_32X32_KEEP32_ID);

        default:
            break;
    }
}

#undef SETUP_UNARY_Q31
#undef SETUP_BINARY_Q31
#undef SETUP_TERNARY_Q31

void DspOpTests::tearDown(Testing::testID_t id, Client::PatternMgr *mgr)
{
    (void)id;
    (void)mgr;
}
