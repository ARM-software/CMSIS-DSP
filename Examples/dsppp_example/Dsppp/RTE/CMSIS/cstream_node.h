/* ----------------------------------------------------------------------
 * Project:      CMSIS Stream Library
 * Title:        cstream_node.h
 * Description:  C interface used by the runtime to access C++ stream nodes
 *
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- 
 *
 * Copyright (C) 2026 ARM Limited or its affiliates. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef C_STREAM_NODE_H
#define C_STREAM_NODE_H

#include "cg_enums.h"

#ifdef   __cplusplus
extern "C"
{
#endif

struct CStreamNode;

/**
 * @brief C-callable view of the standard StreamNode API.
 * 
 * The generated scheduler exposes identified C++ nodes through these function
 * pointers so code outside the scheduler can query and initialize them without
 * depending on their concrete C++ types.
 */
struct StreamNodeInterface
{
    int (*nodeID)(const void *self);
    int (*needsAsynchronousInit)(const void *self);
    void (*subscribe)(void *self,int outputPort,CStreamNode *dst,int dstPort);
    cg_status (*init)(void *self);
};


/**
 * @brief Optional C-callable view of the ContextSwitch API.
 *
 * Nodes implement this interface when they must react to graph pause and
 * resume operations. Typical users are hardware-facing nodes and stateful
 * nodes that must quiesce or restart cleanly around a context switch.
 * 
 */
struct ContextSwitchInterface
{
    int (*pause)(void *self);
    int (*resume)(void *self);
};

/**
 * @brief Opaque handle for an identified C++ stream node.
 *
 * The generated scheduler creates CStreamNode handles only for nodes marked as
 * identified in the Python graph description. Nodes that do not need to be
 * accessed from application or runtime code do not need a handle.
 *
 * If the C++ object does not implement an optional interface, the corresponding
 * pointer is NULL. Nodes that participate in pause/resume context switching
 * must be identified so the runtime can reach their ContextSwitchInterface.
 *
 * Applications may extend this structure with additional interface tables when
 * users need more C-callable node APIs.
 */

struct CStreamNode
{
    void *obj;
    const struct StreamNodeInterface *stream_intf;
    const struct ContextSwitchInterface *context_switch_intf;
};

#ifdef   __cplusplus
}
#endif

#endif
