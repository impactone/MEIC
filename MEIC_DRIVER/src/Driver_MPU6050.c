#include "Driver_MPU6050.h"
//设置MPU6050陀螺仪传感器满量程
//fsr:0,±250dps;1,±500dps;2,±1000dps;3,±2000dps   fsr<<3
//设置MPU6050加速度传感器满量程范围
//fsr:0,±2g;1,±4g;2,±8g;3,±16g
//设置MPU6050的采样率(假定Fs=1KHz)
//rate:4~1000(Hz)	
  //if(rate>1000)rate=1000; 
	//if(rate<4)rate=4;
	//else
	//data=1000/rate-1;
	
//设置MPU6050的数字低通滤波器
//lpf:数字低通滤波频率(Hz) lpf = rate/2
	//if(lpf>=188)data=1;
	//else if(lpf>=98)data=2;
	//else if(lpf>=42)data=3;
	//else if(lpf>=20)data=4;
	//else if(lpf>=10)data=5;
	//else data=6;
Attitude CloudAttitude;
int GyroOffset[3] = {0};   //陀螺仪零点
float MagOffset[6] = {0};  //磁力计零点
extern OS_SEM GyroDRY_Sem;
float q0 = 1,q1 = 0,q2 = 0,q3 = 0;
uint8_t MPU6050_Init(void)	
{
	//检测陀螺仪每一步是否初始化正常
	
u16 res = 0x00;
	OS_ERR err;	
	/*开陀螺仪*/
	res |= IIC_WriteData(MPU6050_DEVICE_ADDRESS,PWR_MGMT_1,0x01)   << 1;         //解除休眠状态
	res |= IIC_WriteData(MPU6050_DEVICE_ADDRESS,CONFIG,0x03)       << 2;         //设置低通滤波器
	res |= IIC_WriteData(MPU6050_DEVICE_ADDRESS,GYRO_CONFIG,0x10)  << 3;         //陀螺仪量程  ±1000dps
	res |= IIC_WriteData(MPU6050_DEVICE_ADDRESS,ACCEL_CONFIG,0x00) << 4;         //加速度计量程  ±2g
	res |= IIC_WriteData(MPU6050_DEVICE_ADDRESS,INT_PIN_CFG,0x02)  << 5;  
	res |= IIC_WriteData(MPU6050_DEVICE_ADDRESS,INT_ENABLE,0x00)   << 6;         //关闭所有中断
  res |= IIC_WriteData(MPU6050_DEVICE_ADDRESS,MPU6050_RA_USER_CTRL,0x00) << 7; //关闭Master模式
	res |= IIC_WriteData(MPU6050_DEVICE_ADDRESS,SMPLRT_DIV,0x01)   << 8;         //陀螺仪采样率	
	res |= IIC_WriteData(MPU6050_DEVICE_ADDRESS,INT_ENABLE,0x01)   << 9;         //开INT
	
	/*开磁力计*/
  res |= IIC_WriteData(HMC5883_ADDRESS, HMC58X3_R_CONFA,0x70) << 10;
	res |= IIC_WriteData(HMC5883_ADDRESS, HMC58X3_R_CONFB,0xA0) << 11;
	res |= IIC_WriteData(HMC5883_ADDRESS, HMC58X3_R_MODE,0x00)  << 12;
	res |= IIC_WriteData(HMC5883_ADDRESS, HMC58X3_R_CONFA,0x18) << 13; 
	OSTimeDly(500,OS_OPT_TIME_DLY,&err); /*此延时不可关，零点确定全靠它了*/
		
	/*是否开启失败*/
	if (res)
	{
	  printf("MPU Error Code:%d\r\n",res);
		return res;
	}

	/*陀螺仪确定零点*/
	Gyro_Cali();
  Mag_Calc();
  return 0;
}

void Mag_Calc(void)
{
  MagOffset[0] = (MagMaxX + MagMinX) / 2.0f;
	MagOffset[1] = (MagMaxY + MagMinY) / 2.0f;
	MagOffset[2] = (MagMaxZ + MagMinZ) / 2.0f;	
	MagOffset[3] = 1.0f;
	MagOffset[4] = (MagMaxX - MagMinX) / (MagMaxY - MagMinY);
	MagOffset[5] = (MagMaxX - MagMinX) / (MagMaxZ - MagMinZ);
}



