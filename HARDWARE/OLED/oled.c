/* ����:��Ƕ.������ */
#include "includes.h"

static GPIO_InitTypeDef   			GPIO_InitStructure;
static uint32_t g_oled_direction=0;

static uint8_t g_oled_gddram    [8][128]={0};
static uint8_t g_oled_gddram_bak[8][128]={0};

//OLED���Դ�
//��Ÿ�ʽ����.
//[0]0 1 2 3 ... 127	
//[1]0 1 2 3 ... 127	
//[2]0 1 2 3 ... 127	
//[3]0 1 2 3 ... 127	
//[4]0 1 2 3 ... 127	
//[5]0 1 2 3 ... 127	
//[6]0 1 2 3 ... 127	
//[7]0 1 2 3 ... 127 			   
static void i2c_sda_mode(GPIOMode_TypeDef mode)
{
	/* ����ָ������Ϊ���ģʽ */
	GPIO_InitStructure.GPIO_Pin =  SDA_PIN;					//ָ������
	GPIO_InitStructure.GPIO_Mode = mode;					//�������/����ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;			//��©
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;		//����IO���ٶ�Ϊ100MHz��Ƶ��Խ������Խ�ã�Ƶ��Խ�ͣ�����Խ��
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;		//����Ҫ����������
	GPIO_Init(SDA_PORT, &GPIO_InitStructure);
}

static void i2c_start(void)
{

	//��֤SDA����Ϊ���ģʽ
	i2c_sda_mode(GPIO_Mode_OUT);
	
	SCL=1;
	SDA_W=1;
	
	//��ʱ1us
	delay_us(1);
	
	SDA_W=0;
	
	//��ʱ1us
	delay_us(1);	
	
	
	//ռ��ʱ���ߣ����������ӻ�����æ��״̬
	SCL=0;
	
	//��ʱ1us
	delay_us(1);
}

/**********************************************
//IIC Stop
**********************************************/
static void i2c_stop(void)
{
	//��֤SDA����Ϊ���ģʽ
	i2c_sda_mode(GPIO_Mode_OUT);
	
	SCL=0;
	SDA_W=0;

	//��ʱ1us
	delay_us(1);
	
	SCL=1;	
	
	//��ʱ1us
	delay_us(1);	
	
	SDA_W=1;

	//��ʱ1us
	delay_us(1);	
}

static uint8_t i2c_wait_ack(void)
{

	uint8_t ack=0;
	
	//��֤SDA����Ϊ����ģʽ
	i2c_sda_mode(GPIO_Mode_IN);
	
	SCL=1;
	
	//��ʱ1us
	delay_us(1);
	
	//�ж�SDA���ŵĵ�ƽ
	if(SDA_R)
	{
		ack=1;
		
		//��ֹ����i2cͨ��
		i2c_stop();
	}
	
	SCL=0;
	
	//��ʱ1us
	delay_us(1);
	
	return ack;
}

void Write_IIC_Byte(unsigned char IIC_Byte)
{
	uint32_t i=0;
	
	//��֤SDA����Ϊ���ģʽ
	i2c_sda_mode(GPIO_Mode_OUT);
	
	SCL=0;
	SDA_W=0;

	//��ʱ1us
	delay_us(1);
	
	for(i=0; i<8; i++)
	{
		//��MSB�����Чλ�������ݵķ���
		if(IIC_Byte & (1<<(7-i)))
			SDA_W=1;
		else
			SDA_W=0;
		
		//��ʱ1us
		delay_us(1);
		
		//����ʱ����Ϊ�ߵ�ƽ�����ߴӻ����ڿ��Զ�ȡSDA���ŵĵ�ƽ
		SCL=1;	
		
		//��ʱ1us
		delay_us(1);	

		//����ʱ����Ϊ�͵�ƽ�����ߴӻ����ڲ����Զ�ȡSDA���ŵĵ�ƽ����Ϊ��������Ҫ����SDA���ŵĵ�ƽ
		SCL=0;	
		
		//��ʱ1us
		delay_us(1);			
	}
}

void Write_IIC_Command(unsigned char IIC_Command)
{
	i2c_start();
	Write_IIC_Byte(0x78);         
	i2c_wait_ack();	
	Write_IIC_Byte(0x00);		
	i2c_wait_ack();	
	Write_IIC_Byte(IIC_Command); 
	i2c_wait_ack();	
	i2c_stop();
}

