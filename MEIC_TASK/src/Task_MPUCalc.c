#include "Task_IMUCalc.h"
extern Attitude CloudAttitude;
extern int GyroOffset[3];
u8 init_finished  =1;
extern OS_SEM GyroDRY_Sem;

void Task_IMU(void *p_arg)
{
	  OS_ERR err;	
	  (void )p_arg;		
    float init_pitch=  0;
    float init_roll = 0;
		float last_pitch = 0;
    float last_roll  = 0;
    float last_yaw   = 0;		
		for (;;)
	  {
		  OSSemPend(&GyroDRY_Sem,0,OS_OPT_PEND_BLOCKING,NULL,&err);				
			IMU_GetPitchRollYaw(&CloudAttitude);	
		  if(fabs(last_pitch - CloudAttitude.pitch) <= 0.001 && \
			   fabs(last_roll  - CloudAttitude.roll)  <= 0.001 && \
		     fabs(last_yaw   - CloudAttitude.yaw)   <= 0.001&&init_finished)
        {			
  		    init_roll	 = CloudAttitude.roll ;
    			init_pitch =  CloudAttitude.pitch;     
					init_finished	 = 0;
			  }	
          if(init_finished)				  
	        {
					last_pitch = CloudAttitude.pitch;
		      last_roll  = CloudAttitude.roll;	
					}						
				if(!init_finished)
	 			{
					CloudAttitude.pitch = CloudAttitude.pitch-init_pitch;
					CloudAttitude.roll = CloudAttitude.roll- init_roll;
					
				}					
		      OSTimeDly(5,OS_OPT_TIME_DLY,&err); 
	  }	
}


