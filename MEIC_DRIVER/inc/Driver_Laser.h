#ifndef __DRIVER_LASER_H
#define __DRIVER_LASER_H

#include "stm32f4xx.h"

#define filterNum 10
#define isTooLong GPIO_ReadInputDataBit(GPIOC,GPIO_Pin_5)

void Laser_Init(void);
u16 Get_Adc(u8 ch);
float Get_Dis(void);
float ave_Filter(u32 temp_val);

#endif
