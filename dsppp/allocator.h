#pragma once

#include <map>
#include <utility>
#include <string>
#include <iostream>
#include "test_config.h"


// Allocator for temporaries
// But when in test mode (like in github action), malloc allocator is used instead
#if !defined(TESTMODE)
#if defined(POOL_ALLOCATOR)
#define TMP_ALLOC pool_allocator
#else 
#define TMP_ALLOC stat_allocator
#endif
#endif

#include <dsppp/memory_pool.hpp>


using namespace arm_cmsis_dsp;


constexpr int NBVEC_2 = 2;
constexpr int NBVEC_3 = 3;
constexpr int NBVEC_4 = 4;
constexpr int NBVEC_8 = 8;
constexpr int NBVEC_9 = 9;
constexpr int NBVEC_16 = 16;
constexpr int NBVEC_32 = 32;
constexpr int NBVEC_44 = 44;
constexpr int NBVEC_47 = 47;
constexpr int NBVEC_64 = 64;
constexpr int NBVEC_128 = 128;
constexpr int NBVEC_256 = 256;
constexpr int NBVEC_258 = 258;
constexpr int NBVEC_512 = 512;
constexpr int NBVEC_1024 = 1024;
constexpr int NBVEC_2048 = 2048;


template<int L>
struct pool_allocator;

#define POOL(BYTES)                                                                 \
constexpr int POOL_BLOCK_##BYTES = BYTES;                                            \
extern  MemoryPool<POOL_BLOCK_##BYTES,user_allocator_aligned_malloc>  vecPool_##BYTES;\
template<>                                                                          \
struct pool_allocator<BYTES> {                                                      \
    static  char* allocate  () noexcept{                                            \
        return(vecPool_##BYTES.get_new_buffer());                                    \
    }                                                                               \
                                                                                    \
    static void destroy  ( char* ptr ) noexcept {                                   \
        vecPool_##BYTES.recycle_buffer(ptr);                                         \
    }                                                                               \
                                                                                    \
};


#if !defined(TESTMODE)
#if defined(POOL_ALLOCATOR)
#include "allocation/all.h"
#endif 

template<>
struct pool_allocator<DYNAMIC> {
    /* Dynamic size allocations */
    static  char* allocate  ( std::size_t sz) noexcept{
        return(reinterpret_cast<char*>(std::malloc(sz)));
    }

    static void destroy  ( char* ptr ) noexcept {
        std::free(ptr);
    }
   
};

extern std::map<int, int> current_stats;
extern std::map<int, int> max_stats;
extern std::map<void*, std::size_t> current_dyn_stats;


template<int L>
struct stat_allocator {

    /* Dynamic allocations */
    static  char* allocate  ( std::size_t sz) noexcept{
        current_stats[-sz]++;
        if (current_stats[-sz]>max_stats[-sz])
        {
            max_stats[-sz] = current_stats[-sz];
        }
        void *ptr = std::malloc(sz);
        current_dyn_stats[ptr]=sz;
        return(reinterpret_cast<char*>(ptr));
    }

    /* Size known at build time */
    static  char* allocate  () noexcept{
        current_stats[L]++;
        if (current_stats[L]>max_stats[L])
        {
            max_stats[L] = current_stats[L];
        }
        return(reinterpret_cast<char*>(std::malloc(L)));
    }
    
    static void destroy  ( char* ptr ) noexcept {
        if (L<0)
        {
           std::size_t sz = current_dyn_stats[ptr];
           current_stats[-sz]--;
        }
        else
        {
           current_stats[L]--;
        }
        std::free(ptr);
    }
   
};

extern void check_current_stats();
extern void reset_current_stats();

#endif

extern void print_map(std::string comment);

