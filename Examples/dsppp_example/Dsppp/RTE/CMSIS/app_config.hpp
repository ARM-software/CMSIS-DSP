#pragma once 

/*
 * Application configuration for a CMSIS-Stream scheduler.
 *
 * Generated schedulers include this copied configuration header before
 * stream_platform_config.hpp. Use it for scheduler-specific application hooks.
 * Put shared CMSIS-Stream runtime overrides in stream_runtime_config.hpp; those
 * overrides are included by stream_platform_config.hpp.
 *
 * CG_BEFORE_BUFFER is used when the generated scheduler uses memory optimization
 * settings with memory sharing between different FIFOs. In this case, the buffers 
 * must be aligned.
 *
 * CG_BEFORE_NODE_EXECUTION is called by the generated scheduler before each
 * node execution. The default hook connects the scheduler to the CMSIS-RTOS
 * runtime so pause and stop requests can interrupt the graph cleanly. The hook
 * can also be extended with application tracing or diagnostics.
 */

extern "C"
{
#include "RTE_Components.h"
#include CMSIS_device_header

#include "cmsis_os2.h" /* CMSIS-RTOS2 API */

#include "hello_params.h"

}

#include "cg_enums.h"
#include "stream_rtos_events.h"



extern osEventFlagsId_t cg_streamEvent;

// FIFO buffers are placed in a dedicated section so the linker can apply the
// memory layout selected for the generated graph. 
// When memory optimization setting is used in the Python generator, the memory
// for FIFOs is shared and the buffers must be aligned to avoid unaligned accesses.
#define CG_BEFORE_BUFFER __ALIGNED(16) __attribute__((section(".bss.stream_fifo")))

#define CG_BEFORE_NODE_EXECUTION(id)                                                 \
    {                                                                                \
        uint32_t res =                                                               \
            osEventFlagsWait(cg_streamEvent, STREAM_PAUSE_EVENT | STREAM_DONE_EVENT, \
                             osFlagsWaitAny, 0);                                     \
        if (!(res & 0x80000000))                                                     \
        {                                                                            \
            if ((res & STREAM_DONE_EVENT) != 0)                                      \
            {                                                                        \
                cgStaticError = CG_STOP_SCHEDULER;                                   \
                goto errorHandling;                                                  \
            }                                                                        \
            if ((res & STREAM_PAUSE_EVENT) != 0)                                     \
            {                                                                        \
                cgStaticError = CG_PAUSED_SCHEDULER;                                 \
                goto errorHandling;                                                  \
            }                                                                        \
        }                                                                            \
    }
