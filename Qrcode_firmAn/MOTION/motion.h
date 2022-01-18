#ifndef __MOTION_H
#define __MOTION_H

#include <main.h>
#include <agv_info.h>
#include <imu.h>
#include <motion.h>
#include <camera.h>
/*--------------------------------------------------------------KINCO-------------------------------------------------*/

typedef enum
{
	Straight	=	1,
	Clockwise	= 2,
	Counter_Clockwise =3,
}DIR_ENUM;

typedef enum
{
	M_left	=	1,
	M_right	=	2,
	M_all		=	3,
}Motion;

typedef struct
{
	double vel_left;
	double vel_right;
	int32_t encoder_left;
	int32_t encoder_right;
 
}Motion_Typedef;

void Kinco_Init(void);
void Kinco_Enable(void);
void Kinco_Disable(void);

void Kinco_request(void);
void Kinco_control(DEVICE_DEF *dev,CHASSIC_DEF  *CHASSIC,DIR_ENUM dir, double vel,Motion motion);

/*-------------------------------------------------------------CONTROL-----------------------------------------*/

double PID_cal_angle(double Kp, double Ki, double Kd, IMU_Typedef *IMU, double angle_setpoint);

void control_agv_distance(DEVICE_DEF *dev,CHASSIC_DEF *CHASSIC,IMU_Typedef *IMU,Camera_Typedef* Cam,DIR_ENUM dir, double distance, double dev_dis, double vel);
void Control_rotate(DEVICE_DEF *dev, CHASSIC_DEF * cha,IMU_Typedef *imu,	Camera_Typedef *cam,DIR_ENUM dir, double vel, double angle_dir);

#endif
