#include "includes.h"



void led_init(void)
{    	 
	GPIO_InitTypeDef  		GPIO_InitStructure;
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	//ʹ��GPIOEʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);	//ʹ��GPIOFʱ��

	//GPIOE13,GPIOE14��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_14 ;				//13������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//��ʼ��GPIO
	
	GPIO_SetBits(GPIOE,GPIO_Pin_13|GPIO_Pin_14);			
	
	//GPIOF9,GPIOF10��ʼ������
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9 | GPIO_Pin_10;	//9�ź�10������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;			//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_Init(GPIOF, &GPIO_InitStructure);					//��ʼ��GPIO
	
	GPIO_SetBits(GPIOF,GPIO_Pin_9 | GPIO_Pin_10);			
}

void led_ctrl(uint32_t n,uint32_t sta)
{
	sta=!sta;

	if(n==0)PFout(9)=sta;
	if(n==1)PFout(10)=sta;
	if(n==2)PEout(13)=sta;
	if(n==3)PEout(14)=sta;		
}

	
void led_breath_init(void)
{
	GPIO_InitTypeDef  		GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef  		TIM_OCInitStructure;	
	//ʹ��GPIOEʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOE, ENABLE);	

	//ʹ��TIM1��Ӳ��ʱ��
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14 ;				//14������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;			//��ͨ���ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;			//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//100MHz
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;			//����
	GPIO_Init(GPIOE, &GPIO_InitStructure);					//��ʼ��GPIO
	
	//PE14���ӵ�TIM1
	GPIO_PinAFConfig(GPIOE, GPIO_PinSource14, GPIO_AF_TIM1);	

	//����TIM1�ļ���ֵ��������������Ƶ��
	TIM_TimeBaseStructure.TIM_Period = 10000/100-1;	//������������ɼ���ֵ�������������Ƶ��Ϊ100Hz
	TIM_TimeBaseStructure.TIM_Prescaler = 16800-1;	//Ԥ��Ƶֵ=16800-1+1=16800��Ƶ����һ��Ƶ
	TIM_TimeBaseStructure.TIM_ClockDivision = 0;	//ʱ�ӷ�Ƶ��Ҳ��֮Ϊ�ڶ��η�Ƶ������F407��֧��
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//���ϼ�������0 -> TIM_Period
	TIM_TimeBaseInit(TIM1, &TIM_TimeBaseStructure);
	
	/* ���ͨ��3������PWM1ģʽ */
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;	//ֻҪ TIMx_CNT������ֵ�� < TIMx_CCR1���Ƚ�ֵ����ͨ�� 1 ��Ϊ��Ч״̬������Ϊ��Ч״̬��
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;//�����������
	TIM_OCInitStructure.TIM_Pulse = 0;	//�Ƚ�ֵ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_Low;//ͨ������Ч״̬Ϊ�͵�ƽ
	
	//TIM1��ͨ��4����
	TIM_OC4Init(TIM1, &TIM_OCInitStructure);
	
	//Ĭ��TIM1������
	TIM_Cmd(TIM1, DISABLE);
	
	//Ĭ�ϲ�ʹ��TIM1�������
	TIM_CtrlPWMOutputs(TIM1,DISABLE);	
}

/* ���������� 
������
	brightness 0 ~ 100,��ֵԽ������Խ��
*/
void led_breath_brightness(uint8_t brightness)
{
	TIM_SetCompare4(TIM1,brightness);
}

void led_breath_run(void)
{
	TIM_Cmd(TIM1, ENABLE);
	TIM_CtrlPWMOutputs(TIM1,ENABLE);
	led_breath_brightness(0);
}

void led_breath_stop(void)
{
	led_breath_brightness(0);
	TIM_Cmd(TIM1, DISABLE);
	TIM_CtrlPWMOutputs(TIM1,DISABLE);
}





