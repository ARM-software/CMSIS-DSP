/**
 * @file stream_init.cpp
 * @brief Configure, start, and release CMSIS-Stream applications.
 *
 * This Pack configuration file connects generated schedulers to the shared
 * CMSIS-Stream runtime. Adapt it for the generated graph headers, application
 * parameter blocks, hardware drivers, event queues, and stream execution
 * contexts used by the target application.
 */

// Application configuration included by each generated scheduler. It contains
// scheduler-specific hooks. Shared runtime overrides belong in
// stream_runtime_config.hpp and are picked up through stream_platform_config.hpp.
#include "app_config.hpp"

// Application parameter block for graph A. 
// Define the data types required by your application to
// communicate settings to the nodes of the graph
extern "C"
{
#include "hello_params.h"
}

// Generated scheduler header for graph A. The name is selected by the Python
// graph description. Include one generated scheduler header per graph when the
// application switches between several graphs at runtime.
// The name of the generated scheduler may be different from the
// name used below.
#include "scheduler_hello.h"

// CMSIS-Stream standard headers.
#include "EventQueue.hpp"
#include "StreamNode.hpp"

// Runtime initialization helpers.
#include "stream_runtime_init.hpp"

#include "stream_init.hpp"


using namespace arm_cmsis_stream;


/**
 * @brief Index of the stream application currently selected for execution.
 */
int currentNetwork = 0;

/**
 * @brief Array of stream execution contexts, one per network.
 */
stream_execution_context_t contexts[NB_APPS];

/**
 * @brief Event queues used by each stream application.
 */
EventQueue *queue_app[NB_APPS];


/*
 * Pause all context-switchable nodes in a graph.
 *
 * The runtime calls this application helper through stream_execution_context_t.
 * It stays in the configuration layer because CStreamNode can be extended by
 * the application, and the shared runtime should not depend on those extensions.
 */
static void pause_scheduler_app(const stream_execution_context_t *context)
{
    // Visit every identified node in the generated scheduler context.
    for (int32_t nodeid = 0; nodeid < (int32_t)context->nb_identified_nodes; nodeid++)
    {
        // The runtime stores nodes as opaque pointers; cast back to the app node type.
        CStreamNode *cnode = static_cast<CStreamNode *>(context->get_node_by_id(nodeid));
        if (cnode != nullptr)
        {
            // Pause only nodes that provide the optional context-switch API.
            if ((cnode->obj != nullptr) && (cnode->context_switch_intf != nullptr))
            {
                cnode->context_switch_intf->pause(cnode->obj);
            }
        }
    }
}

/*
 * Resume all context-switchable nodes in a graph.
 *
 * This helper mirrors pause_scheduler_app() and is called before the selected
 * graph starts so nodes can publish initial events if needed.
 */
static void resume_scheduler_app(const stream_execution_context_t *context)
{
    // Visit every identified node in the generated scheduler context.
    for (int32_t nodeid = 0; nodeid < (int32_t)context->nb_identified_nodes; nodeid++)
    {
        // The runtime stores nodes as opaque pointers; cast back to the app node type.
        CStreamNode *cnode = static_cast<CStreamNode *>(context->get_node_by_id(nodeid));

        if (cnode != nullptr)
        {
            // Resume only nodes that provide the optional context-switch API.
            if ((cnode->obj != nullptr) && (cnode->context_switch_intf != nullptr))
            {
                cnode->context_switch_intf->resume(cnode->obj);
            }
        }
    }
}

/*
 * Wrap generated node accessors behind the runtime's void* API.
 *
 * Generated get_scheduler_*_node functions return application-specific
 * CStreamNode pointers. The runtime stores them as opaque pointers so different
 * graphs may use different application node interfaces.
 */
static void *get_hello_node(int32_t nodeID)
{
    // Hide the application-specific node type behind the runtime's void* hook.
    return static_cast<void *>(get_scheduler_hello_node(nodeID));
}

uint32_t exec1;

static void timer_callback(void *argument)
{
    CMSISSTREAM_LOG_DBG("Timer callback\n");
}

static void handle_error(int32_t origin, int32_t error_code, int32_t info)
{
    CMSISSTREAM_LOG_ERR("Error from origin %d with code %d\n", origin, error_code);
    /*
     * This handler can be called from a runtime thread or from a graph node.
     * stream_free_all(true) may delete nodes, FIFOs, queues, and scheduler
     * resources that are still present on the current call stack. It is safe
     * here because the example exits immediately after freeing resources.
     *
     * If an application wants to recover and restart a graph, do not return
     * from this handler after freeing resources. Instead, signal a supervisor
     * thread and let that thread stop/free/recreate/start the graph.
     */
    stream_free_all(true);
    CMSISSTREAM_LOG_DBG("Exiting application\n");
    // To quit FVP
    exit(1);
}

