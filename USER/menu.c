/* 作者:粤嵌.温子祺 */
#include "includes.h"

/* 菜单相关 */
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



/*	一级菜单
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
/*	一级菜单
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

/*	一级菜单
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
	二级菜单
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
	二级菜单
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
	二级菜单
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
	二级菜单
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
	三级菜单
	DHT、RTC、MPU6050等运行控制
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
	4级菜单

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
 * @brief 密码管理界面
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details .识别到ENTER键
 * 			.识别到BACK键
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

	/* 识别到ENTER按键编码 */
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
			/* 设置要显示的图片 */
			oled.x=0;
			oled.y=0;
			oled.pic_width=50;
			oled.pic_height=8;
			oled.ctrl=OLED_CTRL_SHOW_PICTURE;
			oled.pic=pic_locker_icon;

			xReturn = xQueueSend( 	g_queue_oled,	/* 消息队列的句柄 */
									&oled,			/* 发送的消息内容 */
									100);			/* 等待时间 100 Tick */
			if(xReturn != pdPASS)
				dgb_printf_safe("[menu_pass_man_fun] xQueueSend oled picture error code is %d\r\n",xReturn);	


			/* 显示汉字-旧密码 */
			oled.ctrl = OLED_CTRL_SHOW_CHINESE;
			oled.y = 2;
			chn_tbl[0]=39;chn_tbl[1]=40;chn_tbl[2]=41;
		
			for(i=0; i<3; i++)
			{
				oled.x = 66+i*16;
				oled.chinese = chn_tbl[i];

				xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
									&oled,		   /* 发送的消息内容 */
									100);		   /* 等待时间 100 Tick */
				if (xReturn != pdPASS)
					dgb_printf_safe("[menu_pass_man_fun] xQueueSend oled string error code is %d\r\n", xReturn);

			}

			/* 清空密码显示区 */
			oled.ctrl = OLED_CTRL_SHOW_STRING;
			oled.x = 66;
			oled.y = 4;
			oled.str = "------";
			oled.font_size = 16;

			xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
								&oled,		   /* 发送的消息内容 */
								100);		   /* 等待时间 100 Tick */
			if (xReturn != pdPASS)
				dgb_printf_safe("[menu_pass_man_fun] xQueueSend oled string error code is %d\r\n", xReturn);	


			/* 通知矩阵键盘任务复位操作内容 */
			key_val='C';
			xReturn = xQueueSend(g_queue_kbd, 	/* 消息队列的句柄 */
								&key_val,		/* 发送的消息内容 */
								100);		   	/* 等待时间 100 Tick */
			if (xReturn != pdPASS)
				dgb_printf_safe("[menu_pass_man_fun] xQueueSend kbd error code is %d\r\n", xReturn);					
		}

		if(g_pass_man_what == FLAG_PASS_MAN_MODIFY)
		{
			/* 设置要显示的图片 */
			oled.x=0;
			oled.y=0;
			oled.pic_width=64;
			oled.pic_height=8;
			oled.ctrl=OLED_CTRL_SHOW_PICTURE;
			oled.pic=pic_pass_modify_icon;

			xReturn = xQueueSend( 	g_queue_oled,	/* 消息队列的句柄 */
									&oled,			/* 发送的消息内容 */
									100);			/* 等待时间 100 Tick */
			if(xReturn != pdPASS)
				dgb_printf_safe("[menu_pass_man_fun] xQueueSend oled picture error code is %d\r\n",xReturn);	


			/* 显示汉字-旧密码 */
			oled.ctrl = OLED_CTRL_SHOW_CHINESE;
			oled.y = 0;
			chn_tbl[0]=39;chn_tbl[1]=40;chn_tbl[2]=41;
		
			for(i=0; i<3; i++)
			{
				oled.x = 66+i*16;
				oled.chinese = chn_tbl[i];

				xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
									&oled,		   /* 发送的消息内容 */
									100);		   /* 等待时间 100 Tick */
				if (xReturn != pdPASS)
					dgb_printf_safe("[menu_pass_man_fun] xQueueSend oled string error code is %d\r\n", xReturn);

			}


			/* 清空旧密码显示区 */
			oled.ctrl = OLED_CTRL_SHOW_STRING;
			oled.x = 66;
			oled.y = 2;
			oled.str = "------";
			oled.font_size = 16;

			xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
								&oled,		   /* 发送的消息内容 */
								1000);		   /* 等待时间 100 Tick */
			if (xReturn != pdPASS)
				dgb_printf_safe("[menu_pass_man_fun] xQueueSend oled string error code is %d\r\n", xReturn);

			/* 清空新密码显示区 */
			oled.ctrl = OLED_CTRL_SHOW_STRING;
			oled.x = 66;			
			oled.y = 6;
			oled.str = "------";
			oled.font_size = 16;
			xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
								&oled,		   /* 发送的消息内容 */
								1000);		   /* 等待时间 100 Tick */
			if (xReturn != pdPASS)
				dgb_printf_safe("[menu_pass_man_fun] xQueueSend oled string error code is %d\r\n", xReturn);


			/* 显示汉字-新密码 */
			oled.ctrl = OLED_CTRL_SHOW_CHINESE;
			oled.x = 66;
			oled.y = 4;
			chn_tbl[0]=38;chn_tbl[1]=40;chn_tbl[2]=41;
		
			for(i=0; i<3; i++)
			{
				oled.x = 66+i*16;
				oled.chinese = chn_tbl[i];

				xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
									&oled,		   /* 发送的消息内容 */
									100);		   /* 等待时间 100 Tick */
				if (xReturn != pdPASS)
					dgb_printf_safe("[menu_pass_man_fun] xQueueSend oled string error code is %d\r\n", xReturn);

			}

			/* 通知矩阵键盘任务复位操作内容 */
			key_val='C';
			xReturn = xQueueSend(g_queue_kbd, 	/* 消息队列的句柄 */
								&key_val,		/* 发送的消息内容 */
								100);		   	/* 等待时间 100 Tick */
			if (xReturn != pdPASS)
				dgb_printf_safe("[menu_pass_man_fun] xQueueSend kbd error code is %d\r\n", xReturn);
		}

		/* 恢复矩阵键盘任务 */
		vTaskResume(app_task_kbd_handle);		
	}

	/* 识别到BACK按键编码，则停止矩阵键盘任务运行 */	
	if(menu_ext->key_fn == KEY_BACK)
	{
		/* 挂起矩阵键盘任务 */
		vTaskSuspend(app_task_kbd_handle);
	}	
}