void Write_IIC_Data(unsigned char IIC_Data)
{
	i2c_start();
	Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	i2c_wait_ack();	
	Write_IIC_Byte(0x40);			//write data
	i2c_wait_ack();	
	Write_IIC_Byte(IIC_Data);
	i2c_wait_ack();	
	i2c_stop();
}
void OLED_WR_Byte(unsigned dat,unsigned cmd)
{
	if(cmd)
	{

		Write_IIC_Data(dat);

	}
	else 
	{
		Write_IIC_Command(dat);
		
	}
}

void Write_IIC_Datas(unsigned char *IIC_Data,uint32_t len)
{
	unsigned char *p = IIC_Data;
	
	i2c_start();
	Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	i2c_wait_ack();	
	

	Write_IIC_Byte(0x40);			
	i2c_wait_ack();	

	while(len--)
	{	
		Write_IIC_Byte(*p++);
		i2c_wait_ack();		
	}

	i2c_stop();
}

void Write_IIC_Datas_Fix(unsigned char IIC_Data,uint32_t len)
{
	
	i2c_start();
	Write_IIC_Byte(0x78);			//D/C#=0; R/W#=0
	i2c_wait_ack();	
	

	Write_IIC_Byte(0x40);			
	i2c_wait_ack();	

	while(len--)
	{	
		Write_IIC_Byte(IIC_Data);
		i2c_wait_ack();		
	}

	i2c_stop();
}


void OLED_Set_Direction(uint32_t direction)
{
	uint32_t i;
	
	g_oled_direction = direction;
	
	if(direction == 0)
	{
		OLED_WR_Byte(0xC8,OLED_CMD);//��ɨ��˳�򣺴��ϵ���
		OLED_WR_Byte(0xA1,OLED_CMD);//��ɨ��˳�򣺴�����	
	}

	if(direction == 180)
	{
		OLED_WR_Byte(0xC0,OLED_CMD);//��ɨ��˳�򣺴��µ���
		OLED_WR_Byte(0xA0,OLED_CMD);//��ɨ��˳�򣺴��ҵ���
	}
	
			
    /* ������ʾ */	
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ

		Write_IIC_Datas((unsigned char *)&g_oled_gddram[i],128);
			
	} 
}


void OLED_GDDRAM_Backup(void)
{
	
	memcpy(g_oled_gddram_bak,g_oled_gddram,sizeof(g_oled_gddram));

}

void OLED_GDDRAM_Restore(void)
{
	uint32_t i;
	
	memcpy(g_oled_gddram,g_oled_gddram_bak,sizeof(g_oled_gddram));
	
    /* ������ʾ */	
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ

		Write_IIC_Datas((unsigned char *)&g_oled_gddram[i],128);
			
	} 	

}

uint32_t OLED_Get_Direction(void)
{
	return g_oled_direction;
}

//��������

void OLED_Set_Pos(unsigned char x, unsigned char y) 
{ 	
	OLED_WR_Byte(0xb0+y,OLED_CMD);
	OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
	OLED_WR_Byte((x&0x0f),OLED_CMD); 
}   	  
//����OLED��ʾ    
void OLED_Display_On(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X14,OLED_CMD);  //DCDC ON
	OLED_WR_Byte(0XAF,OLED_CMD);  //DISPLAY ON
}
//�ر�OLED��ʾ     
void OLED_Display_Off(void)
{
	OLED_WR_Byte(0X8D,OLED_CMD);  //SET DCDC����
	OLED_WR_Byte(0X10,OLED_CMD);  //DCDC OFF
	OLED_WR_Byte(0XAE,OLED_CMD);  //DISPLAY OFF
}		   			 
//��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!	  
void OLED_Clear(void)  
{  
	u8 i;		
    
	for(i=0;i<8;i++)  
	{  
		OLED_WR_Byte (0xb0+i,OLED_CMD);    //����ҳ��ַ��0~7��
		OLED_WR_Byte (0x00,OLED_CMD);      //������ʾλ�á��е͵�ַ
		OLED_WR_Byte (0x10,OLED_CMD);      //������ʾλ�á��иߵ�ַ

		Write_IIC_Datas_Fix(0,128);
		
		/* ��oledÿ�����ص�����д��g_oled_gddram */
		memset(&g_oled_gddram[i],0,128);
		
	} //������ʾ
}

