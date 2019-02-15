#ifndef __DRIVER_CAN_H
#define __DRIVER_CAN_H

#include "stm32f4xx.h"
#include "includes.h"
#include "Driver_PID.h"
#include "Driver_MPU6050.h"
#include "Driver_Monitor.h"
#define RM3510_MOTOR1ID 0X201
#define RM3510_MOTOR2ID 0X202
#define RM3510_MOTOR3ID 0X203
#define RM3510_MOTOR4ID 0X204
#define RM6623_YAWID    0X205
#define RM6623_PITCHID  0X206
#define Pitch_Middle_Up 6098
#define Pitch_Middle_Down 6098
#define Yaw_Middle_East  8100
#define Yaw_Middle_West  -135
void CAN_To_Cloud(int16_t pitch,int16_t yaw);
void CAN_To_Chaise(int16_t v[4]);
void  CAN2_Process(CanRxMsg rm);
void  CAN1_Process(CanRxMsg rm);
#endif