/**
 * @brief 进入温湿度菜单后执行相关的控制与配置
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details .识别到ENTER键，恢复温湿度任务运行；
 * 			.识别到BACK键，挂起温湿度任务
 */
static void menu_dht_fun(void* pvParameters)
{
	oled_t oled;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;

	BaseType_t xReturn;

	dgb_printf_safe("[menu_dht_fun] ...\r\n");

	/* 识别到ENTER按键编码，则恢复DHT任务运行并获取温度/湿度 */	
	if(menu_ext->key_fn == KEY_ENTER)
	{
		if(menu_ext->item_cursor == 0)
			g_dht_get_what = FLAG_DHT_GET_TEMP;

		if(menu_ext->item_cursor == 1)
			g_dht_get_what = FLAG_DHT_GET_HUMI;

		if(g_dht_get_what!=FLAG_DHT_GET_TEMP && g_dht_get_what!=FLAG_DHT_GET_HUMI)
			return;			

		/* 设置要显示的图片 */
		oled.x=0;
		oled.y=0;
		oled.pic_width=32;
		oled.pic_height=8;
		oled.ctrl=OLED_CTRL_SHOW_PICTURE;
		oled.pic=pic_dht_icon;

		xReturn = xQueueSend( 	g_queue_oled,	/* 消息队列的句柄 */
								&oled,			/* 发送的消息内容 */
								100);			/* 等待时间 100 Tick */
		if(xReturn != pdPASS)
			dgb_printf_safe("[menu_dht_fun] xQueueSend oled picture error code is %d\r\n",xReturn);	

		/* 挂起矩阵键盘任务 */
		vTaskSuspend(app_task_kbd_handle);

		/* 恢复DHT任务运行 */ 
		vTaskResume(app_task_dht_handle);
	}

	/* 识别到BACK按键编码，则停止DHT任务运行 */	
	if(menu_ext->key_fn == KEY_BACK)
	{
		/* 挂起DHT任务 */
		vTaskSuspend(app_task_dht_handle);

		/* 恢复矩阵键盘任务 */
		vTaskResume(app_task_kbd_handle);
	}	
}
/**
 * @brief 进入低功耗蓝牙菜单后执行相关的控制与配置
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 暂无		
 */
