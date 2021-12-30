#ifndef __CAN_QR_H
#define __CAN_QR_H

#include <stm32f4xx.h>
#include <agv_info.h>
#include "stdbool.h"
#include "math.h"
/*
	value = (rpm*10000*512)/1875
	rpm = value*1875/(10000*512*30)
	m/p = (rpm*R_wheel*2*pi)
*/
typedef enum
{
	Straigh = 1,
	Clockwise =2,
	counter_clockwise=3
} DIR;

void Kinco_Init(void);
void enable_driver(DEVICE_DEF dev);
void disable_driver(void);

void kinco_control(DEVICE_DEF dev,DIR dir, float vel);
void kinco_get_data(void);

void Odome(IMU_DATA_DEF Imu);
#endif
