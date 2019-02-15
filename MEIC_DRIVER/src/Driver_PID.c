#include "Driver_PID.h"
PID_Struct PID_Pitch_P;
PID_Struct PID_Pitch_V;
PID_Struct PID_Yaw_P;
PID_Struct PID_Yaw_V;
void ALL_PID_Init(void)
{	
	PIDStruct_Init(&PID_Pitch_P);
	PIDStruct_Init(&PID_Pitch_V);
	PIDStruct_Init(&PID_Yaw_P);
	PIDStruct_Init(&PID_Yaw_V);
	PID_Param_Init(&PID_Pitch_P,40,0,0,1000,5000);
	PID_Param_Init(&PID_Pitch_V,50,0,0,1000,5000);
	PID_Param_Init(&PID_Yaw_P,25,0,0,1000,5000);
	PID_Param_Init(&PID_Yaw_V,40,0,0,1000,5000);
//  PID_Yaw_P.Expect= 301.99;
//  PID_Pitch_P.Expect= 84.4;
 
}
void PIDStruct_Init(PID_Struct *pid)
{
	pid->P          = 0; 
	pid->I          = 0;
	pid->D          = 0;
	pid->Imax       = 0;
	pid->NowErr     = 0;
	pid->LstErr     = 0;
	pid->SumErr     = 0;
	
	pid->Expect     = 0;
	pid->Measured   = 0;
	
	pid->OutPut     = 0;
	pid->MaxOutput  = 0;
}

void PID_Param_Init(PID_Struct *pid,float p,float i,float d,float imax,float maxoutput)
{
	pid -> P = p;
	pid -> I = i;
	pid -> D = d;
	pid -> Imax = imax;
	pid -> MaxOutput = maxoutput;	
}
float  PID_Calc(PID_Struct *pid)
{
	
  float Pout=0,Iout=0,Dout=0;
	pid -> NowErr = pid -> Expect - pid -> Measured;	
	pid -> SumErr += (pid ->I==0) ? 0 : pid -> NowErr;	
	//PID �������
	Pout = pid -> P * pid -> NowErr;
	Iout = pid -> I * pid -> SumErr;
	Dout = pid -> D * (pid -> NowErr- pid -> LstErr);
	/* ���֣�����޷�*/
	if(Iout >= pid -> Imax)	
	{
		pid -> SumErr -= pid -> NowErr;
    Iout = pid -> Imax;
	}
	else if(Iout <=- pid -> Imax)	
	{
    pid -> SumErr -= pid -> NowErr; 	  
		Iout = -pid -> Imax;
	}
	 pid->OutPut = Pout + Iout + Dout;
	
	if (pid->OutPut >= pid->MaxOutput) 
	{
		pid->OutPut = pid->MaxOutput;
	}
	else if (pid->OutPut <= -pid->MaxOutput)
	{
		pid->OutPut = -pid->MaxOutput;
	}	
	/*������*/
	 pid->LstErr = pid->NowErr;
	
	 return pid -> OutPut;
}
void Expect_Limit(float *data,float max,float min)
{
	if(*data>max)
	{
		*data=max;
	}
	if(*data<min)
	{
		*data=min;
	}	
}












