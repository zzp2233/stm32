/* ����:��Ƕ.������ */
#include "includes.h"

/* �˵���� */
static menu_t menu_rtc[];
static menu_t menu_rtc_date[];
static menu_t menu_rtc_time[];

static menu_t menu_dht[];
static menu_t menu_dht_temp[];
static menu_t menu_dht_humi[];

static menu_t menu_mpu6050[];
static menu_t menu_gesture[];
static menu_t menu_ble[];
static menu_t menu_eeprom[];
static menu_t menu_spi_flash[];
static menu_t menu_author[];

static menu_t menu_pass_man[];
static menu_t menu_pass_auth[];
static menu_t menu_pass_modify[];

static menu_t menu_power_man[];
static menu_t menu_power_man_sleep[];
static menu_t menu_power_man_stop[];
static menu_t menu_power_man_stop_set[];
static menu_t menu_power_man_standby[];
static menu_t menu_power_man_standby_set[];
static menu_t menu_power_man_sleep_enable[];
static menu_t menu_power_man_sleep_disable[];

static void menu_dht_fun(void* pvParameters);
static void menu_rtc_fun(void* pvParameters);
static void menu_mpu6050_fun(void* pvParameters);
static void menu_gesture_fun(void* pvParameters);

static void menu_ble_fun(void* pvParameters);
static void menu_eeprom_fun(void* pvParameters);
static void menu_spi_flash_fun(void* pvParameters);
static void menu_author_fun(void* pvParameters);
static void menu_pass_man_fun(void* pvParameters);
static void menu_power_man_sleep_fun(void* pvParameters);
static void menu_power_man_stop_fun(void* pvParameters);
static void menu_power_man_standby_fun(void* pvParameters);



/*	һ���˵�
	1.DHT11
	2.Date & Time
	3.MPU6050
	4.Gesture
*/
menu_t menu_main_1[]=
{
	{"1.DHT11"			,NULL,NULL,menu_main_2,NULL,menu_dht},	
	{"2.Date & Time "	,NULL,NULL,menu_main_2,NULL,menu_rtc},
	{"3.MPU6050"		,NULL,NULL,menu_main_2,NULL,menu_mpu6050},
	{"4.Gesture"		,NULL,NULL,menu_main_2,NULL,menu_gesture},	
	{NULL,NULL,NULL,NULL,NULL,NULL},
};
/*	һ���˵�
	5.ble
	6.eeprom
	7.spi flash
	8.author
*/
menu_t menu_main_2[]=
{
	{"5.ble"			,NULL,menu_main_1,menu_main_3,NULL,menu_ble},	
	{"6.eeprom"			,NULL,menu_main_1,menu_main_3,NULL,menu_eeprom},
	{"7.spi flash"		,NULL,menu_main_1,menu_main_3,NULL,menu_spi_flash},
	{"8.author"			,NULL,menu_main_1,menu_main_3,NULL,menu_author},	
	{NULL,NULL,NULL,NULL,NULL,NULL},
};

/*	һ���˵�
	9.Password
	10.none
	11.none
	12.none
*/
menu_t menu_main_3[]=
{
	{"9.Password"	,NULL,menu_main_2,NULL,NULL,menu_pass_man},	
	{"10.Power"		,NULL,menu_main_2,NULL,NULL,menu_power_man},
	{"11.none"		,NULL,menu_main_2,NULL,NULL,NULL},
	{"12.none"		,NULL,menu_main_2,NULL,NULL,NULL},	
	{NULL,NULL,NULL,NULL,NULL,NULL},
};

menu_t *menu_main_tbl[]={
	menu_main_1,
	menu_main_2,
	menu_main_3,	
	NULL,
};


/*
	�����˵�
	[1] Temp
	[2] Humi
*/
static menu_t menu_dht[]=
{
	{"[1] Temp",NULL,NULL,NULL,menu_main_1,menu_dht_temp},
	{"[2] Humi",NULL,NULL,NULL,menu_main_1,menu_dht_humi},
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};
/*
	�����˵�
	[1] Date
	[2] Time
*/
static menu_t menu_rtc[]=
{
	{"[1] Date",NULL,NULL,NULL,menu_main_1,menu_rtc_date},
	{"[2] Time",NULL,NULL,NULL,menu_main_1,menu_rtc_time},
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};

/*
	�����˵�
	[1] Auth
	[2] Modify
*/
static menu_t menu_pass_man[]=
{
	{"[1] Auth"  ,NULL,NULL,NULL,menu_main_3,menu_pass_auth},
	{"[2] Modify",NULL,NULL,NULL,menu_main_3,menu_pass_modify},
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};

/*
	�����˵�
	[1] Sleep
	[2] Stop
	[3] Standby
*/
static menu_t menu_power_man[]=
{
	{"[1] Sleep",NULL,NULL,NULL,menu_main_3,menu_power_man_sleep},
	{"[2] Stop"	,NULL,NULL,NULL,menu_main_3,menu_power_man_stop},
	{"[3] Standby",NULL,NULL,NULL,menu_main_3,menu_power_man_standby},	
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};

/*
	�����˵�
	DHT��RTC��MPU6050�����п���
*/
static menu_t menu_rtc_date[]=
{
	{"-",menu_rtc_fun,NULL,NULL,menu_rtc,NULL},
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};

static menu_t menu_rtc_time[]=
{
	{"-",menu_rtc_fun,NULL,NULL,menu_rtc,NULL},
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};

static menu_t menu_dht_temp[]=
{
	{"-",menu_dht_fun,NULL,NULL,menu_dht,NULL},
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};

static menu_t menu_dht_humi[]=
{
	{"-",menu_dht_fun,NULL,NULL,menu_dht,NULL},
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};

