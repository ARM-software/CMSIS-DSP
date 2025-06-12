gen_test() {

echo $1
python ../processTests.py -p ../Patterns -d ../Parameters -gen .. -e -f ../Output_neon.pickle $2
ninja 
./test > $3.txt
python ../processResult.py --noerr -e -f ../Output_neon.pickle -r $3.txt -html > $3.html 

}

gen_test "Transform Tests" TransformTests result_transform_neon

gen_test "Binary F32 Tests" BinaryTestsNeonF32 result_binaryf32_neon
gen_test "Binary Complex F32 Tests" BinaryComplexTestsNeonF32 result_binary_complexf32_neon

gen_test "Binary F16 Tests" BinaryTestsNeonF16 result_binaryf16_neon
gen_test "Binary Complex F16 Tests" BinaryComplexTestsNeonF16 result_binary_complexf16_neon

gen_test "Binary Q31 Tests" BinaryTestsNeonQ31 result_binaryq31_neon
gen_test "Binary Complex Q31 Tests" BinaryComplexTestsNeonQ31 result_binary_complexq31_neon

gen_test "Binary Q15 Tests" BinaryTestsNeonQ15 result_binaryq15_neon
gen_test "Binary Complex Q15 Tests" BinaryComplexTestsNeonQ15 result_binary_complexq15_neon

gen_test "Binary Q7 Tests" BinaryTestsNeonQ7 result_binaryq7_neon


