#include <can_qr.h>
#include <main.h>
#include <agv_info.h>

#define Pi	3.14159265359
	CAN_TxHeaderTypeDef txHeader;
	CAN_RxHeaderTypeDef	rxHeader;
	
	uint32_t TxMailbox;
	
	extern CAN_HandleTypeDef hcan1;
	extern DEVICE_DEF DEVICE;
	extern CHASSIC_DEF CHASSIC;
	extern LOCALIZATION_DEF	LOCALIZATION;
	extern IMU_DATA_DEF	IMU_DATA;
	
void Kinco_Init(void)
{
	HAL_CAN_Start(&hcan1);
	HAL_CAN_ActivateNotification(&hcan1,CAN_IT_RX_FIFO0_MSG_PENDING);
		
		txHeader.DLC = 7;
		txHeader.ExtId = 0;
		txHeader.IDE	= CAN_ID_STD;
		txHeader.RTR		= CAN_RTR_DATA;
		txHeader.TransmitGlobalTime = DISABLE;
	
	
}

void enable_driver(void)
{
	
	LOCALIZATION.time_stamp = HAL_GetTick();
	uint8_t TxData[7] = {0xFD, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00};
	txHeader.StdId = 0x200 + DEVICE.ID_motion_left;
	txHeader.DLC = 7;
	if (HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox) != HAL_OK)
	{
		 Error_Handler ();
	}
	txHeader.StdId = 0x200 + DEVICE.ID_motion_right;
	if (HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox) != HAL_OK)
	{
		 Error_Handler ();
	}
	HAL_Delay(50);
	
	TxData[0] = 0x01;
	TxData[1] = 0x00;
	txHeader.StdId = 0x00;
	txHeader.DLC = 2;
	if (HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox) != HAL_OK)
	{
		 Error_Handler ();
	}

	HAL_Delay(50);
	TxData[0] = 0xfd;
	TxData[1] = 0x0f;
	txHeader.StdId = 0x200 + DEVICE.ID_motion_left;
	txHeader.DLC = 7;
	if (HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox) != HAL_OK)
	{
		 Error_Handler ();
	}
	txHeader.StdId = 0x200 + DEVICE.ID_motion_right;
	if (HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox) != HAL_OK)
	{
		 Error_Handler ();
	}
	HAL_Delay(50);
}

int32_t convert_speeds(float vel)
{
	return  (int32_t)((vel*5120000*CHASSIC.gear*60)/(1875*2*Pi*CHASSIC.R_Wheel));
}

void kinco_control(DIR dir, float vel)	/* mm/s */
{
	uint8_t TxData[7] = {0xFD, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00};
	txHeader.StdId =0x80;
	txHeader.DLC	= 0;
	if (HAL_CAN_AddTxMessage(&hcan1, &txHeader, NULL, &TxMailbox) != HAL_OK)
	{
		 Error_Handler ();
	}
	switch(dir)
	{
		case Straigh:
		{
			TxData[6]	= convert_speeds(vel)>>24;
			TxData[5] = (convert_speeds(vel)&0x00ff0000)>>16;
			TxData[4] =	(convert_speeds(vel)&0x0000ff00)>>8;
			TxData[3] = convert_speeds(vel)&0x000000ff;
			
			txHeader.StdId = 0x200 + DEVICE.ID_motion_left;
			txHeader.DLC	= 7;
			if (HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox) != HAL_OK)
			{
				 Error_Handler ();
			}
			
			TxData[6]	= convert_speeds(0.0-vel)>>24;
			TxData[5] = (convert_speeds(0.0-vel)&0x00ff0000)>>16;
			TxData[4] =	(convert_speeds(0.0-vel)&0x0000ff00)>>8;
			TxData[3] = convert_speeds(0.0-vel)&0x000000ff;
			
			txHeader.StdId = 0x200 + DEVICE.ID_motion_right;
			txHeader.DLC	= 7;
			if (HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox) != HAL_OK)
			{
				 Error_Handler ();
			}
			break;
		}
		case Clockwise:
		{
			TxData[6]	= convert_speeds(vel)>>24;
			TxData[5] = (convert_speeds(vel)&0x00ff0000)>>16;
			TxData[4] =	(convert_speeds(vel)&0x0000ff00)>>8;
			TxData[3] = convert_speeds(vel)&0x000000ff;
			
			txHeader.StdId = 0x200 + DEVICE.ID_motion_left;
			txHeader.DLC	= 7;
			if (HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox) != HAL_OK)
			{
				 Error_Handler ();
			}

			txHeader.StdId = 0x200 + DEVICE.ID_motion_right;
			txHeader.DLC	= 7;
			if (HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox) != HAL_OK)
			{
				 Error_Handler ();
			}
			break;
		}
		case counter_clockwise:
		{
			TxData[6]	= convert_speeds(0.0-vel)>>24;
			TxData[5] = (convert_speeds(0.0-vel)&0x00ff0000)>>16;
			TxData[4] =	(convert_speeds(0.0-vel)&0x0000ff00)>>8;
			TxData[3] = convert_speeds(0.0-vel)&0x000000ff;
			
			txHeader.StdId = 0x200 + DEVICE.ID_motion_left;
			txHeader.DLC	= 7;
			if (HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox) != HAL_OK)
			{
				 Error_Handler ();
			}
			txHeader.StdId = 0x200 + DEVICE.ID_motion_right;
			txHeader.DLC	= 7;
			if (HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox) != HAL_OK)
			{
				 Error_Handler ();
			}
			break;
		}
	}
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	uint8_t rxData[8];
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, rxData);
	if (rxHeader.DLC == 8)
	{
		LOCALIZATION.angle = IMU_DATA.Z_axis_azimuth;
		LOCALIZATION.time_stamp = HAL_GetTick();
		switch (rxHeader.StdId)
		{
			case 0x181:
			{
				LOCALIZATION.vel_left = (float)(((rxData[7]<<24)|(rxData[6]<<16)|(rxData[5]<<8)|rxData[4])*1875*(2*Pi*CHASSIC.R_Wheel)/(5120000*CHASSIC.gear*60));
				break;
			}
			case 0x182:
			{
				LOCALIZATION.vel_right = (float)(((rxData[7]<<24)|(rxData[6]<<16)|(rxData[5]<<8)|rxData[4])*1875*(2*Pi*CHASSIC.R_Wheel)/(5120000*CHASSIC.gear*60));
				break;
			}
		}
	}
}
