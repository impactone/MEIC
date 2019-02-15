#ifndef __TASK_SYSINIT_H
#define __TASK_SYSINIT_H

#include  "includes.h"
#include "BSP_ALL.h"
#include "Driver_MPU6050.h"
#include "Driver_PID.h"
#include "Driver_Motor.h"
#include "Driver_Remoter.h"
#include "Driver_RoMoudle.h"
#include "Driver_Laser.h"
#include "Driver_OLED.h"
void  Task_SysInit (void *p_arg);
void  SysModule_Init(void);

#endif
