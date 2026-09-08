#include "allocators.hpp"



using namespace arm_cmsis_dsp;


MemoryPool<256,user_allocator_aligned_malloc>  vecPool_256(4);

void *arena;
std::size_t arena_size;
std::size_t arena_offset;

#if defined(STAT_ALLOCATOR)
// Size -> Number of buffer allocated with this size
// Negative size means dynamic sized buffer. Positive size means static sized buffer.
// At the end of the computation, this should be zero for all sizes because all buffers
// should have been freed.
std::map<int, int> current_stats;
// Maximum number of buffer allocated with this size. THis value can be used
// to size the memory pool when the pool allocator is used
std::map<int, int> max_stats;
// ptr -> size
// Use for dynamically sized buffers to recover the size from the pointer.
std::map<void*, std::size_t> current_dyn_stats;

void print_map(std::string comment)
{

    std::cout << comment << "\r\n";

    std::size_t total_static=0;
    std::size_t total_dynamic=0;

    for (const auto v : max_stats)
    {
        // Only count allocations with size known at build time
        if (v.first > 0)
        {
           std::cout << "Define memory pool of dimension " << v.first << " with " << v.second << " buffers.\r\n";
           total_static += v.first * v.second;
        }
    }
 
 
    std::cout << "\r\n";

    std::cout << "Total static bytes: " << total_static << std::hex << " (0x" << total_static << ") << std::dec <<\r\n";

    total_dynamic = 0;
    std::cout << "\r\nDynamic allocations\r\n";
    for (const auto v : max_stats)
    {
        // Only count allocations with size known at build time
        if (v.first < 0)
        {
            std::cout << std::dec << -v.first << " : " << v.second << "\r\n";
            total_dynamic += (-v.first) * v.second;
        }
    }
    std::cout << "Total dynamic bytes: " << total_dynamic << std::hex << " (0x" << total_dynamic << ") << std::dec << \r\n";
    std::cout << "Total static + dynamic bytes: " << (total_static+total_dynamic) << std::hex << " (0x" << (total_static+total_dynamic) << ")"  << std::dec << "\r\n";


}


void reset_current_stats()
{
    for (auto v : current_stats)
    {
        v.second = 0;
    }
}

void check_current_stats()
{
    for (const auto v : current_stats)
    {
        if (v.second > 0)
        {
            if (v.first>0)
            {
                std::cout << "Error memory pool " << v.first << " not empty = " << v.second << "\r\n";
            }
            else 
            {
                std::cout << "Error dynamic alloc " << -v.first << " not empty = " << v.second << "\r\n";
            }
        }
    }

    reset_current_stats();

}
#endif
