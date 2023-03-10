#include <cstdio>
#include <cstdint>
#if defined(COMMAND_LINE)
#include <cstdlib>
#endif

#include <cassert>

#include "RTE_Components.h"
#include  CMSIS_device_header

#include "arm_math_types.h"
#include "GenericNodes.h"

#define BUFSIZE 100
#define FIFOSIZE0 100

float32_t buf1[BUFSIZE]={0};

template<typename T, int length>
class FIFOPublicSync : public FIFO<T,length,0,0>
{
public:
    FIFOPublicSync(T *buffer,int delay=0):FIFO<T,length,0,0>(buffer,delay){};

    int getReadPos() const{return(this->readPos);};
    int getWritePos() const{return(this->writePos);};
};

template<typename T, int length>
class FIFOPublicAsync : public FIFO<T,length,0,1>
{
public:
    FIFOPublicAsync(T *buffer,int delay=0):FIFO<T,length,0,1>(buffer,delay){};

    int getReadPos() const{return(this->readPos);};
    int getWritePos() const{return(this->writePos);};
};

int main(void)
{
  float32_t *in,*out;
  // System Initialization
  SystemCoreClockUpdate();

  (void)in;
  (void)out;

  FIFOPublicSync<float32_t,FIFOSIZE0> fifo_sync(buf1);

  printf("SYNC\r\n");
  memset(buf1,0,sizeof(float32_t)*BUFSIZE);

  printf("r=%d, w=%d\n\r",fifo_sync.getReadPos(),fifo_sync.getWritePos());
  assert(fifo_sync.getWritePos() + 50 <= FIFOSIZE0);
  out = fifo_sync.getWriteBuffer(50);
  
  printf("r=%d, w=%d\n\r",fifo_sync.getReadPos(),fifo_sync.getWritePos());
  assert(fifo_sync.getWritePos() + 50 <= FIFOSIZE0);
  out = fifo_sync.getWriteBuffer(30);
  
  printf("r=%d, w=%d\n\r",fifo_sync.getReadPos(),fifo_sync.getWritePos());
  in = fifo_sync.getReadBuffer(20);
  
  printf("r=%d, w=%d\n\r",fifo_sync.getReadPos(),fifo_sync.getWritePos());
  in = fifo_sync.getReadBuffer(20);
  
  printf("r=%d, w=%d\n\r",fifo_sync.getReadPos(),fifo_sync.getWritePos());
  assert(fifo_sync.getWritePos() + 50 > FIFOSIZE0);
  out = fifo_sync.getWriteBuffer(30);
  assert(fifo_sync.getWritePos() <= FIFOSIZE0);

  printf("r=%d, w=%d\n\r",fifo_sync.getReadPos(),fifo_sync.getWritePos());

  FIFOPublicAsync<float32_t,FIFOSIZE0> fifo_async(buf1);

  printf("\r\nASYNC\r\n");
  memset(buf1,0,sizeof(float32_t)*BUFSIZE);
  printf("r=%d, w=%d\n\r",fifo_async.getReadPos(),fifo_async.getWritePos());
  assert(fifo_async.getWritePos() + 50 <= FIFOSIZE0);
  out = fifo_async.getWriteBuffer(50);
  
  printf("r=%d, w=%d\n\r",fifo_async.getReadPos(),fifo_async.getWritePos());
  assert(fifo_async.getWritePos() + 50 <= FIFOSIZE0);
  out = fifo_async.getWriteBuffer(30);
  
  printf("r=%d, w=%d\n\r",fifo_async.getReadPos(),fifo_async.getWritePos());
  in = fifo_async.getReadBuffer(20);
  
  printf("r=%d, w=%d\n\r",fifo_async.getReadPos(),fifo_async.getWritePos());
  in = fifo_async.getReadBuffer(20);
  
  printf("r=%d, w=%d\n\r",fifo_async.getReadPos(),fifo_async.getWritePos());
  assert(fifo_async.getWritePos() + 50 > FIFOSIZE0);
  out = fifo_async.getWriteBuffer(30);
  assert(fifo_async.getWritePos() <= FIFOSIZE0);

  printf("r=%d, w=%d\n\r",fifo_async.getReadPos(),fifo_async.getWritePos());


  
  
}
