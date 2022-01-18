#ifndef __IMU_H
#define __IMU_H

typedef struct
{
	double	Z_axis_angular;
	double	Y_axis_acceleration;
	double	Z_axis_azimuth;
}IMU_Typedef;


void IMU_Init(void);
void IMU_GET(IMU_Typedef *imu);
void IMU_SETZERO(void);
#endif
