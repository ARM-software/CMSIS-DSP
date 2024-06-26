#include "allocator.h"

#if !defined(TESTMODE)

#define ALLOC_POOL(BYTES,NB) \
MemoryPool<POOL_BLOCK_##BYTES,user_allocator_aligned_malloc>  vecPool_##BYTES(NB);

#if defined(POOL_ALLOCATOR)
#include "allocation/all.cpp"
#endif

std::map<int, int> current_stats;
std::map<int, int> max_stats;
std::map<void*, std::size_t> current_dyn_stats;

#endif

void print_map(std::string comment)
{

    std::cout << comment << "\r\n";
#if !defined(POOL_ALLOCATOR) && !defined(TESTMODE)
    std::size_t total_static=0;
    std::size_t total_dynamic=0;

    for (const auto v : max_stats)
    {
        // Only count allocations with size known at build time
        if (v.first > 0)
        {
           std::cout << "ALLOC_POOL(" << v.first << "," << v.second << "); \r\n";
           total_static += v.first * v.second;
        }
    }
 
    for (const auto v : max_stats)
    {
        // Only count allocations with size known at build time
        if (v.first > 0)
        {
           std::cout << "POOL(" << v.first << "); \r\n";
        }
    }
 
    std::cout << "\r\n";

    std::cout << "Total static bytes: " << total_static << std::hex << " (0x" << total_static << ")\r\n";

    total_dynamic = 0;
    std::cout << "\r\nDynamic allocations\r\n";
    for (const auto v : max_stats)
    {
        // Only count allocations with size known at build time
        if (v.first < 0)
        {
            // Count is meaningless for dynamic allocation
            // since we can track the destroy (destroy has no length
            // argument contrary to allocate and so can only get
            // the length from the static value).
            std::cout << std::dec << -v.first << " : " << v.second << "\r\n";
            total_dynamic += (-v.first) * v.second;
        }
    }
    std::cout << "Total dynamic bytes: " << total_dynamic << std::hex << " (0x" << total_dynamic << ")\r\n";
    std::cout << "Total bytes: " << (total_static+total_dynamic) << std::hex << " (0x" << (total_static+total_dynamic) << ")\r\n";


#endif
}

#if !defined(TESTMODE)

void reset_current_stats()
{
#if !defined(POOL_ALLOCATOR)
    for (auto v : current_stats)
    {
        v.second = 0;
    }
#endif
}

void check_current_stats()
{
#if !defined(POOL_ALLOCATOR)
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
#endif
}

#endif