static void menu_ble_fun(void* pvParameters)
{
	oled_t oled;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;

	BaseType_t xReturn;

	dgb_printf_safe("[menu_ble_fun] ...\r\n");

	/* 识别到ENTER按键编码*/	
	if(menu_ext->key_fn == KEY_ENTER)
	{
		/* 设置要显示的图片 */
		oled.x=0;
		oled.y=0;
		oled.pic_width=128;
		oled.pic_height=8;
		oled.ctrl=OLED_CTRL_SHOW_PICTURE;
		oled.pic=pic_face1_icon;

		xReturn = xQueueSend( 	g_queue_oled,	/* 消息队列的句柄 */
								&oled,			/* 发送的消息内容 */
								100);			/* 等待时间 100 Tick */
		if(xReturn != pdPASS)
			dgb_printf_safe("[menu_ble_fun] xQueueSend oled picture error code is %d\r\n",xReturn);	
	}

}
/**
 * @brief 进入eeprom菜单后执行相关的控制与配置
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 暂无		
 */
static void menu_eeprom_fun(void* pvParameters)
{
	oled_t oled;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;

	BaseType_t xReturn;

	dgb_printf_safe("[menu_eeprom_fun] ...\r\n");

	/* 识别到ENTER按键编码*/	
	if(menu_ext->key_fn == KEY_ENTER)
	{
		/* 设置要显示的图片 */
		oled.x=0;
		oled.y=0;
		oled.pic_width=128;
		oled.pic_height=8;
		oled.ctrl=OLED_CTRL_SHOW_PICTURE;
		oled.pic=pic_face2_icon;

		xReturn = xQueueSend( 	g_queue_oled,	/* 消息队列的句柄 */
								&oled,			/* 发送的消息内容 */
								100);			/* 等待时间 100 Tick */
		if(xReturn != pdPASS)
			dgb_printf_safe("[menu_eeprom_fun] xQueueSend oled picture error code is %d\r\n",xReturn);	
	}	
}
/**
 * @brief 进入spi flash菜单后执行相关的控制与配置
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 暂无	
 */
static void menu_spi_flash_fun(void* pvParameters)
{
	oled_t oled;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;

	BaseType_t xReturn;

	dgb_printf_safe("[menu_spi_flash_fun] ...\r\n");

	/* 识别到ENTER按键编码*/	
	if(menu_ext->key_fn == KEY_ENTER)
	{
		/* 设置要显示的图片 */
		oled.x=0;
		oled.y=0;
		oled.pic_width=128;
		oled.pic_height=8;
		oled.ctrl=OLED_CTRL_SHOW_PICTURE;
		oled.pic=pic_face3_icon;

		xReturn = xQueueSend( 	g_queue_oled,	/* 消息队列的句柄 */
								&oled,			/* 发送的消息内容 */
								100);			/* 等待时间 100 Tick */
		if(xReturn != pdPASS)
			dgb_printf_safe("[menu_spi_flash_fun] xQueueSend oled picture error code is %d\r\n",xReturn);	
	}	
}
/**
 * @brief 进入作者信息菜单后执行相关的控制与配置
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 暂无	
 */
static void menu_author_fun(void* pvParameters)
{
	oled_t oled;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;

	BaseType_t xReturn;

	dgb_printf_safe("[menu_author_fun] ...\r\n");

	/* 识别到ENTER按键编码*/	
	if(menu_ext->key_fn == KEY_ENTER)
	{
		/* 设置要显示的图片 */
		oled.x=0;
		oled.y=0;
		oled.pic_width=128;
		oled.pic_height=8;
		oled.ctrl=OLED_CTRL_SHOW_PICTURE;
		oled.pic=pic_author_icon;

		xReturn = xQueueSend( 	g_queue_oled,	/* 消息队列的句柄 */
								&oled,			/* 发送的消息内容 */
								100);			/* 等待时间 100 Tick */
		if(xReturn != pdPASS)
			dgb_printf_safe("[menu_author_fun] xQueueSend oled picture error code is %d\r\n",xReturn);	
		
		
		vTaskDelay(3000);
		
		vTaskResume(app_task_oled_animation_handle);
	}
	
	
	/* 识别到BACK按键编码，则停止显示动画 */	
	if(menu_ext->key_fn == KEY_BACK)
		vTaskSuspend(app_task_oled_animation_handle);
}

