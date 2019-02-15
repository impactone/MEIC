#include "Driver_Remoter.h"
DBUS_Type Remoter;
/*|1 1 1 1 left right 1 1 1 1|*/
uint8_t Remoter_Status = 0;
uint8_t Remoter_Prestatus = 0;
void DBUS_Init(void)
{
	Remoter.ch0 = 1024;
	Remoter.ch1 = 1024;
	Remoter.ch2 = 1024;
	Remoter.ch3 = 1024;
	Remoter.left_switch = 0;
	Remoter.right_switch = 0;

	Remoter.mouse.x = 0;
	Remoter.mouse.y = 0;
	Remoter.mouse.z = 0;
	Remoter.mouse.l = 0;
	Remoter.mouse.r = 0;
	Remoter.key = 0;
	Remoter.res = 0;
	Remoter.Key.Key_W = 0;
  Remoter.Key.Key_S = 0;
  Remoter.Key.Key_A = 0;
  Remoter.Key.Key_D = 0;
  Remoter.Key.Key_Q = 0;
  Remoter.Key.Key_E = 0;
	Remoter.Key.Shift  = 0;
	Remoter.Key.Ctrl = 0;
}
int16_t GetRemoterMsg(Receive_Type code)
{
	int16_t tmp = 0;
	switch (code)
	{
		case CH0:
			tmp = Remoter.ch0;
			break;
		case CH1:
			tmp = Remoter.ch1;
			break;
		case CH2:
			tmp = Remoter.ch2;
			break;
		case CH3:
			tmp = Remoter.ch3;
			break;
		case LEFT_SWITCH:
			tmp = Remoter.left_switch;
			break;
		case RIGHT_SWTICH:
			tmp = Remoter.right_switch;
			break;
		case MOUSE_X:
			tmp = Remoter.mouse.x;
			break;
		case MOUSE_Y:
			tmp = Remoter.mouse.y;
			break;
		case MOUSE_Z:
			tmp = Remoter.mouse.z;
			break;
		case MOUSE_L:
			tmp = Remoter.mouse.l;
			break;
		case MOUSE_R:
		  tmp = Remoter.mouse.r;
		  break;
		case KEY:
			tmp = Remoter.key;
			break;	
		default:
			break;
	}
	return tmp;
}

