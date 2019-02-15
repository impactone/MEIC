#include "Task_CANTX.h"

extern OS_Q  CanMsgQue;
extern u8 suc;
void Task_CanTx(void *p_arg)
{
	(void)p_arg;
	OS_ERR err;
	OS_MSG_SIZE size;	
	CanTxMsg *TxMsg;	
  CAN_TypeDef	*CANx;
	while(1)
	{
		CANx = (CAN_TypeDef *)OSQPend(&CanMsgQue,0,OS_OPT_PEND_BLOCKING,&size,NULL,&err);		    
    TxMsg  = (CanTxMsg *)OSQPend(&CanMsgQue,0,OS_OPT_PEND_BLOCKING,&size,NULL,&err);		    
		suc = 0;
	  CAN_Transmit(CANx,TxMsg);
		while (suc== 0);		
	}	
}

