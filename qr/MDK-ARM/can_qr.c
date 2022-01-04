#include <can_qr.h>
#include <main.h>


#define Pi	3.14159265359
	CAN_TxHeaderTypeDef txHeader;
	CAN_RxHeaderTypeDef	rxHeader;
	
	uint32_t TxMailbox;
	
	extern CAN_HandleTypeDef hcan1;
	//extern DEVICE_DEF DEVICE;
	extern CHASSIC_DEF CHASSIC;
	extern LOCALIZATION_DEF	LOCALIZATION;
	//extern IMU_DATA_DEF	IMU_DATA;
	
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

void enable_driver(DEVICE_DEF dev)
{
	
	LOCALIZATION.time_stamp = HAL_GetTick();
	uint8_t TxData[7] = {0xFD, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00};
	txHeader.StdId = 0x200 + dev.ID_motion_left;
	txHeader.DLC = 7;
	if (HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox) != HAL_OK)
	{
		 Error_Handler ();
	}
	txHeader.StdId = 0x200 + dev.ID_motion_right;
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
	txHeader.StdId = 0x200 + dev.ID_motion_left;
	txHeader.DLC = 7;
	if (HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox) != HAL_OK)
	{
		 Error_Handler ();
	}
	txHeader.StdId = 0x200 + dev.ID_motion_right;
	if (HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox) != HAL_OK)
	{
		 Error_Handler ();
	}
	HAL_Delay(50);
	
	kinco_get_data();
	LOCALIZATION.encoder_left_last= LOCALIZATION.encoder_left;
	LOCALIZATION.encoder_right_last= LOCALIZATION.encoder_right;
}

int32_t convert_speeds(float vel)
{
	return  (int32_t)((vel*5120000*CHASSIC.gear*60)/(1875*2*Pi*CHASSIC.R_Wheel));
}

