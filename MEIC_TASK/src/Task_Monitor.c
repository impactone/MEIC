#include "Task_Monitor.h"
void Task_Monitor(void *p_arg)
{
	  (void)p_arg;
	  OS_ERR err;	
    while(1)
		{
      OSTimeDly(200,OS_OPT_TIME_DLY,&err); 				
      Monitor_Process();	
			Flag_Reint();
		}			
}