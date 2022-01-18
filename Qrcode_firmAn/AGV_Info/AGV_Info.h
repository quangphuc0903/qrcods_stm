#ifndef __AGV_INFO_H
#define __AGV_INFO_H

#include  <main.h>
#define Pi	3.14159265359

	

typedef	struct
	{
		uint8_t ID_driver;
		char* type_motion;
		char* type_camera;
		char* type_imu;
		uint8_t ID_motion_left;
		uint8_t ID_motion_right;
	}DEVICE_DEF;
	
typedef	struct
	{
		float height;
		float width;
		float hight;
		float DB_Wheel;
		float R_Wheel;
		uint8_t gear;
	}CHASSIC_DEF;
typedef struct
{
	int X;
	int Y;
	double angle;
}POSE_Typedef;
typedef struct
{
	double distance;
	double angle;
}PosCalib_Typedef;

void AGV_init(DEVICE_DEF* DEV, CHASSIC_DEF* CHA);
#endif
