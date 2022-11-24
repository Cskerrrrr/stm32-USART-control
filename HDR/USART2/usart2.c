#include "usart2.h"
#include "usart.h"
#include "stm32f10x_usart.h"

void USART2_Init(u32 bound)
{
    GPIO_InitTypeDef GPIO_InitStructure;
    NVIC_InitTypeDef NVIC_InitStructure;
    USART_InitTypeDef USART_InitStructure;;
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART2,ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);
    
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_3;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    GPIO_Init(GPIOA, &GPIO_InitStructure); 
    

    NVIC_InitStructure.NVIC_IRQChannel = USART2_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority= 3;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
    
    USART_InitStructure.USART_BaudRate = bound;
    USART_InitStructure.USART_WordLength = USART_WordLength_8b;
    USART_InitStructure.USART_StopBits = USART_StopBits_1;
    USART_InitStructure.USART_Parity = USART_Parity_No;
    USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
    USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
    USART_Init(USART2, &USART_InitStructure);
    
    USART_ITConfig(USART2,USART_IT_RXNE,ENABLE);
    USART_Cmd(USART2, ENABLE);
}


void USART2_SendData(u8 data)
{
	while((USART2->SR&0X40)==0); 
	USART2->DR = data;
}

void USART2_SendString(u8 *DAT,u8 len)
{
	u8 i;
	for(i=0;i<len;i++)
	{
	 	USART2_SendData(*DAT++);
	}	
}



int32_t MotorPosition = 0;
int32_t jiaodu = 0;

void USART2_IRQHandler(void)
{

    u32 res; // ���ܵ���ʱ����
	static u8 RxCounter1=0; // ����ֵ
	static u8 RxBuffer1[5]; // ���ܻ�����
    static u8 RxState = 0;
	
	if(USART_GetITStatus(USART2, USART_IT_RXNE) != RESET)   //���յ�������
	{
		USART_ClearITPendingBit(USART2,USART_IT_RXNE);   //����жϱ�־
		res =USART_ReceiveData(USART2);//(USART3->DR);	//��ȡ���յ�������
	    //֡ͷ	
		if(RxState==0 && res==0x01)  
		{
			RxState=1,RxCounter1=0;
		}			
		// ��ʼ������Ч����
		else if(RxState==1) 
		{  
				RxBuffer1[RxCounter1++]=res;
				if(RxCounter1 > 5)  //RxBuffer1��������,�������ݽ���
				{	
					    RxState=0;		
						if(RxBuffer1[4] == 0x6B)//֡β
						{			
						 MotorPosition = (int32_t)RxBuffer1[0] << 24 | (int32_t)RxBuffer1[1] << 16 | (int32_t)RxBuffer1[2] << 8 | (int32_t)RxBuffer1[3] << 0; 
						 jiaodu = MotorPosition/65536;
						}
				}	  	
		}
		//�����쳣
    else  RxState = 0;		
  } 
}



