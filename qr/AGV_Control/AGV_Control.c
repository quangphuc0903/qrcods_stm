#include <AGV_Control.h>
#include <math.h>
#include <can_qr.h>

#define Pi	3.14159265359

float error_01_angle=0, error_02_angle=0;
float sum_error_angle,del_error_angle,duty_angle;

extern LOCALIZATION_DEF LOCALIZATION;
extern IMU_DATA_DEF IMU_DATA;
extern CHASSIC_DEF CHASSIC;



float PID_cal_angle(float Kp, float Ki, float Kd)
{
	error_02_angle = LOCALIZATION.angle;
	sum_error_angle = error_01_angle + error_02_angle;
	del_error_angle = error_02_angle - error_01_angle;
	error_01_angle = error_02_angle;
	return (Kp*error_02_angle + Ki*sum_error_angle + Kd*del_error_angle);
}

bool local_is_true(LOCALIZATION_DEF local,POSE pos)
{
	if((local.X>(pos.X-5)) && (local.X<(pos.X+5)))
	{
		if((local.Y>(pos.Y-5)) && (local.Y<(pos.Y+5)))
		{
			return true;
		}
	}
		return false;
}


void control_AGV_p2p(DEVICE_DEF dev, POSE pose, float vel)
{
	while((local_is_true(LOCALIZATION, pose)!= true))
	{
		IMU_GET();
		kinco_control(dev,Straigh,vel*(1+PID_cal_angle(1,0.1,0)),M_left);
		kinco_control(dev,Straigh,vel*(1-PID_cal_angle(1,0.1,0)),M_right);
		Odome(IMU_DATA);
		HAL_Delay(50);
	}
	kinco_control(dev,Straigh,0.0,M_all);
	LOCALIZATION.X = pose.X;
	LOCALIZATION.Y= pose.Y;
	HAL_Delay(50);
}

void control_agv_distance(DEVICE_DEF dev, float distance, float dev_dis, float vel)
	{
		kinco_get_data();
		int encoder_left = LOCALIZATION.encoder_left;
		int count_ = LOCALIZATION.encoder_left;
		
		int count_encoder = ((distance*CHASSIC.gear*10000)/(2*Pi*CHASSIC.R_Wheel));
		
		int error_1 = count_encoder*dev_dis;
		int error_2 = 0;
		float pid_cal = 1;
		float vel_temp = 0;
		
		while(vel_temp<vel)
			{
				kinco_control(dev,Straigh,vel_temp,M_all);
				vel_temp = vel_temp + vel*0.01;
				HAL_Delay(10);
			}
			
		while(count_<(encoder_left+count_encoder))
			{
				kinco_get_data();
				IMU_GET();
				kinco_control(dev,Straigh,vel*pid_cal*(1+PID_cal_angle(0.1,0.01,0)),M_left);
				kinco_control(dev,Straigh,vel*pid_cal*(1-PID_cal_angle(0.1,0.01,0)),M_right);
				count_ = LOCALIZATION.encoder_left;
				
				if(count_ > (int)(encoder_left+count_encoder*dev_dis))
					{
						error_2 = encoder_left + count_encoder - count_;
						int sum_error = error_2 + error_1;
						int dev_error = error_1 - error_2;
						error_1 = error_2;
						pid_cal = (0.1*error_2 + 0.01*sum_error + 0.01*dev_error)/(2*CHASSIC.R_Wheel*100);
					}
					if(pid_cal>1)
						{
							pid_cal = 1;
						}
					else if(pid_cal <0.2)
						{
							pid_cal = 0.2;
						}
			HAL_Delay(20);
			}
			kinco_control(dev,Straigh,0,M_all);	
	}