/*遥控器解码*/
int16_t  Quick_Shift = 0;
int16_t  Quick_Lift = 0;
uint8_t  Follow_Switch = 0 ;
float	 Pitch_Shift = 0; //float型为了键鼠和遥控切换而生
float  Yaw_Shift = 0;
int16_t	 Xaxes_Shift = 0 ;
int16_t  Yaxes_Shift = 0;
uint8_t  Up = 0;
uint8_t  Down = 0;
uint8_t  Follow = 0;
int16_t Rotate = 0;
uint8_t  Fricmotor =0;
uint8_t  View_Conversion = 0;
uint8_t  Rview = 0;
uint8_t  Lview = 0;
uint8_t  Uview = 0;
uint8_t  Dview = 0;
uint8_t Takebomb_Cmd = 0;
uint8_t Liftup_Cmd = 0;
uint8_t Liftdown_Cmd = 0;
uint8_t Cylinderback_Cmd = 0;
uint8_t LiftReset_Cmd ;
uint8_t Rotate_Flag =0;
uint8_t Fric_Cmd = 0;
uint8_t Win_Cmd = 0;
uint8_t  Shoot= 0;
uint8_t  Start = 0;
int  Click = 0;
uint8_t  Full_Speed_Back = 0;
uint8_t  Full_Speed_Front = 0;
uint8_t  Half_Speed = 0;
void DBUS_Decode(void)
{   
 	static uint8_t  Key_F_Loose = 1;  
	static uint8_t  Key_V_Loose = 1;
	static uint8_t  Key_Q_Loose = 1;
	static uint8_t  Key_E_Loose = 1;
	static uint8_t  Key_B_Loose = 1;	
	static uint8_t  Key_R_Loose = 1;	
	static uint8_t  Takebomb_Cmd_Flag= 0;
	Remoter.ch0 = (DBUS_BUFF[0] | (DBUS_BUFF[1] << 8)) & 0x07ff;
	Remoter.ch1 = ((DBUS_BUFF[1] >> 3) | (DBUS_BUFF[2] << 5)) & 0x07ff;
	Remoter.ch2 = ((DBUS_BUFF[2] >> 6) | (DBUS_BUFF[3] << 2) | (DBUS_BUFF[4] << 10)) & 0x07ff;
	Remoter.ch3 = ((DBUS_BUFF[4] >> 1) | (DBUS_BUFF[5] << 7)) & 0x07ff;
	Remoter.left_switch  = ((DBUS_BUFF[5] >> 4) & 0x000C) >> 2;  
	Remoter.right_switch = ((DBUS_BUFF[5] >> 4) & 0x0003); 	
	Remoter.mouse.x = DBUS_BUFF[6]  | (DBUS_BUFF[7] << 8); 
	Remoter.mouse.y = DBUS_BUFF[8]  | (DBUS_BUFF[9] << 8);
	Remoter.mouse.z = DBUS_BUFF[10] | (DBUS_BUFF[11] << 8);
	Remoter.mouse.l = DBUS_BUFF[12];
	Remoter.mouse.r = DBUS_BUFF[13];	
	Remoter.key = DBUS_BUFF[14] | (DBUS_BUFF[15] << 8);		
  Remoter.Key.Key_W =   Remoter.key&0x0001; //!< Bit 0
  Remoter.Key.Key_S =  (Remoter.key&0x0002)>>1; //!< Bit 1
  Remoter.Key.Key_A =  (Remoter.key&0x0004)>>2; //!< Bit 2
  Remoter.Key.Key_D =  (Remoter.key&0x0008)>>3; //!<Bit 3
  Remoter.Key.Shift =  (Remoter.key&0x0010)>>4; //!< Bit 4
  Remoter.Key.Ctrl =   (Remoter.key&0x0020)>>5; //!< Bit 5
	Remoter.Key.Key_Q =  (Remoter.key&0x0040)>>6;//!< Bit 6
	Remoter.Key.Key_E =  (Remoter.key&0x0080)>>7;//!< Bit 7 
	Remoter.Key.Key_R=   (Remoter.key&0x0100)>>8;//!< Bit 8
	Remoter.Key.Key_F =  (Remoter.key&0x0200)>>9;//!< Bit 9 
	Remoter.Key.Key_G =  (Remoter.key&0x0400)>>10;//!< Bit 10 
	Remoter.Key.Key_Z =  (Remoter.key&0x0800)>>11;//!< Bit 11 
	Remoter.Key.Key_X =  (Remoter.key&0x1000)>>12;//!< Bit 12
	Remoter.Key.Key_C =  (Remoter.key&0x2000)>>13;//!< Bit 	13
	Remoter.Key.Key_V =  (Remoter.key&0x4000)>>14;//!< Bit 14
	Remoter.Key.Key_B =  (Remoter.key&0x8000)>>15;//!< Bit 15
	Remoter_Status = Remoter.right_switch |(Remoter.left_switch<<4);	
	if(Remoter.Key.Key_X&&Remoter.Key.Ctrl)
	{
		Takebomb_Cmd =1;
		Takebomb_Cmd_Flag =1;
	}
	else
	{
		Takebomb_Cmd =0;
	}
	if(Remoter.Key.Key_G&&Remoter.Key.Key_S)
	{
		Full_Speed_Back = 1;		
	}
	else
	{		
		Full_Speed_Back = 0;		
	}
	if(Remoter.Key.Key_G&&Remoter.Key.Key_W)		
	{
		Full_Speed_Front = 1;		
	}
	else
	{
		Full_Speed_Front = 0;		
	}
	if(Remoter.Key.Key_X&&Remoter.Key.Key_W)
	{
		Liftup_Cmd = 1;
	}
	else
	{
		Liftup_Cmd = 0;
	}
	if(Remoter.Key.Key_X&&Remoter.Key.Key_S)
	{
		Liftdown_Cmd = 1;
	}
	else
	{
		Liftdown_Cmd = 0;
	}
	if(Presskey_Judge(Remoter.Key.Key_R,&Key_R_Loose))
	{ 
		Half_Speed =!Half_Speed;
	}
	if(Presskey_Judge(Remoter.Key.Key_B,&Key_B_Loose))
	{  	 
		LiftReset_Cmd=!LiftReset_Cmd;		
		Takebomb_Cmd_Flag = 0;
  }		
  if(Remoter.mouse.l)
  {
		Start = 1;
		Fricmotor = 1;
		if(Click>1000&&Start)
		{
			 Shoot  =1 ;
		}
		if(Takebomb_Cmd_Flag)
		{
		   Fric_Cmd = 1;  
			 Fricmotor = 0;
			 Shoot  = 0;
		}
		else
		{  			
			 Fric_Cmd = 0;  
		}			
	}
	else
	{
		 Shoot  =0;
	}
	if(Remoter.mouse.r)
	{			
		Start = 0;
    if(Takebomb_Cmd_Flag)
		{
			Win_Cmd = 1; 		  
		}
		else
		{
			Win_Cmd = 0; 
		}
	  Fricmotor =0;
		Click  = 0;
	}
	if(Remoter.Key.Shift)
	{			   
		if(Remoter.Key.Key_S||Remoter.Key.Key_A||Remoter.Key.Key_W||Remoter.Key.Key_D)
		{
			Quick_Shift += 20;				
		}	
	}  
  if(!Remoter.Key.Key_W&&!Remoter.Key.Key_D&&!Remoter.Key.Key_A&&!Remoter.Key.Key_S)
	{
		  Quick_Shift = 0;
	}     
	Pitch_Shift+=Remoter.mouse.y; 
  Yaw_Shift  += Remoter.mouse.x;
  Limit_Input(&Pitch_Shift,400);    						
	if(!Remoter.Key.Key_C&&!Remoter.Key.Key_V&&!Remoter.Key.Key_B&&!Remoter.Key.Key_Q&&!Remoter.Key.Key_E&&!Remoter.Key.Key_X&&!Remoter.Key.Key_Z)
	{
	  Yaxes_Shift = (Standard_Speed+Quick_Shift)*(Remoter.Key.Key_S-Remoter.Key.Key_W);
	  Xaxes_Shift = (Standard_Speed+Quick_Shift)*(Remoter.Key.Key_A-Remoter.Key.Key_D);
	}
	else		
	{
		Yaxes_Shift  = 0;
		Xaxes_Shift  = 0;
	}	
	Follow_Switch = Presskey_Judge(Remoter.Key.Key_F,&Key_F_Loose);	  		
	if(Presskey_Judge(Remoter.Key.Key_V,&Key_V_Loose))
	{
		View_Conversion =!View_Conversion;
	}
	if(Presskey_Judge(Remoter.Key.Key_E,&Key_E_Loose))		
	{  
		Rotate++;
		Rotate_Flag =1;
	}
  if(Presskey_Judge(Remoter.Key.Key_Q,&Key_Q_Loose))		
	{  
		Rotate--;
		Rotate_Flag =1;
	}	
	if(View_Conversion)
	{
	  if(Remoter.Key.Key_Z&&Remoter.Key.Key_A)
		{
			 Lview = 1;
		}
		else
		{
			Lview = 0;
		}
		if(Remoter.Key.Key_Z&&Remoter.Key.Key_D)
		{
			Rview = 1;
		}
		else
		{
			Rview = 0;
		}
		
		if(Remoter.Key.Key_Z&&Remoter.Key.Key_S)
		{
			Dview = 1;
		}	
		else
		{
			Dview = 0;
		}
	  
		if(Remoter.Key.Key_Z&&Remoter.Key.Key_W)
		{
			Uview = 1;
		}
		else
		{
			Uview = 0;
		}		
	}
	if(Follow_Switch)
	{
		Follow=!Follow;		
	}	
}
uint8_t Judge(uint8_t x,uint8_t y) 
{
	if(x!=0)
	{
	 if(((y&0x0f)==0)||((y&0xf0)==0))
   {	    
		 return ((x&0x0f)==y||(x&0xf0)==y)?1:0;
	 }
	 else
	 {		 
		 return (x==y)?1:0;
	 }
  }
	return 0;
}
uint8_t  Presskey_Judge(uint8_t key,uint8_t* loose)
{		
	if(key)
	{
		*loose = 0;
	}
  if(!key&&!*loose)
	{				
		*loose = 1;
		return 1;
	}				
	return 0;
}
void Limit_Input(float *t,int16_t max)
{
	if(*t>max)
	{
		*t=max;		
	}
	if(*t<-max)
	{
		*t=-max;
	}
}
void Remoter_SetState(uint8_t set)
{
/*     1 3 2*//* 1 3 2*/
	/*|0 0 0 1   0 0 0 1|*/
	Remoter_Status = set;
  
}
void Limits16_t(int16_t *t)
{
	if(*t>30000)
	{
		*t=30000;		
	}
	if(*t<-30000)
	{
		*t=-30000;
	}	
}
void Limits32_t(int32_t *t)
{
  if(*t>0x7fffffff)
	{
		*t= 0x7fffffff;
	}
	if(*t<-0x7fffffff)
	{
		*t = -0x7fffffff;
	}	
}