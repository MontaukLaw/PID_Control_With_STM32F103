#ifndef __DATA_PRTOCOL_H
#define __DATA_PRTOCOL_H
 
#include "sys.h"

extern unsigned char DataScope_OutPut_Buffer[42];	   //������֡���ݻ�����


void DataScope_Get_Channel_Data(float Data,unsigned char Channel);    // дͨ�������� ������֡���ݻ�����

unsigned char DataScope_Data_Generate(unsigned char Channel_Number);  // ����֡�������ɺ��� 
 
void DataScope(void);

void DataScopeWithData(float currentVel,float targetVel);

#endif 



