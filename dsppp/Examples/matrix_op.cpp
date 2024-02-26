
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

    std::cout << "Matrix operation examples\r\n";

    constexpr int ROWS = 8;
    constexpr int COLS = 8;

    Matrix<float32_t,ROWS,COLS> a;
    Matrix<float32_t,ROWS,COLS> b;

    for(std::size_t i=0;i<ROWS*COLS;i++)
    {
       a[i] = float32_t(i);
    }
    
    for(std::size_t row=0; row<ROWS; row++)
    {
       for(std::size_t col=0; col<COLS; col++)
       {
            b(row,col) = float32_t(row*col);
       }
    }
    
    Matrix<float32_t,ROWS,COLS> result = a * a + b;

    std::cout << "Result = " << std::endl << result ;

    // Vector views

    // Rows
    result.row(1) = 0.0f;
    std::cout << "Result = " << std::endl << result ;

    // Row with stride
    // setting odd elements of 3rd row to 0
    result.row<2>(2,1) = 0.0f;
    std::cout << "Result = " << std::endl << result ;

    // Column with stride
    result.col<2>(2,1) = 5.0f;
    std::cout << "Result = " << std::endl << result ;

    // Matrix view
    result.sub(4,8,4,8) = result.sub(4,8,4,8) + result.sub(4,8,4,8);
    std::cout << "Result = " << std::endl << result ;

    // operators
    // dot
    result = dot(a,b);
    std::cout << "Result = " << std::endl << result ;

    // diagonal
    Vector<float32_t,ROWS> c;

    for(int i = 0;i< ROWS;i++)
    {
        c[i] = i;
    }
    result = Matrix<float32_t,ROWS,COLS>::diagonal(c);
    
    std::cout << "Result = " << std::endl << result ;

    // identity matrix
    result = Matrix<float32_t,ROWS,COLS>::identity();
    
    std::cout << "Result = " << std::endl << result ;

    // transpose matrix
    result = a.transpose();
    
    std::cout << "Result = " << std::endl << result ;

    transposeTo(result,a);
    
    std::cout << "Result = " << std::endl << result ;

    // outer product
    result = outer(c,c);
    std::cout << "Result = " << std::endl << result ;
      
    
#if defined(MPS3)
    while(1);
#endif
}