static menu_t menu_mpu6050[]=
{
	{"-",menu_mpu6050_fun,NULL,NULL,menu_main_1,NULL},
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};

static menu_t menu_gesture[]=
{
	{"-",menu_gesture_fun,NULL,NULL,menu_main_1,NULL},
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};

static menu_t menu_ble[]=
{
	{"-",menu_ble_fun,NULL,NULL,menu_main_2,NULL},
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};

static menu_t menu_eeprom[]=
{
	{"-",menu_eeprom_fun,NULL,NULL,menu_main_2,NULL},
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};

static menu_t menu_spi_flash[]=
{
	{"-",menu_spi_flash_fun,NULL,NULL,menu_main_2,NULL},
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};

static menu_t menu_author[]=
{
	{"-",menu_author_fun,NULL,NULL,menu_main_2,NULL},
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};

static menu_t menu_pass_auth[]=
{
	{"-",menu_pass_man_fun,NULL,NULL,menu_pass_man,NULL},
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};

static menu_t menu_pass_modify[]=
{
	{"-",menu_pass_man_fun,NULL,NULL,menu_pass_man,NULL},
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};


static menu_t menu_power_man_sleep[]=
{
	{"[1] Enable" ,NULL,NULL,NULL,menu_power_man,menu_power_man_sleep_enable},
	{"[2] Disable",NULL,NULL,NULL,menu_power_man,menu_power_man_sleep_disable},	
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};

static menu_t menu_power_man_stop[]=
{
	{"[1] KEY4 Wake",NULL,NULL,NULL,menu_power_man,menu_power_man_stop_set},
	{"[2] 10s Wake",NULL,NULL,NULL,menu_power_man,menu_power_man_stop_set},	
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};

static menu_t menu_power_man_standby[]=
{
	{"[1] KEY1 Wake",NULL,NULL,NULL,menu_power_man,menu_power_man_standby_set},
	{"[2] 10s Wake",NULL,NULL,NULL,menu_power_man,menu_power_man_standby_set},	
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};

/*
	4���˵�

*/
static menu_t menu_power_man_sleep_enable[]=
{
	{"-" ,menu_power_man_sleep_fun,NULL,NULL,menu_power_man_sleep,NULL},
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};

static menu_t menu_power_man_sleep_disable[]=
{
	{"-" ,menu_power_man_sleep_fun,NULL,NULL,menu_power_man_sleep,NULL},
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};

static menu_t menu_power_man_stop_set[]=
{
	{"-" ,menu_power_man_stop_fun,NULL,NULL,menu_power_man_stop,NULL},
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};

static menu_t menu_power_man_standby_set[]=
{
	{"-" ,menu_power_man_standby_fun,NULL,NULL,menu_power_man_standby,NULL},
	{NULL,NULL,NULL,NULL,NULL,NULL},	
};

/**
 * @brief ����������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details .ʶ��ENTER��
 * 			.ʶ��BACK��
 */
