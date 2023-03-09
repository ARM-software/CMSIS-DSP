cmake_minimum_required (VERSION 3.21)
include(CMakePrintHelpers)

set(Python_FIND_REGISTRY "LAST")

find_package (Python COMPONENTS Interpreter)

function(sdf TARGET SCRIPT DOTNAME)
    if (DOT)
    add_custom_command(TARGET ${TARGET} PRE_BUILD 
        BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/${DOTNAME}.pdf
        COMMAND ${DOT} -Tpdf -o ${CMAKE_CURRENT_SOURCE_DIR}/${DOTNAME}.pdf ${CMAKE_CURRENT_SOURCE_DIR}/${DOTNAME}.dot
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${DOTNAME}.dot
        VERBATIM
        )
    endif()

    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/generated/scheduler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/${DOTNAME}.dot
        COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/${SCRIPT}
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/${SCRIPT}
        VERBATIM
        )
    target_sources(${TARGET} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/generated/scheduler.cpp)
endfunction()

function(sdfpython TARGET)
    if (DOT)
    add_custom_command(TARGET ${TARGET} PRE_BUILD 
        BYPRODUCTS ${CMAKE_CURRENT_SOURCE_DIR}/test.pdf
        COMMAND ${DOT} -Tpdf -o ${CMAKE_CURRENT_SOURCE_DIR}/test.pdf ${CMAKE_CURRENT_SOURCE_DIR}/test.dot
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/test.dot
        VERBATIM
        )
    endif()

    add_custom_command(OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/sched.py
        ${CMAKE_CURRENT_SOURCE_DIR}/test.dot
        COMMAND ${Python_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/graph.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
        DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/graph.py
        VERBATIM
        )
endfunction()

set(CGDIR ${CMAKE_CURRENT_SOURCE_DIR}/../cg)

set(DSP ${CMAKE_CURRENT_SOURCE_DIR}/../..)

function(add_sdf_dir TARGET)
    target_include_directories(${TARGET} PRIVATE ${CGDIR}/src)
    target_include_directories(${TARGET} PRIVATE ${CGDIR}/nodes/cpp)
    target_include_directories(${TARGET} PRIVATE ${CMSISCORE})
    target_include_directories(${TARGET} PRIVATE ${DSP}/Include)
endfunction()

project(Examples)

# Add DSP folder to module path
list(APPEND CMAKE_MODULE_PATH ${DSP})

add_subdirectory(${DSP}/Source bin_dsp)


add_subdirectory(example1 bin_example1)
add_subdirectory(example2 bin_example2)
add_subdirectory(example3 bin_example3)
add_subdirectory(example6 bin_example6)
add_subdirectory(example8 bin_example8)
add_subdirectory(example9 bin_example9)
add_subdirectory(example10 bin_example10)
add_subdirectory(simple bin_simple)
add_subdirectory(simpledsp bin_simpledsp)
add_subdirectory(cyclo bin_cyclo)

# Python examples
add_subdirectory(example4 bin_example4)
add_subdirectory(example5 bin_example5)
add_subdirectory(example7 bin_example7)
