
#include "RTE_Components.h"
#include  CMSIS_device_header

#if defined(MPS3)
#include "cmsis_driver_config.h"
#include "stdout_USART.h"
#endif 

#include <iostream>

#include <dsppp/memory_pool.hpp>
#include <dsppp/matrix.hpp>

using namespace arm_cmsis_dsp;


int main(void)
{
#if defined(MPS3)
    stdout_init();
#endif

    std::cout << "Dot product example\r\n";

    constexpr int NB = 32;

    Vector<float32_t,NB> a;
    Vector<float32_t,NB> b;
    Vector<float32_t,NB> c;
    Vector<float32_t,NB> d;

    float32_t scale = 0.5;

    for(int i = 0;i< NB;i++)
    {
        a[i] = b[i] = c[i] = d[i] = i;
    }

    float32_t r;

    r = dot(scale*(a+b),c*d);

    std::cout << "Result = " << r << "\r\n";
 

#if defined(MPS3)
    while(1);
#endif
}