static void menu_pass_man_fun(void* pvParameters)
{
	BaseType_t 	xReturn;
	oled_t 		oled;
	char 		key_val='N';
	uint8_t		chn_tbl[8]={0};
	uint32_t	i;

	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;

	dgb_printf_safe("[menu_pass_man_fun] ...\r\n");

	/* ʶ��ENTER�������� */
	if(menu_ext->key_fn == KEY_ENTER)
	{
		if(menu_ext->item_cursor == 0)
			g_pass_man_what = FLAG_PASS_MAN_AUTH;

		if(menu_ext->item_cursor == 1)
			g_pass_man_what = FLAG_PASS_MAN_MODIFY;

		if(g_pass_man_what!=FLAG_PASS_MAN_AUTH && g_pass_man_what!=FLAG_PASS_MAN_MODIFY)
			return;	

		if(g_pass_man_what == FLAG_PASS_MAN_AUTH)
		{
			/* ����Ҫ��ʾ��ͼƬ */
			oled.x=0;
			oled.y=0;
			oled.pic_width=50;
			oled.pic_height=8;
			oled.ctrl=OLED_CTRL_SHOW_PICTURE;
			oled.pic=pic_locker_icon;

			xReturn = xQueueSend( 	g_queue_oled,	/* ��Ϣ���еľ�� */
									&oled,			/* ���͵���Ϣ���� */
									100);			/* �ȴ�ʱ�� 100 Tick */
			if(xReturn != pdPASS)
				dgb_printf_safe("[menu_pass_man_fun] xQueueSend oled picture error code is %d\r\n",xReturn);	


			/* ��ʾ����-������ */
			oled.ctrl = OLED_CTRL_SHOW_CHINESE;
			oled.y = 2;
			chn_tbl[0]=39;chn_tbl[1]=40;chn_tbl[2]=41;
		
			for(i=0; i<3; i++)
			{
				oled.x = 66+i*16;
				oled.chinese = chn_tbl[i];

				xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
									&oled,		   /* ���͵���Ϣ���� */
									100);		   /* �ȴ�ʱ�� 100 Tick */
				if (xReturn != pdPASS)
					dgb_printf_safe("[menu_pass_man_fun] xQueueSend oled string error code is %d\r\n", xReturn);

			}

			/* ���������ʾ�� */
			oled.ctrl = OLED_CTRL_SHOW_STRING;
			oled.x = 66;
			oled.y = 4;
			oled.str = "------";
			oled.font_size = 16;

			xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
								&oled,		   /* ���͵���Ϣ���� */
								100);		   /* �ȴ�ʱ�� 100 Tick */
			if (xReturn != pdPASS)
				dgb_printf_safe("[menu_pass_man_fun] xQueueSend oled string error code is %d\r\n", xReturn);	


			/* ֪ͨ�����������λ�������� */
			key_val='C';
			xReturn = xQueueSend(g_queue_kbd, 	/* ��Ϣ���еľ�� */
								&key_val,		/* ���͵���Ϣ���� */
								100);		   	/* �ȴ�ʱ�� 100 Tick */
			if (xReturn != pdPASS)
				dgb_printf_safe("[menu_pass_man_fun] xQueueSend kbd error code is %d\r\n", xReturn);					
		}

		if(g_pass_man_what == FLAG_PASS_MAN_MODIFY)
		{
			/* ����Ҫ��ʾ��ͼƬ */
			oled.x=0;
			oled.y=0;
			oled.pic_width=64;
			oled.pic_height=8;
			oled.ctrl=OLED_CTRL_SHOW_PICTURE;
			oled.pic=pic_pass_modify_icon;

			xReturn = xQueueSend( 	g_queue_oled,	/* ��Ϣ���еľ�� */
									&oled,			/* ���͵���Ϣ���� */
									100);			/* �ȴ�ʱ�� 100 Tick */
			if(xReturn != pdPASS)
				dgb_printf_safe("[menu_pass_man_fun] xQueueSend oled picture error code is %d\r\n",xReturn);	


			/* ��ʾ����-������ */
			oled.ctrl = OLED_CTRL_SHOW_CHINESE;
			oled.y = 0;
			chn_tbl[0]=39;chn_tbl[1]=40;chn_tbl[2]=41;
		
			for(i=0; i<3; i++)
			{
				oled.x = 66+i*16;
				oled.chinese = chn_tbl[i];

				xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
									&oled,		   /* ���͵���Ϣ���� */
									100);		   /* �ȴ�ʱ�� 100 Tick */
				if (xReturn != pdPASS)
					dgb_printf_safe("[menu_pass_man_fun] xQueueSend oled string error code is %d\r\n", xReturn);

			}


			/* ��վ�������ʾ�� */
			oled.ctrl = OLED_CTRL_SHOW_STRING;
			oled.x = 66;
			oled.y = 2;
			oled.str = "------";
			oled.font_size = 16;

			xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
								&oled,		   /* ���͵���Ϣ���� */
								1000);		   /* �ȴ�ʱ�� 100 Tick */
			if (xReturn != pdPASS)
				dgb_printf_safe("[menu_pass_man_fun] xQueueSend oled string error code is %d\r\n", xReturn);

			/* �����������ʾ�� */
			oled.ctrl = OLED_CTRL_SHOW_STRING;
			oled.x = 66;			
			oled.y = 6;
			oled.str = "------";
			oled.font_size = 16;
			xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
								&oled,		   /* ���͵���Ϣ���� */
								1000);		   /* �ȴ�ʱ�� 100 Tick */
			if (xReturn != pdPASS)
				dgb_printf_safe("[menu_pass_man_fun] xQueueSend oled string error code is %d\r\n", xReturn);


			/* ��ʾ����-������ */
			oled.ctrl = OLED_CTRL_SHOW_CHINESE;
			oled.x = 66;
			oled.y = 4;
			chn_tbl[0]=38;chn_tbl[1]=40;chn_tbl[2]=41;
		
			for(i=0; i<3; i++)
			{
				oled.x = 66+i*16;
				oled.chinese = chn_tbl[i];

				xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
									&oled,		   /* ���͵���Ϣ���� */
									100);		   /* �ȴ�ʱ�� 100 Tick */
				if (xReturn != pdPASS)
					dgb_printf_safe("[menu_pass_man_fun] xQueueSend oled string error code is %d\r\n", xReturn);

			}

			/* ֪ͨ�����������λ�������� */
			key_val='C';
			xReturn = xQueueSend(g_queue_kbd, 	/* ��Ϣ���еľ�� */
								&key_val,		/* ���͵���Ϣ���� */
								100);		   	/* �ȴ�ʱ�� 100 Tick */
			if (xReturn != pdPASS)
				dgb_printf_safe("[menu_pass_man_fun] xQueueSend kbd error code is %d\r\n", xReturn);
		}

		/* �ָ������������ */
		vTaskResume(app_task_kbd_handle);		
	}

	/* ʶ��BACK�������룬��ֹͣ��������������� */	
	if(menu_ext->key_fn == KEY_BACK)
	{
		/* �������������� */
		vTaskSuspend(app_task_kbd_handle);
	}	
}


/**
 * @brief ������ʪ�Ȳ˵���ִ����صĿ���������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details .ʶ��ENTER�����ָ���ʪ���������У�
 * 			.ʶ��BACK����������ʪ������
 */
static void menu_dht_fun(void* pvParameters)
{
	oled_t oled;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;

	BaseType_t xReturn;

	dgb_printf_safe("[menu_dht_fun] ...\r\n");

	/* ʶ��ENTER�������룬��ָ�DHT�������в���ȡ�¶�/ʪ�� */	
	if(menu_ext->key_fn == KEY_ENTER)
	{
		if(menu_ext->item_cursor == 0)
			g_dht_get_what = FLAG_DHT_GET_TEMP;

		if(menu_ext->item_cursor == 1)
			g_dht_get_what = FLAG_DHT_GET_HUMI;

		if(g_dht_get_what!=FLAG_DHT_GET_TEMP && g_dht_get_what!=FLAG_DHT_GET_HUMI)
			return;			

		/* ����Ҫ��ʾ��ͼƬ */
		oled.x=0;
		oled.y=0;
		oled.pic_width=32;
		oled.pic_height=8;
		oled.ctrl=OLED_CTRL_SHOW_PICTURE;
		oled.pic=pic_dht_icon;

		xReturn = xQueueSend( 	g_queue_oled,	/* ��Ϣ���еľ�� */
								&oled,			/* ���͵���Ϣ���� */
								100);			/* �ȴ�ʱ�� 100 Tick */
		if(xReturn != pdPASS)
			dgb_printf_safe("[menu_dht_fun] xQueueSend oled picture error code is %d\r\n",xReturn);	

		/* �������������� */
		vTaskSuspend(app_task_kbd_handle);

		/* �ָ�DHT�������� */ 
		vTaskResume(app_task_dht_handle);
	}

	/* ʶ��BACK�������룬��ֹͣDHT�������� */	
	if(menu_ext->key_fn == KEY_BACK)
	{
		/* ����DHT���� */
		vTaskSuspend(app_task_dht_handle);

		/* �ָ������������ */
		vTaskResume(app_task_kbd_handle);
	}	
}
/**
 * @brief ����͹��������˵���ִ����صĿ���������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details ����		
 */
