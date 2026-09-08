/**
 * @addtogroup groupExamples
 * @{
 *
 * @defgroup DspppExample DSP++ Vector and Memory Allocator Example
 *
 * \par Description:
 * \par
 * Demonstrates fused vector expressions and custom memory allocators using the
 * CMSIS-DSP C++ template library (DSP++). Two tests evaluate the same expression
 * with statically and dynamically sized vectors of 64 floating-point elements.
 *
 * \par Algorithm:
 * \par
 * The input vectors are initialized with <code>a[i] = b[i] = c[i] = i</code>.
 * The expression <code>d = a + b * c</code> computes
 * <code>d[i] = i + i * i</code> for indices 0 through 63.
 * Addition and multiplication are fused into a single vectorized loop, avoiding
 * an intermediate array for the product. On Helium, the loop uses predication.
 * Both tests print the resulting vector.
 *
 * \par Memory Allocators:
 * \par
 * - \c test1 uses statically sized vectors and the allocator selected by
 *   \c TMP_ALLOC in \c Dsppp/Dsppp.cproject.yml. The default \c pool_allocator
 *   supplies four 256-byte buffers, one for each vector of 64 floats.
 * - \c test2 uses dynamically sized vectors with an explicit \c arena_allocator.
 *   A 1024-byte arena is allocated with 64-byte alignment. Each allocation rounds
 *   the arena offset up to a multiple of \c ALIGN (64 by default), assuming the
 *   arena base is already aligned. Individual deallocations do nothing; resetting
 *   the arena reclaims all its allocations. The test prints the consumed bytes
 *   before resetting and freeing the arena.
 * - Temporary vectors still use \c TMP_ALLOC. When their allocator differs from
 *   the destination allocator, their data is copied instead of moved.
 *
 * \par Allocation Statistics:
 * \par
 * To determine memory pool sizes, change \c TMP_ALLOC to \c stat_allocator and
 * enable \c STAT_ALLOCATOR in \c Dsppp/Dsppp.cproject.yml. After \c test1, the
 * example prints peak allocation counts and checks for outstanding allocations.
 *
 * \par Variables Description:
 * \par
 * - \c a, \c b, \c c are the input vectors.
 * - \c d is the result vector.
 * - \c NB is the number of elements in each vector (64).
 * - \c ARENA_SIZE is the arena capacity in bytes (1024).
 *
 * \par CMSIS-DSP C++ Library Features Used:
 * \par
 * - Vector templates with static and dynamic dimensions.
 * - Fused element-wise addition and multiplication.
 * - MemoryPool and custom allocator templates.
 *
 * See also \ref dsppp_memory_allocator for the custom allocator interface.
 *
 * <b> Refer </b>
 * \link dsppp_example/Dsppp/main.cpp \endlink
 *
 * \example dsppp_example/Dsppp/main.cpp
 *
 * @} */

#include "RTE_Components.h"
#include CMSIS_device_header

#include <cstdio>
#include <cstdlib>

#include "allocators.hpp"

#include <dsppp/fixed_point.hpp>
#include <dsppp/matrix.hpp>
#include <dsppp/memory_pool.hpp>

#include "init_data.hpp"


using namespace arm_cmsis_dsp;


void test1() {
  constexpr int NB = 64;

  // By default, the Vector class template uses the macro TMP_ALLOC to select
  // the memory allocator. The macro is defined in the cproject file and uses
  // the pool_allocator stateless template. It can be changed to the
  // stat_allocator to get the sizes that needs to be used for the memory pools.
  Vector<float32_t, NB> a;
  Vector<float32_t, NB> b;
  Vector<float32_t, NB> c;

  init_vectors(NB,a.ptr(),b.ptr(),c.ptr());

  // Fusion loop : The addition and multiplication are fused into a single
  // vectorized loop by the library
  Vector<float32_t, NB> d = a + b * c;

  std::cout << "Result = " << d;
  
}

void test2() {
  constexpr int NB = 64;

  constexpr std::size_t ARENA_SIZE = 1024; // 1MB for the arena
  void *arena_ptr = aligned_malloc(
      ARENA_SIZE, 64); // Allocate 1MB for the arena with 64-byte alignment
  create_arena(arena_ptr, ARENA_SIZE);

  // Use the arena allocator.
  // WARNING : Temporary allocations still uses the TMP_ALLOC allocator.
  // If it is different from the arena_allocator, temporaries will be copied
  // (instead of moved) to the vectors using arena_allocator.
  using ArenaVector = Vector<float32_t, DYNAMIC, arena_allocator>;

  // Dyamically sized vectors using the arena allocator
  ArenaVector a(NB);
  ArenaVector b(NB);
  ArenaVector c(NB);

  init_vectors(NB,a.ptr(),b.ptr(),c.ptr());

  // Fusion loop : The addition and multiplication are fused into a single
  // vectorized loop by the library
  ArenaVector d = a + b * c;

  std::cout << "Result = " << d;

  std::size_t offset = get_arena_offset();
  std::cout << "Arena offset after computation: " << offset << "\r\n";
  reset_arena();

  free(arena_ptr);
}

int main() {

  setvbuf(stdout, NULL, _IONBF, 0);

  printf("CMSIS-DSPP C++ examples\n");

// Stat allocztor can be enabled in the cproject.
// The stat_allocator keeps track of memory usage and can be used to print
// memory allocation statistics. Those statistics can be used to optimize memory
// pool sizes and improve memory usage efficiency.
#if defined(STAT_ALLOCATOR)
  reset_current_stats();
#endif

  std::cout << "Test 1" << std::endl;
  test1();

#if defined(STAT_ALLOCATOR)
  print_map("Memory allocation statistics");
  check_current_stats();
#endif

  std::cout << "Test 2" << std::endl;
  test2();

error:
  exit(0);
}
