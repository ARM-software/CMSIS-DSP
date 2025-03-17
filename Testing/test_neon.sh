echo "Transform Tests"
python ../processTests.py -p ../Patterns -d ../Parameters -gen .. -e -f ../Output_neon.pickle TransformTests
ninja 
./test > result_transform_neon.txt
python ../processResult.py --noerr -e -f ../Output_neon.pickle -r result_transform_neon.txt -html > result_transform_neon.html 

echo "Binary F32 Tests"
python ../processTests.py -p ../Patterns -d ../Parameters -gen .. -e -f ../Output_neon.pickle BinaryTestsNeonF32
ninja 
./test > result_binaryf32_neon.txt
python ../processResult.py --noerr -e -f ../Output_neon.pickle -r result_binaryf32_neon.txt -html > result_binaryf32_neon.html 

echo "Binary F16 Tests"
python ../processTests.py -p ../Patterns -d ../Parameters -gen .. -e -f ../Output_neon.pickle BinaryTestsNeonF16
ninja 
./test > result_binaryf16_neon.txt
python ../processResult.py --noerr -e -f ../Output_neon.pickle -r result_binaryf16_neon.txt -html > result_binaryf16_neon.html 

echo "Binary Q31 Tests"
python ../processTests.py -p ../Patterns -d ../Parameters -gen .. -e -f ../Output_neon.pickle BinaryTestsNeonQ31
ninja 
./test > result_binaryq31_neon.txt
python ../processResult.py --noerr -e -f ../Output_neon.pickle -r result_binaryq31_neon.txt -html > result_binaryq31_neon.html 

echo "Binary Q15 Tests"
python ../processTests.py -p ../Patterns -d ../Parameters -gen .. -e -f ../Output_neon.pickle BinaryTestsNeonQ15
ninja 
./test > result_binaryq15_neon.txt
python ../processResult.py --noerr -e -f ../Output_neon.pickle -r result_binaryq15_neon.txt -html > result_binaryq15_neon.html 

echo "Binary Q7 Tests"
python ../processTests.py -p ../Patterns -d ../Parameters -gen .. -e -f ../Output_neon.pickle BinaryTestsNeonQ7
ninja 
./test > result_binaryq7_neon.txt
python ../processResult.py --noerr -e -f ../Output_neon.pickle -r result_binaryq7_neon.txt -html > result_binaryq7_neon.html 