static void menu_ble_fun(void* pvParameters)
{
	oled_t oled;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;

	BaseType_t xReturn;

	dgb_printf_safe("[menu_ble_fun] ...\r\n");

	/* ʶ��ENTER��������*/	
	if(menu_ext->key_fn == KEY_ENTER)
	{
		/* ����Ҫ��ʾ��ͼƬ */
		oled.x=0;
		oled.y=0;
		oled.pic_width=128;
		oled.pic_height=8;
		oled.ctrl=OLED_CTRL_SHOW_PICTURE;
		oled.pic=pic_face1_icon;

		xReturn = xQueueSend( 	g_queue_oled,	/* ��Ϣ���еľ�� */
								&oled,			/* ���͵���Ϣ���� */
								100);			/* �ȴ�ʱ�� 100 Tick */
		if(xReturn != pdPASS)
			dgb_printf_safe("[menu_ble_fun] xQueueSend oled picture error code is %d\r\n",xReturn);	
	}

}
/**
 * @brief ����eeprom�˵���ִ����صĿ���������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details ����		
 */
static void menu_eeprom_fun(void* pvParameters)
{
	oled_t oled;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;

	BaseType_t xReturn;

	dgb_printf_safe("[menu_eeprom_fun] ...\r\n");

	/* ʶ��ENTER��������*/	
	if(menu_ext->key_fn == KEY_ENTER)
	{
		/* ����Ҫ��ʾ��ͼƬ */
		oled.x=0;
		oled.y=0;
		oled.pic_width=128;
		oled.pic_height=8;
		oled.ctrl=OLED_CTRL_SHOW_PICTURE;
		oled.pic=pic_face2_icon;

		xReturn = xQueueSend( 	g_queue_oled,	/* ��Ϣ���еľ�� */
								&oled,			/* ���͵���Ϣ���� */
								100);			/* �ȴ�ʱ�� 100 Tick */
		if(xReturn != pdPASS)
			dgb_printf_safe("[menu_eeprom_fun] xQueueSend oled picture error code is %d\r\n",xReturn);	
	}	
}
/**
 * @brief ����spi flash�˵���ִ����صĿ���������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details ����	
 */
static void menu_spi_flash_fun(void* pvParameters)
{
	oled_t oled;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;

	BaseType_t xReturn;

	dgb_printf_safe("[menu_spi_flash_fun] ...\r\n");

	/* ʶ��ENTER��������*/	
	if(menu_ext->key_fn == KEY_ENTER)
	{
		/* ����Ҫ��ʾ��ͼƬ */
		oled.x=0;
		oled.y=0;
		oled.pic_width=128;
		oled.pic_height=8;
		oled.ctrl=OLED_CTRL_SHOW_PICTURE;
		oled.pic=pic_face3_icon;

		xReturn = xQueueSend( 	g_queue_oled,	/* ��Ϣ���еľ�� */
								&oled,			/* ���͵���Ϣ���� */
								100);			/* �ȴ�ʱ�� 100 Tick */
		if(xReturn != pdPASS)
			dgb_printf_safe("[menu_spi_flash_fun] xQueueSend oled picture error code is %d\r\n",xReturn);	
	}	
}
/**
 * @brief ����������Ϣ�˵���ִ����صĿ���������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details ����	
 */
static void menu_author_fun(void* pvParameters)
{
	oled_t oled;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;

	BaseType_t xReturn;

	dgb_printf_safe("[menu_author_fun] ...\r\n");

	/* ʶ��ENTER��������*/	
	if(menu_ext->key_fn == KEY_ENTER)
	{
		/* ����Ҫ��ʾ��ͼƬ */
		oled.x=0;
		oled.y=0;
		oled.pic_width=128;
		oled.pic_height=8;
		oled.ctrl=OLED_CTRL_SHOW_PICTURE;
		oled.pic=pic_author_icon;

		xReturn = xQueueSend( 	g_queue_oled,	/* ��Ϣ���еľ�� */
								&oled,			/* ���͵���Ϣ���� */
								100);			/* �ȴ�ʱ�� 100 Tick */
		if(xReturn != pdPASS)
			dgb_printf_safe("[menu_author_fun] xQueueSend oled picture error code is %d\r\n",xReturn);	
		
		
		vTaskDelay(3000);
		
		vTaskResume(app_task_oled_animation_handle);
	}
	
	
	/* ʶ��BACK�������룬��ֹͣ��ʾ���� */	
	if(menu_ext->key_fn == KEY_BACK)
		vTaskSuspend(app_task_oled_animation_handle);
}

/**
 * @brief ����rtc�˵���ִ����صĿ���������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details .ʶ��ENTER��������ݴ������Ŀ���λ��ָ����ȡ����ʱ�仹�����ڣ�
 * 			 ���ָ�rtc��������У�
 * 			.ʶ��BACK���������rtc����
 */
