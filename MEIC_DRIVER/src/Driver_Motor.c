#include "Driver_Motor.h"
 /*云台的信息初始化*/
extern PID_Struct PID_Pitch_P;
extern PID_Struct PID_Pitch_V;
extern PID_Struct PID_Yaw_P;
extern PID_Struct PID_Yaw_V;
extern Attitude CloudAttitude;
extern DBUS_Type Remoter;
u8 getyaw=1;
u8 getpitch=1;
void CloudYawControl(int16_t *input)
{			
	 if(Remoter.right_switch==1)   
	 {	 
		 if(getyaw)		 
		 {
			 PID_Yaw_P.Expect=  CloudAttitude.enc_yaw/8192.f*360;     					 
		   getyaw = 0;
		 }
		 PID_Yaw_P.Expect+= (Remoter.ch0-1024)*0.0075/600.f;	
		 PID_Yaw_P.Expect =  PID_Yaw_P.Expect>300?300:PID_Yaw_P.Expect;
		 PID_Yaw_P.Expect =  PID_Yaw_P.Expect<290?290:PID_Yaw_P.Expect;
		 PID_Yaw_P.Measured = CloudAttitude.enc_yaw/8192.f*360;
		 PID_Yaw_V.Expect = PID_Calc(&PID_Yaw_P);
		 PID_Yaw_V.Measured = CloudAttitude.gz;	 
		 *input = PID_Calc(&PID_Yaw_V);		
	 }
	 else
	 {
		 getyaw = 1;
	 }	 
}
/*Pitch轴云台*/
void CloudPitchControl(int16_t *input)
{	 
	 if(Remoter.right_switch==1)   
	 {		 
		 if(getpitch)
		 {
			 PID_Pitch_P.Expect=  CloudAttitude.enc_pitch/8192.f*360;     			
			 getpitch =0 ;
		 }
		 				
		 PID_Pitch_P.Expect+= (Remoter.ch3-1024)*0.0075/600.f;
		 PID_Pitch_P.Expect =  PID_Pitch_P.Expect>86?86:PID_Pitch_P.Expect;
		 PID_Pitch_P.Expect =  PID_Pitch_P.Expect<76?76:PID_Pitch_P.Expect;
		 PID_Pitch_P.Measured = CloudAttitude.enc_pitch/8192.f*360; 
		 PID_Pitch_V.Expect = PID_Calc(&PID_Pitch_P);
		 PID_Pitch_V.Measured = -CloudAttitude.gy;	 		  
		 *input = -PID_Calc(&PID_Pitch_V);	
	 }
	 else
	 {
		 getpitch=1;
	 }
}
/*底盘电机控制*/    
void BaseMotorControl (int16_t input[4])
{   

}

