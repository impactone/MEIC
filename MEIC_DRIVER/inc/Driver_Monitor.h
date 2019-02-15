#ifndef Driver_Monitor_H
#define Driver_Monitor_H

#include "stdint.h"
#include "includes.h"

extern uint16_t REMOTER_FRAME_RATE;
//extern uint16_t
//extern uint16_t
//extern uint16_t
#define REMOTER_FRAME_RATE       10
//#define YAW_MOTOR_FRAME_RATE     98
//#define PITCH_MOTOR_FRAME_RATE   98
//#define CHASIS_MOTOR1_FRAME_RATE 10
//#define CHASIS_MOTOR2_FRAME_RATE 10
//#define CHASIS_MOTOR3_FRAME_RATE 10
//#define CHASIS_MOTOR4_FRAME_RATE 10
extern uint8_t RemoterFrameRate            ;
extern uint8_t YawMotorFrameRate           ;
extern uint8_t PitchMotorFrameRate         ;
extern uint8_t BaseMotor1FrameRate       ;
extern uint8_t BaseMotor2FrameRate       ;
extern uint8_t BaseMotor3FrameRate       ;
extern uint8_t BaseMotor4FrameRate       ;
extern uint8_t LiftMotor1FrameRate       ;
extern uint8_t LiftMotor2FrameRate       ;
extern uint8_t LiftMotor3FrameRate       ; 
void Flag_Reint(void);
void Monitor_Process(void);

#endif
