#ifndef __CAMERA_H
#define __CAMERA_H
#include <main.h>


typedef struct
{
	uint8_t status;
	uint64_t ID;
	float Angle;
	float X;
	float Y;
}CAMERA;

CAMERA GET_CAMERA(void);

#endif