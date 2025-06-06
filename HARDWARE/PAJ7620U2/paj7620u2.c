/* ����:��Ƕ.������ */
#include "includes.h"

static GPIO_InitTypeDef	GPIO_InitStructure;
static EXTI_InitTypeDef	EXTI_InitStructure;	
static NVIC_InitTypeDef	NVIC_InitStructure;

//ѡ��PAJ7620U2 BANK����
void paj7620u2_selectBank(bank_e bank)
{
	switch(bank)
	{
		case BANK0: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK0);break;//BANK0�Ĵ�������
		case BANK1: GS_Write_Byte(PAJ_REGITER_BANK_SEL,PAJ_BANK1);break;//BANK1�Ĵ�������
		default:break;
	}		
}

//PAJ7620U2����
uint8_t paj7620u2_wakeup(void)
{ 
	uint8_t data=0x0a;
	GS_WakeUp();//����PAJ7620U2
	delay_ms(5);//����ʱ��>400us
	GS_WakeUp();//����PAJ7620U2
	delay_ms(5);//����ʱ��>400us
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	data = GS_Read_Byte(0x00);//��ȡ״̬
	if(data!=0x20) 
		return 0; //����ʧ��
	
	return 1;
}

/* 
Ĭ��ԭ��paj7620u2����û��֧���жϴ���
�޸����ڣ�2021/7/25
�޸��ˣ���Ƕ.������
*/
void paj7620u2_irq_init(void)
{
	//ʹ�ܶ˿�E��Ӳ��ʱ�ӣ��˿�E���ܹ�����˵���˾��ǶԶ˿�E�ϵ�
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);

	//ʹ��ϵͳ����Ӳ��ʱ�ӣ�˵���˾��Ƕ�ϵͳ���õ�Ӳ������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);

	//����Ӳ��������GPIO���˿�E����6������
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_6;			//��6������
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN;		//����ģʽ
	GPIO_InitStructure.GPIO_Speed=GPIO_High_Speed;	//���Ÿ��ٹ������յ�ָ������������ȱ�㣺���ĸ�
	GPIO_InitStructure.GPIO_PuPd=GPIO_PuPd_UP;		//�����ⲿû�����������裬��Ҫʹ����������
	GPIO_Init(GPIOE,&GPIO_InitStructure);	

	//��PE6�������ӵ�EXTI6
	SYSCFG_EXTILineConfig(EXTI_PortSourceGPIOE,EXTI_PinSource6);	


	EXTI_InitStructure.EXTI_Line = EXTI_Line6;				//EXTI6
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;		//�жϴ���
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; //�½��ش���
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;				//ʹ��
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;	//�ⲿ�ж�6���жϺ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;//��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x05;						//��Ӧ���ȼ� 0x05
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//NVICʹ��
	NVIC_Init(&NVIC_InitStructure);			
}

//PAJ7620U2��ʼ��
//����ֵ��0:ʧ�� 1:�ɹ�
uint8_t paj7620u2_init(void)
{
	uint8_t i;
	uint8_t status;
	
	/* �����ж� */
	paj7620u2_irq_init();	
	
	GS_i2c_init();//IIC��ʼ��
    status = paj7620u2_wakeup();//����PAJ7620U2
	if(!status) 
		return 0;
	paj7620u2_selectBank(BANK0);//����BANK0�Ĵ�������
	for(i=0;i<INIT_SIZE;i++)
	{
		GS_Write_Byte(init_Array[i][0],init_Array[i][1]);//��ʼ��PAJ7620U2
	}
    paj7620u2_selectBank(BANK0);//�л���BANK0�Ĵ�������

	return 1;
}


//����ʶ�����
uint16_t gesture_get(void)
{
    uint8_t status;
	uint8_t data[2]={0x00};
	uint16_t gesture_data = 0;

	//��ȡ����״̬			
	status = GS_Read_nByte(PAJ_GET_INT_FLAG1,2,&data[0]);
	
	if(!status)
	{   
		gesture_data =(unsigned short int)data[1]<<8 | data[0];
#if 0	
		if(gesture_data) 
		{
			switch(gesture_data)
			{
				case GES_UP:               printf(KERN_DEBUG"Up\r\n");	 	//����		  
				break;
					 
				case GES_DOWN:             printf(KERN_DEBUG"Down\r\n"); 		//����				
				break; 
					 
				case GES_LEFT:             printf(KERN_DEBUG"Left\r\n");   	//����             
				break; 
				
				case GES_RIGHT:            printf(KERN_DEBUG"Right\r\n");     //����          
				break; 
				
				case GES_FORWARD:          printf(KERN_DEBUG"Forward\r\n"); 	//��ǰ            
				break; 
				
				case GES_BACKWARD:         printf(KERN_DEBUG"Backward\r\n"); 	//���           
				break; 
				
				case GES_CLOCKWISE:        printf(KERN_DEBUG"Clockwise\r\n"); //˳ʱ��      
				break; 
				
				case GES_COUNT_CLOCKWISE:  printf(KERN_DEBUG"AntiClockwise\r\n");  //��ʱ��
				break; 
				
				case GES_WAVE:             printf(KERN_DEBUG"Wave\r\n");  	//�Ӷ�              
				break; 
				
				default:   
				break;
				
			}		
		}
#endif		
	}
	
	return gesture_data;
}



void EXTI9_5_IRQHandler(void)
{
	uint32_t ulReturn;

	/* �����ٽ�Σ��ٽ�ο���Ƕ�� */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();	

	/* ����־λ */
	if(EXTI_GetITStatus(EXTI_Line6) == SET)
	{
		xEventGroupSetBitsFromISR(g_event_group,EVENT_GROUP_GESTURE,NULL);
		
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	
	/* �˳��ٽ�� */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );	
}



