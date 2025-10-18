/*
 * Copyright (c) 2020-2023 Arm Limited. All rights reserved.
 *
 * Licensed under the Apache License Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing software
 * distributed under the License is distributed on an "AS IS" BASIS
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef __DEVICE_CFG_H__
#define __DEVICE_CFG_H__

#include "RTE_Components.h"

/**
 * \file device_cfg.h
 * \brief Configuration file native driver re-targeting
 *
 * \details This file can be used to add native driver specific macro
 *          definitions to select which peripherals are available in the build.
 *
 * This is a default device configuration file with all peripherals enabled.
 */

/* Secure only peripheral configuration */

/* ARM MPS3 IO SCC */
#ifdef RTE_MPS3_IO
#define MPS3_IO_S
#define MPS3_IO_DEV                 MPS3_IO_DEV_S
#endif

/* I2C_SBCon */
#ifdef RTE_I2C0
#define I2C0_SBCON_S
#define I2C0_SBCON_DEV              I2C0_SBCON_DEV_S
#endif

/* I2S */
#ifdef RTE_I2S
#define MPS3_I2S_S
#define MPS3_I2S_DEV                MPS3_I2S_DEV_S
#endif

/* ARM UART Controller CMSDK */
#ifdef RTE_USART0
#define UART0_CMSDK_S
#define UART0_CMSDK_DEV          UART0_CMSDK_DEV_S
#endif
#ifdef RTE_USART1
#define UART1_CMSDK_S
#define UART1_CMSDK_DEV          UART1_CMSDK_DEV_S
#endif

#define DEFAULT_UART_BAUDRATE    115200U

/* To be used as CODE and DATA sram */
#ifdef RTE_ISRAM0_MPC
#define MPC_ISRAM0_S
#define MPC_ISRAM0_DEV              MPC_ISRAM0_DEV_S
#endif

#ifdef RTE_ISRAM1_MPC
#define MPC_ISRAM1_S
#define MPC_ISRAM1_DEV              MPC_ISRAM0_DEV_S
#endif

#ifdef RTE_SRAM_MPC
#define MPC_SRAM_S
#define MPC_SRAM_DEV                MPC_SRAM_DEV_S
#endif

#ifdef RTE_QSPI_MPC
#define MPC_QSPI_S
#define MPC_QSPI_DEV                MPC_QSPI_DEV_S
#endif

/** System Counter Armv8-M */
#ifdef RTE_SYSCOUNTER
#define SYSCOUNTER_CNTRL_ARMV8_M_S
#define SYSCOUNTER_CNTRL_ARMV8_M_DEV    SYSCOUNTER_CNTRL_ARMV8_M_DEV_S

#define SYSCOUNTER_READ_ARMV8_M_S
#define SYSCOUNTER_READ_ARMV8_M_DEV     SYSCOUNTER_READ_ARMV8_M_DEV_S
/**
 * Arbitrary scaling values for test purposes
 */
#define SYSCOUNTER_ARMV8_M_DEFAULT_SCALE0_INT           1u
#define SYSCOUNTER_ARMV8_M_DEFAULT_SCALE0_FRACT         0u
#define SYSCOUNTER_ARMV8_M_DEFAULT_SCALE1_INT           1u
#define SYSCOUNTER_ARMV8_M_DEFAULT_SCALE1_FRACT         0u
#endif

/* System timer */
#ifdef RTE_TIMEOUT
#define SYSTIMER0_ARMV8_M_S
#define SYSTIMER0_ARMV8_M_DEV    SYSTIMER0_ARMV8_M_DEV_S
#define SYSTIMER1_ARMV8_M_S
#define SYSTIMER1_ARMV8_M_DEV    SYSTIMER1_ARMV8_M_DEV_S
#define SYSTIMER2_ARMV8_M_S
#define SYSTIMER2_ARMV8_M_DEV    SYSTIMER2_ARMV8_M_DEV_S
#define SYSTIMER3_ARMV8_M_S
#define SYSTIMER3_ARMV8_M_DEV    SYSTIMER3_ARMV8_M_DEV_S

#define SYSTIMER0_ARMV8M_DEFAULT_FREQ_HZ    (32000000ul)
#define SYSTIMER1_ARMV8M_DEFAULT_FREQ_HZ    (32000000ul)
#define SYSTIMER2_ARMV8M_DEFAULT_FREQ_HZ    (32000000ul)
#define SYSTIMER3_ARMV8M_DEFAULT_FREQ_HZ    (32000000ul)
#endif

/* CMSDK GPIO driver structures */
#ifdef RTE_GPIO
#define GPIO0_CMSDK_S
#define GPIO0_CMSDK_DEV GPIO0_CMSDK_DEV_S
#define GPIO1_CMSDK_S
#define GPIO1_CMSDK_DEV GPIO1_CMSDK_DEV_S
#define GPIO2_CMSDK_S
#define GPIO2_CMSDK_DEV GPIO2_CMSDK_DEV_S
#define GPIO3_CMSDK_S
#define GPIO3_CMSDK_DEV GPIO3_CMSDK_DEV_S
#endif

