#include "stm32f10x.h"
#include "sys.h"

  /**************************************************************************
作者：平衡小车之家
我的淘宝小店：http://shop114407458.taobao.com/
**************************************************************************/
int Encoder_Left;             //左右编码器的脉冲计数
u8 Flag_Stop=0,delay_50,delay_flag,Flash_Send;    
int Encoder,Target_Position=0,Target_Velocity=200; //编码器的脉冲计数
float Position_KP=120,Position_KI=0.1,Position_KD=500,Velocity_KP=1.0,Velocity_KI=1.0; 
//float Position_KP=120,Position_KI=0.1,Position_KD=500,Velocity_KP=1.5,Velocity_KI=0.5; 
int Motor; 

void MiniBalance_Motor_Init(void)
{
	RCC->APB2ENR|=1<<3;       //PORTB时钟使能   
	GPIOB->CRH&=0X0000FFFF;   //PORTB12 13 14 15推挽输出
	GPIOB->CRH|=0X22220000;   //PORTB12 13 14 15推挽输出
}
void MiniBalance_PWM_Init(u16 arr,u16 psc)
{		 					 
    MiniBalance_Motor_Init(); //初始化电机控制所需IO
	RCC->APB1ENR|=1<<1;       //TIM3时钟使能    
	RCC->APB2ENR|=1<<3;       //PORTB时钟使能     
	GPIOB->CRL&=0XFFFFFF00;   //PORTB0 1复用输出
	GPIOB->CRL|=0X000000BB;   //PORTB0 1复用输出
	TIM3->ARR=arr;//设定计数器自动重装值 
	TIM3->PSC=psc;//预分频器不分频
	TIM3->CCMR2|=6<<12;//CH4 PWM1模式	
	TIM3->CCMR2|=6<<4; //CH3 PWM1模式	
	TIM3->CCMR2|=1<<11;//CH4预装载使能	 
	TIM3->CCMR2|=1<<3; //CH3预装载使能	  
	TIM3->CCER|=1<<12; //CH4输出使能	   
	TIM3->CCER|=1<<8;  //CH3输出使能	
	TIM3->CR1=0x8000;  //ARPE使能 
	TIM3->CR1|=0x01;   //使能定时器3 	
} 


int main(void)
{ 
    float currentVel;
    float targetVel = 150.0;
    int pwmNoiseTestCounter;
    int pwmNow=0;
    delay_init();	    	            //=====延时函数初始化	
	//uart_init(9600);	            //=====串口初始化
    uart_init(72,128000);    
    //Encoder_Init_TIM2();            //=====编码器接口
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
        DataScopeWithData(currentVel, targetVel);    //发送给上位机做图形分析
        //printf("pwmNoiseTestCounter: %d, pwm now: %d \r\n",pwmNoiseTestCounter,pwmNow);
		//printf("%d\r\n",ReadEncoderVoidInput());
	} 
}

