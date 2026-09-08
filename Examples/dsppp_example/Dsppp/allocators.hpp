#pragma once

#include <map>
#include <utility>

#include <dsppp/memory_pool.hpp>


// Memory allocator using memory pools
template<int L>
struct pool_allocator;

// Memory pool for vectors of 256 byte length
// Use by the pool_allocator<256> specialization
extern  arm_cmsis_dsp::MemoryPool<256,arm_cmsis_dsp::user_allocator_aligned_malloc>  vecPool_256;

// pool_allocator used by vector, matrix of size 256 bytes (whatever the actual type)
// The allocator are stateless templates, meaning they do not hold any state themselves.
// hence the define a different one for each pool size.
// If many sizes are used in the algorithm, then it is better to use dynamically sized
// matrixes and vectors.
template<>                                                                          
struct pool_allocator<256> {                                                      
    static  char* allocate  () noexcept{                                            
        return(vecPool_256.get_new_buffer());                                    
    }                                                                               
                                                                                    
    static void destroy  ( char* ptr ) noexcept {                                   
        vecPool_256.recycle_buffer(ptr);                                         
    }                                                                               
                                                                                    
};

#ifndef ALIGN
#define ALIGN 64
#endif

static_assert(ALIGN > 0, "ALIGN must be positive");

extern void *arena;
extern std::size_t arena_size;
extern std::size_t arena_offset;

__STATIC_INLINE void create_arena(void *ptr, std::size_t size)
{
    arena = ptr;
    arena_size = size;
    arena_offset = 0;
}


__STATIC_INLINE void reset_arena()
{
    arena_offset = 0;
}

__STATIC_INLINE void* allocate_from_arena(std::size_t size)
{
    const std::size_t padding = (ALIGN - arena_offset % ALIGN) % ALIGN;
    const std::size_t remaining = arena_size - arena_offset;
    if (padding <= remaining && size <= remaining - padding)
    {
        const std::size_t aligned_offset = arena_offset + padding;
        void* ptr = static_cast<char*>(arena) + aligned_offset;
        arena_offset = aligned_offset + size;
        return ptr;
    }
    return nullptr;
}

__STATIC_INLINE std::size_t get_arena_size()
{
    return arena_size;
}

__STATIC_INLINE std::size_t get_arena_offset()
{
    return arena_offset;
}


template<int L>
struct arena_allocator
{
    static char* allocate  () noexcept{
        return(static_cast<char*>(allocate_from_arena(L)));
    }

    static void destroy  ( char* ptr ) noexcept {
        // No-op for arena allocator, memory will be reclaimed when the arena is reset
    }

    static char* allocate  ( std::size_t sz) noexcept
    {
        return(static_cast<char*>(allocate_from_arena(sz)));
    }
};




#if defined(STAT_ALLOCATOR)

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



extern void print_map(std::string comment);

#endif