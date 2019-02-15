#include "Driver_Control.h"
ControlModeType ControlMode;
int16_t yawinput;
int16_t pitchinput;
extern PID_Struct PID_Yaw_P;
extern PID_Struct PID_Pitch_P;
extern DBUS_Type Remoter;
void ControlSelect(void)	
{

	  switch(1)
		{
			case 1:
			ControlMode = RM_Mode;
			break;
			case 2:
			ControlMode	= ST_Mode;
			break;
			case 3:
			ControlMode	= RM_Mode; 
			break;
			default:
			ControlMode = ST_Mode;
			break;
	  }
}
ControlModeType GetControlMode(void)
{
	 ControlSelect();
	 return ControlMode;	
}
void RM_Process(void)
{ 	              	   	   
//	 CloudYawControl(&yawinput);
//	 CloudPitchControl(&pitchinput);	
//	 if(Remoter.right_switch==1)   
//	 CAN_To_Cloud(yawinput,pitchinput);	
//	 else
//	 CAN_To_Cloud(0,0);	
}
void	ST_Process(void)
{		   						
	 
}
void  KM_Process(void)
{
	 
}
 