//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//mode:0,������ʾ;1,������ʾ				 
//size:ѡ������ 16/12 
void OLED_ShowChar(u8 x,u8 y,u8 chr,u8 Char_Size)
{      	
	unsigned char c=0;	
	c=chr-' ';//�õ�ƫ�ƺ��ֵ	
	
	if(x>Max_Column-1){x=0;y=y+2;}
	
	if(Char_Size ==16)
	{
		OLED_Set_Pos(x,y);	
		Write_IIC_Datas((unsigned char *)&F8X16[c*16],8);

		/* ��oledÿ�����ص�����д��g_oled_gddram */
		memcpy(&g_oled_gddram[y][x],(unsigned char *)&F8X16[c*16],8);		

		OLED_Set_Pos(x,y+1);
		Write_IIC_Datas((unsigned char *)&F8X16[c*16+8],8);
		
		/* ��oledÿ�����ص�����д��g_oled_gddram */
		memcpy(&g_oled_gddram[y+1][x],(unsigned char *)&F8X16[c*16+8],8);			

	}
	else 
	{	
		OLED_Set_Pos(x,y);
		Write_IIC_Datas((unsigned char *)&F6x8[c],6);
		
		/* ��oledÿ�����ص�����д��g_oled_gddram */
		memcpy(&g_oled_gddram[y][x],(unsigned char *)&F8X16[c],6);			

	}
}
//m^n����
u32 oled_pow(u8 m,u8 n)
{
	u32 result=1;	 
	while(n--)result*=m;    
	return result;
}				  
//��ʾ2������
//x,y :�������	 
//len :���ֵ�λ��
//size:�����С
//mode:ģʽ	0,���ģʽ;1,����ģʽ
//num:��ֵ(0~4294967295);	 		  
void OLED_ShowNum(u8 x,u8 y,u32 num,u8 len,u8 size2)
{         	
	u8 t,temp;
	u8 enshow=0;						   
	for(t=0;t<len;t++)
	{
		temp=(num/oled_pow(10,len-t-1))%10;
		if(enshow==0&&t<(len-1))
		{
			if(temp==0)
			{
				OLED_ShowChar(x+(size2/2)*t,y,' ',size2);
				continue;
			}else enshow=1; 
		 	 
		}
	 	OLED_ShowChar(x+(size2/2)*t,y,temp+'0',size2); 
	}
} 
//��ʾһ���ַ��Ŵ�
void OLED_ShowString(u8 x,u8 y,u8 *chr,u8 Char_Size)
{
	unsigned char j=0;
	while (chr[j]!='\0')
	{		OLED_ShowChar(x,y,chr[j],Char_Size);
			x+=8;
		if(x>120){x=0;y+=2;}
			j++;
	}
}
//��ʾ����
void OLED_ShowCHinese(u8 x,u8 y,u8 no)
{      			    
	OLED_Set_Pos(x,y);	
	Write_IIC_Datas((unsigned char *)&Hzk[2*no],16);

	/* ��oledÿ�����ص�����д��g_oled_gddram */
	memcpy(&g_oled_gddram[y][x],(unsigned char *)&Hzk[2*no],16);		
	
	OLED_Set_Pos(x,y+1);	
	Write_IIC_Datas((unsigned char *)&Hzk[2*no+1],16);	
	
	/* ��oledÿ�����ص�����д��g_oled_gddram */
	memcpy(&g_oled_gddram[y+1][x],(unsigned char *)&Hzk[2*no+1],16);		
	

}
/***********������������ʾ��ʾBMPͼƬ128��64��ʼ������(x,y),x�ķ�Χ0��127��yΪҳ�ķ�Χ0��7*****************/
void OLED_DrawBMP(unsigned char x0, unsigned char y0,unsigned char x1, unsigned char y1,const unsigned char *BMP)
{ 	
	uint32_t j=0;
	unsigned char y;

	if(y1%8==0) 
		y=y1/8;      
	else 
		y=y1/8+1;
	
	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
	
		Write_IIC_Datas((unsigned char *)&BMP[j],x1-x0);

		/* ��oledÿ�����ص�����д��g_oled_gddram */
		memcpy(&g_oled_gddram[y][x0],(unsigned char *)&BMP[j],x1-x0);
		
		j+=(x1-x0);
	
	}
}
void OLED_ClearRegion(unsigned char x0,unsigned y0,unsigned char x1, unsigned char y1)
{
	unsigned char y;
	
	if(y1%8==0) 
		y=y1/8;      
	else 
		y=y1/8+1;

	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
		Write_IIC_Datas_Fix(0,x1-x0);
		
		/* ��oledÿ�����ص�����д��g_oled_gddram */
		memset(&g_oled_gddram[y][x0],0,x1-x0);		
	}
}

