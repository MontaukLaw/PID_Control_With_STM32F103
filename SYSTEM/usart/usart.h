#ifndef __USART_H
#define __USART_H
#include "stdio.h"	
#include "sys.h" 
  /**************************************************************************
���ߣ�ƽ��С��֮��
�ҵ��Ա�С�꣺http://shop114407458.taobao.com/
**************************************************************************/

//void uart_init(u32 bound);

void usart1_send(u8 data);
void uart_init(u32 pclk2,u32 bound);
#endif


