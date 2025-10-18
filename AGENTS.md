# CMSIS-DSP AGENTS.md file

## Dev environment tips

- To build CMSIS-DSP in a cmake project use `add_subdirectory` and use the CMSIS-DSP folder 
- To build for the host PC, set cmake option `HOST` to `ON` for CMSIS-DSP sub-project
- To enable Neon in the build, set the cmake option `NEON` to `ON` for CMSIS-DSP sub-project
- To disable float16 from the build, set the cmake option `DISABLEFLOAT16` to `ON` for CMSIS-DSP sub-project
- For speed optimizations, build CMSIS-DSP with `-O3 -ffast-math`
- Avoid `-fno-builtin` and `-ffreestanding` when the build uses DSP extensions and Q7 or Q15 datatypes are used. 
- `-fno-builtin` prevents the compiler from replacing `memcpy` of small lengths (4) by an inlined code sequence and so it degrades the performances

## Testing instructions for C code
- Change working directory to `Testing` folder
- Create the missing folders `FullBenchmark`, `Output`, `GeneratedInclude`, `GeneratedSource`, `GeneratedIncludeBench`, `GeneratedSourceBench`
- Install required python packages `pip install -r requirements.txt`
- Preprocess the test descriptions files : for each <test_ext.txt> the result go to a pickle file <Output_ext.pickle>
- Test descriptions are processed with `python preprocess.py -f <test_ext.txt> -o <Output_ext.pickle>`
- The test descriptions are `desc.txt`, `desc_f16.txt`, `desc_neon.txt`
- For each of the generated pickle file, generate missing C files with : `python processTests.py -gen . -p Patterns -d Parameters -f <pickle file> -e`
- Switch to `cmsis_build` working directory
- Run python script `python runall.py`
