#ifndef __AGV_CONTROL_H
#define __AGV_CONTROL_H
#include <stm32f4xx.h>
#include <imu.h>
#include <agv_info.h>

void control_AGV_p2p(DEVICE_DEF dev, POSE pose, float vel);
float PID_cal_angle(float Kp, float Ki, float Kd);

void control_agv_distance(DEVICE_DEF dev, float distance, float dev_dis, float vel);
#endif
