#ifndef ARM_COMPILER_SPECIFIC_H_
#define ARM_COMPILER_SPECIFIC_H_

/* CMSIS-DSP definitions for some specific compilers.
   Those definitions should not interfere with client code
   and as consequence are in a private header.
*/

#if defined (_MSC_VER ) 
#pragma warning(push)
#pragma warning(disable:4127)
#pragma warning(disable:4244)
#pragma warning(disable:4456)
#pragma warning(disable:4701)
#pragma warning(disable:4703)
#pragma warning(disable:4310)

#endif

#endif