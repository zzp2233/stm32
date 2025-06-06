#include "includes.h"

static GPIO_InitTypeDef 		GPIO_InitStructure;

void dht11_init(void)
{
	//ʹ�ܶ˿�GӲ��ʱ��
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOG,ENABLE);


	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	
	//��ʼ��
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	//PG9����ߵ�ƽ���������ŵĳ�ʼ��ƽ״̬
	PGout(9)=1;	
}

void dht11_pin_mode(GPIOMode_TypeDef pin_mode)
{

	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode  = pin_mode;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	
	//��ʼ��
	GPIO_Init(GPIOG,&GPIO_InitStructure);
}


int32_t dht11_read_data(uint8_t *pbuf)
{
	uint32_t t=0;
	int32_t i=0,j=0;
	uint8_t d=0;
	uint8_t *p = pbuf;
	uint8_t check_sum=0;
	
	//��֤PG9Ϊ���ģʽ
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG,&GPIO_InitStructure);
	
	//PG9����͵�ƽ
	PGout(9)=0;
	
	//��ʱ����18ms
	delay_ms(18);
	
	//PG9����ߵ�ƽ
	PGout(9)=1;	
	
	//��ʱ30us
	delay_us(30);
	
	//��֤PG9Ϊ����ģʽ
	GPIO_InitStructure.GPIO_Pin   = GPIO_Pin_9;
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_IN;
	GPIO_InitStructure.GPIO_Speed = GPIO_High_Speed;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd  = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOG,&GPIO_InitStructure);

	//�ȴ��͵�ƽ���֣���ӳ�ʱ����
	t=0;
	while(PGin(9))
	{
		t++;
		
		delay_us(1);
		
		if(t>=4000)
			return -1;
	}
	
	//ͨ����ʱ���͵�ƽ�ĺϷ���
	t=0;
	while(PGin(9)==0)
	{
		t++;
		delay_us(1);
		
		if(t>=100)
			return -2;
	}


	//ͨ����ʱ���ߵ�ƽ�ĺϷ���
	t=0;
	while(PGin(9))
	{
		t++;
		delay_us(1);
		
		if(t>=100)
			return -3;
	}
	
	for(j=0; j<5; j++)
	{
		//����һ���ֽڣ���λ���Ƚ������ݣ�bit7 bit6 ...... bit0
		d=0;
		for(i=7; i>=0; i--)
		{
			//ͨ����ʱ���͵�ƽ�ĺϷ���
			t=0;
			while(PGin(9)==0)
			{
				t++;
				delay_us(1);
				
				if(t>=100)
					return -4;
			}
		
			//��ʱ40us~60us����
			delay_us(40);
			
			//�ж����ŵĵ�ƽ
			if(PGin(9))
			{
				//��d������Ӧ��bitλ��1
				//0x01 1<<0:0000 0001
				//0x02 1<<1:0000 0010
				//.......
				//0x80 1<<7:1000 0000
				d|=1<<i;
				
				//�ȴ�ʣ�µĸߵ�ƽ�������
				t=0;
				while(PGin(9))
				{
					t++;
					delay_us(1);
					
					if(t>=100)
						return -5;
				}			
			
			}
		
		}	
	
		p[j]=d;
	}
	
	//У������
	check_sum = (p[0]+p[1]+p[2]+p[3])&0xFF;
	
	if(check_sum != p[4])
		return -6;
	
	return 0;
}
