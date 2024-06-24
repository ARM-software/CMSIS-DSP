cmake_minimum_required(VERSION 3.23)

include(CMakePrintHelpers)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(Tests)

option(HOST "" ON)
option(NEON "" ON)
option(NEONEXPERIMENTAL "" ON)
#option(DISABLEFLOAT16 "" ON)
add_subdirectory(../Source bin_dsp)


add_executable(test main.cpp
	patterndata.c
    testmain.cpp
    FrameworkSource/ArrayMemory.cpp
    FrameworkSource/Calibrate.cpp
    FrameworkSource/Error.cpp
    FrameworkSource/FPGA.cpp
    FrameworkSource/Generators.cpp
    FrameworkSource/IORunner.cpp
    FrameworkSource/Pattern.cpp
    FrameworkSource/PatternMgr.cpp
    FrameworkSource/Test.cpp
    FrameworkSource/Timing.cpp
    GeneratedSource/TestDesc.cpp
    Source/Tests/BasicTestsF16.cpp
    Source/Tests/BasicTestsF32.cpp
    Source/Tests/BasicTestsF64.cpp
    Source/Tests/BasicTestsQ7.cpp
    Source/Tests/BasicTestsQ15.cpp
    Source/Tests/BasicTestsQ31.cpp
    Source/Tests/BayesF16.cpp
    Source/Tests/BayesF32.cpp
    Source/Tests/BinaryTestsF16.cpp
    Source/Tests/BinaryTestsF32.cpp
    Source/Tests/BinaryTestsF64.cpp
    Source/Tests/BinaryTestsQ7.cpp
    Source/Tests/BinaryTestsQ15.cpp
    Source/Tests/BinaryTestsQ31.cpp
    Source/Tests/BIQUADF16.cpp
    Source/Tests/BIQUADF32.cpp
    Source/Tests/BIQUADF64.cpp
    Source/Tests/BIQUADQ15.cpp
    Source/Tests/BIQUADQ31.cpp
    Source/Tests/ComplexTestsF16.cpp
    Source/Tests/ComplexTestsF32.cpp
    Source/Tests/ComplexTestsF64.cpp
    Source/Tests/ComplexTestsQ15.cpp
    Source/Tests/ComplexTestsQ31.cpp
    Source/Tests/DECIMF64.cpp
    Source/Tests/DECIMF32.cpp
    Source/Tests/DECIMQ15.cpp
    Source/Tests/DECIMQ31.cpp
    Source/Tests/DistanceTestsF16.cpp
    Source/Tests/DistanceTestsF32.cpp
    Source/Tests/DistanceTestsF64.cpp
    Source/Tests/DistanceTestsU32.cpp
    Source/Tests/ExampleCategoryF32.cpp
    Source/Tests/ExampleCategoryQ7.cpp
    Source/Tests/ExampleCategoryQ15.cpp
    Source/Tests/ExampleCategoryQ31.cpp
    Source/Tests/FastMathF16.cpp
    Source/Tests/FastMathF32.cpp
    Source/Tests/FastMathF64.cpp
    Source/Tests/FastMathQ15.cpp
    Source/Tests/FastMathQ31.cpp
    Source/Tests/FastMathQ63.cpp
    Source/Tests/FIRF16.cpp
    Source/Tests/FIRF32.cpp
    Source/Tests/FIRF64.cpp
    Source/Tests/FIRQ7.cpp
    Source/Tests/FIRQ15.cpp
    Source/Tests/FIRQ31.cpp
    Source/Tests/InterpolationTestsF16.cpp
    Source/Tests/InterpolationTestsF32.cpp
    Source/Tests/InterpolationTestsQ7.cpp
    Source/Tests/InterpolationTestsQ15.cpp
    Source/Tests/InterpolationTestsQ31.cpp
    Source/Tests/mfccdata.c
    Source/Tests/mfccdata_f16.c
    Source/Tests/MFCCF16.cpp
    Source/Tests/MFCCF32.cpp
    Source/Tests/MFCCQ15.cpp
    Source/Tests/MFCCQ31.cpp
    Source/Tests/MISCF16.cpp
    Source/Tests/MISCF32.cpp
    Source/Tests/MISCF64.cpp
    Source/Tests/MISCQ7.cpp
    Source/Tests/MISCQ15.cpp
    Source/Tests/MISCQ31.cpp
    Source/Tests/QuaternionTestsF32.cpp
    Source/Tests/StatsTestsF16.cpp
    Source/Tests/StatsTestsF32.cpp
    Source/Tests/StatsTestsF64.cpp
    Source/Tests/StatsTestsQ7.cpp
    Source/Tests/StatsTestsQ15.cpp
    Source/Tests/StatsTestsQ31.cpp
    Source/Tests/SupportBarTestsF16.cpp
    Source/Tests/SupportBarTestsF32.cpp
    Source/Tests/SupportTestsF16.cpp
    Source/Tests/SupportTestsF32.cpp
    Source/Tests/SupportTestsF64.cpp
    Source/Tests/SupportTestsQ7.cpp
    Source/Tests/SupportTestsQ15.cpp
    Source/Tests/SupportTestsQ31.cpp
    Source/Tests/SVMF16.cpp
    Source/Tests/SVMF32.cpp
    Source/Tests/TransformCF16.cpp
    Source/Tests/TransformCF32.cpp
    Source/Tests/TransformCF64.cpp
    Source/Tests/TransformCQ15.cpp
    Source/Tests/TransformCQ31.cpp
    Source/Tests/TransformRF16.cpp
    Source/Tests/TransformRF32.cpp
    Source/Tests/TransformRF64.cpp
    Source/Tests/TransformRQ15.cpp
    Source/Tests/TransformRQ31.cpp
    Source/Tests/UnaryTestsF16.cpp
    Source/Tests/UnaryTestsF32.cpp
    Source/Tests/UnaryTestsF64.cpp
    Source/Tests/UnaryTestsQ7.cpp
    Source/Tests/UnaryTestsQ15.cpp
    Source/Tests/UnaryTestsQ31.cpp
    Source/Tests/WindowTestsF32.cpp
    Source/Tests/WindowTestsF64.cpp
	)

target_include_directories(test PUBLIC FrameworkInclude
  GeneratedInclude
  PrivateInclude
  Include/Tests)

target_compile_definitions(test PUBLIC EMBEDDED NOTIMING)

target_compile_options(test PUBLIC -Wsign-compare 
        -Wdouble-promotion 
        -DNDEBUG 
        -Wall 
        -Wextra  
        -Werror
        -Ofast 
        -ffast-math
        -Wno-packed 
        -Wno-missing-variable-declarations 
        -Wno-missing-prototypes 
        -Wno-missing-noreturn 
        -Wno-sign-conversion 
        -Wno-nonportable-include-path 
        -Wno-reserved-id-macro 
        -Wno-unused-macros 
        -Wno-documentation-unknown-command 
        -Wno-documentation 
        -Wno-parentheses-equality 
        -Wno-reserved-identifier
        -ffunction-sections)

target_link_libraries(test PUBLIC CMSISDSP)