static void menu_rtc_fun(void* pvParameters)
{
	oled_t		oled;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;

	BaseType_t	xReturn;

	dgb_printf_safe("[menu_rtc_fun] ... \r\n");

	/* ʶ��ENTER�������룬��ָ�RTC�������в���ȡ����/ʱ�� */	
	if(menu_ext->key_fn == KEY_ENTER)
	{
		if(menu_ext->item_cursor == 0)
			g_rtc_get_what = FLAG_RTC_GET_DATE;

		if(menu_ext->item_cursor == 1)
			g_rtc_get_what = FLAG_RTC_GET_TIME;

		if(g_rtc_get_what!=FLAG_RTC_GET_DATE && g_rtc_get_what!=FLAG_RTC_GET_TIME)
			return;

		if(g_rtc_get_what == FLAG_RTC_GET_DATE)
		{
			/* ����Ҫ��ʾ��ͼƬ */
			oled.x=50;
			oled.y=0;
			oled.pic_width=32;
			oled.pic_height=4;
			oled.ctrl=OLED_CTRL_SHOW_PICTURE;
			oled.pic=pic_date_icon;

			xReturn = xQueueSend( 	g_queue_oled,	/* ��Ϣ���еľ�� */
									&oled,			/* ���͵���Ϣ���� */
									100);			/* �ȴ�ʱ�� 100 Tick */
			if(xReturn != pdPASS)
				dgb_printf_safe("[menu_rtc_fun] xQueueSend oled picture error code is %d\r\n",xReturn);		

		}
		
		if(g_rtc_get_what == FLAG_RTC_GET_TIME)
		{
			/* ����Ҫ��ʾ��ͼƬ */
			oled.x=50;
			oled.y=0;
			oled.pic_width=32;
			oled.pic_height=4;
			oled.ctrl=OLED_CTRL_SHOW_PICTURE;
			oled.pic=pic_clock_icon;

			xReturn = xQueueSend( 	g_queue_oled,	/* ��Ϣ���еľ�� */
									&oled,			/* ���͵���Ϣ���� */
									100);			/* �ȴ�ʱ�� 100 Tick */
			if(xReturn != pdPASS)
				dgb_printf_safe("[menu_rtc_fun] xQueueSend oled picture error code is %d\r\n",xReturn);		

		}
		/* �ָ�RTC�������� */
		vTaskResume(app_task_rtc_handle);
	}

	/* ʶ��BACK�������룬��ֹͣRTC�������� */	
	if(menu_ext->key_fn == KEY_BACK)
		vTaskSuspend(app_task_rtc_handle);
}
/**
 * @brief ����mpu6050�˵���ִ����صĿ���������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details .ʶ��ENTER������������ʾŷ���ǣ�
 * 			.ʶ��BACK���������ò���ʾŷ���ǡ�
 */
static void menu_mpu6050_fun(void* pvParameters)
{
	oled_t		oled;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;

	BaseType_t	xReturn;

	dgb_printf_safe("[menu_mpu6050_fun] ... \r\n");

	/* ʶ��ENTER�������룬���ȡŷ���� */
	if(menu_ext->key_fn == KEY_ENTER)
	{
		/* ����Ҫ��ʾ��ͼƬ */
		oled.x=50;
		oled.y=0;
		oled.pic_width=32;
		oled.pic_height=4;
		oled.ctrl=OLED_CTRL_SHOW_PICTURE;
		oled.pic=pic_mpu6050_icon;

		xReturn = xQueueSend( 	g_queue_oled,	/* ��Ϣ���еľ�� */
								&oled,			/* ���͵���Ϣ���� */
								100);			/* �ȴ�ʱ�� 100 Tick */
		if(xReturn != pdPASS)
			dgb_printf_safe("[menu_mpu6050_fun] xQueueSend oled picture error code is %d\r\n",xReturn);	

		g_mpu6050_get_what = FLAG_MPU6050_GET_EULER;		
	}
		

	/* ʶ��BACK�������룬��ֹͣ���� */		
	if(menu_ext->key_fn == KEY_BACK)
		g_mpu6050_get_what = FLAG_MPU6050_GET_NONE;
}
/**
 * @brief ��������ʶ��˵���ִ����صĿ���������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details .ʶ��ENTER������������ʾ����ͼƬ��
 * 			.ʶ��BACK���������ò���ʾ����ͼƬ��
 * 			.ʶ��g_gesture_valid������Ч�ԣ���������޽�������ʶ��ģ�飬����ʾ����ͼ��
 * 			 ��ʾ����ʶ������Ч��
 */