uint8_t isInARange(float data,float min,float max)
{
	if (data >= min && \
		  data <= max)
	  return 0;
	else
		return 1;
}

float Rad2Degree(float data)
{
	return data * 57.3f;
}

//函数名：invSqrt(void)
//描述：求平方根的倒数
//该函数是经典的Carmack求平方根算法，效率极高，使用魔数0x5f375a86
static float invSqrt(float number) 
{
    volatile long i;
    volatile float x, y;
    volatile const float f = 1.5F;

    x = number * 0.5F;
    y = number;
    i = * (( long * ) &y);
    i = 0x5f375a86 - ( i >> 1 );
    y = * (( float * ) &i);
    y = y * ( f - ( x * y * y ) );
    return y;
}
void Gyro_Cali(void)
{
	int16_t ax,ay,az,gx,gy,gz;
	int16_t cnt = 600;    //采cnt次求均值
	int16_t tmp_cnt = 0;
	OS_ERR err;
	
	while (cnt--)
	{
		OSSemPend(&GyroDRY_Sem,0,OS_OPT_PEND_BLOCKING,NULL,&err);
		if (!Get_MPU6050_Data(&ax,&ay,&az,&gx,&gy,&gz))
		{
			tmp_cnt++;
			GyroOffset[0] += gx;
			GyroOffset[1] += gy;
			GyroOffset[2] += gz;		
		}
	}				
	
	GyroOffset[0] /= tmp_cnt;
	GyroOffset[1] /= tmp_cnt;
	GyroOffset[2] /= tmp_cnt;
	
}

uint8_t Get_MPU6050_Data(int16_t *ax,int16_t *ay,int16_t *az,int16_t *gx,int16_t *gy,int16_t *gz)
{
	uint8_t buf[14];
	if (!IIC_ReadData(MPU6050_DEVICE_ADDRESS,MPU6050_DATA_START,buf,14))
	{
		*ax = ((u16)buf[0] << 8) | buf[1];
		*ay = ((u16)buf[2] << 8) | buf[3];
		*az = ((u16)buf[4] << 8) | buf[5];
		//temp = ((u16)buf[6] << 8) | buf[7]; /*温度*/
		*gx = ((u16)buf[8]  << 8) | buf[9];
		*gy = ((u16)buf[10] << 8) | buf[11];
		*gz = ((u16)buf[12] << 8) | buf[13];	
    
    return 0;		
	}else
	{
		return 1;
	}
}


#define Kp 2.0f
#define Ki 0.001f
float UpdateQ(int16_t *fifo)
{
	  static uint32_t PreT = 0;
	  uint32_t NowT;
	  float halfT;
	  static float exInt = 0.0f, eyInt = 0.0f, ezInt = 0.0f; 
	  float invNorm;                 //向量模的倒数
    float vx,vy,vz;                //四元数转换出的重力矢量
    float ex,ey,ez;								 //误差矢量，由加速度计和四元数转换出的重力矢量叉乘
    float ax,ay,az;                //归一化之后的加速度数据
	  float gx,gy,gz;                //三轴陀螺数据
	  
		float	q0q0 = q0 * q0;
	  float q0q1 = q0 * q1;
	  float q0q2 = q0 * q2;
	  float q1q1 = q1 * q1;
	  float q1q3 = q1 * q3;
	  float q2q2 = q2 * q2;
	  float q2q3 = q2 * q3;
	  float q3q3 = q3 * q3; 
		
		float dq0;
		float dq1;
		float dq2;
    float dq3;
		
		OS_ERR err;
		 
		ax = fifo[0];
		ay = fifo[1];
		az = fifo[2];
		gx = fifo[3] * 0.000532f;   //转换成弧度每秒
    gy = fifo[4] * 0.000532f;	  //转换成弧度每秒
		gz = fifo[5] * 0.000532f;   //转换成弧度每秒
		
    //加速度归一化
    invNorm = invSqrt(ax * ax + ay * ay + az * az);
    ax = ax * invNorm;
    ay = ay * invNorm;
    az = az * invNorm;
		
    //提取重力分量
    vx = 2.0f*(q1q3 - q0q2);
    vy = 2.0f*(q0q1 + q2q3);
    vz = q0q0 - q1q1 - q2q2 + q3q3;

    //误差向量
    ex = (ay*vz - az*vy) ;
    ey = (az*vx - ax*vz) ;
    ez = (ax*vy - ay*vx) ;
    
	  //时间更新
    NowT = OSTimeGet(&err);
		halfT = (NowT - PreT) / 2000.0f;
		PreT = NowT;

    if(ex != 0.0f && ey != 0.0f && ez != 0.0f)
    {
        exInt = exInt + ex * Ki * halfT;
        eyInt = eyInt + ey * Ki * halfT;	
        ezInt = ezInt + ez * Ki * halfT;
        // 用叉积误差来做PI修正陀螺零偏
        gx = gx + Kp*ex + exInt;
        gy = gy + Kp*ey + eyInt;
        gz = gz + Kp*ez + ezInt;
    }
    // 四元数微分方程
    dq0 = q0 + (-q1*gx - q2*gy - q3*gz) * halfT;
    dq1 = q1 + (q0*gx + q2*gz - q3*gy)  * halfT;
    dq2 = q2 + (q0*gy - q1*gz + q3*gx)  * halfT;
    dq3 = q3 + (q0*gz + q1*gy - q2*gx)  * halfT;  

    // 四元数规范化
    invNorm = invSqrt(dq0*dq0 + dq1*dq1 + dq2*dq2 + dq3*dq3);
    q0 = dq0 * invNorm;
    q1 = dq1 * invNorm;
    q2 = dq2 * invNorm;
    q3 = dq3 * invNorm;
		
		//返回间隔时间
		return halfT;
}

