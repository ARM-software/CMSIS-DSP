/* ----------------------------------------------------------------------
 * Project:      CMSIS DSP Library
 * Title:        cg_status.h
 * Description:  Error code for the Compute Graph
 *
 *
 * Target Processor: Cortex-M and Cortex-A cores
 * -------------------------------------------------------------------- 
 *
 * Copyright (C) 2021-2023 ARM Limited or its affiliates. All rights reserved.
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

#ifndef _CG_STATUS_H_


 typedef enum
  {
    CG_SUCCESS                    =  0, /**< No error */
    CG_BUFFER_UNDERFLOW           = -1, /**< FIFO underflow */
    CG_BUFFER_OVERFLOW            = -2, /**< FIFO overflow */
    CG_MEMORY_ALLOCATION_FAILURE  = -3, /**< Memory allocation failure */
    CG_INIT_FAILURE               = -4, /**< Node initialization failure */
    CG_SKIP_EXECUTION             = -5, /**< Skip node execution (asynchronous mode) */
    CG_BUFFER_ERROR               = -6, /**< Stop execution due to FIFO overflow or underflow (asynchronous mode for pure function) */
    CG_OS_ERROR                   = -7  /**< RTOS API error */
  } cg_status;



#endif /* _CG_STATUS_H_ */