/**
 * @brief Application-specific event handler.
 *
 * This function can be called from the graph in a synchronous or asynchronous way.
 * It is a global application handler that can be used by all graphs.
 *
 * @param src_node_id ID of the source node that is calling the application handler.
 * @param data Pointer to any associated data that was passed when the handler was installed.
 * @param evt The event object containing event details.
 * @return true if the event was handled successfully, false otherwise.
 */
static bool application_handler(int src_node_id, void *data, Event &&evt)
{
    int network_id = reinterpret_cast<int>(data);
    if (evt.event_id == kValue)
    {
        int32_t msg = evt.get<int32_t>();
        CMSISSTREAM_LOG_DBG("Application handler received event from node %d in network %d with value: %d\n", src_node_id, network_id, msg);
    }
    else if (evt.event_id == kError)
    {
        if (evt.wellFormed<int32_t,int32_t,int32_t>())
        {
            evt.apply<int32_t,int32_t,int32_t>(&handle_error);
        }
        
    } 
    else if (evt.event_id == kStopGraph)
    {
        CMSISSTREAM_LOG_DBG("Application handler received stop graph event for network\n");
    }
    else 
    {
        CMSISSTREAM_LOG_DBG("Application handler received unknown event ID %d from node %d in network %d\n", evt.event_id, src_node_id, network_id);
    }
    return true;
}

/**
 * @brief Configure resources and start the currently selected stream graph.
 *
 * The startup sequence initializes optional hardware sources, fills the
 * hardware parameter block for each generated graph, allocates runtime memory
 * and event queues, initializes each scheduler, builds execution contexts, then
 * resumes and starts the graph selected by @ref currentNetwork.
 */
void stream_configure_and_start()
{
    int err;

    /*
     * Step 1: configure hardware sources that may be shared by the stream
     * graphs. Remove unused sources or add application-specific drivers here.
     */
    osTimerId_t  timer_id = osTimerNew(timer_callback, osTimerPeriodic, &exec1, nullptr);
    if (timer_id == nullptr)
    {
        CMSISSTREAM_LOG_ERR("Can't create timer for hardware source\n");
        goto error;
    }
    
    /*
     * Step 2: fill generated parameter structures with application-specific
     * data before scheduler initialization.
     */

    helloParams.src.hw_.timer_id = timer_id;
    helloParams.src.val = 1.5f;


    // Step 3: initialize common CMSIS-Stream runtime memory.
    err = stream_init_memory();
    if (err != 0)
    {
        CMSISSTREAM_LOG_ERR("Error initializing stream\n");
        goto error;
    }

    // Step 4: allocate one event queue per graph.
    for (int network = 0; network < NB_APPS; network++)
    {
        queue_app[network] = stream_new_event_queue();

        if (queue_app[network] == nullptr)
        {
            CMSISSTREAM_LOG_ERR("Can't create CMSIS Stream Event Queue for network %d\n", network);
            goto error;
        }
        // Set an application handler for the graph
        queue_app[network]->setHandler((void*)network, application_handler);
    }

    // Step 5: initialize generated schedulers and graph nodes. Scheduler init
    // arguments are configured in the Python graph description; they may be a
    // parameter structure, several node-specific structures, or no parameters.

    err = init_scheduler_hello(queue_app[0], &helloParams);
    if (err != CG_SUCCESS)
    {
        CMSISSTREAM_LOG_ERR("Error: Failure during scheduler initialization for appa.\n");
        goto error;
    }

   

    // Step 6: bind generated scheduler callbacks and metadata to runtime
    // contexts. Some callbacks come directly from the generated scheduler; the
    // wrappers in this file hide application-specific node types from the
    // shared runtime.

    contexts[0] = stream_execution_context_t {
        scheduler_hello,
        reset_fifos_scheduler_hello,
        pause_scheduler_app,
        resume_scheduler_app,
        get_hello_node,
        queue_app[0],
        STREAM_HELLO_NB_IDENTIFIED_NODES,
        STREAM_HELLO_SCHED_LEN
    };


    /*
     * Step 7: resume the selected graph and start the runtime threads.
     *
     * Resume is called before the graph starts so nodes can publish events
     * once the event queue is ready. Node init cannot do this reliably because
     * init does not know whether the graph will run immediately or remain
     * paused. Nodes that need to distinguish first start from later resumes
     * should track that state internally.
     */
    resume_scheduler_app(&contexts[currentNetwork]);
    stream_start_threads(&contexts[currentNetwork]);

    return;
error:
    // Centralized error exit for failures during startup.
    CMSISSTREAM_LOG_ERR("Fatal error in main, stopping execution\n");

}

/**
 * @brief Stop all stream threads and release scheduler/runtime resources.
 */
void stream_free_all(bool callerIsRuntimeThread)
{
    stream_stop_threads(callerIsRuntimeThread);

    // Release resources allocated by the generated schedulers.
    free_scheduler_hello();

    // Destroy the event queues created during stream_configure_and_start().
    for (int network = 0; network < NB_APPS; network++)
    {
        delete queue_app[network];
    }

    // Release common CMSIS-Stream runtime memory last.
    stream_free_memory();
}