void OLED_FillRegion(unsigned char x0,unsigned y0,unsigned char x1, unsigned char y1)
{
	unsigned char y;
	
	if(y1%8==0) 
		y=y1/8;      
	else 
		y=y1/8+1;

	for(y=y0;y<y1;y++)
	{
		OLED_Set_Pos(x0,y);
		
		Write_IIC_Datas_Fix(1,x1-x0);
		
		/* ��oledÿ�����ص�����д��g_oled_gddram */
		memset(&g_oled_gddram[y][x0],0,x1-x0);		
	}
}

//��ʼ��SSD1306					    
void OLED_Init(void)
{ 	

	GPIO_InitTypeDef  GPIO_InitStructure;
	
	//ʹ�ܶ˿ڵ�ʱ��
	RCC_AHB1PeriphClockCmd(SCL_PORT_RCC|SDA_PORT_RCC, ENABLE);		

	/* ����SCL����Ϊ��©���ģʽ */
	GPIO_InitStructure.GPIO_Pin = SCL_PIN;				//ָ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//�������ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		//��©(����ǰ�ӿ�û���ⲿ�������裬��ֻ����oled��������ʹ������������ṩ�㹻����������)
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//����IO���ٶ�Ϊ100MHz��Ƶ��Խ������Խ�ã�Ƶ��Խ�ͣ�����Խ��
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//����Ҫ����������
	
	GPIO_Init(SCL_PORT, &GPIO_InitStructure);


	/* ����SDA����Ϊ��©���ģʽ */
	GPIO_InitStructure.GPIO_Pin = SDA_PIN;				//ָ������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;		//�������ģʽ
	GPIO_InitStructure.GPIO_OType = GPIO_OType_OD;		//��©
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;	//����IO���ٶ�Ϊ100MHz��Ƶ��Խ������Խ�ã�Ƶ��Խ�ͣ�����Խ��
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;	//����Ҫ����������


	GPIO_Init(SDA_PORT, &GPIO_InitStructure);


	delay_ms(200);

	OLED_WR_Byte(0xAE,OLED_CMD);//--display off,����ʾ
	OLED_WR_Byte(0x00,OLED_CMD);//---set low column address
	OLED_WR_Byte(0x10,OLED_CMD);//---set high column address
	OLED_WR_Byte(0x40,OLED_CMD);//--set start line address  
	OLED_WR_Byte(0xB0,OLED_CMD);//--set page address
	OLED_WR_Byte(0x81,OLED_CMD); // contract control
	OLED_WR_Byte(0xFF,OLED_CMD);//--128   
	OLED_WR_Byte(0xA6,OLED_CMD);//--normal / reverse
	OLED_WR_Byte(0xA8,OLED_CMD);//--set multiplex ratio(1 to 64)
	OLED_WR_Byte(0x3F,OLED_CMD);//--1/32 duty


	OLED_WR_Byte(0xC8,OLED_CMD);//��ɨ��˳�򣺴��ϵ���
	OLED_WR_Byte(0xA1,OLED_CMD);//��ɨ��˳�򣺴�����

	OLED_WR_Byte(0xD3,OLED_CMD);//-set display offset
	OLED_WR_Byte(0x00,OLED_CMD);//
	
	OLED_WR_Byte(0xD5,OLED_CMD);//set osc division
	OLED_WR_Byte(0x80,OLED_CMD);//
	
	OLED_WR_Byte(0xD8,OLED_CMD);//set area color mode off
	OLED_WR_Byte(0x05,OLED_CMD);//
	
	OLED_WR_Byte(0xD9,OLED_CMD);//Set Pre-Charge Period
	OLED_WR_Byte(0xF1,OLED_CMD);//
	
	OLED_WR_Byte(0xDA,OLED_CMD);//set com pin configuartion
	OLED_WR_Byte(0x12,OLED_CMD);//
	
	OLED_WR_Byte(0xDB,OLED_CMD);//set Vcomh
	OLED_WR_Byte(0x30,OLED_CMD);//
	
	OLED_WR_Byte(0x8D,OLED_CMD);//set charge pump enable����ѹ����
	OLED_WR_Byte(0x14,OLED_CMD);//
	
	OLED_WR_Byte(0xAF,OLED_CMD);//--turn on oled panel,������ʾ
}  
