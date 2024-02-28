// -*- C++ -*-
/** @file */ 
#pragma once 


#include <cstdio>
#include <cstddef>
#include <vector>
#include "common.hpp"

namespace arm_cmsis_dsp {

/** \addtogroup MEMORY Memory allocator
 *  \ingroup DSPPP
 *  @{
 */

/*

Buffer allocator

Can be used to build memory allocators foe vector
and matrix.

For instance, it is usedin the Memory pool allocator

*/

/** \defgroup MEMBUF Memory buffer allocator
 *  \ingroup MEMORY
 *  Allocators for allocating memory buffers
 */

/** \defgroup MEMVEC Vector / matrix buffer allocator
 *  \ingroup MEMORY
 *  Allocators for allocating vector / matrix buffers
 */

/** \defgroup MEMTOOL Miscellaneous utilities for memory
 *  \ingroup MEMORY
 *  Miscellaneous utilities for implementing memory allocators
 */

/**
 * @ingroup MEMBUF
 * @brief      Malloc memory allocator
 * 
 */
struct default_user_allocator_malloc_free
{
  /**
   * @brief      Allocate a buffer
   *
   * @param[in]  bytes  The bytes
   *
   * @return     A pointer to the allocated buffer
   */
  static char * malloc(const std::size_t bytes)  
  { 
    #if !defined(MEMORY_ALLOCATION_DEBUG)
    return reinterpret_cast<char *>(std::malloc(bytes)); 
    #else
    char *ret=reinterpret_cast<char *>(std::malloc(bytes));
    if (ret==nullptr)
    {
       std::cout << "out of memory for " << bytes << " bytes\r\n";
    }
    return(ret);
    #endif
  } 

  /**
   * @brief      Free a buffer
   *
   * @param      block  The buffer to free
   */
  static void free(char * const block)  
  { 
    #if defined(MEMORY_ALLOCATION_DEBUG)
    if (block==nullptr)
    {
       std::cout << "free null ptr \r\n";
    }
    #endif
    std::free(block); 
  }
};

/**
 * @ingroup MEMBUF
 * @brief      Aligned memory allocation
 *
 * @param[in]  alignment  The alignment of the buffer
 * @param[in]  size       The size of the buffer
 *
 * @return     A pointer to the new buffer
 */
inline void* aligned_malloc(std::size_t alignment, std::size_t size)
{
   void *ptr=std::malloc(size+alignment+sizeof(void*));
   void *aligned = 
   reinterpret_cast<char*>(
    (reinterpret_cast<std::size_t>(ptr)+sizeof(void*)+alignment) & ~(alignment-1)
   );

   *(static_cast<void**>(aligned) - 1) = ptr;
   return(aligned);
}

/**
 * @ingroup MEMBUF
 * @brief      Free an aligned buffer
 *
 * @param      ptr   The pointer
 */
inline void
aligned_free(void* ptr)
{
    if (ptr) {
        std::free(*(static_cast<void**>(ptr) - 1));
    }
};

/**
 * @ingroup MEMBUF
 * @brief      Memory allocation for aligned buffers
 *
 */
struct user_allocator_aligned_malloc
{
  typedef std::size_t size_type;
  typedef std::ptrdiff_t difference_type;

  /**
   * @brief      Allocate a new buffer
   *
   * @param[in]  bytes  The bytes
   *
   * @return     Pointer to the new buffer
   */
  static char * malloc(const size_type bytes)  
  { 
    #if !defined(MEMORY_ALLOCATION_DEBUG)
    return reinterpret_cast<char *>(aligned_malloc(MEMORY_POOL_ALIGNMENT, bytes));
    #else 
    char *ret = reinterpret_cast<char *>(aligned_malloc(MEMORY_POOL_ALIGNMENT, bytes));
    if (ret==nullptr)
    {
        std::cout << "out of memory for " << bytes << " bytes\r\n";
    }
    return(ret);
    #endif
  }

  /**
   * @brief      Free a buffer
   *
   * @param      block  Pointer to the buffer
   */
  static void free(char * const block)  
  { 
    #if defined(MEMORY_ALLOCATION_DEBUG)
    if (block==nullptr)
    {
       std::cout << "free null ptr \r\n";
    }
    #endif
    aligned_free(block); 
  }
};

/*

Memory allocator for vector and matrix.

*/

// Default allocator
// Other allocator must be provided by user of the library

/**
 * @ingroup MEMVEC
 * @brief      Default memory allocator for vectors and matrixes
 *
 * @tparam     L     Size known at build time in bytes
 */
template<int L>
struct malloc_allocator {
    /**
     * @brief      Allocate a buffer with size known at runtime
     *
     * @param[in]  sz    The size
     *
     * @return     Pointer to the buffer
     */
    static  char* allocate  ( vector_length_t sz) noexcept{
        char *res;
        res=reinterpret_cast<char*>(std::malloc(sz));
        #if defined(MEMORY_ALLOCATION_DEBUG)
        if (res==nullptr)
        {
           std::cout << "out of memory for " << sz << " bytes\r\n";
        }
        #endif
        return(res);
    }

