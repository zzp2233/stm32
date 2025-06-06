/* 作者:粤嵌.温子祺 */
#include "includes.h"

static struct usart_packet_t g_ble_packet={0};

void ble_init(uint32_t baud)
{
	//GPIO端口设置
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//端口B硬件时钟打开
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//串口3硬件时钟打开
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	//配置PB10 PB11为AF模式（复用功能）
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_11; 	//10 11号引脚
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;//输出模式
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;//高速，速度越高，响应越快，但是功耗会更高
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;//不使能上下拉电阻
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//由于引脚支持很多功能，需要指定该引脚的功能，当前要制定支持USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);	
	
	//配置USART1相关参数：波特率、数据位、停止位、校验位
	USART_InitStructure.USART_BaudRate = baud;					//波特率，就是通信的速度
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //8位数据位
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//1个停止位
	USART_InitStructure.USART_Parity = USART_Parity_No;			//不需要校验位
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//硬件流控制功能不需要
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//串口允许发送和接收数据
	USART_Init(USART3, &USART_InitStructure);
	
	//配置中断触发方式，接收到一个字节，就通知CPU处理
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	//USART3 NVIC 配置
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;		//串口3中断通道
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;//抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =6;		//响应优先级6
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQ通道使能
	NVIC_Init(&NVIC_InitStructure);							//根据指定的参数初始化NVIC寄存器

	//使能USART3工作
	USART_Cmd(USART3, ENABLE);
	
	//蓝牙连接状态引脚
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6; 			//6号引脚
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;			//输入模式
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;		//高速，速度越高，响应越快，但是功耗会更高
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;			//使能下拉电阻
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

uint32_t ble_connect_sta_get(void)
{
	if(PBin(6))
		return BLE_CONNECT;

	return BLE_DISCONNECT;
}


//串口3中断服务程序
void USART3_IRQHandler(void)                	
{
static 
	uint32_t i=0;

	uint8_t d;
	
	uint32_t ulReturn;

	BaseType_t  xHigherPriorityTaskWoken = pdFALSE;

	/* 进入临界段，临界段可以嵌套 */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();	
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  
	{
		//接收串口数据
		d=USART_ReceiveData(USART3);

		g_ble_packet.rx_buf[i++]=d;
		
		//检测到'#'符或接收的数据满的时候则发送数据
		if(d=='#' || i>=(sizeof g_ble_packet.rx_buf))
		{
			g_ble_packet.rx_len = i;
			
			xQueueSendFromISR(g_queue_usart,(void *)&g_ble_packet,&xHigherPriorityTaskWoken);
			
			memset(&g_ble_packet,0,sizeof g_ble_packet);
			
			i=0;
		}			
		
		//清空串口接收中断标志位
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	} 
	
	/* 若接收消息队列任务的优先级高于当前运行的任务，则退出中断后立即进行任务切换，执行前者;
	   否则等待下一个时钟节拍才进行任务切换
	*/
	if(xHigherPriorityTaskWoken)
	{
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}	
	
	/* 退出临界段 */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );		
} 


