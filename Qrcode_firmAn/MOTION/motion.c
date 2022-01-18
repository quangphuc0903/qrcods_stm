#include <motion.h>
#include <math.h>
/*-------------------------------------------------------KINCO---------------------------------------------------*/
	CAN_TxHeaderTypeDef txHeader;
	CAN_RxHeaderTypeDef	rxHeader;
	uint32_t TxMailbox;
	extern CAN_HandleTypeDef hcan1;
	
	extern Motion_Typedef agv_Motion;
	extern CHASSIC_DEF agv_Chassic;


	
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
void Kinco_Enable(void)
{
	uint8_t TxData[7] = {0xfd, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00};
	txHeader.StdId  = 0x201;
	txHeader.DLC	=	7;
	if(HAL_CAN_AddTxMessage(&hcan1,&txHeader, TxData, &TxMailbox) != HAL_OK)
	{
		Error_Handler();
	}
	txHeader.StdId	 =	0x202;
	if(HAL_CAN_AddTxMessage(&hcan1,&txHeader, TxData, &TxMailbox)	!=	HAL_OK)
	{
		Error_Handler();
	}
	
	
	HAL_Delay(10);
	
	
	TxData[0] = 0x01;
	TxData[1]	=	0x00;
	txHeader.StdId	=	0x00;
	txHeader.DLC	=	7;
	if(HAL_CAN_AddTxMessage(&hcan1,&txHeader, TxData, &TxMailbox)	!=	HAL_OK)
	{
		Error_Handler();
	}
	
	
	HAL_Delay(10);
	
	TxData[0]	=	0xfd;
	TxData[1]	=	0x0f;
	txHeader.StdId = 0x201;
	txHeader.DLC = 7;
	if(HAL_CAN_AddTxMessage(&hcan1,&txHeader, TxData, &TxMailbox)	!=	HAL_OK)
	{
		Error_Handler();
	}
	txHeader.StdId	=	0x202;
	if(HAL_CAN_AddTxMessage(&hcan1,&txHeader, TxData, &TxMailbox)	!=	HAL_OK)
	{
		Error_Handler();
	}
	HAL_Delay(10);
	
	HAL_GPIO_WritePin(LED_RUN_GPIO_Port, LED_RUN_Pin,	GPIO_PIN_RESET);
	for(int i =0; i<5; i++)
	{
		HAL_GPIO_TogglePin(LED_RUN_GPIO_Port, LED_RUN_Pin);
		HAL_Delay(250);
	}
	HAL_GPIO_WritePin(LED_RUN_GPIO_Port, LED_RUN_Pin,	GPIO_PIN_RESET);
}


void Kinco_request(void)
{
	txHeader.StdId = 0x80;
	txHeader.DLC	= 0;
	HAL_CAN_AddTxMessage(&hcan1, &txHeader, NULL, &TxMailbox);
	HAL_Delay(1);
}

int32_t convert_speeds(CHASSIC_DEF *CHASSIC, double vel)
{
	return  (int32_t)((vel*5120000*agv_Chassic.gear*60)/(1875*2*Pi*agv_Chassic.R_Wheel));
}

