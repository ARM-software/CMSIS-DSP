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
