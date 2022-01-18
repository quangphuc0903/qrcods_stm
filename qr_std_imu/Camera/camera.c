#include <camera.h>
#include <modbus_master.h>

CAMERA GET_CAMERA(void)
{
	CAMERA cam;
	uint8_t result;
	result =	ModbusMaster_readInputRegisters(2,0,10);
	if(result == 0x00)
	{
		cam.status =	ModbusMaster_getResponseBuffer(0);
		cam.ID		 =	(((uint64_t)ModbusMaster_getResponseBuffer(2)<<48)|
									((uint64_t)ModbusMaster_getResponseBuffer(3)<<32)|
									((uint64_t)ModbusMaster_getResponseBuffer(4)<<16)|
									((uint64_t)ModbusMaster_getResponseBuffer(5)))/256;
		
		int16_t angle_cam = ModbusMaster_getResponseBuffer(6);
		
		int16_t x_coor = ModbusMaster_getResponseBuffer(7);
		
		int32_t y_coor =(ModbusMaster_getResponseBuffer(8)<<16)|
														(ModbusMaster_getResponseBuffer(8));

		cam.Angle = (float)angle_cam/40;
		cam.X			= (float)x_coor/40;
		cam.Y			= (float)y_coor/2560;
	}
	return cam;
}