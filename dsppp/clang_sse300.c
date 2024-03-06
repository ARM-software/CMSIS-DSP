#include "RTE_Components.h"
#include <stdio.h>

#include "Driver_USART.h"
#include "stdout_USART.h"




static int stdin_getc(FILE *file) {
  (void)file;
  return(0);
}


// iostream has references to stdin and stderr and there is a link
// error if not defined.
static FILE __stdin = FDEV_SETUP_STREAM(NULL,
                                        stdin_getc,
                                        NULL,
                                        _FDEV_SETUP_READ);
FILE *const stdin = &__stdin;

static int stderr_putc(char c, FILE *file) {
  (void)file;
  return(0);
}

static FILE __stderr = FDEV_SETUP_STREAM(stderr_putc,
                                         NULL,
                                         NULL,
                                         _FDEV_SETUP_WRITE);
FILE *const stderr = &__stderr;

//-------- <<< Use Configuration Wizard in Context Menu >>> --------------------

// <h>STDOUT USART Interface

//   <o>Connect to hardware via Driver_USART# <0-255>
//   <i>Select driver control block for USART interface
#define USART_DRV_NUM           0

//   <o>Baudrate
#define USART_BAUDRATE          115200

// </h>


#define _USART_Driver_(n)  Driver_USART##n
#define  USART_Driver_(n) _USART_Driver_(n)

extern ARM_DRIVER_USART  USART_Driver_(USART_DRV_NUM);
#define ptrUSART       (&USART_Driver_(USART_DRV_NUM))

int stdout_putchar(const unsigned char ch) {
    uint8_t buf[1];

    buf[0] = ch;
    if (ptrUSART->Send(buf, 1) != ARM_DRIVER_OK) {
      return (-1);
    }
    while (ptrUSART->GetTxCount() != 1);
    return (ch);
}

