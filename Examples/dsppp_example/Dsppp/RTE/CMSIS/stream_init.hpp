#pragma once

/**
 * @file stream_init.hpp
 * @brief Public initialization interface for CMSIS-Stream applications.
 *
 * This Pack configuration header exposes the top-level entry points and stream
 * execution contexts used by the application. Adapt NB_APPS and the matching
 * implementation in stream_init.cpp when the project contains more than one
 * generated graph.
 */

#include "EventQueue.hpp"
#include "StreamNode.hpp"

#include "stream_runtime_init.hpp"

/**
 * @brief Number of stream applications configured for this project.
 */
#define NB_APPS 1

/**
 * @brief Stream execution contexts, one entry per available application.
 *
 * Each context binds a generated scheduler, FIFO reset function, node access
 * wrapper, event queue, and metadata needed by the runtime context switch code.
 */
extern stream_execution_context_t contexts[NB_APPS];

/**
 * @brief Index of the stream application currently selected for execution.
 *
 * The index selects an entry in @ref contexts and the corresponding event queue.
 */
extern int currentNetwork;

/**
 * @brief Configure hardware, initialize stream graphs, and start the selected graph.
 *
 * The implementation initializes required hardware drivers, prepares graph
 * parameters, allocates runtime memory and event queues, initializes generated
 * schedulers, builds stream execution contexts, then starts the active graph.
 */
void stream_configure_and_start();

/**
 * @brief Stop stream execution and release all stream resources.
 *
 * @param callerIsRuntimeThread Use false when called from a normal control
 * thread after stream execution has ended. Use true when called from an
 * application handler or graph node running on a runtime thread. Peer runtime
 * threads are still stopped before graph resources are freed.
 */
void stream_free_all(bool callerIsRuntimeThread = false);
