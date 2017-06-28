#include "sys.h"

u8 Flag_Velocity=1;

int myabs(int a)
{ 		   
    int temp;
	if(a<0)  temp=-a;  
	else temp=a;
	return temp;
}

void Set_Pwm(int moto)
{
    //if(moto<0)			AIN2=1,			AIN1=0;
    //else 	          AIN2=0,			AIN1=1;
    PWMA=myabs(moto);
}

void pwmLimit(void)
{	
	  int Amplitude=3500;    
	  if(Motor<-Amplitude) Motor = -Amplitude;	
	  if(Motor>Amplitude)  Motor = Amplitude;		
}

int Incremental_PI (int Encoder,int Target)
{ 	
	 static float Bias,Pwm,Last_bias;
	 Bias = Encoder-Target; //计算偏差
	 Pwm+= Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //增量式PI控制器
	 Last_bias=Bias;	                                     //保存上一次偏差 
	 return Pwm;                                           //增量输出
}


int TIM1_UP_IRQHandler(void)  
{    
	if(TIM1->SR & 0X0001)//5ms定时中断
	{   
        TIM1->SR&=~(1 << 0);                             //===清除定时器1中断标志位	      

	    if(delay_flag==1)
	    {
		    if(++delay_50==5)	 delay_50=0,delay_flag=0;    //===给主函数提供50ms的精准延时
	    }
        
        if(Flag_Velocity > 2)                         //===速度控制20ms一次，一般建议10ms，因为这里使用USB供电，速度比较慢，20ms可以延长获取速度的单位时间，提高数值
		{
	        Flag_Velocity=1;
			Encoder = ReadEncoderVoidInput();                      //===更新速度信息
            
			Motor=Incremental_PI(Encoder,Target_Velocity);  //===速度PI控制器
		}  
        Flag_Velocity++;        
        pwmLimit();
        Set_Pwm(Motor);        
	}       	
	return 0;	  
} 

#if 0
int TIM1_UP_IRQHandler(void)  
{    
	if(TIM1->SR&0X0001)//5ms定时中断
	{   
		  TIM1->SR&=~(1<<0);                             //===清除定时器1中断标志位	      

	  if(++Flag_Velocity>2)                         //===速度控制20ms一次，一般建议10ms，因为这里使用USB供电，速度比较慢，20ms可以延长获取速度的单位时间，提高数值
		{
				Flag_Velocity=1;
				Encoder=Read_Velocity(2);                      //===更新速度信息
			  if(Encoder<0) Encoder=0;
				Moto=Incremental_PI(Encoder,Target_Velocity);  //===速度PI控制器
		}
	  if(delay_flag==1)
	  {
		 if(++delay_50==5)	 delay_50=0,delay_flag=0;    //===给主函数提供50ms的精准延时
	  }		
	  Xianfu_Pwm();                                    //===PWM限幅
		if(Flag_Stop==0)			Set_Pwm(Moto);             //===赋值给PWM寄存器
		else 	               	Set_Pwm(0);                //===赋值给PWM寄存器
	  Led_Flash(100);                                  //===LED闪烁指示系统正常运行 
		Key();                                           //===扫描按键变化
		Voltage=Get_battery_volt();                      //===采集供电电压
	}       	
	 return 0;	  
} 

#endif
