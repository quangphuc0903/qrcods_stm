#ifndef __CAMERA_H
#define __CAMERA_H

#include <main.h>


typedef struct
{
	uint32_t ID;
	double Angle;
	double Deg;
	double X;
	double Y;
	
}Camera_Typedef;

// Khoi Tao CAMERA -- PF PGV100-F200-R4-1,5M
void CAMERA_Init(void);

// Lay Gia Tri cua CAMERA

// return:	1 lay gia trij thanh cong, data duoc update
//					0 that bai
HAL_StatusTypeDef CAMERA_Get(Camera_Typedef *CAM);

//Checksum messenger Camera
uint8_t check_messenger(uint8_t *mes,uint8_t len);

#endif
