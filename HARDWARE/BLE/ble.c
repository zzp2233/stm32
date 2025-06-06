/* ����:��Ƕ.������ */
#include "includes.h"

static struct usart_packet_t g_ble_packet={0};

void ble_init(uint32_t baud)
{
	//GPIO�˿�����
	GPIO_InitTypeDef GPIO_InitStructure;
	USART_InitTypeDef USART_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	//�˿�BӲ��ʱ�Ӵ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB,ENABLE);
	
	//����3Ӳ��ʱ�Ӵ�
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3,ENABLE);
	
	//����PB10 PB11ΪAFģʽ�����ù��ܣ�
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_10 | GPIO_Pin_11; 	//10 11������
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF;//���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;//���٣��ٶ�Խ�ߣ���ӦԽ�죬���ǹ��Ļ����
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;//��ʹ������������
	GPIO_Init(GPIOB,&GPIO_InitStructure);

	//��������֧�ֺܶ๦�ܣ���Ҫָ�������ŵĹ��ܣ���ǰҪ�ƶ�֧��USART3
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource10,GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOB,GPIO_PinSource11,GPIO_AF_USART3);	
	
	//����USART1��ز����������ʡ�����λ��ֹͣλ��У��λ
	USART_InitStructure.USART_BaudRate = baud;					//�����ʣ�����ͨ�ŵ��ٶ�
	USART_InitStructure.USART_WordLength = USART_WordLength_8b; //8λ����λ
	USART_InitStructure.USART_StopBits = USART_StopBits_1;		//1��ֹͣλ
	USART_InitStructure.USART_Parity = USART_Parity_No;			//����ҪУ��λ
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;//Ӳ�������ƹ��ܲ���Ҫ
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;//���������ͺͽ�������
	USART_Init(USART3, &USART_InitStructure);
	
	//�����жϴ�����ʽ�����յ�һ���ֽڣ���֪ͨCPU����
	USART_ITConfig(USART3, USART_IT_RXNE, ENABLE);

	//USART3 NVIC ����
	NVIC_InitStructure.NVIC_IRQChannel = USART3_IRQn;		//����3�ж�ͨ��
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority =6;		//��Ӧ���ȼ�6
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;			//IRQͨ��ʹ��
	NVIC_Init(&NVIC_InitStructure);							//����ָ���Ĳ�����ʼ��NVIC�Ĵ���

	//ʹ��USART3����
	USART_Cmd(USART3, ENABLE);
	
	//��������״̬����
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_6; 			//6������
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;			//����ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;		//���٣��ٶ�Խ�ߣ���ӦԽ�죬���ǹ��Ļ����
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_DOWN;			//ʹ����������
	GPIO_Init(GPIOB,&GPIO_InitStructure);
}

uint32_t ble_connect_sta_get(void)
{
	if(PBin(6))
		return BLE_CONNECT;

	return BLE_DISCONNECT;
}


//����3�жϷ������
void USART3_IRQHandler(void)                	
{
static 
	uint32_t i=0;

	uint8_t d;
	
	uint32_t ulReturn;

	BaseType_t  xHigherPriorityTaskWoken = pdFALSE;

	/* �����ٽ�Σ��ٽ�ο���Ƕ�� */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();	
	
	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  
	{
		//���մ�������
		d=USART_ReceiveData(USART3);

		g_ble_packet.rx_buf[i++]=d;
		
		//��⵽'#'������յ���������ʱ����������
		if(d=='#' || i>=(sizeof g_ble_packet.rx_buf))
		{
			g_ble_packet.rx_len = i;
			
			xQueueSendFromISR(g_queue_usart,(void *)&g_ble_packet,&xHigherPriorityTaskWoken);
			
			memset(&g_ble_packet,0,sizeof g_ble_packet);
			
			i=0;
		}			
		
		//��մ��ڽ����жϱ�־λ
		USART_ClearITPendingBit(USART3, USART_IT_RXNE);
	} 
	
	/* ��������Ϣ������������ȼ����ڵ�ǰ���е��������˳��жϺ��������������л���ִ��ǰ��;
	   ����ȴ���һ��ʱ�ӽ��ĲŽ��������л�
	*/
	if(xHigherPriorityTaskWoken)
	{
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}	
	
	/* �˳��ٽ�� */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );		
} 


