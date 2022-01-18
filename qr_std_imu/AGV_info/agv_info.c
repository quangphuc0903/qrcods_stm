#include <agv_info.h>

	CHASSIC_DEF CHASSIC;
	DEVICE_DEF DEVICE;
	LOCALIZATION_DEF LOCALIZATION;
void AGV_init(void)
{
	/*Init CHASSIC*/
	CHASSIC.DB_Wheel	=	470;	//milimetter
	CHASSIC.height		=	730;	//milimetter
	CHASSIC.hight			=	200;	//milimetter
	CHASSIC.width			= 530;	//milimetter
	CHASSIC.R_Wheel		= 75;		//milimetter
	CHASSIC.gear			= 20;
	/*Init DEVICE*/
	DEVICE.ID_driver 				= 0x01;
	DEVICE.ID_motion_left 	= 0x01;
	DEVICE.ID_motion_right	= 0x02;
	DEVICE.type_camera 			= "PF PGV100F200A";
	DEVICE.type_imu					= "TL720";
	DEVICE.type_motion			=	"KINCO FD124x";
	
	/*Init Local*/
	LOCALIZATION.angle			= 0.0;	//do
	LOCALIZATION.X					=	0;		//milimetter
	LOCALIZATION.Y					= 0;		//milimetter
	LOCALIZATION.vel_left		= 0.0;	// mm/s
	LOCALIZATION.vel_right	= 0.0;	// mm/s
}
