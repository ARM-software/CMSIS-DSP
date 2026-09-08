/* ----------------------------------------------------------------------
 * Project:      CMSIS Stream Library
 * Title:        IdentifiedNode.hpp
 * Description:  Helper template that exposes identified C++ nodes through C
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
#pragma once 

extern "C" {
   #include "cstream_node.h"
}

#include "StreamNode.hpp"

/**
 * @brief Create a CStreamNode handle for an identified C++ node.
 *
 * The helper detects supported C++ base classes at compile time and installs
 * the matching C interface tables in the returned CStreamNode. Interface
 * pointers remain NULL when the node type does not implement the corresponding
 * API.
 *
 * Extend this function with additional interface tables when the application
 * adds more C-callable APIs to CStreamNode.
 */
template <typename T>
CStreamNode createStreamNode(T &obj)
{
    const StreamNodeInterface *stream_intf_current = nullptr;
    const ContextSwitchInterface *context_switch_intf_current = nullptr;
    if constexpr (std::is_base_of<arm_cmsis_stream::StreamNode, T>::value)
    {
        static const StreamNodeInterface stream_intf = {
            [](const void *self) -> int {
                return(static_cast<const T *>(self)->nodeID());
            }
            ,
            [](const void *self) -> int {
                return(static_cast<const T *>(self)->needsAsynchronousInit());
            },
            [](void *self, int outputPort, CStreamNode *dst, int dstPort) {
                static_cast<T *>(self)->subscribe(outputPort, 
                    *static_cast<arm_cmsis_stream::StreamNode*>(dst->obj), 
                    dstPort);
            },
            [](void *self) -> cg_status {
                return(static_cast<T *>(self)->init());
            },
        };
        stream_intf_current = &stream_intf;
    }

    
    if constexpr (std::is_base_of<ContextSwitch, T>::value)
    {
        static const ContextSwitchInterface context_switch_intf = {
            [](void *self) -> int {
                return(static_cast<T *>(self)->pause());
            },
            [](void *self) -> int {
                return(static_cast<T *>(self)->resume());
            }
        };
        context_switch_intf_current = &context_switch_intf;
    }
   

    return CStreamNode {&obj,stream_intf_current,context_switch_intf_current};

};

