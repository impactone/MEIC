#ifndef _DRIVER_CONTROL_H
#define _DRIVER_CONTROL_H
#include "stm32f4xx_it.h"
#include "Driver_Remoter.h"
#include "Driver_Motor.h"
#include "Driver_RoMoudle.h"
typedef enum {ST_Mode,RM_Mode,KM_Mode} ControlModeType;
void ControlSelect(void);
ControlModeType GetControlMode(void);
void ST_Process(void);
void RM_Process(void);
void KM_Process(void);
#endif



