#include "Task_DebugMsg.h"
#include "Task_SysInit.h"
extern float init_pitch;
extern Attitude CloudAttitude;
extern PID_Struct PID_Pitch_P;
extern PID_Struct PID_Pitch_V;
extern PID_Struct PID_Yaw_P;
extern PID_Struct PID_Yaw_V;
extern DBUS_Type Remoter;
extern int16_t yawinput;
extern int16_t pitchinput;
void Task_DebugMsg(void *p_arg)
{
	 OS_ERR err;	
	 (void )p_arg;
	 for (;;)
	 {		 		 
		 OSTimeDly(10,OS_OPT_TIME_DLY,&err);
		 printf("success\r\n");
	 }
}

