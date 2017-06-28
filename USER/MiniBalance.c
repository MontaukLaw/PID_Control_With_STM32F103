#include "stm32f10x.h"
#include "sys.h"

  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/
int Encoder_Left;             //���ұ��������������
u8 Flag_Stop=0,delay_50,delay_flag,Flash_Send;    
int Encoder,Target_Position=0,Target_Velocity=200; //���������������
float Position_KP=120,Position_KI=0.1,Position_KD=500,Velocity_KP=1.0,Velocity_KI=1.0; 
//float Position_KP=120,Position_KI=0.1,Position_KD=500,Velocity_KP=1.5,Velocity_KI=0.5; 
int Motor; 

void MiniBalance_Motor_Init(void)
{
	RCC->APB2ENR|=1<<3;       //PORTBʱ��ʹ��   
	GPIOB->CRH&=0X0000FFFF;   //PORTB12 13 14 15�������
	GPIOB->CRH|=0X22220000;   //PORTB12 13 14 15�������
}
void MiniBalance_PWM_Init(u16 arr,u16 psc)
{		 					 
    MiniBalance_Motor_Init(); //��ʼ�������������IO
	RCC->APB1ENR|=1<<1;       //TIM3ʱ��ʹ��    
	RCC->APB2ENR|=1<<3;       //PORTBʱ��ʹ��     
	GPIOB->CRL&=0XFFFFFF00;   //PORTB0 1�������
	GPIOB->CRL|=0X000000BB;   //PORTB0 1�������
	TIM3->ARR=arr;//�趨�������Զ���װֵ 
	TIM3->PSC=psc;//Ԥ��Ƶ������Ƶ
	TIM3->CCMR2|=6<<12;//CH4 PWM1ģʽ	
	TIM3->CCMR2|=6<<4; //CH3 PWM1ģʽ	
	TIM3->CCMR2|=1<<11;//CH4Ԥװ��ʹ��	 
	TIM3->CCMR2|=1<<3; //CH3Ԥװ��ʹ��	  
	TIM3->CCER|=1<<12; //CH4���ʹ��	   
	TIM3->CCER|=1<<8;  //CH3���ʹ��	
	TIM3->CR1=0x8000;  //ARPEʹ�� 
	TIM3->CR1|=0x01;   //ʹ�ܶ�ʱ��3 	
} 


int main(void)
{ 
    float currentVel;
    float targetVel = 150.0;
    int pwmNoiseTestCounter;
    int pwmNow=0;
    delay_init();	    	            //=====��ʱ������ʼ��	
	//uart_init(9600);	            //=====���ڳ�ʼ��
    uart_init(72,128000);    
    //Encoder_Init_TIM2();            //=====�������ӿ�
	//TIM3_Int_Init(499,7199);
    MiniBalance_PWM_Init(3599,0);
    //MiniBalance_PWM_Init(7199,0);
    
    TIM3->CCR4=0;
    AIN2=0;
    AIN1=1;
	EXTIX_Init();
    Timer1_Init(99,7199); 
    while(1)
	{
        
        pwmNoiseTestCounter++;
        if(pwmNoiseTestCounter==80){
            Target_Velocity = 1000;
            //pwmNoiseTestCounter=0;
            
            //pwmNow = pwmNow + 50;
            //if(pwmNow>=3500){
               //pwmNow = 0;
            //}
            //TIM3->CCR4 = pwmNow;
        }else if(pwmNoiseTestCounter==160){
            Target_Velocity = 200;
            pwmNoiseTestCounter = 0;
            TIM3->CCR4 = 0;
        }
        
        delay_flag=1;
        while(delay_flag);
        //currentVel = ReadEncoderVoidInput()/10;        
        currentVel = Encoder/10;
        targetVel= Target_Velocity/10;
        DataScopeWithData(currentVel, targetVel);    //���͸���λ����ͼ�η���
        //printf("pwmNoiseTestCounter: %d, pwm now: %d \r\n",pwmNoiseTestCounter,pwmNow);
		//printf("%d\r\n",ReadEncoderVoidInput());
	} 
}