/* System Watchdogs */
#ifdef RTE_WATCHDOG
#define SYSWDOG_ARMV8_M_S
#define SYSWDOG_ARMV8_M_DEV SYSWDOG_ARMV8_M_DEV_S
#endif

/* ARM MPC SIE 310 driver structures */
#ifdef RTE_VM0_MPC
#define MPC_VM0_S
#define MPC_VM0_DEV MPC_VM0_DEV_S
#endif
#ifdef RTE_VM1_MPC
#define MPC_VM1_S
#define MPC_VM1_DEV MPC_VM1_DEV_S
#endif
#ifdef RTE_SSRAM2_MPC
#define MPC_SSRAM2_S
#define MPC_SSRAM2_DEV MPC_SSRAM2_DEV_S
#endif
#ifdef RTE_SSRAM3_MPC
#define MPC_SSRAM3_S
#define MPC_SSRAM3_DEV MPC_SSRAM3_DEV_S
#endif

/* ARM PPC driver structures */
#ifdef RTE_MAIN0_PPC_CORSTONE310
#define PPC_CORSTONE310_MAIN0_S
#define PPC_CORSTONE310_MAIN0_DEV PPC_CORSTONE310_MAIN0_DEV_S
#endif
#ifdef RTE_MAIN_EXP0_PPC_CORSTONE310
#define PPC_CORSTONE310_MAIN_EXP0_S
#define PPC_CORSTONE310_MAIN_EXP0_DEV PPC_CORSTONE310_MAIN_EXP0_DEV_S
#endif
#ifdef RTE_MAIN_EXP1_PPC_CORSTONE310
#define PPC_CORSTONE310_MAIN_EXP1_S
#define PPC_CORSTONE310_MAIN_EXP1_DEV PPC_CORSTONE310_MAIN_EXP1_DEV_S
#endif
#ifdef RTE_MAIN_EXP2_PPC_CORSTONE310
#define PPC_CORSTONE310_MAIN_EXP2_S
#define PPC_CORSTONE310_MAIN_EXP2_DEV PPC_CORSTONE310_MAIN_EXP2_DEV_S
#endif
#ifdef RTE_MAIN_EXP3_PPC_CORSTONE310
#define PPC_CORSTONE310_MAIN_EXP3_S
#define PPC_CORSTONE310_MAIN_EXP3_DEV PPC_CORSTONE310_MAIN_EXP3_DEV_S
#endif
#ifdef RTE_PERIPH0_PPC_CORSTONE310
#define PPC_CORSTONE310_PERIPH0_S
#define PPC_CORSTONE310_PERIPH0_DEV PPC_CORSTONE310_PERIPH0_DEV_S
#endif
#ifdef RTE_PERIPH1_PPC_CORSTONE310
#define PPC_CORSTONE310_PERIPH1_S
#define PPC_CORSTONE310_PERIPH1_DEV PPC_CORSTONE310_PERIPH1_DEV_S
#endif
#ifdef RTE_PERIPH_EXP0_PPC_CORSTONE310
#define PPC_CORSTONE310_PERIPH_EXP0_S
#define PPC_CORSTONE310_PERIPH_EXP0_DEV PPC_CORSTONE310_PERIPH_EXP0_DEV_S
#endif
#ifdef RTE_PERIPH_EXP1_PPC_CORSTONE310
#define PPC_CORSTONE310_PERIPH_EXP1_S
#define PPC_CORSTONE310_PERIPH_EXP1_DEV PPC_CORSTONE310_PERIPH_EXP1_DEV_S
#endif
#ifdef RTE_PERIPH_EXP2_PPC_CORSTONE310
#define PPC_CORSTONE310_PERIPH_EXP2_S
#define PPC_CORSTONE310_PERIPH_EXP2_DEV PPC_CORSTONE310_PERIPH_EXP2_DEV_S
#endif
#ifdef RTE_PERIPH_EXP3_PPC_CORSTONE310
#define PPC_CORSTONE310_PERIPH_EXP3_S
#define PPC_CORSTONE310_PERIPH_EXP3_DEV PPC_CORSTONE310_PERIPH_EXP3_DEV_S
#endif

/* DMA350 */
#ifdef RTE_DMA350
#define DMA350_DMA0_S
#define DMA350_DMA0_DEV             DMA350_DMA0_DEV_S

#define DMA350_CH0_S
#define DMA350_DMA0_CH0_S
#define DMA350_CH1_S
#define DMA350_DMA0_CH1_S
#endif

/* ARM SPI PL022 */
/* Invalid device stubs are not defined */
#define DEFAULT_SPI_SPEED_HZ  4000000U /* 4MHz */
#ifdef RTE_SPI1
#define SPI1_PL022_S
#define SPI1_PL022_DEV SPI1_PL022_DEV_S
#endif

#endif  /* __DEVICE_CFG_H__ */