static void menu_gesture_fun(void* pvParameters)
{
	oled_t 		oled;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;
	BaseType_t xReturn;

	dgb_printf_safe("[menu_gesture_fun] ...\r\n");

	/* ����ʶ��ģ����Ч������ʾͼƬ������������ʾ */
	if(!g_gesture_valid)
	{
		dgb_printf_safe("[menu_gesture_fun] gesture is invalid, please press KEY_BACK to return\r\n");
		
		/* ����Ҫ��ʾ��ͼƬ */
		oled.x=45;
		oled.y=0;
		oled.pic_width=32;
		oled.pic_height=4;
		oled.ctrl=OLED_CTRL_SHOW_PICTURE;
		oled.pic=pic_warn_icon;

		xReturn = xQueueSend( 	g_queue_oled,	/* ��Ϣ���еľ�� */
								&oled,			/* ���͵���Ϣ���� */
								100);			/* �ȴ�ʱ�� 100 Tick */
		if(xReturn != pdPASS)
			dgb_printf_safe("[app_task_gesture] xQueueSend oled picture error code is %d\r\n",xReturn);	

		/* ����Ҫ��ʾ���ַ��� */
		oled.x=2;
		oled.y=5;
		oled.str="gesture invalid";
		oled.font_size=12;
		oled.ctrl=OLED_CTRL_SHOW_STRING;

		xReturn = xQueueSend( 	g_queue_oled,	/* ��Ϣ���еľ�� */
								&oled,			/* ���͵���Ϣ���� */
								100);			/* �ȴ�ʱ�� 100 Tick */
		if(xReturn != pdPASS)
			dgb_printf_safe("[app_task_gesture] xQueueSend oled string error code is %d\r\n",xReturn);
	}

	/* ʶ��ENTER�������� */
	if(menu_ext->key_fn == KEY_ENTER)
	{
		/* ����ʶ��ģ����Ч����������ʾ׼����ʼ����ʶ����� */
		if(g_gesture_valid)
		{
			/* ����Ҫ��ʾ���ַ��� */
			oled.x=15;
			oled.y=3;
			oled.str="Let's Begin!";
			oled.font_size=16;
			oled.ctrl=OLED_CTRL_SHOW_STRING;

			xReturn = xQueueSend( 	g_queue_oled,	/* ��Ϣ���еľ�� */
									&oled,			/* ���͵���Ϣ���� */
									100);			/* �ȴ�ʱ�� 100 Tick */
			if(xReturn != pdPASS)
				dgb_printf_safe("[app_task_gesture] xQueueSend oled string error code is %d\r\n",xReturn);

			g_gesture_display_flag = FLAG_GESTURE_DISPLAY_ON;

			/* ��ʱ2�� */
			vTaskDelay(2000);

			/* ���� */
			oled.ctrl=OLED_CTRL_CLEAR;

			xReturn = xQueueSend( 	g_queue_oled,	/* ��Ϣ���еľ�� */
									&oled,			/* ���͵���Ϣ���� */
									100);			/* �ȴ�ʱ�� 100 Tick */
			if(xReturn != pdPASS)
				dgb_printf_safe("[app_task_gesture] xQueueSend oled clear error code is %d\r\n",xReturn);			
		}
	}

	/* ʶ��BACK�������룬��ֹͣ���� */		
	if(menu_ext->key_fn == KEY_BACK)
		g_gesture_display_flag = FLAG_GESTURE_DISPLAY_OFF;
}
/**
 * @brief ��ȡ��ǰ�˵�����Ŀ����
 * @param .menu ָ��ǰ�˵�
 * @retval ��ǰ�˵���Ŀ����
 * @details ��
 */
uint32_t menu_item_total(menu_t *menu)
{
	menu_t *m = menu;

	uint32_t item_count=0;

	while(m->item)
	{
		/* ָ����һ���˵� */
		m++;

		/* ͳ����Ŀ���� */
		item_count++;
	}
		
	return item_count;
}
/**
 * @brief ��ȡ��ǰ�˵�����Ŀ����
 * @param .menu ָ��ǰ�˵�
 * @retval ��ǰ�˵���Ŀ����
 * @details ��
 */
void item_cursor_show(uint8_t x,uint8_t y,uint8_t *cursor)
{
	oled_t   oled;
	oled.ctrl=OLED_CTRL_SHOW_STRING;
	oled.font_size=16;	
	oled.x=x;
	oled.y=y;
	oled.str=cursor;
	xQueueSend(g_queue_oled,&oled,100);
}
/**
 * @brief ��ʾ�˵���Ŀ
 * @param .menu_ext ָ��ǰ�˵�
 * @retval ��ǰ�˵���Ŀ����
 * @details ��
 */
void menu_show(menu_ext_t *menu_ext)
{
	oled_t   oled;
	uint8_t  x,y=0;

	menu_ext_t *m_ext = menu_ext;
	menu_t *m = m_ext->menu;

	/* ���� */
	oled.ctrl=OLED_CTRL_CLEAR;
	xQueueSend(g_queue_oled,&oled,100);	

	/* ӵ���Ӳ˵�����ʾ��ͷ */
	if(m->child)
	{
		m_ext->item_cursor = 0;
		item_cursor_show(5,y,">");
	}
		
	/* ��ʾ��ǰ�˵�������Ŀ���� */
	oled.ctrl=OLED_CTRL_SHOW_STRING;
	oled.font_size=16;
	x=20;
	y=0;

	while(y<8)
	{
		/* ���˵���Ŀ������Ч�� */
		if(m->item == NULL || m->item[0]=='-' )
			break;		

		dgb_printf_safe("[menu_show] m->item is %s \r\n",m->item);

		oled.x=x;
		oled.y=y;
		oled.str=m->item;
		xQueueSend(g_queue_oled,&oled,100);	

		/* ָ����һ���˵� */
		m++;

		/* y������һ�� */
		y+=2;
	}
}

/**
 * @brief �������ģʽ
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details ����		
 */
