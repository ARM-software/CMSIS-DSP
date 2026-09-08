#pragma once

/*
 * Shared CMSIS-Stream runtime configuration.
 *
 * Define runtime and stream-core overrides here when the target application
 * needs values different from the library defaults. This copied configuration
 * header is included by stream_platform_config.hpp before the platform defaults
 * are defined, so any CMSISSTREAM_* macro placed here overrides the default
 * used by CMSIS-Stream runtime sources and generated schedulers.
 *
 * Keep application-only scheduler hooks in app_config.hpp.
 */


// Uncomment and adapt these macros to route CMSIS-Stream logs through the
// application logging backend.
//#define CMSISSTREAM_LOG_ERR(fmt, ...) fprintf(stderr, "[ERR] " fmt, ##__VA_ARGS__)
//#define CMSISSTREAM_LOG_DBG(fmt, ...) fprintf(stderr, "[DBG] " fmt, ##__VA_ARGS__)

// <<< Use Configuration Wizard in Context Menu >>>

// <h>Event Queue Configuration

// <o CMSISSTREAM_EVENT_QUEUE_LENGTH>Event queue length <1..1024>
// <i>Maximum number of events that can wait in one CMSIS-Stream event queue.
// <i>New events are dropped when the queue is full.
// <d> 20
#define CMSISSTREAM_EVENT_QUEUE_LENGTH 20

// </h>

// <h>Runtime Thread Configuration

// <o CMSISSTREAM_EVT_THREAD_STACK_SIZE>Event thread stack size [bytes] <128..65536:8>
// <i>Stack size used by the CMSIS-Stream event-processing thread.
// <d> 4096
#define CMSISSTREAM_EVT_THREAD_STACK_SIZE 4096

// <o CMSISSTREAM_STREAM_THREAD_STACK_SIZE>Stream thread stack size [bytes] <128..65536:8>
// <i>Stack size used by the CMSIS-Stream dataflow scheduler thread.
// <d> 4096
#define CMSISSTREAM_STREAM_THREAD_STACK_SIZE 4096

// <o CMSISSTREAM_STREAM_THREAD_PRIORITY>Stream thread priority
// <osPriorityLow=> Low
// <osPriorityBelowNormal=> Below Normal
// <osPriorityNormal=> Normal
// <osPriorityAboveNormal=> Above Normal
// <osPriorityHigh=> High
// <osPriorityRealtime=> Realtime
// <i>Priority used by the CMSIS-Stream dataflow scheduler thread.
// <i>This priority should normally be at least as high as event priorities.
// <d> osPriorityRealtime
#define CMSISSTREAM_STREAM_THREAD_PRIORITY osPriorityRealtime

// <o CMSISSTREAM_EVT_HIGH_PRIORITY>Event thread high priority
// <osPriorityLow=> Low
// <osPriorityBelowNormal=> Below Normal
// <osPriorityNormal=> Normal
// <osPriorityAboveNormal=> Above Normal
// <osPriorityHigh=> High
// <osPriorityRealtime=> Realtime
// <i>Priority used while the event thread processes high-priority events.
// <d> osPriorityHigh
#define CMSISSTREAM_EVT_HIGH_PRIORITY osPriorityHigh

// <o CMSISSTREAM_EVT_NORMAL_PRIORITY>Event thread normal priority
// <osPriorityLow=> Low
// <osPriorityBelowNormal=> Below Normal
// <osPriorityNormal=> Normal
// <osPriorityAboveNormal=> Above Normal
// <osPriorityHigh=> High
// <osPriorityRealtime=> Realtime
// <i>Priority used while the event thread processes normal-priority events.
// <d> osPriorityNormal
#define CMSISSTREAM_EVT_NORMAL_PRIORITY osPriorityNormal

// <o CMSISSTREAM_EVT_LOW_PRIORITY>Event thread low priority
// <osPriorityLow=> Low
// <osPriorityBelowNormal=> Below Normal
// <osPriorityNormal=> Normal
// <osPriorityAboveNormal=> Above Normal
// <osPriorityHigh=> High
// <osPriorityRealtime=> Realtime
// <i>Priority used while the event thread processes low-priority events.
// <d> osPriorityLow
#define CMSISSTREAM_EVT_LOW_PRIORITY osPriorityLow

// </h>

// <h>Runtime Memory Pools

// <o CMSISSTREAM_NB_MAX_EVENTS>Maximum number of event objects <1..1024>
// <i>Maximum number of CMSIS-Stream events expected in the system at one time.
// <i>All event queues share this pool, so this can be larger than one queue length.
// <d> 16
#define CMSISSTREAM_NB_MAX_EVENTS 16

// <o CMSISSTREAM_NB_MAX_BUFS>Maximum number of shared buffer objects <1..1024>
// <i>Maximum number of buffers or tensors that can be referenced at one time.
// <i>An event may reference several buffers, so this can be larger than the event count.
// <d> 16
#define CMSISSTREAM_NB_MAX_BUFS 16

// <o CMSISSTREAM_SHARED_OVERHEAD>Shared pointer allocation overhead [bytes] <0..256:4>
// <i>Extra bytes reserved by pool allocators for std::allocate_shared control data.
// <d> 16
#define CMSISSTREAM_SHARED_OVERHEAD 16

// <s.64>Memory pool linker section
// <i>Linker section used for CMSIS-Stream runtime memory pool storage.
// <d> ".bss.evt_pool"
#define CMSISSTREAM_POOL_SECTION ".bss.evt_pool"

// </h>

// <h>Event Data Configuration

// <o CMSISSTREAM_MAX_NUMBER_EVENT_ARGUMENTS>Maximum number of event arguments <1..64>
// <i>Maximum number of values that can be carried by one CMSIS-Stream event.
// <i>This value affects the size of event-system memory pools.
// <d> 8
#define CMSISSTREAM_MAX_NUMBER_EVENT_ARGUMENTS 8

// <o CMSISSTREAM_TENSOR_MAX_DIMENSIONS>Maximum number of tensor dimensions <1..8>
// <i>Maximum number of dimensions stored in CMSIS-Stream tensor metadata.
// <d> 3
#define CMSISSTREAM_TENSOR_MAX_DIMENSIONS 3

// </h>

// <<< end of configuration section >>>

// Override default logging macros to route CMSIS-Stream logs through the application logging backend.
// Note that this can't be defined in the application configuration header that is only included
// by the graphs. But the runtime can also generate logs.
//#define CMSISSTREAM_LOG_ERR(fmt,...) printf("[ERR] " fmt, ##__VA_ARGS__)
//#define CMSISSTREAM_LOG_DBG(fmt,...) printf("[DBG] " fmt, ##__VA_ARGS__)