void Kinco_control(DEVICE_DEF *dev,CHASSIC_DEF  *CHASSIC,DIR_ENUM dir, double vel,Motion motion)
{
	uint8_t TxData[7] = {0xFD, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	int32_t negative_vel	=	convert_speeds(CHASSIC, -vel);
	int32_t positive_vel	=	convert_speeds(CHASSIC,	vel);
	
	if(dir == Straight)
	{
		if(motion == M_left)
		{
			TxData[6]	= positive_vel>>24;
			TxData[5] = (positive_vel&0x00ff0000)>>16;
			TxData[4] =	(positive_vel&0x0000ff00)>>8;
			TxData[3] = positive_vel&0x000000ff;
			
			txHeader.StdId = 0x201;
			txHeader.DLC	= 7;
			HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox);
		}
		else if(motion == M_right)
		{
			TxData[6]	= convert_speeds(CHASSIC,	-vel)>>24;
			TxData[5] = (convert_speeds(CHASSIC, -vel)&0x00ff0000)>>16;
			TxData[4] =	(convert_speeds(CHASSIC, -vel)&0x0000ff00)>>8;
			TxData[3] = convert_speeds(CHASSIC,	-vel)&0x000000ff;
			
			txHeader.StdId = 0x202;
			txHeader.DLC	= 7;
			HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox);
		}
		else
		{
			TxData[6]	= positive_vel>>24;
			TxData[5] = (positive_vel&0x00ff0000)>>16;
			TxData[4] =	(positive_vel&0x0000ff00)>>8;
			TxData[3] = positive_vel&0x000000ff;
			
			txHeader.StdId = 0x201;
			txHeader.DLC	= 7;
			HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox);
			
			TxData[6]	= negative_vel>>24;
			TxData[5] = (negative_vel&0x00ff0000)>>16;
			TxData[4] =	(negative_vel&0x0000ff00)>>8;
			TxData[3] = negative_vel&0x000000ff;
			
			txHeader.StdId = 0x202;
			txHeader.DLC	= 7;
			HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox);
			HAL_Delay(1);
		}
		
	}
	else if(dir == Clockwise)
	{
		TxData[6]	= positive_vel>>24;
		TxData[5] = (positive_vel&0x00ff0000)>>16;
		TxData[4] =	(positive_vel&0x0000ff00)>>8;
		TxData[3] = positive_vel&0x000000ff;
		
		txHeader.StdId = 0x201;
		txHeader.DLC	= 7;
		HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox);


		txHeader.StdId = 0x202;
		txHeader.DLC	= 7;
		HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox);
		HAL_Delay(1);
	}
	else if(dir == Counter_Clockwise)
	{
		TxData[6]	= negative_vel>>24;
		TxData[5] = (negative_vel&0x00ff0000)>>16;
		TxData[4] =	(negative_vel&0x0000ff00)>>8;
		TxData[3] = negative_vel&0x000000ff;
		
		txHeader.StdId = 0x201;
		txHeader.DLC	= 7;
		HAL_CAN_AddTxMessage(&hcan1, &txHeader, TxData, &TxMailbox);
		
		txHeader.StdId = 0x202;
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
		if (rxHeader.StdId == 0x181)
		{
			agv_Motion.vel_left = (double)(((rxData[7]<<24)|(rxData[6]<<16)|(rxData[5]<<8)|rxData[4])*1875*(2*Pi*agv_Chassic.R_Wheel)/(5120000*agv_Chassic.gear*60));
			agv_Motion.encoder_left = (rxData[3]<<24)|(rxData[2]<<16)|(rxData[1]<<8)|rxData[0];
		}
		else if(rxHeader.StdId == 0x182)
		{
			agv_Motion.vel_right = (double)(((rxData[7]<<24)|(rxData[6]<<16)|(rxData[5]<<8)|rxData[4])*1875*(2*Pi*agv_Chassic.R_Wheel)/(5120000*agv_Chassic.gear*60));
			agv_Motion.encoder_right = (rxData[3]<<24)|(rxData[2]<<16)|(rxData[1]<<8)|rxData[0];
		}
	}
}
/*-------------------------------------------------------------CONTROL------------------------------------*/

	
void control_agv_distance(DEVICE_DEF *dev,CHASSIC_DEF *CHASSIC,IMU_Typedef *IMU,Camera_Typedef* Cam,DIR_ENUM dir, double distance, double dev_dis, double vel)
{
	// Run P2P
		Kinco_request();
		int encoder_left = agv_Motion.encoder_left;
		int count_ = agv_Motion.encoder_left;
		
		int count_encoder = ((distance*CHASSIC->gear*10000)/(2*Pi*CHASSIC->R_Wheel));
		
		int error_1 = count_encoder*dev_dis;
		int error_2 = 0;
		double pid_cal = 1;
		double vel_temp = 0;
		while(vel_temp<vel)
			{
				Kinco_control(dev,	CHASSIC,	dir,	vel_temp,	M_all);
				vel_temp += vel*0.001;
			}
			
		while(count_<(encoder_left+count_encoder))
			{
				double VEL_TEMP;
				Kinco_request();
				//IMU_GET();
				//IMU.Z_axis_azimuth = (IMU_DATA.Z_axis_azimuth*Pi/180.0);
				count_ = agv_Motion.encoder_left;
			
				
				if(count_ > (int)(encoder_left+count_encoder*dev_dis))
					{
						error_2 = encoder_left + count_encoder - count_;
						int sum_error = error_2 + error_1;
						int dev_error = error_1 - error_2;
						error_1 = error_2;
						pid_cal = (0.1*error_2 + 0.01*sum_error + 0.01*dev_error)/(2*CHASSIC->R_Wheel*100);
					}
					
				VEL_TEMP = vel*pid_cal;
				
				if(pid_cal>1)
					{
						pid_cal = 1;
						VEL_TEMP = vel*pid_cal;
					}
					
				else if(pid_cal<0.2)
				{
					if(vel>0)
					{
							if(VEL_TEMP<10)
							{
								VEL_TEMP =10;
							}
					}	
					else if(vel<0)
					{	
						if(VEL_TEMP>-10)
							{
								VEL_TEMP = -10;
							}
					}
				}
				
			Kinco_control(dev,	CHASSIC,	dir,	VEL_TEMP,	M_all);
			HAL_Delay(1);
			}
			Kinco_control(dev,	CHASSIC,	dir,	0,	M_all);
}


PosCalib_Typedef Calib_possittion(POSE_Typedef *pos,double angule_dir,double distan_pos)
{
	PosCalib_Typedef calib;
	if(angule_dir==0) 
	{
		calib.distance = sqrtl(pow(distan_pos-pos->X,2)+pow(pos->Y,2));
		calib.angle    = atanhl((distan_pos-pos->X)/(pos->Y));
	}
	else if(angule_dir == 90)
	{
		calib.distance = sqrtf(pow(pos->X,2)+pow(distan_pos+pos->Y,2));
		calib.angle    = atanhl((pos->X)/(distan_pos+pos->Y));
	}
	else if(angule_dir == 180)
	{
		calib.distance = sqrtf(pow(distan_pos+pos->X,2)+pow(pos->Y,2));
		calib.angle    = atanhl((distan_pos-pos->X)/(pos->Y));
	}
	else if(angule_dir == 270)
	{
		calib.distance = sqrtf(pow(pos->X,2)+pow(distan_pos-pos->Y,2));
		calib.angle    = atanhl((pos->X)/(distan_pos-pos->Y));
	}
}

void Control_rotate(DEVICE_DEF *dev, CHASSIC_DEF * cha,IMU_Typedef *imu,	Camera_Typedef *cam,DIR_ENUM dir, double vel, double angle_dir)
{
	double distance = (double)(cha->DB_Wheel*Pi*angle_dir)/180;
	control_agv_distance(dev,cha,imu,cam,Straight,distance,0.5,vel);
}
void Calib_rotate(DEVICE_DEF *dev, CHASSIC_DEF * cha,IMU_Typedef *imu,	Camera_Typedef *cam,DIR_ENUM dir, double vel, double angle_dir)
{
	
}