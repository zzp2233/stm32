/* ����:��Ƕ.������ */
#include "includes.h"

void beep_init(void)
{    	 
	GPIO_InitTypeDef  GPIO_InitStructure;

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOF, ENABLE);			

	//GPIOF8��ʼ������ 
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_8;		
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;			    //��ͨ���ģʽ��
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;				//�������
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		    //100MHz
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_UP;				 //����
	GPIO_Init(GPIOF, &GPIO_InitStructure);						//��ʼ��GPIOF�������õ�����д��Ĵ���
	
	//����������
	BEEP(0);
}

