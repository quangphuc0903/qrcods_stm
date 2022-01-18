#ifndef __CONTROL_H
#define __CONTROL_H

#include <main.h>

typedef struct
{
	int16_t ID;
	double  distance;
	double 	radian;
}CONTROL_Queue;

CONTROL_Queue Control_page[8];

void Control_Add(CONTROL_Queue* Control_page, CONTROL_Queue control_adder);
void Control_Removce(CONTROL_Queue* Control_page, uint16_t Postion);
#endif