/**
 * @brief 进入rtc菜单后执行相关的控制与配置
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details .识别到ENTER键，则根据传入的项目光标位置指定获取的是时间还是日期，
 * 			 并恢复rtc任务的运行；
 * 			.识别到BACK键，则挂起rtc任务。
 */
static void menu_rtc_fun(void* pvParameters)
{
	oled_t		oled;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;

	BaseType_t	xReturn;

	dgb_printf_safe("[menu_rtc_fun] ... \r\n");

	/* 识别到ENTER按键编码，则恢复RTC任务运行并获取日期/时间 */	
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
			/* 设置要显示的图片 */
			oled.x=50;
			oled.y=0;
			oled.pic_width=32;
			oled.pic_height=4;
			oled.ctrl=OLED_CTRL_SHOW_PICTURE;
			oled.pic=pic_date_icon;

			xReturn = xQueueSend( 	g_queue_oled,	/* 消息队列的句柄 */
									&oled,			/* 发送的消息内容 */
									100);			/* 等待时间 100 Tick */
			if(xReturn != pdPASS)
				dgb_printf_safe("[menu_rtc_fun] xQueueSend oled picture error code is %d\r\n",xReturn);		

		}
		
		if(g_rtc_get_what == FLAG_RTC_GET_TIME)
		{
			/* 设置要显示的图片 */
			oled.x=50;
			oled.y=0;
			oled.pic_width=32;
			oled.pic_height=4;
			oled.ctrl=OLED_CTRL_SHOW_PICTURE;
			oled.pic=pic_clock_icon;

			xReturn = xQueueSend( 	g_queue_oled,	/* 消息队列的句柄 */
									&oled,			/* 发送的消息内容 */
									100);			/* 等待时间 100 Tick */
			if(xReturn != pdPASS)
				dgb_printf_safe("[menu_rtc_fun] xQueueSend oled picture error code is %d\r\n",xReturn);		

		}
		/* 恢复RTC任务运行 */
		vTaskResume(app_task_rtc_handle);
	}

	/* 识别到BACK按键编码，则停止RTC任务运行 */	
	if(menu_ext->key_fn == KEY_BACK)
		vTaskSuspend(app_task_rtc_handle);
}
/**
 * @brief 进入mpu6050菜单后执行相关的控制与配置
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details .识别到ENTER键，则配置显示欧拉角；
 * 			.识别到BACK键，则配置不显示欧拉角。
 */
static void menu_mpu6050_fun(void* pvParameters)
{
	oled_t		oled;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;

	BaseType_t	xReturn;

	dgb_printf_safe("[menu_mpu6050_fun] ... \r\n");

	/* 识别到ENTER按键编码，则获取欧拉角 */
	if(menu_ext->key_fn == KEY_ENTER)
	{
		/* 设置要显示的图片 */
		oled.x=50;
		oled.y=0;
		oled.pic_width=32;
		oled.pic_height=4;
		oled.ctrl=OLED_CTRL_SHOW_PICTURE;
		oled.pic=pic_mpu6050_icon;

		xReturn = xQueueSend( 	g_queue_oled,	/* 消息队列的句柄 */
								&oled,			/* 发送的消息内容 */
								100);			/* 等待时间 100 Tick */
		if(xReturn != pdPASS)
			dgb_printf_safe("[menu_mpu6050_fun] xQueueSend oled picture error code is %d\r\n",xReturn);	

		g_mpu6050_get_what = FLAG_MPU6050_GET_EULER;		
	}
		

	/* 识别到BACK按键编码，则停止工作 */		
	if(menu_ext->key_fn == KEY_BACK)
		g_mpu6050_get_what = FLAG_MPU6050_GET_NONE;
}
/**
 * @brief 进入手势识别菜单后执行相关的控制与配置
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details .识别到ENTER键，则配置显示手势图片；
 * 			.识别到BACK键，则配置不显示手势图片；
 * 			.识别g_gesture_valid变量有效性，如果开机无接上手势识别模块，则显示警告图标
 * 			 提示手势识别功能无效。
 */
