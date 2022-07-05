/*
 * Copyright (c) 2009-2022 Arm Limited. All rights reserved.
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

/*
 * This file is derivative of CMSIS system_ARMCM85.c
 * Git SHA: 61ad1303bc50450130cfb540caa384875a260b91
 */

#include "SSE310MPS3.h"

/*----------------------------------------------------------------------------
  Define clocks
 *----------------------------------------------------------------------------*/
 #define  XTAL             (32000000UL)
 #define  SYSTEM_CLOCK     (XTAL)
 #define  PERIPHERAL_CLOCK (25000000UL)

/*----------------------------------------------------------------------------
  Exception / Interrupt Vector table
 *----------------------------------------------------------------------------*/
extern const VECTOR_TABLE_Type __VECTOR_TABLE[496];

/*----------------------------------------------------------------------------
  System Core Clock Variable
 *----------------------------------------------------------------------------*/
uint32_t SystemCoreClock = SYSTEM_CLOCK;
uint32_t PeripheralClock = PERIPHERAL_CLOCK;

/*----------------------------------------------------------------------------
  System Core Clock update function
 *----------------------------------------------------------------------------*/
void SystemCoreClockUpdate (void)
{
    SystemCoreClock = SYSTEM_CLOCK;
    PeripheralClock = PERIPHERAL_CLOCK;
}

/*----------------------------------------------------------------------------
  System initialization function
 *----------------------------------------------------------------------------*/
void SystemInit (void)
{
#if defined (__VTOR_PRESENT) && (__VTOR_PRESENT == 1U)
    SCB->VTOR = (uint32_t)(&__VECTOR_TABLE[0]);
#endif

    /* Set CPDLPSTATE.RLPSTATE to 0
       Set CPDLPSTATE.ELPSTATE to 0, to stop the processor from trying to switch the EPU into retention state.
       Set CPDLPSTATE.CLPSTATE to 0, so PDCORE will not enter low-power state. */
    PWRMODCTL->CPDLPSTATE &= ~(PWRMODCTL_CPDLPSTATE_RLPSTATE_Msk |
                               PWRMODCTL_CPDLPSTATE_ELPSTATE_Msk |
                               PWRMODCTL_CPDLPSTATE_CLPSTATE_Msk  );

#if (defined (__FPU_USED) && (__FPU_USED == 1U)) || \
    (defined (__ARM_FEATURE_MVE) && (__ARM_FEATURE_MVE > 0U))
    SCB->CPACR |= ((3U << 10U*2U) |           /* enable CP10 Full Access */
                   (3U << 11U*2U)  );         /* enable CP11 Full Access */

    /* Favor best FP/MVE performance by default, avoid EPU switch-ON delays */
    /* PDEPU ON, Clock OFF */
    PWRMODCTL->CPDLPSTATE |= 0x1 << PWRMODCTL_CPDLPSTATE_ELPSTATE_Pos;
#endif

#ifdef UNALIGNED_SUPPORT_DISABLE
    SCB->CCR |= SCB_CCR_UNALIGN_TRP_Msk;
#endif

    /* Enable Loop and branch info cache */
    SCB->CCR |= SCB_CCR_LOB_Msk;

    /* Enable Branch Prediction */
    SCB->CCR |= SCB_CCR_BP_Msk;

    __DSB();
    __ISB();
}
