/******************************************************************************

 @file  _sbl_spi.c

 @brief This file contains the interface to the H/W transport driver by SPI,
        4P1, FD for the boot loader.

 Group: WCS, BTS
 Target Device: CC2540, CC2541

 ******************************************************************************
 
 Copyright (c) 2011-2021, Texas Instruments Incorporated
 All rights reserved.

 IMPORTANT: Your use of this Software is limited to those specific rights
 granted under the terms of a software license agreement between the user
 who downloaded the software, his/her employer (which must be your employer)
 and Texas Instruments Incorporated (the "License"). You may not use this
 Software unless you agree to abide by the terms of the License. The License
 limits your use, and you acknowledge, that the Software may not be modified,
 copied or distributed unless embedded on a Texas Instruments microcontroller
 or used solely and exclusively in conjunction with a Texas Instruments radio
 frequency transceiver, which is integrated into your product. Other than for
 the foregoing purpose, you may not use, reproduce, copy, prepare derivative
 works of, modify, distribute, perform, display or sell this Software and/or
 its documentation for any purpose.

 YOU FURTHER ACKNOWLEDGE AND AGREE THAT THE SOFTWARE AND DOCUMENTATION ARE
 PROVIDED �AS IS� WITHOUT WARRANTY OF ANY KIND, EITHER EXPRESS OR IMPLIED,
 INCLUDING WITHOUT LIMITATION, ANY WARRANTY OF MERCHANTABILITY, TITLE,
 NON-INFRINGEMENT AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT SHALL
 TEXAS INSTRUMENTS OR ITS LICENSORS BE LIABLE OR OBLIGATED UNDER CONTRACT,
 NEGLIGENCE, STRICT LIABILITY, CONTRIBUTION, BREACH OF WARRANTY, OR OTHER
 LEGAL EQUITABLE THEORY ANY DIRECT OR INDIRECT DAMAGES OR EXPENSES
 INCLUDING BUT NOT LIMITED TO ANY INCIDENTAL, SPECIAL, INDIRECT, PUNITIVE
 OR CONSEQUENTIAL DAMAGES, LOST PROFITS OR LOST DATA, COST OF PROCUREMENT
 OF SUBSTITUTE GOODS, TECHNOLOGY, SERVICES, OR ANY CLAIMS BY THIRD PARTIES
 (INCLUDING BUT NOT LIMITED TO ANY DEFENSE THEREOF), OR OTHER SIMILAR COSTS.

 Should you have any questions regarding your right to use this Software,
 contact Texas Instruments Incorporated at www.TI.com.

 ******************************************************************************
 Release Name: ble_sdk_1.5.2.0
 Release Date: 2021-03-10 17:38:59
 *****************************************************************************/

/* ------------------------------------------------------------------------------------------------
 *                                          Includes
 * ------------------------------------------------------------------------------------------------
 */

#include "hal_dma.h"
#include "hal_types.h"
#include "hal_uart.h"
#include "sbl_exec.h"

/* ------------------------------------------------------------------------------------------------
 *                                          Global Variables
 * ------------------------------------------------------------------------------------------------
 */

halDMADesc_t dmaCh1234[4];

/* ------------------------------------------------------------------------------------------------
 *                                          Local Variables
 * ------------------------------------------------------------------------------------------------
 */

static uint8 sblReset;

/* ------------------------------------------------------------------------------------------------
 *                                           Local Functions
 * ------------------------------------------------------------------------------------------------
 */

#include "_hal_uart_spi.c"

/**************************************************************************************************
 * @fn          sblRxCB
 *
 * @brief       Callback function for the SPI Rx message ready.
 *
 * input parameters
 *
 * None.
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 **************************************************************************************************
 */
static void sblRxCB(uint8 port, uint8 event)
{
  (void)port;
  (void)event;

  sblReset |= sblPoll();
}

/**************************************************************************************************
 * @fn          sblRun
 *
 * @brief       Serial Boot run code for the SPI transport.
 *
 * input parameters
 *
 * None.
 *
 * output parameters
 *
 * None.
 *
 * @return      None.
 */
static void sblRun(void)
{
  halUARTCfg_t uartConfig;
  uartConfig.callBackFunc = sblRxCB;

  HAL_DMA_SET_ADDR_DESC1234(dmaCh1234);

  HalUARTInitSPI();
  HalUARTOpenSPI(&uartConfig);

  while (1)
  {
    HalUARTPollSPI();

    if (HAL_DMA_CHECK_IRQ(HAL_SPI_CH_TX))
    {
      HAL_DMA_CLEAR_IRQ(HAL_SPI_CH_TX);
      HalUART_DMAIsrSPI();

      if (sblReset)
      {
        HAL_SYSTEM_RESET();
      }
    }
  }
}

/**************************************************************************************************
 * @fn          sblWait
 *
 * @brief       Serial Boot wait & poll for a Force SBL or Force Jump after a hard reset.
 *
 * input parameters
 *
 * None.
 *
 * output parameters
 *
 * None.
 *
 * @return      TRUE or FALSE to Force Boot Loader mode or not.
 */
static uint8 sblWait(void)
{
  return FALSE;  // TODO - what is the SPI protocol and wait time for a force SBL mode?
}


uint16 HalUARTRead(uint8 port, uint8 *pBuffer, uint16 length)
{
  (void)port;

  return HalUARTReadSPI(pBuffer, length);
}

uint16 HalUARTWrite(uint8 port, uint8 *pBuffer, uint16 length)
{
  (void)port;

  return HalUARTWriteSPI(pBuffer, length);
}

/**************************************************************************************************
*/