    /**
     * @brief      Allocate a buffer with size known at build time
     *
     * @return     Pointer to the buffer
     */
    static  char* allocate  ( ) noexcept{
        char *res;
        res=reinterpret_cast<char*>(std::malloc(L));
        #if defined(MEMORY_ALLOCATION_DEBUG)
        if (res==nullptr)
        {
           std::cout << "out of memory for " << L << " bytes\r\n";
        }
        #endif
        return(res);
    }
    
    /**
     * @brief      Destroys the given pointer.
     *
     * @param      ptr   The pointer
     */
    static void destroy  ( char* ptr ) noexcept {
        #if defined(MEMORY_ALLOCATION_DEBUG)
        if (ptr==nullptr)
        {
           std::cout << "free null ptr \r\n";
        }
        #endif
        std::free(ptr);
    }
   
};


/*

Memory pool

Memory pool  is using a buffer 
allocator (aligned or normal malloc)

A memory pool can be used to by a memory allocator for
vectors and matrixes.


*/

struct ListElem;

/**
 * @ingroup MEMTOOL
 * @brief      Simple list of elements
 *
 */
struct ListElem {
    ListElem *next;
};

/**
 * @ingroup MEMTOOL
 * @brief      This class describes a memory pool that can be used to build
 *             a memory allocator for vectors and matrixes
 *
 * @tparam     BUF_SIZE       Size of a buffer known at build time
 * @tparam     UserAllocator  Memory allocator to allocate the memory buffer
 */
template<int BUF_SIZE,typename UserAllocator = default_user_allocator_malloc_free>
class MemoryPool {
public:
    /**
     * @brief      Create a new memory pool
     *
     * @param[in]  nbBufs  The number of buffers to pre-allocate
     */
    explicit MemoryPool(const uint16_t nbBufs) 
    {
        buffer_list.reserve(nbBufs);
        buffer_list.assign(nbBufs,nullptr);
        for(auto p=buffer_list.begin();p != buffer_list.end(); ++p)
        {
            *p = UserAllocator::malloc(BUF_SIZE < sizeof(ListElem) ? sizeof(ListElem) : BUF_SIZE);            
        }
        reset();
    };

    /**
     * @brief      Destroys the object.
     */
    ~MemoryPool() 
    {
        for(auto p=buffer_list.begin();p != buffer_list.end(); ++p)
        {
            UserAllocator::free(*p);
        }
    }

    MemoryPool(const MemoryPool& other) = delete;

    MemoryPool(MemoryPool&& other) = delete;
    

    MemoryPool& operator=(const MemoryPool& other) = delete;
    
    MemoryPool& operator=(MemoryPool&& other) = delete;

    /**
     * @brief      Gets the new free buffer.
     *
     * @return     The new buffer.
     */
    char* get_new_buffer() noexcept
    {
         /* No error handling.
            The sizing of the pool must have been done, for
            instance, with a statistic allocator.
            Allocation is thus assumed to succeed */
         char* res = reinterpret_cast<char*>(free);
         free = free->next;
         #if defined(MEMORY_ALLOCATION_DEBUG)
         if (res == nullptr)
         {
           std::cout << "memory pool alloc error " << BUF_SIZE << " bytes\r\n";
         }
         #endif
         return(res);
    }

    /**
     * @brief      Release the buffer so that it can be reused
     *
     * @param      buf   The buffer
     */
    void recycle_buffer(char* buf)  noexcept
    {
        ListElem *l = reinterpret_cast<ListElem*>(buf);
        #if defined(MEMORY_ALLOCATION_DEBUG)
        if (l == nullptr)
        {
           std::cout << "memory pool free error " << BUF_SIZE << " bytes\r\n";
        }
        #endif
        l->next = free;
        free = l;
    }

    /**
     * @brief      Release all the buffers so that they can be reused
     */
    void reset()  noexcept
    {
        const int nbBufs = buffer_list.size();
        for(int i=0;i<nbBufs-1;i++)
        {
            ListElem *l=reinterpret_cast<ListElem*>(buffer_list[i]);
            l->next = reinterpret_cast<ListElem*>(buffer_list[i+1]);
        }
        ListElem *l=reinterpret_cast<ListElem*>(buffer_list[nbBufs-1]);
        l->next = nullptr;
        free = reinterpret_cast<ListElem*>(buffer_list[0]);
    }

  

protected:
    ListElem *free;
    std::vector<char*> buffer_list;
};


/*! @} */

}