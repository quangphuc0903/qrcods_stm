#ifndef __AGV_INFO_H
#define __AGV_INFO_H
#include <stm32f4xx.h>
void AGV_init(void);
void update_vel(void);

typedef	struct
	{
		float Z_axis_angular;
		float Y_axis_acceleration;
		float Z_axis_azimuth;
	} IMU_DATA_DEF;
	
typedef	struct
	{
		int		X;
		int		Y;
		float angle;
		float vel_left;
		float vel_right;
		int32_t encoder_left;
		int32_t encoder_right;
		int32_t encoder_left_last;
		int32_t encoder_right_last;
		int32_t time_stamp;
	} LOCALIZATION_DEF;
	
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
	char* label;
	int X;
	int Y;
	float angle;
}POSE;

#endif
