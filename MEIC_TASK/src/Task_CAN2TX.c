#include "Task_CAN2TX.h"

extern OS_Q  Can2MsgQue;
extern u8 suc2;
void Task_Can2Tx(void *p_arg)
{
	(void)p_arg;
	OS_ERR err;
	OS_MSG_SIZE size;	
	CanTxMsg *TxMsg;	                      
	while(1)
	{	 		
    TxMsg = (CanTxMsg *)OSQPend(&Can2MsgQue,0,OS_OPT_PEND_BLOCKING,&size,NULL,&err);		
    suc2 = 0;		 	
	  CAN_Transmit(CAN2,TxMsg);
		while (suc2==0);		
	}	
}

