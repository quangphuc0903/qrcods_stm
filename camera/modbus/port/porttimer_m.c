/*
 * FreeModbus Libary: RT-Thread Port
 * Copyright (C) 2013 Armink <armink.ztl@gmail.com>
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 * File: $Id: porttimer_m.c,v 1.60 2013/08/13 15:07:05 Armink add Master Functions$
 */

/* ----------------------- Platform includes --------------------------------*/
#include "port.h"

/* ----------------------- Modbus includes ----------------------------------*/
#include "mb.h"
#include "mb_m.h"
#include "mbport.h"

#if MB_MASTER_RTU_ENABLED > 0 || MB_MASTER_ASCII_ENABLED > 0
/* ----------------------- Variables ----------------------------------------*/
extern TIM_HandleTypeDef htim3;


USHORT usT35TimeOut50us = 0;
 uint16_t downcounterMaster = 0;

/* ----------------------- Start implementation -----------------------------*/
BOOL xMBMasterPortTimersInit(USHORT usTimeOut50us)
{
  TIM_MasterConfigTypeDef sMasterConfig;
  
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = (HAL_RCC_GetPCLK1Freq() / 1000000) - 1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 50 - 1;
  
    /* backup T35 ticks */
  usT35TimeOut50us = usTimeOut50us;
  
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    return FALSE;
  }
  
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_UPDATE;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    return FALSE;
  }
  
  return TRUE;
}

void vMBMasterPortTimersT35Enable()
{
    downcounterMaster = (50 * usT35TimeOut50us)
            / (1000);

    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_T35);

	  HAL_TIM_Base_Start_IT(&htim3);
}

void vMBMasterPortTimersConvertDelayEnable()
{
    downcounterMaster = MB_MASTER_DELAY_MS_CONVERT;

    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_CONVERT_DELAY);

	  HAL_TIM_Base_Start_IT(&htim3);
}

void vMBMasterPortTimersRespondTimeoutEnable()
{
   downcounterMaster = MB_MASTER_TIMEOUT_MS_RESPOND;

    /* Set current timer mode, don't change it.*/
    vMBMasterSetCurTimerMode(MB_TMODE_RESPOND_TIMEOUT);

	  HAL_TIM_Base_Start_IT(&htim3);
}

void vMBMasterPortTimersDisable()
{
  /* Disable any pending timers. */
  HAL_TIM_Base_Stop_IT(&htim3);
}

//void prvvTIMERExpiredISR(void)
//{
//    (void) pxMBMasterPortCBTimerExpired();
//}


#endif
