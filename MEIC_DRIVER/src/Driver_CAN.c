#include "Driver_CAN.h"
extern OS_Q  CanMsgQue;
extern OS_Q  Can2MsgQue;
extern Attitude CloudAttitude;
#define h8b(x) (x>>8)
#define l8b(x) ((x)&0xff)
void CAN_To_Cloud(int16_t yaw,int16_t pitch)
{
	  OS_ERR   err;	
		static CanTxMsg TxMsg;	  
		TxMsg.StdId = 0x1ff;
		TxMsg.IDE = CAN_Id_Standard;
		TxMsg.RTR = CAN_RTR_Data;
		TxMsg.DLC = 0x08;
		TxMsg.Data[0] = h8b(yaw);
		TxMsg.Data[1] = l8b(yaw);
		TxMsg.Data[2] = h8b(pitch);
		TxMsg.Data[3] = l8b(pitch);	  	  	  	  	  	 	  	 
	  TxMsg.Data[4] = 0;
		TxMsg.Data[5] = 0;
		TxMsg.Data[6] = 0;
		TxMsg.Data[7] = 0;	
	  OSQPost(&CanMsgQue,CAN1,sizeof(*CAN1),OS_OPT_POST_FIFO,&err);  
	  OSQPost(&CanMsgQue,&TxMsg,sizeof(TxMsg),OS_OPT_POST_FIFO,&err);
	  
}
void  CAN2_Process(CanRxMsg RxMsg)
{	
	
}
void  CAN1_Process(CanRxMsg RxMsg)
{
  switch (RxMsg.StdId)
	{
		case RM6623_YAWID:		
      CloudAttitude.enc_yaw = (int16_t)(RxMsg.Data[0]<<8 | RxMsg.Data[1]);			
			break; 		
		case RM6623_PITCHID:
			CloudAttitude.enc_pitch = (int16_t)(RxMsg.Data[0]<<8 | RxMsg.Data[1]);			
			break;
		default:			
			break;
	}
}
