#include "stm32f10x.h"
#include "usart.h"
#include "usart2.h"
#include "delay.h"
#include "sys.h"
extern u8 RxState;
extern int32_t MotorPosition,jiaodu;

int main()
{
	uint8_t cmd[10] = {0};
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);//w4级抢占,4级响应。
	uart_init(38400);
	USART2_Init(38400);
	delay_init(); 
	while(1)
	{		
     	    cmd[0] = 0x01;	/* 地址 */
			cmd[1] = 0x36;	/* 功能码 */
			cmd[2] = 0x6B;  
		    /* 发送命令 */
			USART2_SendData(cmd[0]),USART2_SendData(cmd[1]),USART2_SendData(cmd[2]);
			printf("%d \r\n",jiaodu);
	}
	
	
}
