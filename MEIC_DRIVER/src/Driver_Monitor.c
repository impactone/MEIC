#include "Driver_Monitor.h"
#include "Driver_Remoter.h"
extern uint8_t Remoter_Status;
extern uint8_t Remoter_Prestatus;
 uint8_t RemoterFrameRate          ;
 uint8_t YawMotorFrameRate         ;
 uint8_t PitchMotorFrameRate       ;
 uint8_t BaseMotor1FrameRate       ;
 uint8_t BaseMotor2FrameRate       ;
 uint8_t BaseMotor3FrameRate       ;
 uint8_t BaseMotor4FrameRate       ; 
 uint8_t LiftMotor1FrameRate       ;
 uint8_t LiftMotor2FrameRate       ;
 uint8_t LiftMotor3FrameRate       ; 
void Monitor_Process(void)
{    	
//	if(RemoterFrameRate<REMOTER_FRAME_RATE)
//	{		  
//		  DBUS_Init();	
//      Remoter_Status = 0;
//		  Remoter_Prestatus = 0;
//	}
	//uint8_t Monitor_Flag;
}
void Flag_Reint(void)
{
	PitchMotorFrameRate=0;
	RemoterFrameRate=0;
	YawMotorFrameRate=0;
	BaseMotor1FrameRate=0;
	BaseMotor2FrameRate=0;
	BaseMotor3FrameRate=0;
	BaseMotor4FrameRate=0;
	LiftMotor1FrameRate=0;
  LiftMotor2FrameRate=0;
  LiftMotor3FrameRate=0; 
}
