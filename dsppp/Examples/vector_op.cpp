
#include "RTE_Components.h"
#include  CMSIS_device_header

#if defined(MPS3)
#include "cmsis_driver_config.h"
#include "stdout_USART.h"
#endif 

#include <iostream>

#include <dsppp/memory_pool.hpp>
#include <dsppp/fixed_point.hpp>
#include <dsppp/matrix.hpp>

using namespace arm_cmsis_dsp;


int main(void)
{
#if defined(MPS3)
    stdout_init();
#endif

    std::cout << "Vector operation examples\r\n";

    constexpr int NB = 32;

    // float32 example

    Vector<float32_t,NB> a;
    Vector<float32_t,NB> b;
    Vector<float32_t,NB> c;

    for(int i = 0;i< NB;i++)
    {
        a[i] = b[i] = c[i] = i;
    }
    

    Vector<float32_t,NB> d = a + b * c;


    std::cout << "Result = " << d ;

    // Vector view example 1
    auto subD = d.sub(2);
    subD = subD + 2.0f;

    // d vector has been modified starting from the 3rd element 
    // (index 2)
    std::cout << "Result = " << d ;

    // Now we set all odd elements to 0.
    d.sub<2>(1) = 0.0f;
    std::cout << "Result = " << d ;


    // Q15 example
    Vector<Q15,NB> aQ15;
    Vector<Q15,NB> bQ15;
    Vector<Q15,NB> cQ15;

    for(int i = 0;i< NB;i++)
    {
        aQ15[i] = bQ15[i] = cQ15[i] = Q15(i);
    }
    

    Vector<Q15,NB> dQ15 = aQ15 + bQ15 * cQ15;


    std::cout << "Result = " << dQ15 ;


 

#if defined(MPS3)
    while(1);
#endif
}