static void menu_gesture_fun(void* pvParameters)
{
	oled_t 		oled;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;
	BaseType_t xReturn;

	dgb_printf_safe("[menu_gesture_fun] ...\r\n");

	/* 手势识别模块无效，则显示图片与文字用作提示 */
	if(!g_gesture_valid)
	{
		dgb_printf_safe("[menu_gesture_fun] gesture is invalid, please press KEY_BACK to return\r\n");
		
		/* 设置要显示的图片 */
		oled.x=45;
		oled.y=0;
		oled.pic_width=32;
		oled.pic_height=4;
		oled.ctrl=OLED_CTRL_SHOW_PICTURE;
		oled.pic=pic_warn_icon;

		xReturn = xQueueSend( 	g_queue_oled,	/* 消息队列的句柄 */
								&oled,			/* 发送的消息内容 */
								100);			/* 等待时间 100 Tick */
		if(xReturn != pdPASS)
			dgb_printf_safe("[app_task_gesture] xQueueSend oled picture error code is %d\r\n",xReturn);	

		/* 设置要显示的字符串 */
		oled.x=2;
		oled.y=5;
		oled.str="gesture invalid";
		oled.font_size=12;
		oled.ctrl=OLED_CTRL_SHOW_STRING;

		xReturn = xQueueSend( 	g_queue_oled,	/* 消息队列的句柄 */
								&oled,			/* 发送的消息内容 */
								100);			/* 等待时间 100 Tick */
		if(xReturn != pdPASS)
			dgb_printf_safe("[app_task_gesture] xQueueSend oled string error code is %d\r\n",xReturn);
	}

	/* 识别到ENTER按键编码 */
	if(menu_ext->key_fn == KEY_ENTER)
	{
		/* 手势识别模块有效，则文字提示准备开始手势识别测试 */
		if(g_gesture_valid)
		{
			/* 设置要显示的字符串 */
			oled.x=15;
			oled.y=3;
			oled.str="Let's Begin!";
			oled.font_size=16;
			oled.ctrl=OLED_CTRL_SHOW_STRING;

			xReturn = xQueueSend( 	g_queue_oled,	/* 消息队列的句柄 */
									&oled,			/* 发送的消息内容 */
									100);			/* 等待时间 100 Tick */
			if(xReturn != pdPASS)
				dgb_printf_safe("[app_task_gesture] xQueueSend oled string error code is %d\r\n",xReturn);

			g_gesture_display_flag = FLAG_GESTURE_DISPLAY_ON;

			/* 延时2秒 */
			vTaskDelay(2000);

			/* 清屏 */
			oled.ctrl=OLED_CTRL_CLEAR;

			xReturn = xQueueSend( 	g_queue_oled,	/* 消息队列的句柄 */
									&oled,			/* 发送的消息内容 */
									100);			/* 等待时间 100 Tick */
			if(xReturn != pdPASS)
				dgb_printf_safe("[app_task_gesture] xQueueSend oled clear error code is %d\r\n",xReturn);			
		}
	}

	/* 识别到BACK按键编码，则停止工作 */		
	if(menu_ext->key_fn == KEY_BACK)
		g_gesture_display_flag = FLAG_GESTURE_DISPLAY_OFF;
}
/**
 * @brief 获取当前菜单的项目总数
 * @param .menu 指向当前菜单
 * @retval 当前菜单项目总数
 * @details 无
 */
uint32_t menu_item_total(menu_t *menu)
{
	menu_t *m = menu;

	uint32_t item_count=0;

	while(m->item)
	{
		/* 指向下一个菜单 */
		m++;

		/* 统计项目数量 */
		item_count++;
	}
		
	return item_count;
}
/**
 * @brief 获取当前菜单的项目总数
 * @param .menu 指向当前菜单
 * @retval 当前菜单项目总数
 * @details 无
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
 * @brief 显示菜单项目
 * @param .menu_ext 指向当前菜单
 * @retval 当前菜单项目总数
 * @details 无
 */