//采用弧度制
void IMU_GetPitchRoll(Attitude *angles)
{
	angles->pitch = -asin(2.0f * (q0 * q2 -   q1 * q3));  //Pitch
	angles->roll = atan2(2.0f * (q2 * q3 + q0 * q1), 1.0f - 2 * (q1 * q1 + q2 * q2)); //Roll
	//angles->yaw =  -atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3 * q3 + 1);  //Yaw
}
void IMU_GetPitchRollYaw(Attitude *angles)
{
	  int16_t fifo[9];
	  static float yaw = 0;
	  float halfT;	  	  	        
	
	 //Step1 获取9轴数据
		Get_9Motion_Data(fifo);
	
	 //Step2 更新四元数 
		halfT = UpdateQ(fifo);
	
	 //Step3 获取四元数解算结果
		IMU_GetPitchRoll(angles);
	  	 	  		
		/*弧度转换成角度*/
		angles->pitch  = Rad2Degree(angles->pitch);  //Pitch
		angles->roll   = Rad2Degree(angles->roll);   //Roll	
				
		/*获取角速度*/
    angles->gx     = fifo[3] * GYRO_SCALE;
		angles->gy     = fifo[4] * GYRO_SCALE;
		angles->gz     = fifo[5] * GYRO_SCALE;
}

void Get_9Motion_Data(int16_t *fifo)
{
		int16_t mx,my,mz;
		int16_t ax,ay,az,gx,gy,gz;
		Get_MPU6050_Data(&ax,&ay,&az,&gx,&gy,&gz);
		Get_HML5883L_Data(&mx,&my,&mz);
	  Get_HML5883L_Data(&mx,&my,&mz);  /*非得读两次才能读出来，我不懂你啊*/	  
		fifo[0] = (s16)ax;
		fifo[1] = (s16)ay;
		fifo[2] = (s16)az;
		fifo[3] = (s16)gx - GyroOffset[0];
		fifo[4] = (s16)gy - GyroOffset[1];
		fifo[5] = (s16)gz - GyroOffset[2];	 
}


uint8_t Get_HML5883L_Data(int16_t *mx,int16_t *my,int16_t *mz)
{
	uint8_t buf[10];
	if (!IIC_ReadData(HMC5883_ADDRESS,HMC58X3_R_XM,buf,6))
	{
		*mx = ((u16)buf[4] << 8) | buf[5];
		*my = ((u16)buf[0] << 8) | buf[1];
		*mz = ((u16)buf[2] << 8) | buf[3];
		
		return 0;
	}else
	{
		return 1;
	}
}
