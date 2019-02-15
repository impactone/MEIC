#include "Task_Control.h"

void Task_Control(void *p_arg)
{
	 (void)p_arg;
	 OS_ERR err;	
	 while(1)
   { 
		 OSTimeDly(5,OS_OPT_TIME_DLY,&err); 
		 switch(GetControlMode())
		 {			 
			 case ST_Mode:
				  	ST_Process();			 
			 break; 
			 case RM_Mode:
			      RM_Process();			 
			 break;
			 case KM_Mode:
				    KM_Process();
			 break; 
			 default:
			 break;
		 }
		  
	 }
	 
}
		 


