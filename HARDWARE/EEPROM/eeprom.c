/* ����:��Ƕ.������ */
#include "includes.h"


void at24c02_init(void)
{
	GPIO_InitTypeDef   GPIO_InitStructure;	
	
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);

	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8|GPIO_Pin_9;		//��8 9������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;				//�������ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;				//��©ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;			//����IO���ٶ�Ϊ100MHz��Ƶ��Խ������Խ�ã�Ƶ��Խ�ͣ�����Խ��
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;			//����Ҫ��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

	//ֻҪ�����ģʽ���г�ʼ��ƽ״̬
	EEPROM_SCL_W=1;
	EEPROM_SDA_W=1;
}

static void sda_pin_mode(GPIOMode_TypeDef pin_mode)
{
	GPIO_InitTypeDef   GPIO_InitStructure;	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;		//��9������
	GPIO_InitStructure.GPIO_Mode = pin_mode;	//�������/����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;	//��©ģʽ
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;//����IO���ٶ�Ϊ100MHz��Ƶ��Խ������Խ�ã�Ƶ��Խ�ͣ�����Խ��
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;//����Ҫ��������
	GPIO_Init(GPIOB, &GPIO_InitStructure);

}
static void i2c_start(void)
{
	//��֤SDA����Ϊ���ģʽ
	sda_pin_mode(GPIO_Mode_OUT);
	
	EEPROM_SCL_W=1;
	EEPROM_SDA_W=1;
	delay_us(5);
	
	EEPROM_SDA_W=0;
	delay_us(5);

	EEPROM_SCL_W=0;
	delay_us(5);
}


static void i2c_stop(void)
{
	//��֤SDA����Ϊ���ģʽ
	sda_pin_mode(GPIO_Mode_OUT);
	
	EEPROM_SCL_W=1;
	EEPROM_SDA_W=0;
	delay_us(5);
	
	EEPROM_SDA_W=1;
	delay_us(5);
}

static void i2c_send_byte(uint8_t byte)
{
	int32_t i;
	
	//��֤SDA����Ϊ���ģʽ
	sda_pin_mode(GPIO_Mode_OUT);
	
	EEPROM_SCL_W=0;
	EEPROM_SDA_W=0;
	delay_us(5);
	
	for(i=7; i>=0; i--)
	{
		if(byte & (1<<i))
			EEPROM_SDA_W=1;
		else
			EEPROM_SDA_W=0;
	
		delay_us(5);
	
		EEPROM_SCL_W=1;
		delay_us(5);
		
		EEPROM_SCL_W=0;
		delay_us(5);		
	}
}

static void i2c_ack(uint8_t ack)
{
	//��֤SDA����Ϊ���ģʽ
	sda_pin_mode(GPIO_Mode_OUT);
	
	EEPROM_SCL_W=0;
	EEPROM_SDA_W=0;
	delay_us(5);
	
	if(ack)
		EEPROM_SDA_W=1;
	else
		EEPROM_SDA_W=0;

	delay_us(5);

	EEPROM_SCL_W=1;
	delay_us(5);
	
	EEPROM_SCL_W=0;
	delay_us(5);		
}

static uint8_t i2c_wait_ack(void)
{
	uint8_t ack;
	//��֤SDA����Ϊ����ģʽ
	sda_pin_mode(GPIO_Mode_IN);

	//�����ŵھŸ�ʱ�����ڣ���SCL����
	EEPROM_SCL_W=1;
	delay_us(5);
	
	if(EEPROM_SDA_R)
		ack=1;
	else
		ack=0;
	
	//��������ռ������
	EEPROM_SCL_W=0;
	delay_us(5);

	return ack;
}

static uint8_t i2c_recv_byte(void)
{

	uint8_t d=0;
	int32_t i;
	
	
	//��֤SDA����Ϊ����ģʽ
	sda_pin_mode(GPIO_Mode_IN);

	
	for(i=7; i>=0; i--)
	{
		EEPROM_SCL_W=1;
		delay_us(5);
		
		if(EEPROM_SDA_R)
			d|=1<<i;
		
		//��������ռ������
		EEPROM_SCL_W=0;
		delay_us(5);	
	}

	return d;
}


int32_t  at24c02_write(uint8_t addr,uint8_t *buf,uint8_t len)
{
	uint8_t ack;
	uint8_t *p=buf;
	
	//������ʼ�ź�
	i2c_start();
	
	//�����豸Ѱַ��д���ʵ�ַΪ0xA0
	i2c_send_byte(0xA0);
	
	ack=i2c_wait_ack();
	if(ack)
	{
		printf("device address error\r\n");
		return -1;
	}

	//����Ҫ����eeprom���ݵ���ʼ��ַ
	i2c_send_byte(addr);
	
	ack=i2c_wait_ack();
	if(ack)
	{
		printf("word address error\r\n");
		return -2;
	}
	
	//������������
	while(len--)
	{
		i2c_send_byte(*p++);
		
		ack=i2c_wait_ack();
		if(ack)
		{
			printf("write data error\r\n");
			return -3;
		}	
	}
	
	i2c_stop();
	printf("write data success\r\n");
	return 0;

}


int32_t  at24c02_read(uint8_t addr,uint8_t *buf,uint8_t len)
{
	uint8_t ack;
	uint8_t *p=buf;
	
	//������ʼ�ź�
	i2c_start();
	
	//�����豸Ѱַ��д���ʵ�ַΪ0xA0
	i2c_send_byte(0xA0);
	
	ack=i2c_wait_ack();
	if(ack)
	{
		printf("device address error\r\n");
		return -1;
	}

	//����Ҫ����eeprom���ݵ���ʼ��ַ
	i2c_send_byte(addr);
	
	ack=i2c_wait_ack();
	if(ack)
	{
		printf("word address error\r\n");
		return -2;
	}
	
	//������ʼ�źţ������л����ʷ��򣬴�д�����л�������
	i2c_start();
	
	//�����豸Ѱַ��д���ʵ�ַΪ0xA1
	i2c_send_byte(0xA1);
	ack=i2c_wait_ack();
	if(ack)
	{
		printf("device address error\r\n");
		return -3;
	}	
	
	
	//������ȡ����
	len=len-1;
	
	while(len--)
	{
		//��ȡ����
		*p=i2c_recv_byte();
		
		//����Ӧ���ź�
		i2c_ack(0);
		
		p++;
	}
	
	//��ȡ���һ���ֽ�
	*p=i2c_recv_byte();
	//������Ӧ���ź�
	i2c_ack(1);
	
	i2c_stop();

	return 0;
}

