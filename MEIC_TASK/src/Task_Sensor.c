#include "Task_Sensor.h"

uint16_t Temp_Val;
void Task_Sensor(void *p_arg)
{	    
	   OS_ERR err;
	   (void )p_arg;	
	   while(1)
		 {       	      			  
			OSTimeDly(20,OS_OPT_TIME_DLY,&err);
      //printf("%f\r\n",Distance.Left[0]);  
			//UltraSonic_Trigger();
	   }	
}