static void menu_power_man_standby_fun(void* pvParameters)
{
	uint32_t i=0;
	oled_t oled;
	beep_t beep;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;
	BaseType_t xReturn;

	/* ����ͼƬ�б� */
	const uint8_t *pic_num_tbl[] = {pic_num_3_64x64,pic_num_2_64x64,pic_num_1_64x64};

	dgb_printf_safe("[menu_power_man_standby_fun] ...\r\n");

	/* ʶ��ENTER��������*/	
	if(menu_ext->key_fn == KEY_ENTER)
	{
		dgb_printf_safe("[menu_power_man_standby_fun] cpu run into standby...\r\n");

		/* ��ʾ���� */
		for(i=0; i<3; i++)
		{
			/* ����Ҫ��ʾ��ͼƬ-3��2��1 */
			oled.x=32;
			oled.y=0;
			oled.pic_width=64;
			oled.pic_height=8;
			oled.ctrl=OLED_CTRL_SHOW_PICTURE;
			oled.pic=pic_num_tbl[i];

			xReturn = xQueueSend( 	g_queue_oled,	/* ��Ϣ���еľ�� */
									&oled,			/* ���͵���Ϣ���� */
									100);			/* �ȴ�ʱ�� 100 Tick */
			if(xReturn != pdPASS)
				dgb_printf_safe("[menu_power_man_standby_fun] xQueueSend oled picture error code is %d\r\n",xReturn);

			vTaskDelay(1000);
		}
		

		/* OLED�������� */
		oled.ctrl = OLED_CTRL_DISPLAY_OFF;

		xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
								&oled,		   /* ���͵���Ϣ���� */
								100);		   /* �ȴ�ʱ�� 100 Tick */
		if (xReturn != pdPASS)
			dgb_printf_safe("[menu_power_man_standby_fun] xQueueSend oled error code is %d\r\n", xReturn);	

		/* ��һ��ʾ�� */
		beep.sta = 1;
		beep.duration = 100;

		xReturn = xQueueSend(g_queue_beep, /* ��Ϣ���еľ�� */
							&beep,		   /* ���͵���Ϣ���� */
							100);		   /* �ȴ�ʱ�� 100 Tick */				
		if (xReturn != pdPASS)
			dgb_printf_safe("[menu_power_man_standby_fun] xQueueSend beep error code is %d\r\n", xReturn);

		/* [��ѡ]�����ȴ��ź���������ȷ��������ɶ�beep�Ŀ��� */
		xReturn = xSemaphoreTake(g_sem_beep, portMAX_DELAY);
		if (xReturn != pdPASS)
			dgb_printf_safe("[menu_power_man_standby_fun] xSemaphoreTake g_sem_beep error code is %d\r\n", xReturn);	

		g_cpu_operating_mode = CPU_STOP;

		/* ��������� */
		vTaskSuspendAll();

		/* �����ٽ��� */
		taskENTER_CRITICAL();

		/* �رջ����ж� */
		RTC_ITConfig(RTC_IT_WUT, DISABLE);

		/* �ر�ϵͳ��ʱ�� */
		SysTick->CTRL = 0x00;
		
		/* �˳��ٽ��� */
		taskEXIT_CRITICAL();
	

		/* С��ʱһ�� */
		i=0x100000;
		while(i--);


		if(menu_ext->item_cursor == 0)
		{
			/* ͨ��PA0���������ػ��� */
			PWR_WakeUpPinCmd(ENABLE);
		}

		if(menu_ext->item_cursor == 1)
		{
			/* 10����Զ����� */
			PWR_WakeUpPinCmd(DISABLE);
			rtc_alarm_set_in_second(10);
		}
		PWR_ClearFlag(PWR_FLAG_SB);	
		PWR_ClearFlag(PWR_FLAG_WU);

		taskENTER_CRITICAL();
		g_cpu_operating_mode = CPU_STANDBY;
		taskEXIT_CRITICAL();
		
		/* �������ģʽ */
		PWR_EnterSTANDBYMode();
	}	
}

/**
 * @brief ����ֹͣģʽ
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details ����		
 */