void menu_show(menu_ext_t *menu_ext)
{
	oled_t   oled;
	uint8_t  x,y=0;

	menu_ext_t *m_ext = menu_ext;
	menu_t *m = m_ext->menu;

	/* 清屏 */
	oled.ctrl=OLED_CTRL_CLEAR;
	xQueueSend(g_queue_oled,&oled,100);	

	/* 拥有子菜单，显示箭头 */
	if(m->child)
	{
		m_ext->item_cursor = 0;
		item_cursor_show(5,y,">");
	}
		
	/* 显示当前菜单所有项目名称 */
	oled.ctrl=OLED_CTRL_SHOW_STRING;
	oled.font_size=16;
	x=20;
	y=0;

	while(y<8)
	{
		/* 检查菜单项目标题有效性 */
		if(m->item == NULL || m->item[0]=='-' )
			break;		

		dgb_printf_safe("[menu_show] m->item is %s \r\n",m->item);

		oled.x=x;
		oled.y=y;
		oled.str=m->item;
		xQueueSend(g_queue_oled,&oled,100);	

		/* 指向下一个菜单 */
		m++;

		/* y坐标下一行 */
		y+=2;
	}
}

/**
 * @brief 进入待机模式
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 暂无		
 */
static void menu_power_man_standby_fun(void* pvParameters)
{
	uint32_t i=0;
	oled_t oled;
	beep_t beep;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;
	BaseType_t xReturn;

	/* 倒数图片列表 */
	const uint8_t *pic_num_tbl[] = {pic_num_3_64x64,pic_num_2_64x64,pic_num_1_64x64};

	dgb_printf_safe("[menu_power_man_standby_fun] ...\r\n");

	/* 识别到ENTER按键编码*/	
	if(menu_ext->key_fn == KEY_ENTER)
	{
		dgb_printf_safe("[menu_power_man_standby_fun] cpu run into standby...\r\n");

		/* 显示倒数 */
		for(i=0; i<3; i++)
		{
			/* 设置要显示的图片-3、2、1 */
			oled.x=32;
			oled.y=0;
			oled.pic_width=64;
			oled.pic_height=8;
			oled.ctrl=OLED_CTRL_SHOW_PICTURE;
			oled.pic=pic_num_tbl[i];

			xReturn = xQueueSend( 	g_queue_oled,	/* 消息队列的句柄 */
									&oled,			/* 发送的消息内容 */
									100);			/* 等待时间 100 Tick */
			if(xReturn != pdPASS)
				dgb_printf_safe("[menu_power_man_standby_fun] xQueueSend oled picture error code is %d\r\n",xReturn);

			vTaskDelay(1000);
		}
		

		/* OLED灭屏控制 */
		oled.ctrl = OLED_CTRL_DISPLAY_OFF;

		xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
								&oled,		   /* 发送的消息内容 */
								100);		   /* 等待时间 100 Tick */
		if (xReturn != pdPASS)
			dgb_printf_safe("[menu_power_man_standby_fun] xQueueSend oled error code is %d\r\n", xReturn);	

		/* 嘀一声示意 */
		beep.sta = 1;
		beep.duration = 100;

		xReturn = xQueueSend(g_queue_beep, /* 消息队列的句柄 */
							&beep,		   /* 发送的消息内容 */
							100);		   /* 等待时间 100 Tick */				
		if (xReturn != pdPASS)
			dgb_printf_safe("[menu_power_man_standby_fun] xQueueSend beep error code is %d\r\n", xReturn);

		/* [可选]阻塞等待信号量，用于确保任务完成对beep的控制 */
		xReturn = xSemaphoreTake(g_sem_beep, portMAX_DELAY);
		if (xReturn != pdPASS)
			dgb_printf_safe("[menu_power_man_standby_fun] xSemaphoreTake g_sem_beep error code is %d\r\n", xReturn);	

		g_cpu_operating_mode = CPU_STOP;

		/* 挂起调度器 */
		vTaskSuspendAll();

		/* 进入临界区 */
		taskENTER_CRITICAL();

		/* 关闭唤醒中断 */
		RTC_ITConfig(RTC_IT_WUT, DISABLE);

		/* 关闭系统定时器 */
		SysTick->CTRL = 0x00;
		
		/* 退出临界区 */
		taskEXIT_CRITICAL();
	

		/* 小延时一会 */
		i=0x100000;
		while(i--);


		if(menu_ext->item_cursor == 0)
		{
			/* 通过PA0引脚上升沿唤醒 */
			PWR_WakeUpPinCmd(ENABLE);
		}

		if(menu_ext->item_cursor == 1)
		{
			/* 10秒后自动唤醒 */
			PWR_WakeUpPinCmd(DISABLE);
			rtc_alarm_set_in_second(10);
		}
		PWR_ClearFlag(PWR_FLAG_SB);	
		PWR_ClearFlag(PWR_FLAG_WU);

		taskENTER_CRITICAL();
		g_cpu_operating_mode = CPU_STANDBY;
		taskEXIT_CRITICAL();
		
		/* 进入待机模式 */
		PWR_EnterSTANDBYMode();
	}	
}

