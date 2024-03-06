#include "allocator.h"
#include <dsppp/arch.hpp>
#include <dsppp/fixed_point.hpp>
#include <dsppp/matrix.hpp>

#include "cmsis_tests.h"

extern "C" {
   void memory_pool_stats();
}

#if 0
template<>
void init_array(Vector_Base<float16_t> &pDst,std::size_t nb)
{
   for(std::size_t i=0;i<nb;i++)
   {
      pDst[i] = 0.1*i;
   }
}
#endif

template<>
bool validate(const float32_t* a, 
              const float32_t* b, 
              std::size_t nb,
              float abser, 
              float reler)
{
   for(std::size_t i=0;i<nb;i++)
   {
      if (ERROR(a[i],b[i],abser,reler) )
      {
         std::cout << "Error at :" << i << " ; res=" << a[i] << " ; ref=" << b[i] << "\r\n";
         ERRVAL(a[i],b[i],abser,reler);
         return(false);
      }
   }
   return(true);
}




void memory_pool_stats()
{
    print_map("Stats");
}
