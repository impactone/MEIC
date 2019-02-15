#ifndef _DRIVER_MOTOR_H
#define _DRIVER_MOTOR_H
#include "Driver_PID.h"
#include "Driver_CAN.h"
#include "Driver_Remoter.h"
#include "Driver_Ramp.h"
#include "Driver_Monitor.h"
#include "Driver_RoMoudle.h"

void CloudYawControl(int16_t *input);
void CloudPitchControl(int16_t *input);
void BaseMotorControl (int16_t input[4]);
#endif