/**
 * @brief 进入停止模式
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 暂无		
 */
static void menu_power_man_stop_fun(void* pvParameters)
{
	uint32_t i=0;
	oled_t oled;
	beep_t beep;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;
	BaseType_t xReturn;
	/* 倒数图片列表 */
	const uint8_t *pic_num_tbl[] = {pic_num_3_64x64,pic_num_2_64x64,pic_num_1_64x64};	

	dgb_printf_safe("[menu_power_man_stop_fun] ...\r\n");

	/* 识别到ENTER按键编码*/	
	if(menu_ext->key_fn == KEY_ENTER)
	{
		dgb_printf_safe("[menu_power_man_stop_fun] cpu run into stop...\r\n");
		
		/* 进入临界区 */
		taskENTER_CRITICAL();

		/* CPU运行模式标志-停止模式 */
		g_cpu_operating_mode = CPU_STOP;

		/* 退出临界区 */
		taskEXIT_CRITICAL();

		/* 显示倒数 */
		for(i=0; i<3; i++)
		{
			/* 设置要显示的图片-3、2、1 */
			oled.x=32;
			oled.y=0;
			oled.pic_width=64;
			oled.pic_height=8;
			oled.ctrl=OLED_CTRL_SHOW_PICTURE;
			oled.pic=pic_num_tbl[i];

			xReturn = xQueueSend( 	g_queue_oled,	/* 消息队列的句柄 */
									&oled,			/* 发送的消息内容 */
									100);			/* 等待时间 100 Tick */
			if(xReturn != pdPASS)
				dgb_printf_safe("[menu_power_man_stop_fun] xQueueSend oled picture error code is %d\r\n",xReturn);

			vTaskDelay(1000);
		}

		/* 清屏 */
		oled.ctrl=OLED_CTRL_CLEAR;
		xQueueSend(g_queue_oled,&oled,100);	

		/* OLED灭屏控制 */
		oled.ctrl = OLED_CTRL_DISPLAY_OFF;

		xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
								&oled,		   /* 发送的消息内容 */
								100);		   /* 等待时间 100 Tick */
		if (xReturn != pdPASS)
			dgb_printf_safe("[menu_power_man_stop_fun] xQueueSend oled error code is %d\r\n", xReturn);	

		/* 嘀一声示意 */
		beep.sta = 1;
		beep.duration = 100;

		xReturn = xQueueSend(g_queue_beep, /* 消息队列的句柄 */
							&beep,		   /* 发送的消息内容 */
							100);		   /* 等待时间 100 Tick */				
		if (xReturn != pdPASS)
			dgb_printf_safe("[menu_power_man_stop_fun] xQueueSend beep error code is %d\r\n", xReturn);

		/* [可选]阻塞等待信号量，用于确保任务完成对beep的控制 */
		xReturn = xSemaphoreTake(g_sem_beep, portMAX_DELAY);
		if (xReturn != pdPASS)
			dgb_printf_safe("[menu_power_man_stop_fun] xSemaphoreTake g_sem_beep error code is %d\r\n", xReturn);	


		/* 挂起调度器 */
		vTaskSuspendAll();

		/* 进入临界区 */
		taskENTER_CRITICAL();

		/* 关闭唤醒中断 */
		RTC_ITConfig(RTC_IT_WUT, DISABLE);

		/* 关闭系统定时器 */
		SysTick->CTRL = 0x00;

		/* 退出临界区 */
		taskEXIT_CRITICAL();


		/* 10秒后自动唤醒 */
		if(menu_ext->item_cursor == 1)
			rtc_alarm_set_in_second(10);

		/* 小延时一会 */
		i=0x100000;
		while(i--);

		//让CPU进入停止模式，CPU不仅仅停止运行任何指令，而且所有的时钟都停止， PLL、 HSI
		//和 HSE RC 振荡器也被禁止,内部 SRAM 和寄存器内容将保留。
		PWR_EnterSTOPMode(PWR_LowPowerRegulator_ON,PWR_STOPEntry_WFI);
				
		//退出停止模式之后，M4会自动选择HSI RC振荡器作为时钟源，但是实际上我们之前的代码都一直使用PLL，则重新进行时钟的初始化
		SystemInit();
	
		/* 小延时一会 */
		i=0x100000;
		while(i--);
		
		printf("[menu_power_man_stop_fun] cpu wake up from stop...\r\n");

		/* 重新使能系统定时器 */
		SysTick_Config(SystemCoreClock/configTICK_RATE_HZ);	

		//配置中断的触发方式：唤醒中断
		RTC_ITConfig(RTC_IT_WUT, ENABLE);
		RTC_ClearFlag(RTC_FLAG_WUTF);
		EXTI_ClearITPendingBit(EXTI_Line22);

		/* 恢复调度器 */
		xTaskResumeAll();

		/* OLED亮屏控制 */
		oled.ctrl = OLED_CTRL_DISPLAY_ON;

		xReturn = xQueueSend(g_queue_oled, 	/* 消息队列的句柄 */
								&oled,		/* 发送的消息内容 */
								100);		/* 等待时间 100 Tick */
		if (xReturn != pdPASS)
			dgb_printf_safe("[menu_power_man_stop_fun] xQueueSend oled error code is %d\r\n", xReturn);

		taskENTER_CRITICAL();
		g_cpu_operating_mode = CPU_RUN;
		taskEXIT_CRITICAL();

		/* mpu6050的特殊性，需要重新初始化 */
		MPU_Init();

		mpu_dmp_init();		

		/* 发送KEY_BACK按键事件 */
		xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_BACK);			
	
	}	
}