void kinco_control(DEVICE_DEF dev,DIR dir, float vel,MOTION mo)	/* mm/s */
{
	uint8_t TxData[7] = {0xFD, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	if(dir == Straigh)
	{
		if(mo == M_left)
		{
			TxData[6]	= convert_speeds(vel)>>24;
			TxData[5] = (convert_speeds(vel)&0x00ff0000)>>16;
			TxData[4] =	(convert_speeds(vel)&0x0000ff00)>>8;
			TxData[3] = convert_speeds(vel)&0x000000ff;
			
			txHeader.StdId = 0x200 + dev.ID_motion_left;
			txHeader.DLC	= 7;
			HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox);
		}
		else if(mo == M_right)
		{
			TxData[6]	= convert_speeds(-vel)>>24;
			TxData[5] = (convert_speeds(-vel)&0x00ff0000)>>16;
			TxData[4] =	(convert_speeds(-vel)&0x0000ff00)>>8;
			TxData[3] = convert_speeds(-vel)&0x000000ff;
			
			txHeader.StdId = 0x200 + dev.ID_motion_right;
			txHeader.DLC	= 7;
			HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox);
		}
		else
		{
			TxData[6]	= convert_speeds(vel)>>24;
			TxData[5] = (convert_speeds(vel)&0x00ff0000)>>16;
			TxData[4] =	(convert_speeds(vel)&0x0000ff00)>>8;
			TxData[3] = convert_speeds(vel)&0x000000ff;
			
			txHeader.StdId = 0x200 + dev.ID_motion_left;
			txHeader.DLC	= 7;
			HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox);
			
			TxData[6]	= convert_speeds(-vel)>>24;
			TxData[5] = (convert_speeds(-vel)&0x00ff0000)>>16;
			TxData[4] =	(convert_speeds(-vel)&0x0000ff00)>>8;
			TxData[3] = convert_speeds(-vel)&0x000000ff;
			
			txHeader.StdId = 0x200 + dev.ID_motion_right;
			txHeader.DLC	= 7;
			HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox);
			HAL_Delay(1);
		}
		
	}
	else if(dir == Clockwise)
	{
		TxData[6]	= convert_speeds(vel)>>24;
		TxData[5] = (convert_speeds(vel)&0x00ff0000)>>16;
		TxData[4] =	(convert_speeds(vel)&0x0000ff00)>>8;
		TxData[3] = convert_speeds(vel)&0x000000ff;
		
		txHeader.StdId = 0x200 + dev.ID_motion_left;
		txHeader.DLC	= 7;
		HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox);


		txHeader.StdId = 0x200 + dev.ID_motion_right;
		txHeader.DLC	= 7;
		HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox);
		HAL_Delay(1);
	}
	else if(dir == counter_clockwise)
	{
		TxData[6]	= convert_speeds(-vel)>>24;
		TxData[5] = (convert_speeds(-vel)&0x00ff0000)>>16;
		TxData[4] =	(convert_speeds(-vel)&0x0000ff00)>>8;
		TxData[3] = convert_speeds(-vel)&0x000000ff;
		
		txHeader.StdId = 0x200 + dev.ID_motion_left;
		txHeader.DLC	= 7;
		HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox);
		
		txHeader.StdId = 0x200 + dev.ID_motion_right;
		txHeader.DLC	= 7;
		HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox);
		HAL_Delay(1);
	}
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
	uint8_t rxData[8];
	HAL_CAN_GetRxMessage(hcan, CAN_RX_FIFO0, &rxHeader, rxData);
	if (rxHeader.DLC == 8)
	{
		LOCALIZATION.time_stamp = HAL_GetTick();
		if (rxHeader.StdId == 0x181)
		{
			LOCALIZATION.vel_left = (float)(((rxData[7]<<24)|(rxData[6]<<16)|(rxData[5]<<8)|rxData[4])*1875*(2*Pi*CHASSIC.R_Wheel)/(5120000*CHASSIC.gear*60));
			LOCALIZATION.encoder_left = (rxData[3]<<24)|(rxData[2]<<16)|(rxData[1]<<8)|rxData[0];
		}
		else if(rxHeader.StdId == 0x182)
		{
			LOCALIZATION.vel_right = (float)(((rxData[7]<<24)|(rxData[6]<<16)|(rxData[5]<<8)|rxData[4])*1875*(2*Pi*CHASSIC.R_Wheel)/(5120000*CHASSIC.gear*60));
			LOCALIZATION.encoder_right = (rxData[3]<<24)|(rxData[2]<<16)|(rxData[1]<<8)|rxData[0];
		}
	}
}

void kinco_get_data(void)
{
	txHeader.StdId = 0x80;
	txHeader.DLC	= 0;
	HAL_CAN_AddTxMessage(&hcan1, &txHeader, NULL, &TxMailbox);
	HAL_Delay(1);
}
float check;
void Odome(IMU_DATA_DEF Imu)
{
	kinco_get_data();
	int Dl_tick = abs((int)(LOCALIZATION.encoder_left-LOCALIZATION.encoder_left_last));
	int Dr_tick = abs((int)(LOCALIZATION.encoder_right-LOCALIZATION.encoder_right_last));
	 
	float DL = (Dl_tick*2.0*Pi*CHASSIC.R_Wheel)/(10000*CHASSIC.gear);
	float DR = (Dr_tick*2.0*Pi*CHASSIC.R_Wheel)/(10000*CHASSIC.gear);
	float DC =  (DL+DR)/2;
	
	check = DR;
	int Delta_X = DC*sin(LOCALIZATION.angle);
	int Delta_Y = DC*cos(LOCALIZATION.angle);
	
	LOCALIZATION.angle = (Imu.Z_axis_azimuth*Pi/180.0);
	LOCALIZATION.X = LOCALIZATION.X + Delta_X;
	LOCALIZATION.Y = LOCALIZATION.Y + Delta_Y;
	LOCALIZATION.encoder_left_last = LOCALIZATION.encoder_left;
	LOCALIZATION.encoder_right_last = LOCALIZATION.encoder_right;
}

