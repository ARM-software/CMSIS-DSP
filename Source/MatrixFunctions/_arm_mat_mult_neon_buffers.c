#include "dsp/matrix_functions.h"
#include "arm_neon_private.h"

// Max size is a complex scalar so 8 bytes
#define MAXDT_PACKEDA 8
// Max size is a complex scalar so 8 bytes
#define MAXDT_PACKEDB 8
// Max size is 2*int64 for complex q15 acc or a complex so two float
#define MAXDT_PACKEDC 16


__ALIGNED(16) char ARM_CACHE_PACKEDB[MAXDT_PACKEDA*INNER_BLOCK*MAXDC] SECTION_NOINIT;
__ALIGNED(16) char ARM_CACHE_PACKEDA[MAXDT_PACKEDB*ROWS_BLOCK*INNER_BLOCK] SECTION_NOINIT;
// PACKEDC contains the accumulators
__ALIGNED(16) char ARM_CACHE_PACKEDC[MAXDT_PACKEDC*ROWS_BLOCK*COLS_BLOCK] SECTION_NOINIT;
