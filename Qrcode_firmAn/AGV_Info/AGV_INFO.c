#include <agv_info.h>


void AGV_init(DEVICE_DEF* DEV, CHASSIC_DEF* CHA)
{
	/*Init CHASSIC*/
	CHA->DB_Wheel	=	463.4;	//milimetter
	CHA->height		=	730;	//milimetter
	CHA->hight			=	200;	//milimetter
	CHA->width			= 530;	//milimetter
	CHA->R_Wheel		= 75;		//milimetter
	CHA->gear			= 20; 
	/*Init DEVICE*/
	DEV->ID_driver 				= 0x01;
	DEV->ID_motion_left 	= 0x01;
	DEV->ID_motion_right	= 0x02;
	DEV->type_camera 			= "PF PGV100F200A";
	DEV->type_imu					= "TL720";
	DEV->type_motion			=	"KINCO FD124x";
}
