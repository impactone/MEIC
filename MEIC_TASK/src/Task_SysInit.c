#include "Task_SysInit.h"
#include "Task_DebugMsg.h"
#include "Task_CANTX.h"
#include "Task_IMUCalc.h"
#include "Task_Control.h"
#include "Task_Monitor.h"
#include "Task_Sensor.h"
/*任务优先级定义*/ 
#define  TASK_IMU_PRIO            5u
#define  TASK_Control_PRIO        7u
#define  TASK_DEBUGMSG_PRIO       12u
#define  TASK_CAN_PRIO            6u


/*任务堆栈及大小*/
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
/*任务控制块*/
OS_TCB  Task_DebugMsgTCB;
OS_TCB  Task_CANTCB;
OS_TCB  Task_IMUTCB;
OS_TCB  Task_ControlTCB;
OS_TCB  Task_MonitorTCB;
OS_TCB  TASK_SensorTCB;
OS_Q  CanMsgQue;

OS_SEM GyroDRY_Sem;
/*两块主控板，一块是传感器控制，一块运动控制，CAN2通信*/
/*开始任务*/	
void  Task_SysInit (void *p_arg)
{
    OS_ERR  err;
	  CPU_SR_ALLOC();	 
   (void)p_arg;
	 /*创建消息队列*/ 
	  OSQCreate(&CanMsgQue,"CanMsgQue",10,&err);    
	 /*系统各模块初始化*/
	  SysModule_Init();  
	/*经验：板子初始化与外设初始化之间延时,系统原因>800ms*/    	 
	  OSTimeDly(800,OS_OPT_TIME_DLY,&err);	
   /*参数初始化*/	  
	  ALL_PID_Init();    
	  DBUS_Init();
	  ALL_Ramp_Init();	  	  
	 /*单片机内设初始化*/
	  BSP_ALL_Init();	   	   
	  
	 /*陀螺仪初始化*/	  	
	  MPU6050_Init();
	 /*进入临界区*/
	  OS_CRITICAL_ENTER();	
    OSSemSet(&GyroDRY_Sem,0,&err);		
	 /*创建调试任务*/
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
				 /*CAN通信发送任务*/
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
//					/*陀螺仪*/
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
				/*控制*/
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
		 /*退出临界区*/
		 OS_CRITICAL_EXIT();	
	   /*不再调用*/
	   OSTaskDel(NULL , & err);
}

/*系统各项功能初始化*/
void SysModule_Init(void)
{
	OS_ERR  err;
	  
	  /*CPU配置初始化*/
	  CPU_Init();   
	
    /*初始化SysTick定时器*/	
    OS_CPU_SysTickInit(BSP_CPU_ClkFreq()
	                     /(CPU_INT32U)OSCfg_TickRate_Hz);   
	
	  /*内存管理初始化*/
//	  Mem_Init();     
	
#if OS_CFG_STAT_TASK_EN > 0u
	  /*开启统计任务*/
    OSStatTaskCPUUsageInit(&err);                       
#endif

#ifdef CPU_CFG_INT_DIS_MEAS_EN
    CPU_IntDisMeasMaxCurReset();
#endif
	
#if OS_CFG_SCHED_ROUND_ROBIN_EN
    /*开启时间片调度*/
		OSSchedRoundRobinCfg((CPU_BOOLEAN )DEF_ENABLED,
												 (OS_TICK     )0,
												 (OS_ERR     *)&err);
#endif	
	
}
