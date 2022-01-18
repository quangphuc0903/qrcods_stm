#include <camera.h>

uint8_t data_tranfer[2]	=	{0xC8,0x37};

extern UART_HandleTypeDef huart2;
uint8_t check_messenger(uint8_t *mes,uint8_t len);

#include <agv_info.h>

float f_conver_deg2rad(double deg)
{
	if(deg>180.0)
	{
		deg = 180.0 - deg;
	}
	return (deg*Pi/180.0);
}


// Khoi Tao CAMERA -- PF PGV100-F200-R4-1,5M
void CAMERA_Init(void)
{
	HAL_GPIO_WritePin(uart2_dr_GPIO_Port,uart2_dr_Pin,GPIO_PIN_RESET);// San sang nhan du lieu
	
	HAL_GPIO_WritePin(LED_RUN_GPIO_Port, LED_RUN_Pin, GPIO_PIN_SET);
	HAL_Delay(100);
	HAL_GPIO_WritePin(LED_RUN_GPIO_Port, LED_RUN_Pin, GPIO_PIN_RESET);
	HAL_Delay(100);
}

// Lay Gia Tri cua CAMERA

// return:	1 lay gia trij thanh cong, data duoc update
//					0 that bai
HAL_StatusTypeDef CAMERA_Get(Camera_Typedef *CAM)
{
	uint8_t Response_msg[21];
	HAL_Delay(50);
	//Request CAMERA 
	HAL_GPIO_WritePin(uart2_dr_GPIO_Port,uart2_dr_Pin,GPIO_PIN_SET);
	HAL_GPIO_WritePin(LED_RUN_GPIO_Port, LED_RUN_Pin, GPIO_PIN_SET);
	HAL_UART_Transmit(&huart2,data_tranfer,2,0xff);
	HAL_GPIO_WritePin(uart2_dr_GPIO_Port,uart2_dr_Pin,GPIO_PIN_RESET);
	
	//Response CAMERA
	HAL_UART_Receive_IT(&huart2,Response_msg,21);
	
	if((Response_msg[0] == 0x00) && (check_messenger(Response_msg,20) == Response_msg[20]))
			{
				//ID
				uint64_t Tag_ID = (((uint64_t)Response_msg[8]&0x7F)<<57)|    //TAG_49 _> TAG 55
													(((uint64_t)Response_msg[9]&0x7F)<<50)|		//TAG_42 _> TAG 48
													(((uint64_t)Response_msg[12]&0x7F)<<43)|		//TAG_35 _> TAG 41
													(((uint64_t)Response_msg[13]&0x7F)<<36)|		//TAG_28 _> TAG 34
													(((uint64_t)Response_msg[14]&0x7F)<<29)|		//TAG_21 _> TAG 27
													(((uint64_t)Response_msg[15]&0x7F)<<22)|		//TAG_14 _> TAG 20
													(((uint64_t)Response_msg[16]&0x7F)<<15)|		//TAG_7 _> TAG 13
													(((uint64_t)Response_msg[17]&0x7F)<<8);					//TAG_0 _> TAG 6
								
				//angle
				uint16_t angle =	(((uint16_t)Response_msg[10]&0x7f)<<9)|
													(((uint16_t)Response_msg[11]&0x7f)<<2);	// angle
			
				//Y
				int16_t y_coor = (((int16_t)Response_msg[6]&0x7F)<<9)|
													(((int16_t)Response_msg[7]&0x7F)<<2);		// Y
				
				//X
				int32_t x_coor =	((int32_t)((Response_msg[2]&0x7F)<<29)|
									((int32_t)(Response_msg[3]&0x7F)<<22)|
									((int32_t)(Response_msg[4]&0x7f)<<15)|
									((int32_t)Response_msg[5]&0x7f)<<8); //X
				
				//Update Camera
				
				CAM->ID 		=	Tag_ID/ 256;
				CAM->Deg =	(double)angle/40;
				CAM->Angle = f_conver_deg2rad(CAM->Deg);
				CAM->Y			=	(double)y_coor/40;
				CAM->X			=	(double)x_coor/2560;
				
				
				
				HAL_GPIO_WritePin(LED_RUN_GPIO_Port, LED_RUN_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_ERROR_GPIO_Port, LED_ERROR_Pin, GPIO_PIN_RESET);
				if(check_messenger(Response_msg,20) != 0x00)
				{
					return HAL_OK;
				}
				else
				{
					return HAL_ERROR;
				}
				
			}
			else
			{
				HAL_GPIO_WritePin(LED_RUN_GPIO_Port, LED_RUN_Pin, GPIO_PIN_RESET);
				HAL_GPIO_WritePin(LED_ERROR_GPIO_Port, LED_ERROR_Pin, GPIO_PIN_SET);
				return HAL_ERROR;
			}
}


//Checksum messenger Camera
uint8_t check_messenger(uint8_t *mes,uint8_t len)
{
	uint8_t check_sum =0x00;
	for(int i =0; i<len; i++)
	{
		check_sum ^= mes[i]; // Byte[20] = XOR(byte0->byte19)
	}
	return check_sum;
}
