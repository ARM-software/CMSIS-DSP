#ifndef REGIONS_V2M_MPS3_SSE_300_FVP_H
#define REGIONS_V2M_MPS3_SSE_300_FVP_H


//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

// <n>Device pack:   ARM::V2M_MPS3_SSE_300_BSP@1.4.0
// <i>Device pack used to generate this file

// <h>ROM Configuration
// =======================
// </h>

// <h>RAM Configuration
// =======================
// <h> IROM1=<__RAM0>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x10000000
#define __RAM0_BASE 0x10000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00200000
#define __RAM0_SIZE 0x00200000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM0_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM0_NOINIT 0
// </h>

// <h> IROM2=<__RAM1>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x00000000
#define __RAM1_BASE 0x00000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00200000
#define __RAM1_SIZE 0x00200000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM1_DEFAULT 0
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM1_NOINIT 0
// </h>

// <h> IRAM1=<__RAM2>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x30000000
#define __RAM2_BASE 0x30000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00020000
#define __RAM2_SIZE 0x00020000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM2_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM2_NOINIT 0
// </h>

// <h> IRAM2=<__RAM3>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x20000000
#define __RAM3_BASE 0x20000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00020000
#define __RAM3_SIZE 0x00020000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM3_DEFAULT 0
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM3_NOINIT 0
// </h>

// <h> ITCM_NS=<__RAM4>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x00000000
#define __RAM4_BASE 0x00000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00080000
#define __RAM4_SIZE 0x00080000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM4_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM4_NOINIT 0
// </h>

// <h> SRAM_NS=<__RAM5>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x01000000
#define __RAM5_BASE 0x01000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00100000
#define __RAM5_SIZE 0x00100000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM5_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM5_NOINIT 0
// </h>

// <h> DTCM0_NS=<__RAM6>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x20000000
#define __RAM6_BASE 0x20000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00020000
#define __RAM6_SIZE 0x00020000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM6_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM6_NOINIT 0
// </h>

// <h> DTCM1_NS=<__RAM7>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x20020000
#define __RAM7_BASE 0x20020000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00020000
#define __RAM7_SIZE 0x00020000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM7_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM7_NOINIT 0
// </h>

// <h> DTCM2_NS=<__RAM8>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x20040000
#define __RAM8_BASE 0x20040000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00020000
#define __RAM8_SIZE 0x00020000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM8_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM8_NOINIT 0
// </h>

// <h> DTCM3_NS=<__RAM9>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x20060000
#define __RAM9_BASE 0x20060000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00020000
#define __RAM9_SIZE 0x00020000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM9_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM9_NOINIT 0
// </h>

// <h> ISRAM0_NS=<__RAM10>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x21000000
#define __RAM10_BASE 0x21000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00100000
#define __RAM10_SIZE 0x00100000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM10_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM10_NOINIT 0
// </h>

// <h> ISRAM1_NS=<__RAM11>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x21100000
#define __RAM11_BASE 0x21100000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00100000
#define __RAM11_SIZE 0x00100000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM11_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM11_NOINIT 0
// </h>

// <h> QSPI_SRAM_NS=<__RAM12>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x28000000
#define __RAM12_BASE 0x28000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00800000
#define __RAM12_SIZE 0x00800000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM12_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM12_NOINIT 0
// </h>

// <h> ITCM_S=<__RAM13>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x10000000
#define __RAM13_BASE 0x10000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00080000
#define __RAM13_SIZE 0x00080000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM13_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM13_NOINIT 0
// </h>

// <h> SRAM_S=<__RAM14>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x11000000
#define __RAM14_BASE 0x11000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00100000
#define __RAM14_SIZE 0x00100000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM14_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM14_NOINIT 0
// </h>

// <h> DTCM0_S=<__RAM15>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x30000000
#define __RAM15_BASE 0x30000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00020000
#define __RAM15_SIZE 0x00020000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM15_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM15_NOINIT 0
// </h>

// <h> DTCM1_S=<__RAM16>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x30020000
#define __RAM16_BASE 0x30020000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00020000
#define __RAM16_SIZE 0x00020000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM16_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM16_NOINIT 0
// </h>

// <h> DTCM2_S=<__RAM17>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x30040000
#define __RAM17_BASE 0x30040000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00020000
#define __RAM17_SIZE 0x00020000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM17_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM17_NOINIT 0
// </h>

// <h> DTCM3_S=<__RAM18>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x30060000
#define __RAM18_BASE 0x30060000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00020000
#define __RAM18_SIZE 0x00020000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM18_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM18_NOINIT 0
// </h>

// <h> ISRAM0_S=<__RAM19>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x31000000
#define __RAM19_BASE 0x31000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00100000
#define __RAM19_SIZE 0x00100000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM19_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM19_NOINIT 0
// </h>

// <h> ISRAM1_S=<__RAM20>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x31100000
#define __RAM20_BASE 0x31100000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00100000
#define __RAM20_SIZE 0x00100000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM20_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM20_NOINIT 0
// </h>

// <h> QSPI_SRAM_S=<__RAM21>
//   <o> Base address <0x0-0xFFFFFFFF:8>
//   <i> Defines base address of memory region.
//   <i> Default: 0x38000000
#define __RAM21_BASE 0x38000000
//   <o> Region size [bytes] <0x0-0xFFFFFFFF:8>
//   <i> Defines size of memory region.
//   <i> Default: 0x00800000
#define __RAM21_SIZE 0x00800000
//   <q>Default region
//   <i> Enables memory region globally for the application.
#define __RAM21_DEFAULT 1
//   <q>No zero initialize
//   <i> Excludes region from zero initialization.
#define __RAM21_NOINIT 0
// </h>

// </h>

// <h>Stack / Heap Configuration
//   <o0> Stack Size (in Bytes) <0x0-0xFFFFFFFF:8>
//   <o1> Heap Size (in Bytes) <0x0-0xFFFFFFFF:8>
#define __STACK_SIZE 0x00000200
#define __HEAP_SIZE 0x00038000
// </h>


#endif /* REGIONS_V2M_MPS3_SSE_300_FVP_H */