/**
 * @brief 进入睡眠模式
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 暂无		
 */
static void menu_power_man_sleep_fun(void* pvParameters)
{
	oled_t oled;
	beep_t beep;
	menu_ext_t *menu_ext = (menu_ext_t *)pvParameters;

	BaseType_t xReturn;

	dgb_printf_safe("[menu_power_man_sleep_fun] ...\r\n");

	/* 识别到ENTER按键编码*/	
	if(menu_ext->key_fn == KEY_ENTER)
	{
		taskENTER_CRITICAL();
		if(menu_ext->item_cursor == 0)
			g_cpu_operating_mode = CPU_SLEEP;

		if(menu_ext->item_cursor == 1)
			g_cpu_operating_mode = CPU_RUN;
		taskEXIT_CRITICAL();

		/* 设置要显示的图片-表情（成功） */
		oled.x=32;
		oled.y=0;
		oled.pic_width=64;
		oled.pic_height=8;
		oled.ctrl=OLED_CTRL_SHOW_PICTURE;
		oled.pic=pic_face_success_icon;

		xReturn = xQueueSend( 	g_queue_oled,	/* 消息队列的句柄 */
								&oled,			/* 发送的消息内容 */
								100);			/* 等待时间 100 Tick */
		if(xReturn != pdPASS)
			dgb_printf_safe("[app_task_pass_man] xQueueSend oled picture error code is %d\r\n",xReturn);

		/* 嘀一声示意 */
		beep.sta = 1;
		beep.duration = 100;

		xReturn = xQueueSend(g_queue_beep, /* 消息队列的句柄 */
							&beep,		   /* 发送的消息内容 */
							100);		   /* 等待时间 100 Tick */				
		if (xReturn != pdPASS)
			dgb_printf_safe("[app_task_pass_man] xQueueSend beep error code is %d\r\n", xReturn);

		/* [可选]阻塞等待信号量，用于确保任务完成对beep的控制 */
		xReturn = xSemaphoreTake(g_sem_beep, portMAX_DELAY);
		if (xReturn != pdPASS)
			dgb_printf_safe("[app_task_pass_man] xSemaphoreTake g_sem_beep error code is %d\r\n", xReturn);	
		
		vTaskDelay(2000);

		/* 发送KEY_BACK按键事件 */
		xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_BACK);			
	}	
}
