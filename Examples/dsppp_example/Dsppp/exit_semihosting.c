#include <stdint.h>

#define SEMIHOST_SYS_EXIT_EXTENDED        0x20u
#define ADP_STOPPED_APPLICATION_EXIT      0x20026u

__attribute__((noreturn))
static void semihost_exit_extended(int status)
{
    uint32_t args[2];

    args[0] = ADP_STOPPED_APPLICATION_EXIT;
    args[1] = (uint32_t)status;

    register uint32_t r0 __asm("r0") = SEMIHOST_SYS_EXIT_EXTENDED;
    register void *r1 __asm("r1") = args;

    __asm volatile (
        "bkpt #0xAB"
        :
        : "r"(r0), "r"(r1)
        : "memory"
    );

    for (;;) {
        /* In case semihosting returns or is disabled. */
    }
}

__attribute__((noreturn))
void _exit(int status)
{
    semihost_exit_extended(status);
}

__attribute__((noreturn))
void _sys_exit(int status)
{
    semihost_exit_extended(status);
}