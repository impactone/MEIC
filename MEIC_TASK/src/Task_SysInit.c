#include "Task_SysInit.h"
#include "Task_DebugMsg.h"
#include "Task_CANTX.h"
#include "Task_IMUCalc.h"
#include "Task_Control.h"
#include "Task_Monitor.h"
#include "Task_Sensor.h"
/*�������ȼ�����*/ 
#define  TASK_IMU_PRIO            5u
#define  TASK_Control_PRIO        7u
#define  TASK_DEBUGMSG_PRIO       12u
#define  TASK_CAN_PRIO            6u


/*�����ջ����С*/
#define  TASK_DEBUGMSG_STK_SIZE   256u
#define  TASK_SENSOR_STK_SIZE  256u
#define  TASK_CAN_STK_SIZE        256u
#define  TASK_IMU_STK_SIZE        256u
#define  TASK_CONTROL_STK_SIZE    256u
#define  TASK_MONITOR_STK_SIZE    256u
CPU_STK  Task_DebugMsgStk[TASK_DEBUGMSG_STK_SIZE];
CPU_STK  Task_CANTxStk[TASK_CAN_STK_SIZE];
CPU_STK  Task_IMUStk[TASK_IMU_STK_SIZE];
CPU_STK  Task_ControlStk[TASK_CONTROL_STK_SIZE];
CPU_STK  Task_MonitorStk[TASK_MONITOR_STK_SIZE];
CPU_STK   TASK_SensorStk[TASK_SENSOR_STK_SIZE];
/*������ƿ�*/
OS_TCB  Task_DebugMsgTCB;
OS_TCB  Task_CANTCB;
OS_TCB  Task_IMUTCB;
OS_TCB  Task_ControlTCB;
OS_TCB  Task_MonitorTCB;
OS_TCB  TASK_SensorTCB;
OS_Q  CanMsgQue;

OS_SEM GyroDRY_Sem;
/*�������ذ壬һ���Ǵ��������ƣ�һ���˶����ƣ�CAN2ͨ��*/
/*��ʼ����*/	
void  Task_SysInit (void *p_arg)
{
    OS_ERR  err;
	  CPU_SR_ALLOC();	 
   (void)p_arg;
	 /*������Ϣ����*/ 
	  OSQCreate(&CanMsgQue,"CanMsgQue",10,&err);    
	 /*ϵͳ��ģ���ʼ��*/
	  SysModule_Init();  
	/*���飺���ӳ�ʼ���������ʼ��֮����ʱ,ϵͳԭ��>800ms*/    	 
	  OSTimeDly(800,OS_OPT_TIME_DLY,&err);	
   /*������ʼ��*/	  
	  ALL_PID_Init();    
	  DBUS_Init();
	  ALL_Ramp_Init();	  	  
	 /*��Ƭ�������ʼ��*/
	  BSP_ALL_Init();	   	   
	  
	 /*�����ǳ�ʼ��*/	  	
	  MPU6050_Init();
	 /*�����ٽ���*/
	  OS_CRITICAL_ENTER();	
    OSSemSet(&GyroDRY_Sem,0,&err);		
	 /*������������*/
		OSTaskCreate((OS_TCB       *)&Task_DebugMsgTCB,         
							 (CPU_CHAR     *)"DebugMsg Task",
							 (OS_TASK_PTR   )Task_DebugMsg,
							 (void         *)0u,
							 (OS_PRIO       )TASK_DEBUGMSG_PRIO,
							 (CPU_STK      *)Task_DebugMsgStk,
							 (CPU_STK_SIZE  )TASK_DEBUGMSG_STK_SIZE / 10u,
							 (CPU_STK_SIZE  )TASK_DEBUGMSG_STK_SIZE,
							 (OS_MSG_QTY    )0u,
							 (OS_TICK       )0u,
							 (void         *)0u,
							 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
							 (OS_ERR       *)&err);
				 /*CANͨ�ŷ�������*/
//			OSTaskCreate((OS_TCB       *)&Task_CANTCB,         
//								 (CPU_CHAR     *)"CANTx Task",
//								 (OS_TASK_PTR   ) Task_CanTx,
//								 (void         *)0u,
//								 (OS_PRIO       )TASK_CAN_PRIO,
//								 (CPU_STK      *)Task_CANTxStk,
//								 (CPU_STK_SIZE  )TASK_CAN_STK_SIZE / 10u,
//								 (CPU_STK_SIZE  )TASK_CAN_STK_SIZE,
//								 (OS_MSG_QTY    )0u,
//								 (OS_TICK       )0u,
//								 (void         *)0u,
//								 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
//								 (OS_ERR       *)&err);
//					/*������*/
		OSTaskCreate((OS_TCB      *)&Task_IMUTCB,         
							 (CPU_CHAR     *)"IMU Task",
							 (OS_TASK_PTR   )Task_IMU,
							 (void         *)0u,
							 (OS_PRIO       )TASK_IMU_PRIO,
							 (CPU_STK      *)Task_IMUStk,
							 (CPU_STK_SIZE  )TASK_IMU_STK_SIZE / 10u,
							 (CPU_STK_SIZE  )TASK_IMU_STK_SIZE,
							 (OS_MSG_QTY    )0u,
							 (OS_TICK       )0u,
							 (void         *)0u,
							 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
							 (OS_ERR       *)&err);
				/*����*/
	 OSTaskCreate((OS_TCB      *)&Task_ControlTCB,         
							 (CPU_CHAR     *)"Control Task",
							 (OS_TASK_PTR   )Task_Control,
							 (void         *)0u,
							 (OS_PRIO       )TASK_Control_PRIO,
							 (CPU_STK      *)Task_ControlStk,
							 (CPU_STK_SIZE  )TASK_CONTROL_STK_SIZE / 10u,
							 (CPU_STK_SIZE  )TASK_CONTROL_STK_SIZE,
							 (OS_MSG_QTY    )0u,
							 (OS_TICK       )0u,
							 (void         *)0u,
							 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR),
							 (OS_ERR       *)&err);
		 /*�˳��ٽ���*/
		 OS_CRITICAL_EXIT();	
	   /*���ٵ���*/
	   OSTaskDel(NULL , & err);
}

/*ϵͳ����ܳ�ʼ��*/
void SysModule_Init(void)
{
	OS_ERR  err;
	  
	  /*CPU���ó�ʼ��*/
	  CPU_Init();   
	
    /*��ʼ��SysTick��ʱ��*/	
    OS_CPU_SysTickInit(BSP_CPU_ClkFreq()
	                     /(CPU_INT32U)OSCfg_TickRate_Hz);   
	
	  /*�ڴ�����ʼ��*/
//	  Mem_Init();     
	
#if OS_CFG_STAT_TASK_EN > 0u
	  /*����ͳ������*/
    OSStatTaskCPUUsageInit(&err);                       
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif
	
#if OS_CFG_SCHED_ROUND_ROBIN_EN
    /*����ʱ��Ƭ����*/
		OSSchedRoundRobinCfg((CPU_BOOLEAN )DEF_ENABLED,
												 (OS_TICK     )0,
												 (OS_ERR     *)&err);
#endif	
	
}
