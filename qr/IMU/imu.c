#include <imu.h>
#include <modbus_master.h>
#include <main.h>
#include <agv_info.h>

IMU_DATA_DEF IMU_DATA;
void IMU_Init(void)
{
	HAL_GPIO_WritePin(dr485_GPIO_Port,dr485_Pin,GPIO_PIN_RESET);
	ModbusMaster_begin();
	/*imu set zero*/
	ModbusMaster_writeSingleRegister(1,16,255);
}
void IMU_GET(void)
{
	uint8_t result;
	uint16_t Input_Result[6];
	result = ModbusMaster_readHoldingRegisters(0x01,0x02, 6);
		if (result == 0x00)
		{
			Input_Result[0] = ModbusMaster_getResponseBuffer(0x00);
			Input_Result[1] = ModbusMaster_getResponseBuffer(0x01);
			Input_Result[2] = ModbusMaster_getResponseBuffer(0x02);
			Input_Result[3] = ModbusMaster_getResponseBuffer(0x03);
			Input_Result[4] = ModbusMaster_getResponseBuffer(0x04);
			Input_Result[5] = ModbusMaster_getResponseBuffer(0x05);
		}
		IMU_DATA.Z_axis_angular      =   (((Input_Result[0]>>8)|((Input_Result[0]&0x00ff)<<8)|((Input_Result[1]>>8)<<16)|((Input_Result[1]&0x00ff)<<24))-150000)*0.01;
		IMU_DATA.Y_axis_acceleration =   (((Input_Result[2]>>8)|((Input_Result[2]&0x00ff)<<8)|((Input_Result[3]>>8)<<16)|((Input_Result[3]&0x00ff)<<24))-20000)*0.001;
		IMU_DATA.Z_axis_azimuth      =   (((Input_Result[4]>>8)|((Input_Result[4]&0x00ff)<<8)|((Input_Result[5]>>8)<<16)|((Input_Result[5]&0x00ff)<<24))-18000)*0.01;
}

void IMU_SETZERO(void)
{
	/*imu set zero*/
	ModbusMaster_writeSingleRegister(1,16,255);
}