static void menu_power_man_stop_fun(void* pvParameters)
{
	uint32_t i=0;
	oled_t oled;
	beep_t beep;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;
	BaseType_t xReturn;
	/* ����ͼƬ�б� */
	const uint8_t *pic_num_tbl[] = {pic_num_3_64x64,pic_num_2_64x64,pic_num_1_64x64};	

	dgb_printf_safe("[menu_power_man_stop_fun] ...\r\n");

	/* ʶ��ENTER��������*/	
	if(menu_ext->key_fn == KEY_ENTER)
	{
		dgb_printf_safe("[menu_power_man_stop_fun] cpu run into stop...\r\n");
		
		/* �����ٽ��� */
		taskENTER_CRITICAL();

		/* CPU����ģʽ��־-ֹͣģʽ */
		g_cpu_operating_mode = CPU_STOP;

		/* �˳��ٽ��� */
		taskEXIT_CRITICAL();

		/* ��ʾ���� */
		for(i=0; i<3; i++)
		{
			/* ����Ҫ��ʾ��ͼƬ-3��2��1 */
			oled.x=32;
			oled.y=0;
			oled.pic_width=64;
			oled.pic_height=8;
			oled.ctrl=OLED_CTRL_SHOW_PICTURE;
			oled.pic=pic_num_tbl[i];

			xReturn = xQueueSend( 	g_queue_oled,	/* ��Ϣ���еľ�� */
									&oled,			/* ���͵���Ϣ���� */
									100);			/* �ȴ�ʱ�� 100 Tick */
			if(xReturn != pdPASS)
				dgb_printf_safe("[menu_power_man_stop_fun] xQueueSend oled picture error code is %d\r\n",xReturn);

			vTaskDelay(1000);
		}

		/* ���� */
		oled.ctrl=OLED_CTRL_CLEAR;
		xQueueSend(g_queue_oled,&oled,100);	

		/* OLED�������� */
		oled.ctrl = OLED_CTRL_DISPLAY_OFF;

		xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
								&oled,		   /* ���͵���Ϣ���� */
								100);		   /* �ȴ�ʱ�� 100 Tick */
		if (xReturn != pdPASS)
			dgb_printf_safe("[menu_power_man_stop_fun] xQueueSend oled error code is %d\r\n", xReturn);	

		/* ��һ��ʾ�� */
		beep.sta = 1;
		beep.duration = 100;

		xReturn = xQueueSend(g_queue_beep, /* ��Ϣ���еľ�� */
							&beep,		   /* ���͵���Ϣ���� */
							100);		   /* �ȴ�ʱ�� 100 Tick */				
		if (xReturn != pdPASS)
			dgb_printf_safe("[menu_power_man_stop_fun] xQueueSend beep error code is %d\r\n", xReturn);

		/* [��ѡ]�����ȴ��ź���������ȷ��������ɶ�beep�Ŀ��� */
		xReturn = xSemaphoreTake(g_sem_beep, portMAX_DELAY);
		if (xReturn != pdPASS)
			dgb_printf_safe("[menu_power_man_stop_fun] xSemaphoreTake g_sem_beep error code is %d\r\n", xReturn);	


		/* ��������� */
		vTaskSuspendAll();

		/* �����ٽ��� */
		taskENTER_CRITICAL();

		/* �رջ����ж� */
		RTC_ITConfig(RTC_IT_WUT, DISABLE);

		/* �ر�ϵͳ��ʱ�� */
		SysTick->CTRL = 0x00;

		/* �˳��ٽ��� */
		taskEXIT_CRITICAL();


		/* 10����Զ����� */
		if(menu_ext->item_cursor == 1)
			rtc_alarm_set_in_second(10);

		/* С��ʱһ�� */
		i=0x100000;
		while(i--);

		//��CPU����ֹͣģʽ��CPU������ֹͣ�����κ�ָ��������е�ʱ�Ӷ�ֹͣ�� PLL�� HSI
		//�� HSE RC ����Ҳ����ֹ,�ڲ� SRAM �ͼĴ������ݽ�������
		PWR_EnterSTOPMode(PWR_LowPowerRegulator_ON,PWR_STOPEntry_WFI);
				
		//�˳�ֹͣģʽ֮��M4���Զ�ѡ��HSI RC������Ϊʱ��Դ������ʵ��������֮ǰ�Ĵ��붼һֱʹ��PLL�������½���ʱ�ӵĳ�ʼ��
		SystemInit();
	
		/* С��ʱһ�� */
		i=0x100000;
		while(i--);
		
		printf("[menu_power_man_stop_fun] cpu wake up from stop...\r\n");

		/* ����ʹ��ϵͳ��ʱ�� */
		SysTick_Config(SystemCoreClock/configTICK_RATE_HZ);	

		//�����жϵĴ�����ʽ�������ж�
		RTC_ITConfig(RTC_IT_WUT, ENABLE);
		RTC_ClearFlag(RTC_FLAG_WUTF);
		EXTI_ClearITPendingBit(EXTI_Line22);

		/* �ָ������� */
		xTaskResumeAll();

		/* OLED�������� */
		oled.ctrl = OLED_CTRL_DISPLAY_ON;

		xReturn = xQueueSend(g_queue_oled, 	/* ��Ϣ���еľ�� */
								&oled,		/* ���͵���Ϣ���� */
								100);		/* �ȴ�ʱ�� 100 Tick */
		if (xReturn != pdPASS)
			dgb_printf_safe("[menu_power_man_stop_fun] xQueueSend oled error code is %d\r\n", xReturn);

		taskENTER_CRITICAL();
		g_cpu_operating_mode = CPU_RUN;
		taskEXIT_CRITICAL();

		/* mpu6050�������ԣ���Ҫ���³�ʼ�� */
		MPU_Init();

		mpu_dmp_init();		

		/* ����KEY_BACK�����¼� */
		xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_BACK);			
	
	}	
}

/**
 * @brief ����˯��ģʽ
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details ����		
 */
static void menu_power_man_sleep_fun(void* pvParameters)
{
	oled_t oled;
	beep_t beep;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;

	BaseType_t xReturn;

	dgb_printf_safe("[menu_power_man_sleep_fun] ...\r\n");

	/* ʶ��ENTER��������*/	
	if(menu_ext->key_fn == KEY_ENTER)
	{
		taskENTER_CRITICAL();
		if(menu_ext->item_cursor == 0)
			g_cpu_operating_mode = CPU_SLEEP;

		if(menu_ext->item_cursor == 1)
			g_cpu_operating_mode = CPU_RUN;
		taskEXIT_CRITICAL();

		/* ����Ҫ��ʾ��ͼƬ-���飨�ɹ��� */
		oled.x=32;
		oled.y=0;
		oled.pic_width=64;
		oled.pic_height=8;
		oled.ctrl=OLED_CTRL_SHOW_PICTURE;
		oled.pic=pic_face_success_icon;

		xReturn = xQueueSend( 	g_queue_oled,	/* ��Ϣ���еľ�� */
								&oled,			/* ���͵���Ϣ���� */
								100);			/* �ȴ�ʱ�� 100 Tick */
		if(xReturn != pdPASS)
			dgb_printf_safe("[app_task_pass_man] xQueueSend oled picture error code is %d\r\n",xReturn);

		/* ��һ��ʾ�� */
		beep.sta = 1;
		beep.duration = 100;

		xReturn = xQueueSend(g_queue_beep, /* ��Ϣ���еľ�� */
							&beep,		   /* ���͵���Ϣ���� */
							100);		   /* �ȴ�ʱ�� 100 Tick */				
		if (xReturn != pdPASS)
			dgb_printf_safe("[app_task_pass_man] xQueueSend beep error code is %d\r\n", xReturn);

		/* [��ѡ]�����ȴ��ź���������ȷ��������ɶ�beep�Ŀ��� */
		xReturn = xSemaphoreTake(g_sem_beep, portMAX_DELAY);
		if (xReturn != pdPASS)
			dgb_printf_safe("[app_task_pass_man] xSemaphoreTake g_sem_beep error code is %d\r\n", xReturn);	
		
		vTaskDelay(2000);

		/* ����KEY_BACK�����¼� */
		xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_BACK);			
	}	
}
