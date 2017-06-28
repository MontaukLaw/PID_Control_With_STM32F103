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
	 Bias = Encoder-Target; //����ƫ��
	 Pwm+= Velocity_KP*(Bias-Last_bias)+Velocity_KI*Bias;   //����ʽPI������
	 Last_bias=Bias;	                                     //������һ��ƫ�� 
	 return Pwm;                                           //�������
}


int TIM1_UP_IRQHandler(void)  
{    
	if(TIM1->SR & 0X0001)//5ms��ʱ�ж�
	{   
        TIM1->SR&=~(1 << 0);                             //===�����ʱ��1�жϱ�־λ	      

	    if(delay_flag==1)
	    {
		    if(++delay_50==5)	 delay_50=0,delay_flag=0;    //===���������ṩ50ms�ľ�׼��ʱ
	    }
        
        if(Flag_Velocity > 2)                         //===�ٶȿ���20msһ�Σ�һ�㽨��10ms����Ϊ����ʹ��USB���磬�ٶȱȽ�����20ms�����ӳ���ȡ�ٶȵĵ�λʱ�䣬�����ֵ
		{
	        Flag_Velocity=1;
			Encoder = ReadEncoderVoidInput();                      //===�����ٶ���Ϣ
            
			Motor=Incremental_PI(Encoder,Target_Velocity);  //===�ٶ�PI������
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
	if(TIM1->SR&0X0001)//5ms��ʱ�ж�
	{   
		  TIM1->SR&=~(1<<0);                             //===�����ʱ��1�жϱ�־λ	      

	  if(++Flag_Velocity>2)                         //===�ٶȿ���20msһ�Σ�һ�㽨��10ms����Ϊ����ʹ��USB���磬�ٶȱȽ�����20ms�����ӳ���ȡ�ٶȵĵ�λʱ�䣬�����ֵ
		{
				Flag_Velocity=1;
				Encoder=Read_Velocity(2);                      //===�����ٶ���Ϣ
			  if(Encoder<0) Encoder=0;
				Moto=Incremental_PI(Encoder,Target_Velocity);  //===�ٶ�PI������
		}
	  if(delay_flag==1)
	  {
		 if(++delay_50==5)	 delay_50=0,delay_flag=0;    //===���������ṩ50ms�ľ�׼��ʱ
	  }		
	  Xianfu_Pwm();                                    //===PWM�޷�
		if(Flag_Stop==0)			Set_Pwm(Moto);             //===��ֵ��PWM�Ĵ���
		else 	               	Set_Pwm(0);                //===��ֵ��PWM�Ĵ���
	  Led_Flash(100);                                  //===LED��˸ָʾϵͳ�������� 
		Key();                                           //===ɨ�谴���仯
		Voltage=Get_battery_volt();                      //===�ɼ������ѹ
	}       	
	 return 0;	  
} 

#endif
