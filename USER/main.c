/****************************************************************
*名    称:FreeRTOS综合代码实例
*作    者:温子祺
*创建日期:2021/07/21
*知 识 点:
	1.开机图形Logo与中文字符显示
	2.任务创建、删除、挂起、恢复
	3.信号量
		（1）消息队列发送完成后结合信号量通知确保完成。
	4.互斥锁
		（1）任务互斥访问printf	
	5.消息队列
		（1）串口1数据消息队列处理，设置RTC日期与时间
			.设置RTC日期:DATE SET-2021-7-21-3#		
			.设置RTC时间:TIME SET-10-30-45#		
		（2）LED灯状态消息队列处理	
		（3）蜂鸣器数据消息队列处理
		（4）OLED屏亮灭控制、图像显示
	6.事件标志组
		（1）记录按键外部中断事件标志
		（2）记录RTC唤醒中断事件标志
		（3）记录系统复位事件标志
		（4）记录手势识别模块中断事件标志
		（5）记录温湿度、RTC、MPU6050、OLED等事件标志			
	7.临界区
		（1）关键变量、代码的保护
		（2）看门狗喂狗		
	8.程序跑飞捕捉
		（1）捕捉HardFault_Handler
			当触发HardFault，用LED灯的全亮与全灭间隔闪烁告知，并打印相关栈信息
		（2）捕捉有问题的异常中断
			当触发有问题的异常中断，打印其对应的异常中断号
	9.软件定时器
		（1）软件定时器的创建与配置
		（2）软件定时器的回调函数实现独立看门狗喂狗操作	
	10.空闲任务钩子函数
		（1）一旦进入空闲任务，则让CPU进入睡眠模式，降低功耗

*说  明:		
	当前代码规范、结构清晰，尽可能实现了模块化编程，一个任务管理一个硬件。
	
*修改日期:2021/07/22
	.增加长按S1键，实现系统复位；长按S4键，实现OLED屏亮/灭。
	.增加OLED_SAFE宏代码，实现OLED安全互斥访问
	.增加mpu6050抬手唤醒
	
*修改日期:2021/07/23
	.OLED屏由单独的任务进行管理
	.支持两种不同的I2C接口的OLED屏，请在oled.h文件中设置宏OLED_I2C_PORT

*修改日期:2021/07/24
	.增加菜单功能，使用了双向链表（难度升级，适合基础较好的同学阅读）
	.删除原有按键显示图片

*修改日期:2021/07/25
	.增加手势识别功能(开机前须接上模块，自动识别该模块并初始化)
	.增加呼吸灯，当进入熄屏状态时开始工作
	.增加自动熄屏功能（可通过抬手或长按S4键唤醒屏幕）
	
*修改日期:2021/08/03
	.增加可调电阻为转盘，作菜单项选择
	.增强了双向链表菜单，添加了同级主菜单功能，提高了菜单拓展能力
    .增加了串口1/串口3（蓝牙）菜单项选择与控制；
	.长按复位功能由按键1变更为按键3来实现
	.按键1、按键2支持的长按功能为快速选择菜单项
*修改日期:2021/08/17
	.关闭了时间片调度
	.增加了矩阵键盘,同时要处理矩阵键盘的引脚与温湿度模块的冲突
	.增加了密码管理界面，可认证密码、修改密码
	.增加了功耗管理，能够设置睡眠模式、停止模式、待机模式	
	
*修改日期:2023/05/11
	.优化OLED屏底层代码，优化性能
	.增加了抬手切换OLED屏显示方向，0°~ 180°或 180°~ 0°
	.密码管理从原来的明文存储修改为密文存储，加密算法为异或加解密算法
	.进入author界面一会后，自动显示太空人动画
	
*修改日期:2023/09/17
	.HardFault_Handler异常中断代码添加了打印栈信息（R0~R3、R12、LR、PC），便于查找触发HardFault_Handler的代码
	.startup_stm32f40_41xxx.s文件中增加了调用exceptions_catch函数，便于查找有问题的异常中断

[提醒1]
	.当前默认屏蔽了看门狗代码，由宏IWDG_ENABLE使能。
	 原因进入停止模式，没有必要使用看门狗，增加功耗。
[提醒2]	 
	1）睡眠模式
		进入睡眠模式后，芯片功耗相对运行模式大幅降低。同时不影响所有功能的运转。
	2）停止模式
	.进入停止模式，芯片功耗再一次降低。
	.进入停止模式，OS停止运行，mpu6050则无法周期获取欧拉角。
	 恢复到运行模式后，mpu6050得重新初始化
	.按下按键4/闹钟事件唤醒重新进入运行模式
	3）待机模式
	.进入待机模式，芯片功耗降到最低。
	.按下按键1，程序重新执行，等同于软复位效果
	.若使能了闹钟，可以让闹钟唤醒。但是按键1唤醒或者闹钟唤醒只能2选1，芯片决定的。
*****************************************************************/
#include "includes.h"

TaskHandle_t app_task_init_handle 			= NULL;
TaskHandle_t app_task_mpu6050_handle 		= NULL;
TaskHandle_t app_task_key_handle 			= NULL;
TaskHandle_t app_task_dht_handle 			= NULL;
TaskHandle_t app_task_usart_handle 			= NULL;
TaskHandle_t app_task_rtc_handle 			= NULL;
TaskHandle_t app_task_rtc_alarm_handle 		= NULL;
TaskHandle_t app_task_led_handle 			= NULL;
TaskHandle_t app_task_beep_handle 			= NULL;
TaskHandle_t app_task_gesture_handle 		= NULL;
TaskHandle_t app_task_oled_handle 			= NULL;
TaskHandle_t app_task_oled_animation_handle = NULL;
TaskHandle_t app_task_adc_handle 			= NULL;
TaskHandle_t app_task_menu_handle 			= NULL;
TaskHandle_t app_task_system_reset_handle 	= NULL;
TaskHandle_t app_task_led_breath_handle 	= NULL;
TaskHandle_t app_task_kbd_handle 			= NULL;
TaskHandle_t app_task_pass_man_handle 		= NULL;
TaskHandle_t app_task_power_man_handle 		= NULL;


/* 互斥型信号量句柄 */
SemaphoreHandle_t g_mutex_printf;
SemaphoreHandle_t g_mutex_oled;

/* 事件标志组句柄 */
EventGroupHandle_t g_event_group;

/* 消息队列句柄 */
QueueHandle_t g_queue_usart;
QueueHandle_t g_queue_led;
QueueHandle_t g_queue_beep;
QueueHandle_t g_queue_oled;
QueueHandle_t g_queue_kbd;

/* 二值信号量 */
SemaphoreHandle_t g_sem_led;
SemaphoreHandle_t g_sem_beep;

/* 软件定时器句柄 */
static TimerHandle_t soft_timer_Handle = NULL;

/* 全局变量 */
volatile uint32_t g_oled_display_flag = 1;
volatile uint32_t g_dht_get_what = FLAG_DHT_GET_NONE;
volatile uint32_t g_rtc_get_what = FLAG_RTC_GET_NONE;
volatile uint32_t g_mpu6050_get_what = FLAG_MPU6050_GET_NONE;
volatile uint32_t g_gesture_display_flag = FLAG_GESTURE_DISPLAY_OFF; // 记录手势识别模块是否显示手势图片
volatile uint32_t g_gesture_valid = 0;								 // 记录手势识别模块是否连接成功
volatile uint32_t g_system_no_opreation_cnt = 0;
volatile uint32_t g_pass_man_what = FLAG_PASS_MAN_NONE;
volatile uint32_t g_mpu6050_run_flag=1;
volatile cpu_operating_mode_t g_cpu_operating_mode=CPU_RUN;

/* 软件定时器 */
static void soft_timer_callback(TimerHandle_t pxTimer);

/* 任务 */ 
static void app_task_init			(void* pvParameters);  
static void app_task_mpu6050		(void* pvParameters);  
static void app_task_key			(void* pvParameters); 
static void app_task_dht			(void* pvParameters); 
static void app_task_usart			(void* pvParameters);  
static void app_task_rtc			(void* pvParameters); 
static void app_task_rtc_alarm		(void* pvParameters);
static void app_task_led			(void* pvParameters);  
static void app_task_led_breath		(void* pvParameters); 
static void app_task_beep			(void* pvParameters); 
static void app_task_gesture		(void* pvParameters); 
static void app_task_oled			(void* pvParameters); 
static void app_task_oled_animation (void *pvParameters);
static void app_task_adc			(void* pvParameters); 
static void app_task_system_reset	(void* pvParameters); 
static void app_task_menu			(void* pvParameters); 
static void app_task_kbd			(void* pvParameters); 
static void app_task_pass_man		(void* pvParameters);


/* OLED互斥锁高度封装 */
#define OLED_SAFE(__CODE)                                \
	{                                                    \
                                                         \
		do                                               \
		{                                                \
			xSemaphoreTake(g_mutex_oled, portMAX_DELAY); \
			if (g_oled_display_flag)                     \
				__CODE;                                  \
			xSemaphoreGive(g_mutex_oled);                \
		} while (0)                                      \
	}

#define DEBUG_PRINTF_EN 1

void dgb_printf_safe(const char *format, ...)
{
#if DEBUG_PRINTF_EN

	va_list args;
	va_start(args, format);

	/* 获取互斥信号量 */
	xSemaphoreTake(g_mutex_printf, portMAX_DELAY);

	vprintf(format, args);

	/* 释放互斥信号量 */
	xSemaphoreGive(g_mutex_printf);

	va_end(args);
#else
	(void)0;
#endif
}
/**
 * @brief 主函数
 * @param void:无须传入参数
 * @retval 无
 */
int main(void)
{
	/* 设置系统中断优先级分组4 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	/* 系统定时器中断频率为configTICK_RATE_HZ */
	SysTick_Config(SystemCoreClock/configTICK_RATE_HZ);	

	/* 初始化串口1 */
	uart_init(115200);

	/* 创建 app_task_init任务 */
	xTaskCreate((TaskFunction_t)app_task_init,			/* 任务入口函数 */
				(const char *)"app_task_init",			/* 任务名字 */
				(uint16_t)512,							/* 任务栈大小 */
				(void *)NULL,							/* 任务入口函数参数 */
				(UBaseType_t)5,							/* 任务的优先级 */
				(TaskHandle_t *)&app_task_init_handle); /* 任务控制块指针 */

	/* 开启任务调度 */
	vTaskStartScheduler();

	while (1);
}

/* 任务列表 */
static const task_t task_tbl[] = {
	{app_task_mpu6050, 			"app_task_mpu6050", 		1024, NULL, 5, &app_task_mpu6050_handle},
	{app_task_key, 				"app_task_key", 			512, NULL, 5, &app_task_key_handle},
	{app_task_kbd, 				"app_task_kbd", 			512, NULL, 5, &app_task_kbd_handle},	
	{app_task_dht, 				"app_task_dht", 			512, NULL, 5, &app_task_dht_handle},
	{app_task_usart, 			"app_task_usart", 			512, NULL, 5, &app_task_usart_handle},
	{app_task_rtc, 				"app_task_rtc", 			512, NULL, 5, &app_task_rtc_handle},
	{app_task_rtc_alarm, 		"app_task_rtc_alarm", 		512, NULL, 5, &app_task_rtc_alarm_handle},	
	{app_task_led, 				"app_task_led", 			512, NULL, 5, &app_task_led_handle},
	{app_task_led_breath, 		"app_task_led_breath", 		512, NULL, 5, &app_task_led_breath_handle},
	{app_task_beep, 			"app_task_beep", 			512, NULL, 5, &app_task_beep_handle},
	{app_task_system_reset, 	"app_task_system_reset", 	512, NULL, 5, &app_task_system_reset_handle},
	{app_task_oled, 			"app_task_oled", 			512, NULL, 6, &app_task_oled_handle},
	{app_task_oled_animation,	"app_task_oled", 			512, NULL, 6, &app_task_oled_animation_handle},	
	{app_task_adc, 				"app_task_adc", 			512, NULL, 6, &app_task_adc_handle},
	{app_task_pass_man, 		"app_task_pass_man", 		512, NULL, 5, &app_task_pass_man_handle},	
	{app_task_menu, 			"app_task_menu", 			512, &menu_main_tbl, 5, &app_task_menu_handle},
	{0, 0, 0, 0, 0, 0}
};

/**
 * @brief 集中初始化任务
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 */
static void app_task_init(void *pvParameters)
{
	uint32_t i = 0;
	menu_ext_t menu_ext;

	printf("[app_task_init] create success\r\n");

	/* 创建互斥型信号量 */
	g_mutex_printf 	= xSemaphoreCreateMutex();
	g_mutex_oled 	= xSemaphoreCreateMutex();

	/* 创建二值型信号量 */
	g_sem_led  = xSemaphoreCreateBinary();
	g_sem_beep = xSemaphoreCreateBinary();

	/* 创建事件标志组 */
	g_event_group = xEventGroupCreate();

	/* 创建消息队列 */
	g_queue_usart 	= xQueueCreate(QUEUE_USART_LEN, sizeof(struct usart_packet_t));
	g_queue_led  	= xQueueCreate(QUEUE_LED_LEN, 	sizeof(uint8_t));
	g_queue_beep  	= xQueueCreate(QUEUE_BEEP_LEN, 	sizeof(beep_t));
	g_queue_oled 	= xQueueCreate(QUEUE_OLED_LEN, 	sizeof(oled_t));
	g_queue_kbd 	= xQueueCreate(QUEUE_KBD_LEN, 	sizeof(uint8_t));

	/* 初始化OLED */
	OLED_Init();
	
	/* 设置显示方向翻转180 */
	OLED_Set_Direction(180);
	
	OLED_Clear();

#if 1	
	/* 显示logo */
	OLED_DrawBMP(0,0,128,8,(uint8_t *)pic_logo);	
																	
	/* 持续2秒 */
	vTaskDelay(2000);

	OLED_Clear();	
	
	/* 显示“嵌入式实时系统” */
	OLED_ShowCHinese(8,0,0);
	OLED_ShowCHinese(24,0,1);
	OLED_ShowCHinese(40,0,2);	
	OLED_ShowCHinese(56,0,3);	
	OLED_ShowCHinese(72,0,4);		
	OLED_ShowCHinese(88,0,5);		
	OLED_ShowCHinese(104,0,6);
	
	/* 显示“综合实例” */
	OLED_ShowCHinese(32,3,7);
	OLED_ShowCHinese(48,3,8);
	OLED_ShowCHinese(64,3,9);	
	OLED_ShowCHinese(80,3,10);	

	/* 显示“粤嵌温子祺” */
	OLED_ShowCHinese(0,6,11);
	OLED_ShowCHinese(16,6,12);
	OLED_ShowCHinese(82,6,13);	
	OLED_ShowCHinese(98,6,14);	
	OLED_ShowCHinese(112,6,15);
	
	/* 持续1秒 */
	vTaskDelay(1000);
#endif

	/* led初始化 */
	led_init();

	/* 呼吸灯初始化 */
	led_breath_init();

	/* beep初始化 */
	beep_init();

	/* 按键初始化 */
	key_init();

	/* ble初始化 */
	ble_init(9600);

	/* 温湿度模块初始化 */
	dht11_init();

	/* adc初始化 */
	adc_init();

	/* rtc初始化 */
	rtc_init();
	
	/* 闹钟初始化 */
	rtc_alarm_init();
	

	/* PAJ7620U2手势识别传感器连接到开发板，并初始化成功后才创建手势识别任务 */
	if (paj7620u2_init())
	{
		printf("[app_task_init] paj7620u2 init success\r\n");

		/* 记录手势识别模块有效性，1-工作有效 0-工作无效 */
		g_gesture_valid = 1;

		/* 显示手势识别模块图标 */
		OLED_DrawBMP(0, 0, 64, 8, (uint8_t *)pic_gesture_icon);

		/* 显示"连接成功" */
		OLED_ShowCHinese(64, 4, 22);
		OLED_ShowCHinese(90, 4, 23);
		OLED_ShowCHinese(64, 6, 30);
		OLED_ShowCHinese(90, 6, 31);

		/* 持续2秒 */
		vTaskDelay(2000);

		/* 创建app_task_gesture任务 */
		xTaskCreate((TaskFunction_t)app_task_gesture,		   /* 任务入口函数 */
					(const char *)"app_task_gesture",		   /* 任务名字 */
					(uint16_t)512,							   /* 任务栈大小 */
					(void *)NULL,							   /* 任务入口函数参数 */
					(UBaseType_t)5,							   /* 任务的优先级 */
					(TaskHandle_t *)&app_task_gesture_handle); /* 任务控制块指针 */
	}

	/* mpu6050初始化 */
	MPU_Init();

	mpu_dmp_init();

	/* eeprom初始化 */
	at24c02_init();

#if IWDG_ENABLE
	/* 独立看门狗初始化 */
	iwdg_init();
#endif

	/* 矩阵键盘初始化 */
	kbd_init();

	/* 创建用到的任务 */
	taskENTER_CRITICAL();
	i = 0;
	while (task_tbl[i].pxTaskCode)
	{
		xTaskCreate(task_tbl[i].pxTaskCode,		/* 任务入口函数 */
					task_tbl[i].pcName,			/* 任务名字 */
					task_tbl[i].usStackDepth,	/* 任务栈大小 */
					task_tbl[i].pvParameters,	/* 任务入口函数参数 */
					task_tbl[i].uxPriority,		/* 任务的优先级 */
					task_tbl[i].pxCreatedTask); /* 任务控制块指针 */
		i++;
	}
	taskEXIT_CRITICAL();

	/* 创建周期软件定时器 */
	soft_timer_Handle = xTimerCreate((const char *)"AutoReloadTimer",
									 (TickType_t)1000,	  /* 定时器周期 1000(tick) */
									 (UBaseType_t)pdTRUE, /* 周期模式 */
									 (void *)1,			  /* 为每个计时器分配一个索引的唯一ID */
									 (TimerCallbackFunction_t)soft_timer_callback);
	/* 开启周期软件定时器 */
	xTimerStart(soft_timer_Handle, 0);
	
	/* 显示开机动画 */								 
	OLED_Clear();	
									 
	for(i=0; i<16; i++)
	{
		OLED_ShowChar(i*8,3,'>',16);
		vTaskDelay(50);
	}									 
									 
	OLED_Clear();

	/* 显示主菜单 */
	menu_ext.menu = menu_main_tbl[0];
	menu_show(&menu_ext);

	/* 删除任务自身 */
	vTaskDelete(NULL);

	printf("[app_task_init] nerver run here\r\n");
}
/**
  * @brief mpu6050任务
  * @param pvParameters:创建任务时传递的参数
  * @retval 无
  */
static void app_task_mpu6050(void *pvParameters)
{
	uint8_t led_sta;
	uint8_t buf[32];
	uint32_t i;
	float pitch, roll, yaw; /* 欧拉角 */
	BaseType_t xReturn = pdFALSE;
	oled_t oled;
	uint32_t roll_oled_direction_sta=0;

	dgb_printf_safe("[app_task_mpu6050] create success\r\n");
	

	for (;;)
	{
		vTaskDelay(200);

		/* 避免当前任务中途挂起，导致时序紊乱 */
		if(g_cpu_operating_mode == CPU_STOP)
			continue;
		
		/* mpu6050要短时间周期性获取，时间100ms~200ms需获取一次数据 */
		if (0 == mpu_dmp_get_data(&pitch, &roll, &yaw))
		{
			if (g_mpu6050_get_what == FLAG_MPU6050_GET_EULER)
			{
				memset(buf, 0, sizeof buf);
				sprintf((char *)buf, "p%03d r%03d y%03d", (int32_t)pitch, (int32_t)roll, (int32_t)yaw);
					
				dgb_printf_safe("[app_task_mpu6050] %s\r\n",buf);	
				
				buf[16]=0;
				
				i = strlen((char *)buf);

				/* 清空未显示部分 */
				for (; i < 16; i++)
					buf[i] = ' ';

				oled.ctrl = OLED_CTRL_SHOW_STRING;
				oled.x = 0;
				oled.y = 5;
				oled.str = buf;
				oled.font_size = 12;

				xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
									 &oled,		   /* 发送的消息内容 */
									 100);		   /* 等待时间 100 Tick */
				if (xReturn != pdPASS)
					dgb_printf_safe("[app_task_mpu6050] xQueueSend oled string error code is %d\r\n", xReturn);
			}
			

			/* 运用状态机思想：当OLED在亮屏状态下，抬手旋转屏幕 */
			switch(roll_oled_direction_sta)
			{
				case 0:
				{
					if(roll >30)
						roll_oled_direction_sta=1;
				}
				break;
				
				case 1:
				{
					if(roll >30 && g_oled_display_flag)
					{
						
						oled.ctrl = OLED_CTRL_DISPLAY_DIRECTION;
						
						/* 设置屏幕方向从0°到180° */
						if(OLED_Get_Direction()==0)
							oled.direction=180;
						
						/* 设置屏幕方向从180°到0° */
						if(OLED_Get_Direction()==180)
							oled.direction=0;						

						xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
											 &oled,		   /* 发送的消息内容 */
											 100);		   /* 等待时间 100 Tick */
						if (xReturn != pdPASS)
							dgb_printf_safe("[app_task_mpu6050] xQueueSend oled display direction error code is %d\r\n", xReturn);	

						
						roll_oled_direction_sta=2;
					}
					
				}break;	
					
				case 2:
				{
					if(roll < 15)
						roll_oled_direction_sta=0;				
				
				}break;			

				default:break;
			}

			/* 通过三盏LED灯表示横滚角 */
			if (roll < 5)
				led_sta = 0x70;
			else if (roll < 15)
				led_sta = 0x71;
			else if (roll < 30)
				led_sta = 0x73;
			else
				led_sta = 0x77;

			xReturn = xQueueSend(g_queue_led, /* 消息队列的句柄 */
								 &led_sta,	  /* 发送的消息内容 */
								 100);		  /* 等待时间 100 Tick */
			if (xReturn != pdPASS)
			{
				dgb_printf_safe("[app_task_mpu6050] xQueueSend led_sta error code is %d\r\n", xReturn);
				continue;
			}

			/* 阻塞等待信号量，用于确保任务完成对灯的控制 */
			xReturn = xSemaphoreTake(g_sem_led, portMAX_DELAY);

			if (xReturn != pdPASS)
			{
				dgb_printf_safe("[app_task_mpu6050] xSemaphoreTake g_sem_led error code is %d\r\n", xReturn);
				continue;
			}

			/* 当OLED在熄屏状态下，抬手唤醒屏幕 */
			if (roll > 15 && !g_oled_display_flag)
			{
				/* 进入临界区 */
				taskENTER_CRITICAL();

				g_oled_display_flag = 1;

				/* 清零统计系统无操作计数值 */
				g_system_no_opreation_cnt = 0;

				/* 退出临界区 */
				taskEXIT_CRITICAL();

				dgb_printf_safe("[app_task_mpu6050] roll > 15 , oled on\r\n");

				/* 点亮OLED屏 */
				oled.ctrl = OLED_CTRL_DISPLAY_ON;

				xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
									 &oled,		   /* 发送的消息内容 */
									 100);		   /* 等待时间 100 Tick */
				if (xReturn != pdPASS)
					dgb_printf_safe("[app_task_mpu6050] xQueueSend oled error code is %d\r\n", xReturn);
				
				/* 避免唤醒后切换屏幕显示方向 */
				roll_oled_direction_sta=2;
			}
			

		}
	}
}

/**
 * @brief 矩阵键盘任务
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 */
void app_task_kbd(void* pvParameters)
{
	char key_val='N';
	beep_t beep;
	BaseType_t xReturn;
	dgb_printf_safe("[app_task_kbd] create success\r\n");
	
	while(1)
	{
		/* 读取矩阵键盘按键值 */
		key_val=kbd_read();

		if(key_val != 'N')
		{
			dgb_printf_safe("[app_task_kbd] kbd press %c \r\n",key_val);

			xReturn = xQueueSend(g_queue_kbd, 	/* 消息队列的句柄 */
								&key_val,		/* 发送的消息内容 */
								100);		   	/* 等待时间 100 Tick */
			if (xReturn != pdPASS)
				dgb_printf_safe("[app_task_kbd] xQueueSend kbd error code is %d\r\n", xReturn);	

			/* 嘀一声示意 */
			beep.sta = 1;
			beep.duration = 20;

			xReturn = xQueueSend(g_queue_beep, /* 消息队列的句柄 */
								&beep,		   /* 发送的消息内容 */
								100);		   /* 等待时间 100 Tick */				
			if (xReturn != pdPASS)
				dgb_printf_safe("[app_task_kbd] xQueueSend beep error code is %d\r\n", xReturn);

			/* [可选]阻塞等待信号量，用于确保任务完成对beep的控制 */
			xReturn = xSemaphoreTake(g_sem_beep, portMAX_DELAY);
			if (xReturn != pdPASS)
				dgb_printf_safe("[app_task_system_reset] xSemaphoreTake g_sem_beep error code is %d\r\n", xReturn);					

		}	
	}
}

/**
 * @brief 按键任务
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 */
static void app_task_key(void *pvParameters)
{
	EventBits_t EventValue;
	oled_t oled;
	uint32_t t = 0;
	BaseType_t xReturn;

	dgb_printf_safe("[app_task_key] create success\r\n");

	for (;;)
	{
		/* 等待事件组中的相应事件位，或同步 */
		EventValue = xEventGroupWaitBits((EventGroupHandle_t)g_event_group,
										 (EventBits_t)EVENT_GROUP_KEYALL_DOWN,
										 (BaseType_t)pdTRUE,
										 (BaseType_t)pdFALSE,
										 (TickType_t)portMAX_DELAY);

		/* 延时消抖 */
		vTaskDelay(20);

		/* 清零统计系统无操作计数值 */
		taskENTER_CRITICAL();
		g_system_no_opreation_cnt = 0;
		taskEXIT_CRITICAL();

		if (EventValue & EVENT_GROUP_KEY1_DOWN)
		{
			/* 禁止EXTI0触发中断 */
			NVIC_DisableIRQ(EXTI0_IRQn);

			/* 确认是按下 */
			if (PAin(0) == 0)
			{
				t = 0;

				/* 按键长按 */
				while (PAin(0) == 0)
				{
					vTaskDelay(20);

					if (t++ >= 100)
						break;
				}

				if (t >= 100)
				{
					dgb_printf_safe("[app_task_key] S1 Press Continue\r\n");

					/* 识别到长按，则菜单选择0.25秒连续切换 */
					while (PAin(0) == 0)
					{
						/* 发送KEY_UP按键事件 */
						xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_UP);	

						vTaskDelay(250);
					}

				}
				else
				{
					dgb_printf_safe("[app_task_key] S1 Press\r\n");

					/* 发送KEY_UP按键事件 */
					xReturn = xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_UP);
					if (xReturn != pdPASS)
						dgb_printf_safe("[app_task_key] xEventGroupSetBits EVENT_GROUP_FN_KEY_UP error code is %d\r\n", xReturn);	
				}
			}

			/* 允许EXTI0触发中断 */
			NVIC_EnableIRQ(EXTI0_IRQn);
		}

		if (EventValue & EVENT_GROUP_KEY2_DOWN)
		{
			/* 禁止EXTI2触发中断 */
			NVIC_DisableIRQ(EXTI2_IRQn);

			if (PEin(2) == 0)
			{
				dgb_printf_safe("[app_task_key] S2 Press\r\n");

				/* 按键长按 */
				t=0;
				while (PEin(2) == 0)
				{
					vTaskDelay(20);

					if (t++ >= 100)
						break;
				}

				if(t >= 100)
				{
					dgb_printf_safe("[app_task_key] S2 Press Continue\r\n");

					/* 识别到长按，则菜单选择0.25秒连续切换 */
					while (PEin(2) == 0)
					{
						/* 发送KEY_DOWN按键事件 */
						xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_DOWN);	

						vTaskDelay(250);
					}
				}

				/* 发送KEY_DOWN按键事件 */
				xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_DOWN);
			}

			/* 允许EXTI2触发中断 */
			NVIC_EnableIRQ(EXTI2_IRQn);
		}

		if (EventValue & EVENT_GROUP_KEY3_DOWN)
		{
			/* 禁止EXTI3触发中断 */
			NVIC_DisableIRQ(EXTI3_IRQn);

			if (PEin(3) == 0)
			{
				dgb_printf_safe("[app_task_key] S3 Press\r\n");

				/* 按键长按 */
				t=0;
				while (PEin(3) == 0)
				{
					vTaskDelay(20);

					if (t++ >= 100)
						break;
				}

				if(t >= 100)
				{
					dgb_printf_safe("[app_task_key] S3 Press Continue\r\n");

					/* 发送系统复位事件 */
					xReturn = xEventGroupSetBits(g_event_group, EVENT_GROUP_SYSTEM_RESET);
					if (xReturn != pdPASS)
						dgb_printf_safe("[app_task_key] xEventGroupSetBits EVENT_GROUP_SYSTEM_RESET error code is %d\r\n", xReturn);
				}
				else
				{
					dgb_printf_safe("[app_task_key] S3 Press\r\n");					

					/* 发送KEY_ENTER按键事件 */
					xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_ENTER);
				}
			}

			/* 允许EXTI3触发中断 */
			NVIC_EnableIRQ(EXTI3_IRQn);
		}

		if (EventValue & EVENT_GROUP_KEY4_DOWN)
		{
			/* 禁止EXTI4触发中断 */
			NVIC_DisableIRQ(EXTI4_IRQn);

			if (PEin(4) == 0)
			{

				t = 0;
				/* 按键长按，控制对OLED屏的亮灭 */
				while (PEin(4) == 0)
				{
					vTaskDelay(20);

					if (t++ >= 100)
						break;
				}

				if (t >= 100)
				{

					dgb_printf_safe("[app_task_key] S4 Press Continue\r\n");

					/* 进入临界区 */
					taskENTER_CRITICAL();

					g_oled_display_flag = !g_oled_display_flag;

					/* 退出临界区 */
					taskEXIT_CRITICAL();
				
					/* OLED亮/灭屏控制 */
					oled.ctrl = g_oled_display_flag ? OLED_CTRL_DISPLAY_ON : OLED_CTRL_DISPLAY_OFF;

					xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
										 &oled,		   /* 发送的消息内容 */
										 100);		   /* 等待时间 100 Tick */
					if (xReturn != pdPASS)
						dgb_printf_safe("[app_task_key] xQueueSend oled error code is %d\r\n", xReturn);
								
				}
				else
				{
					dgb_printf_safe("[app_task_key] S4 Press\r\n");

					/* 发送KEY_BACK按键事件 */
					xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_BACK);
				}
			}

			/* 允许EXTI4触发中断 */
			NVIC_EnableIRQ(EXTI4_IRQn);
		}
	}
}
/**
 * @brief adc任务
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 通过可调电阻获取的电压值，用作转盘菜单的控制
 */
static void app_task_adc(void *pvParameters)
{
	int32_t adc_vol_last = 0;
	int32_t adc_vol = 0;
	int32_t result;

	dgb_printf_safe("[app_task_adc] create success and suspend self \r\n");

	/* 获取当前电压初值 */
	adc_vol = adc_voltage_get();
	adc_vol_last = adc_vol;

	for (;;)
	{
		/* 获取电压值 */
		adc_vol = adc_voltage_get();

		result = adc_vol - adc_vol_last;

		if (result > 200 || result < -200)
		{
			/* 清零统计系统无操作计数值 */
			taskENTER_CRITICAL();
			g_system_no_opreation_cnt = 0;
			taskEXIT_CRITICAL();

			/* 发送KEY_DOWN/KEY_UP按键事件 */
			xEventGroupSetBits(g_event_group, result > 0 ? EVENT_GROUP_FN_KEY_DOWN : EVENT_GROUP_FN_KEY_UP);

			dgb_printf_safe("[app_task_adc] adc_vol_last=%d adc_vol=%d\r\n", adc_vol_last, adc_vol);

			adc_vol_last = adc_vol;
		}

		vTaskDelay(100);
	}
}
/**
 * @brief 手势识别任务
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 通过获取手势值，用作菜单的控制
 */
static void app_task_gesture(void *pvParameters)
{
	EventBits_t EventValue;
	BaseType_t xReturn;
	oled_t oled;
	uint16_t gesture;
	uint32_t i = 0;

	/* 手势图片列表，NULL表示不显示该手势 */
	const uint8_t *pic_gesture_icon_tbl[] = {
		pic_up_icon,
		pic_down_icon,
		pic_left_icon,
		pic_right_icon,
		NULL,
		NULL,
		pic_clockwise_icon,
		pic_anticlockwise_icon,
		NULL};

	/* 提前设置图片显示的位置、大小 */
	oled.x = 30;
	oled.y = 0;
	oled.pic_width = 64;
	oled.pic_height = 8;

	dgb_printf_safe("[app_task_gesture] create success\r\n");
	dgb_printf_safe("[app_task_gesture] pic_gesture_icon_tbl array size is %d \r\n", ARRAY_SIZE(pic_gesture_icon_tbl));

	for (;;)
	{
		/* 等待事件组中的相应事件位，或同步 */
		EventValue = xEventGroupWaitBits((EventGroupHandle_t)g_event_group,
										 (EventBits_t)EVENT_GROUP_GESTURE,
										 (BaseType_t)pdTRUE,
										 (BaseType_t)pdFALSE,
										 (TickType_t)portMAX_DELAY);

		if (!(EventValue & EVENT_GROUP_GESTURE))
			continue;

		/* 清零统计系统无操作计数值 */
		taskENTER_CRITICAL();
		g_system_no_opreation_cnt = 0;
		taskEXIT_CRITICAL();

		/* 获取手势 */
		gesture = gesture_get();

		/* 提取手势图片 */
		for (i = 0; i < ARRAY_SIZE(pic_gesture_icon_tbl); i++)
		{
			if (gesture & (1 << i))
				break;
		}

		/* 显示手势图片 */
		if ((g_gesture_display_flag == FLAG_GESTURE_DISPLAY_ON) && pic_gesture_icon_tbl[i])
		{
			dgb_printf_safe("[app_task_gesture] gesture is %d \r\n", i);

			oled.ctrl = OLED_CTRL_SHOW_PICTURE;

			oled.pic = pic_gesture_icon_tbl[i];

			xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
								 &oled,		   /* 发送的消息内容 */
								 100);		   /* 等待时间 100 Tick */
			if (xReturn != pdPASS)
				dgb_printf_safe("[app_task_gesture] xQueueSend oled picture error code is %d\r\n", xReturn);
		}

		/* 根据手势，执行对应的按键功能 */
		switch (gesture)
		{
		case GES_UP:
		{
			dgb_printf_safe("[app_task_gesture] GES_UP trigger KEY_UP\r\n");

			if (g_gesture_display_flag == FLAG_GESTURE_DISPLAY_OFF)
				xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_UP);
		}
		break;

		case GES_DOWN:
		{
			dgb_printf_safe("[app_task_gesture] GES_DOWN trigger KEY_DOWN\r\n");

			if (g_gesture_display_flag == FLAG_GESTURE_DISPLAY_OFF)
				xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_DOWN);
		}
		break;

		case GES_RIGHT:
		{
			dgb_printf_safe("[app_task_gesture] GES_RIGHT trigger KEY_ENTER\r\n");

			if (g_gesture_display_flag == FLAG_GESTURE_DISPLAY_OFF)
				xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_ENTER);
		}
		break;

		case GES_CLOCKWISE:
		{
			dgb_printf_safe("[app_task_gesture] GES_CLOCKWISE trigger KEY_ENTER\r\n");

			if (g_gesture_display_flag == FLAG_GESTURE_DISPLAY_OFF)
				xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_ENTER);
		}
		break;

		case GES_LEFT:
		{
			dgb_printf_safe("[app_task_gesture] GES_LEFT trigger KEY_BACK\r\n");

			if (g_gesture_display_flag == FLAG_GESTURE_DISPLAY_OFF)
				xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_BACK);
		}
		break;

		case GES_COUNT_CLOCKWISE:
		{
			dgb_printf_safe("[app_task_gesture] GES_COUNT_CLOCKWISE trigger KEY_BACK\r\n");

			if (g_gesture_display_flag == FLAG_GESTURE_DISPLAY_ON)
			{
				/* 显示逆时针图标后，自动退出 */
				vTaskDelay(1000);
				g_gesture_display_flag = FLAG_GESTURE_DISPLAY_OFF;
			}

			/* 当在显示手势时，只有逆时针触发程序退出 */
			xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_BACK);
		}
		break;

		default:
			break;
		}
	}
}
/**
 * @brief 温湿度任务
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 该任务被创建后，默认为挂起状态。
 */
static void app_task_dht(void *pvParameters)
{
	uint8_t dht11_data[5] = {0};
	uint8_t buf[16] = {0};
	oled_t oled;
	BaseType_t xReturn;
	int32_t rt=-1;

	dgb_printf_safe("[app_task_dht] create success and suspend self \r\n");

	vTaskSuspend(NULL);

	dgb_printf_safe("[app_task_dht] resume success\r\n");

	for (;;)
	{
		rt=dht11_read_data(dht11_data);

		if (rt==0)
		{
			memset(buf, 0, sizeof buf);

			if (g_dht_get_what == FLAG_DHT_GET_TEMP)
				sprintf((char *)buf, "Temp:%02d.%d", dht11_data[2], dht11_data[3]);

			if (g_dht_get_what == FLAG_DHT_GET_HUMI)
				sprintf((char *)buf, "Humi:%02d.%d", dht11_data[0], dht11_data[1]);

			oled.ctrl = OLED_CTRL_SHOW_STRING;
			oled.x = 40;
			oled.y = 3;
			oled.str = buf;
			oled.font_size = 16;

			xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
								 &oled,		   /* 发送的消息内容 */
								 100);		   /* 等待时间 100 Tick */
			if (xReturn != pdPASS)
				dgb_printf_safe("[app_task_dht] xQueueSend oled string error code is %d\r\n", xReturn);

			dgb_printf_safe("[app_task_dht] %s\r\n", buf);
		}

		vTaskDelay(6000);
	}
}
/**
 * @brief 串口任务
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 通过串口消息队列获取相应的数据，可设置日期与时间、闹钟、菜单控制
 */
static void app_task_usart(void *pvParameters)
{
	struct usart_packet_t usart_packet = {0};
	oled_t oled;

	/* RTC_AlarmStructure必须为静态 */
static 
	RTC_AlarmTypeDef 	RTC_AlarmStructure;	

	uint32_t i = 0;

	char *p = NULL;

	RTC_DateTypeDef RTC_DateStructure;
	RTC_TimeTypeDef RTC_TimeStructure;

	BaseType_t xReturn = pdFALSE;

	dgb_printf_safe("[app_task_usart] create success\r\n");

	for (;;)
	{
		xReturn = xQueueReceive(g_queue_usart,	/* 消息队列的句柄 */
								&usart_packet,	/* 得到的消息内容 */
								portMAX_DELAY); /* 等待时间一直等 */
		if (xReturn != pdPASS)
		{
			dgb_printf_safe("[app_task_usart] xQueueReceive usart_packet error code is %d\r\n", xReturn);
			continue;
		}

		/* 清零统计系统无操作计数值 */
		taskENTER_CRITICAL();
		g_system_no_opreation_cnt = 0;
		taskEXIT_CRITICAL();


		/* 设置日期 */
		if (strstr((char *)usart_packet.rx_buf, "DATE SET"))
		{
			/* 以等号分割字符串 */
			p = strtok((char *)usart_packet.rx_buf, "-");
			if (p == NULL)
				goto err_analyse_packet;

			//获取年
			p = strtok(NULL, "-");
			if (p == NULL)
				goto err_analyse_packet;

			//2021-2000=21
			i = atoi(p) - 2000;

			//转换为16进制 21 ->0x21
			i = (i / 10) * 16 + i % 10;
			RTC_DateStructure.RTC_Year = i;

			//获取月
			p = strtok(NULL, "-");
			if (p == NULL)
				goto err_analyse_packet;

			i = atoi(p);
			//转换为16进制
			i = (i / 10) * 16 + i % 10;
			RTC_DateStructure.RTC_Month = i;

			//获取日
			p = strtok(NULL, "-");
			if (p == NULL)
				goto err_analyse_packet;

			i = atoi(p);
			//转换为16进制
			i = (i / 10) * 16 + i % 10;
			RTC_DateStructure.RTC_Date = i;

			//获取星期
			p = strtok(NULL, "-");
			if (p == NULL)
				goto err_analyse_packet;

			i = atoi(p);
			//转换为16进制
			i = (i / 10) * 16 + i % 10;
			RTC_DateStructure.RTC_WeekDay = i;

			//设置日期
			RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
		}
		/* 设置时间 */
		else if (strstr((char *)usart_packet.rx_buf, "TIME SET"))
		{
			/* 以等号分割字符串 */
			p = strtok((char *)usart_packet.rx_buf, "-");
			if (p == NULL)
				goto err_analyse_packet;

			/* 获取时 */
			p = strtok(NULL, "-");
			if (p == NULL)
				goto err_analyse_packet;

			i = atoi(p);

			/* 通过时，判断是AM还是PM */
			if (i < 12)
				RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
			else
				RTC_TimeStructure.RTC_H12 = RTC_H12_PM;

			/* 转换为BCD编码 */
			i = (i / 10) * 16 + i % 10;
			RTC_TimeStructure.RTC_Hours = i;

			/* 获取分 */
			p = strtok(NULL, "-");
			if (p == NULL)
				goto err_analyse_packet;

			i = atoi(p);

			/* 转换为BCD编码 */
			i = (i / 10) * 16 + i % 10;
			RTC_TimeStructure.RTC_Minutes = i;

			/* 获取秒 */
			p = strtok(NULL, "-");
			if (p == NULL)
				goto err_analyse_packet;

			i = atoi(p);

			/* 转换为BCD编码 */
			i = (i / 10) * 16 + i % 10;
			RTC_TimeStructure.RTC_Seconds = i;

			/* 设置RTC时间 */
			RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);

			dgb_printf_safe("[app_task_usart] rtc set time ok\r\n");
		}
		else if(strstr((char *)usart_packet.rx_buf,"ALARM SET"))
		{
			//以等号分割字符串
			p=strtok((char *)usart_packet.rx_buf,"-");
			if (p == NULL)
				goto err_analyse_packet;			
			
			//获取时
			p=strtok(NULL,"-");
			if (p == NULL)
				goto err_analyse_packet;

			i = atoi(p);
			
			//通过时，判断是AM还是PM
			if(i<12)
				RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
			else
				RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_PM;
				
			//转换为BCD编码
			i= (i/10)*16+i%10;
			RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = i;
			
			//获取分
			p=strtok(NULL,"-");
			if (p == NULL)
				goto err_analyse_packet;

			i = atoi(p);						
			//转换为BCD编码
			i= (i/10)*16+i%10;	
			RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = i;
			
			//获取秒
			p=strtok(NULL,"-");
			if (p == NULL)
				goto err_analyse_packet;

			i = atoi(p);	
			
			//转换为BCD编码
			i= (i/10)*16+i%10;					
			RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = i; 	
			
			//每天闹钟都会生效
			//屏蔽星期/日期，也就说闹钟每天都会生效
			RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;				

			//设置闹钟
			rtc_alarm_set(RTC_AlarmStructure); 

			/* 设置要显示的图片-闹钟图标 */
			oled.x=112;
			oled.y=0;
			oled.pic_width=16;
			oled.pic_height=2;
			oled.ctrl=OLED_CTRL_SHOW_PICTURE;
			oled.pic=pic_alarm_icon;

			xReturn = xQueueSend( 	g_queue_oled,	/* 消息队列的句柄 */
									&oled,			/* 发送的消息内容 */
									100);			/* 等待时间 100 Tick */
			if(xReturn != pdPASS)
				dgb_printf_safe("[app_task_usart] xQueueSend oled picture error code is %d\r\n",xReturn);

			
			dgb_printf_safe("[app_task_usart] rtc set alarm a ok\r\n");

		}		
		else if((usart_packet.rx_buf[0]=='^')|| (usart_packet.rx_buf[0]=='v') ||(usart_packet.rx_buf[0]=='<') || (usart_packet.rx_buf[0]=='>'))
		{
			switch(usart_packet.rx_buf[0])
			{
				case '^':
				{
					/* 发送KEY_UP按键事件 */
					xReturn = xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_UP);

					dgb_printf_safe("[app_task_usart] '^' trigger KEY_UP Event\r\n");
				}break;

				case 'v':
				{
					/* 发送KEY_DOWN按键事件 */
					xReturn = xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_DOWN);

					dgb_printf_safe("[app_task_usart] 'v' trigger KEY_Down Event\r\n");

				}break;	

				case '<':
				{
					/* 发送KEY_LEFT按键事件 */
					xReturn = xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_BACK);

					dgb_printf_safe("[app_task_usart] '<' trigger KEY_BACK Event\r\n");
				}break;	

				case '>':
				{
					/* 发送KEY_LEFT按键事件 */
					xReturn = xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_ENTER);

					dgb_printf_safe("[app_task_usart] '>' trigger KEY_ENTER Event\r\n");
				}break;

				default:break;											
			}

			if (xReturn != pdPASS)
					dgb_printf_safe("[app_task_key] xEventGroupSetBits EVENT_GROUP_FN_KEY_UP error code is %d\r\n", xReturn);
		}
		else
		{
err_analyse_packet:
			dgb_printf_safe("[app_task_usart] usart invalid command\r\n");
		}

		memset(&usart_packet, 0, sizeof usart_packet);
	}
}
/**
 * @brief rtc任务
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 显示日期与时间
 */
static void app_task_rtc(void *pvParameters)
{
	uint8_t buf[16] = {0};
	oled_t oled;
	BaseType_t xReturn;
	EventBits_t EventValue;

	RTC_TimeTypeDef RTC_TimeStructure;
	RTC_DateTypeDef RTC_DateStructure;

	dgb_printf_safe("[app_task_rtc] create success and suspend self\r\n");

	vTaskSuspend(NULL);

	dgb_printf_safe("[app_task_rtc] resume success\r\n");

	for (;;)
	{
		/* 等待事件组中的相应事件位，或同步 */
		EventValue = xEventGroupWaitBits((EventGroupHandle_t)g_event_group,
										 (EventBits_t)EVENT_GROUP_RTC_WAKEUP,
										 (BaseType_t)pdTRUE,
										 (BaseType_t)pdFALSE,
										 (TickType_t)portMAX_DELAY);

		if (EventValue & EVENT_GROUP_RTC_WAKEUP)
		{
			if (g_rtc_get_what == FLAG_RTC_GET_TIME)
			{
				/* RTC_GetTime，获取时间 */
				RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure);

				memset(buf, 0, sizeof buf);

				/* 格式化字符串 */
				sprintf((char *)buf, "%02x:%02x:%02x", RTC_TimeStructure.RTC_Hours,
						RTC_TimeStructure.RTC_Minutes,
						RTC_TimeStructure.RTC_Seconds);

				oled.x = 32;
			}

			if (g_rtc_get_what == FLAG_RTC_GET_DATE)
			{
				/* RTC_GetTime，获取日期 */
				RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure);

				memset(buf, 0, sizeof buf);

				/* 格式化字符串 */
				sprintf((char *)buf, "20%02x-%02x-%02x-%d", RTC_DateStructure.RTC_Year,
						RTC_DateStructure.RTC_Month,
						RTC_DateStructure.RTC_Date,
						RTC_DateStructure.RTC_WeekDay);
				oled.x = 15;
			}

			/* oled显示时间 */
			oled.ctrl = OLED_CTRL_SHOW_STRING;
			oled.y = 5;
			oled.str = buf;
			oled.font_size = 16;

			xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
								 &oled,		   /* 发送的消息内容 */
								 100);		   /* 等待时间 100 Tick */
			if (xReturn != pdPASS)
				dgb_printf_safe("[app_task_rtc] xQueueSend oled string error code is %d\r\n", xReturn);

			dgb_printf_safe("[app_task_rtc] %s\r\n", buf);
		}
	}
}

/**
 * @brief rtc警告/闹钟任务
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 警告事件到达，若CPU运行在停止模式，则执行喂狗
 */
static void app_task_rtc_alarm(void *pvParameters)
{
	
	oled_t oled;
	beep_t beep;

	BaseType_t xReturn;
	EventBits_t EventValue;

	uint8_t led_sta;
	uint32_t i,j;

	dgb_printf_safe("[app_task_rtc_alarm] create success\r\n");

	for (;;)
	{
		/* 等待事件组中的相应事件位，或同步 */
		EventValue = xEventGroupWaitBits((EventGroupHandle_t)g_event_group,
										 (EventBits_t)EVENT_GROUP_RTC_ALARM,
										 (BaseType_t)pdTRUE,
										 (BaseType_t)pdFALSE,
										 (TickType_t)portMAX_DELAY);

		if (EventValue & EVENT_GROUP_RTC_ALARM)
		{
			if(g_cpu_operating_mode != CPU_STOP)
			{
				dgb_printf_safe("[app_task_rtc_alarm] alarm success\r\n");

				//配合闹钟，增加效果:控制蜂鸣器与灯光
				for(j=0; j<6; j++)
				{
					for(i=0; i<4; i++)
					{
						/* led */
						if(i%2==0)led_sta=0x11;
						else 	  led_sta=0x10;
							
						xReturn = xQueueSend(g_queue_led, /* 消息队列的句柄 */
											&led_sta,	  /* 发送的消息内容 */
											100);		  /* 等待时间 100 Tick */
						if (xReturn != pdPASS)
						{
							dgb_printf_safe("[app_task_rtc_alarm] xQueueSend led_sta error code is %d\r\n", xReturn);
							continue;
						}

						/* 蜂鸣器 */
						if(i%2==0)beep.sta=0x1;
						else 	  beep.sta=0x0;	
						beep.duration = 0;
						xQueueSend(g_queue_beep, /* 消息队列的句柄 */
								&beep,		 	 /* 发送的消息内容 */
								100);		 	 /* 等待时间 100 Tick */

						/* 阻塞等待信号量，用于确保任务完成对灯的控制 */
						xReturn = xSemaphoreTake(g_sem_led, portMAX_DELAY);

						if (xReturn != pdPASS)
							dgb_printf_safe("[app_task_rtc_alarm] xSemaphoreTake g_sem_led error code is %d\r\n", xReturn);

						/* [可选]阻塞等待信号量，用于确保任务完成对beep的控制 */
						xReturn = xSemaphoreTake(g_sem_beep, portMAX_DELAY);
						if (xReturn != pdPASS)
							dgb_printf_safe("[app_task_rtc_alarm] xSemaphoreTake g_sem_beep error code is %d\r\n", xReturn);	
						vTaskDelay(80);
					}	
					vTaskDelay(600);
				}

				/* 清空右上角的闹钟小图标 */
				oled.ctrl = OLED_CTRL_CLEAR_REGION;
				oled.x = 112;
				oled.y = 0;
				oled.clr_region_width=16;
				oled.clr_region_height=2;
				xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
									&oled,		   /* 发送的消息内容 */
									100);		   /* 等待时间 100 Tick */
				if (xReturn != pdPASS)
					dgb_printf_safe("[app_task_rtc_alarm] xQueueSend oled string error code is %d\r\n", xReturn);

			}
		}
	}
}
/**
 * @brief led任务
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 接收消息队列来控制led
 */
static void app_task_led(void *pvParameters)
{
	uint8_t led_sta = 0;
	BaseType_t xReturn = pdFALSE;

	dgb_printf_safe("[app_task_led] create success\r\n");

	for (;;)
	{
		xReturn = xQueueReceive(g_queue_led,	/* 消息队列的句柄 */
								&led_sta,		/* 得到的消息内容 */
								portMAX_DELAY); /* 等待时间一直等 */
		if (xReturn != pdPASS)
			continue;

		/* 检测到控制LED1 */
		if (led_sta & 0x10)
			PFout(9) = led_sta & 0x01 ? 0 : 1;

		/* 检测到控制LED2 */
		if (led_sta & 0x20)
			PFout(10) = led_sta & 0x02 ? 0 : 1;

		/* 检测到控制LED3 */
		if (led_sta & 0x40)
			PEout(13) = led_sta & 0x04 ? 0 : 1;

		/* 检测到控制LED4 */
		if (led_sta & 0x80)
			PEout(14) = led_sta & 0x08 ? 0 : 1;

		/* 释放信号量，告诉对方，当前led控制任务已经完成 */
		xSemaphoreGive(g_sem_led);
	}
}
/**
 * @brief 呼吸灯任务
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 进入熄屏状态，该任务运行。否则挂起。
 */
static void app_task_led_breath(void *pvParameters)
{
	int8_t brightness = 0;

	dgb_printf_safe("[app_task_led_breath] create success and suspend self\r\n");

	vTaskSuspend(NULL);

	dgb_printf_safe("[app_task_led_breath] resume success\r\n");

	for (;;)
	{
		/* 渐亮 */
		for (brightness = 0; brightness <= 100; brightness++)
		{
			led_breath_brightness(brightness);
			vTaskDelay(20);
		}

		/* 渐灭 */
		for (brightness = 100; brightness >= 0; brightness--)
		{
			led_breath_brightness(brightness);
			vTaskDelay(20);
		}
	}
}
/**
 * @brief 蜂鸣器任务
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 接收消息队列控制蜂鸣器。
 */
static void app_task_beep(void *pvParameters)
{
	beep_t beep;
	BaseType_t xReturn = pdFALSE;

	dgb_printf_safe("[app_task_beep] create success\r\n");

	for (;;)
	{
		xReturn = xQueueReceive(g_queue_beep,	/* 消息队列的句柄 */
								&beep,			/* 得到的消息内容 */
								portMAX_DELAY); /* 等待时间一直等 */
		if (xReturn != pdPASS)
			continue;

		dgb_printf_safe("[app_task_beep] beep.sta=%d beep.duration=%d\r\n", beep.sta, beep.duration);

		/* 检查蜂鸣器是否需要持续工作 */
		if (beep.duration)
		{
			BEEP(beep.sta);

			while (beep.duration--)
				vTaskDelay(1);

			/* 蜂鸣器状态翻转 */
			beep.sta ^= 1;
		}

		BEEP(beep.sta);

		/* 释放信号量，告诉对方，当前beep控制任务已经完成 */
		xSemaphoreGive(g_sem_beep);
	}
}
/**
 * @brief oled任务
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 接收消息队列控制oled显示。
 */
static void app_task_oled(void *pvParameters)
{
	oled_t oled;

	BaseType_t xReturn = pdFALSE;
	dgb_printf_safe("[app_task_oled] create success\r\n");

	for (;;)
	{
		xReturn = xQueueReceive(g_queue_oled,	/* 消息队列的句柄 */
								&oled,			/* 得到的消息内容 */
								portMAX_DELAY); /* 等待时间一直等 */
		if (xReturn != pdPASS)
			continue;

		switch (oled.ctrl)
		{
			case OLED_CTRL_DISPLAY_ON:
			{
				/* 亮屏 */
				OLED_Display_On();

				/* 呼吸灯停止工作 */
				vTaskSuspend(app_task_led_breath_handle);
				led_breath_stop();
			}
			break;

			case OLED_CTRL_DISPLAY_OFF:
			{
				/* 灭屏 */
				OLED_Display_Off();

				/* 呼吸灯开始工作 */
				led_breath_run();
				vTaskResume(app_task_led_breath_handle);
			}
			break;
			
			case OLED_CTRL_DISPLAY_DIRECTION:
			{
				/* 设置屏幕显示方向 */
				OLED_Set_Direction(oled.direction);
			}
			break;			
			

			case OLED_CTRL_CLEAR:
			{
				/* 清屏 */
				OLED_Clear();
			}
			break;

			case OLED_CTRL_CLEAR_REGION:
			{
				/* 清空指定显示区域 */
				OLED_ClearRegion(oled.x,
								oled.y,
								oled.x + oled.clr_region_width,
								oled.y + oled.clr_region_height);
			}
			break;		

			case OLED_CTRL_SHOW_STRING:
			{
				/* 显示字符串 */
				OLED_ShowString(oled.x,
								oled.y,
								oled.str,
								oled.font_size);
			}
			break;

			case OLED_CTRL_SHOW_CHINESE:
			{

				/* 显示汉字 */
				OLED_ShowCHinese(oled.x,
								oled.y,
								oled.chinese);
			}
			break;

			case OLED_CTRL_SHOW_PICTURE:
			{
				/* 显示图片 */
				OLED_DrawBMP(oled.x,
							oled.y,
							oled.x + oled.pic_width,
							oled.y + oled.pic_height,
							oled.pic);
			}
			break;

			default:
				dgb_printf_safe("[app_task_oled] oled ctrl code is invalid\r\n");
				break;
		}
	}
}
/**
 * @brief oled显示动画
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 控制oled显示动画
 */
static void app_task_oled_animation(void *pvParameters)
{
	oled_t oled;

	BaseType_t xReturn = pdFALSE;
	
	int32_t i;
	
	dgb_printf_safe("[app_task_oled_animation] create success and suspend self \r\n");

	vTaskSuspend(NULL);
	

	for(;;)
	{
		for(i=0; i<14; i++)
		{
			/* 设置要显示的图片 */
			oled.x=0;
			oled.y=0;
			oled.pic_width=128;
			oled.pic_height=8;
			oled.ctrl=OLED_CTRL_SHOW_PICTURE;
			oled.pic=pic_astronaut_tbl[i];

			xReturn = xQueueSend( 	g_queue_oled,	/* 消息队列的句柄 */
									&oled,			/* 发送的消息内容 */
									100);			/* 等待时间 100 Tick */
			if(xReturn != pdPASS)
				dgb_printf_safe("[app_task_oled_animation] xQueueSend oled picture error code is %d\r\n",xReturn);		


			vTaskDelay(100);
		
		}
	}
	
}
/**
 * @brief 菜单管理任务
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 接收按键事件后执行相应菜单的
 */
static void app_task_menu(void *pvParameters)
{
	uint32_t item_total;		//记录当前菜单有多少个项目
	uint32_t item_cursor = 0;	//记录当前菜单指向哪个项目
	uint32_t y = 0;
	uint32_t fun_run = 0;		//记录是否有项目功能函数在执行

	EventBits_t EventValue;

	menu_ext_t m_ext;

	menu_t *m;

	/* 通过参数传递获取主菜单页指针 */
	menu_t **m_main = (menu_t **)pvParameters;	

	/* 配置m_ext指向的当前菜单及相关参数 */
	m_ext.menu = *m_main;						
	m_ext.key_fn = KEY_NONE;
	m_ext.item_cursor = item_cursor;
	m_ext.item_total = menu_item_total(m_ext.menu);

	/* 配置记录菜单指针m */
	m = m_ext.menu;

	dgb_printf_safe("[app_task_menu] create success\r\n");

	for (;;)
	{
		/* 等待事件组中的相应事件位，或同步 */
		EventValue = xEventGroupWaitBits((EventGroupHandle_t)g_event_group,
										 (EventBits_t)    EVENT_GROUP_FN_KEY_UP \
										 				| EVENT_GROUP_FN_KEY_DOWN \
										 				| EVENT_GROUP_FN_KEY_ENTER \
										 				| EVENT_GROUP_FN_KEY_BACK,
										 (BaseType_t)pdTRUE,
										 (BaseType_t)pdFALSE,
										 (TickType_t)portMAX_DELAY);

		if (EventValue & EVENT_GROUP_FN_KEY_UP)
		{
			dgb_printf_safe("[app_task_menu] KEY_UP\r\n");

			/* 若有项目功能函数在运行，提示需要返回才能进行项目选择 */
			if (fun_run)
			{
				dgb_printf_safe("[app_task_menu] menu fun is running,please press back\r\n");

				continue;
			}

			/* 清除上个项目光标 */
			item_cursor_show(5, y + item_cursor * 2, " ");

			/* 项目光标位置值自减1 */
			if (item_cursor)
			{
				item_cursor--;

				dgb_printf_safe("[app_task_menu] item_cursor-- is=%d\r\n", item_cursor);

				/* 项目菜单指向上一个菜单 */
				m--;
			}
			else
			{
				/* 若main菜单有同级的左侧菜单 */
				if (m->same_left)
				{
					dgb_printf_safe("[app_task_menu] menu main switch to the left menu\r\n");
					item_cursor = 0;

					m_main--;

					m_ext.menu = *m_main;
					m_ext.key_fn = KEY_NONE;
					m_ext.item_cursor = 0;
					m_ext.item_total = menu_item_total(m_ext.menu);

					m = m_ext.menu;

					/* 显示菜单内容 */
					menu_show(&m_ext);
				}
			}

			/* 项目光标位置显示 > */
			item_cursor_show(5, y + item_cursor * 2, ">");

			dgb_printf_safe("[app_task_menu] item cursor=%d\r\n", item_cursor);
			dgb_printf_safe("[app_task_menu] item name %s\r\n", m->item);

			/* 保存当前按键编码 */
			m_ext.key_fn = KEY_UP;

			/* 保存当前项目光标位置值 */
			m_ext.item_cursor = item_cursor;
		}

		if (EventValue & EVENT_GROUP_FN_KEY_DOWN)
		{
			dgb_printf_safe("[app_task_menu] KEY_DOWN\r\n");

			/* 若有项目功能函数在运行，提示需要返回才能进行项目选择 */
			if (fun_run)
			{
				dgb_printf_safe("[app_task_menu] menu fun is running,please press back\r\n");

				continue;
			}

			/* 获取当前菜单有多少个项目 */
			item_total = menu_item_total(m_ext.menu);

			/* 保存当前菜单有多少个项目 */
			m_ext.item_total = item_total;

			dgb_printf_safe("[app_task_menu] item_total=%d\r\n", item_total);

			/* 清除上个项目光标 */
			item_cursor_show(5, y + item_cursor * 2, " ");

			if (item_cursor < (item_total - 1))
			{
				item_cursor++;

				dgb_printf_safe("[app_task_menu] item_cursor++ is=%d\r\n", item_cursor);

				/* 项目菜单指向下一个菜单 */
				m++;
			}
			else
			{
				/* 若main菜单有同级的右侧菜单 */
				if (m->same_right)
				{
					dgb_printf_safe("[app_task_menu] menu main switch to the right menu\r\n");
					item_cursor = 0;
					m_main++;

					m_ext.menu = *m_main;
					m_ext.key_fn = KEY_NONE;
					m_ext.item_cursor = 0;
					m_ext.item_total = menu_item_total(m_ext.menu);

					m = m_ext.menu;

					/* 显示菜单内容 */
					menu_show(&m_ext);
				}
			}

			/* 项目光标位置显示 > */
			item_cursor_show(5, y + item_cursor * 2, ">");

			dgb_printf_safe("[app_task_menu] item cursor=%d\r\n", item_cursor);
			dgb_printf_safe("[app_task_menu] KEY_DOWN item name %s\r\n", m->item ? m->item : "NULL");

			/* 保存当前按键编码 */
			m_ext.key_fn = KEY_DOWN;

			/* 保存当前项目光标位置值 */
			m_ext.item_cursor = item_cursor;
		}

		if (EventValue & EVENT_GROUP_FN_KEY_ENTER)
		{
			m_ext.key_fn = KEY_ENTER;

			/* 若有项目功能函数在运行，提示需要返回才能进行项目选择 */
			if (fun_run)
			{
				dgb_printf_safe("[app_task_menu] menu fun is running,please press back\r\n");

				continue;
			}

			dgb_printf_safe("[app_task_menu] KEY_ENTER item cursor=%d\r\n", item_cursor);
			dgb_printf_safe("[app_task_menu] KEY_ENTER item name %s\r\n", m->item ? m->item : "NULL");

			/* 子菜单有效 */
			if (m->child)
			{
				/* 指向子菜单 */
				m = m->child;

				/* 保存当前菜单 */
				m_ext.menu = m;

				/* 显示菜单内容 */
				menu_show(&m_ext);

				/* 复位光标值 */
				item_cursor = 0;
			}

			/* 若没有子菜单,则直接执行功能函数 */
			if (!m->child && m->fun)
			{
				/* 标记有项目功能函数在运行 */
				fun_run = 1;

				m->fun(&m_ext);
			}
		}

		if (EventValue & EVENT_GROUP_FN_KEY_BACK)
		{
			m_ext.key_fn = KEY_BACK;

			dgb_printf_safe("[app_task_menu] KEY_BACK item cursor=%d\r\n", item_cursor);
			dgb_printf_safe("[app_task_menu] KEY_BACK item name %s\r\n", m->item ? m->item : "NULL");

			/* 若子菜单功能函数有效，先执行，主要是挂起对应任务 */
			if (m->fun)
			{
				/* 标记有项目功能函数在运行 */
				fun_run = 1;

				m->fun(&m_ext);
			}

			/* 父菜单有效 */
			if (m->parent)
			{
				/* 指向父菜单 */
				m = m->parent;

				/* 保存当前菜单 */
				m_ext.menu = m;

				/* 复位光标值 */
				item_cursor = 0;
				m_ext.item_cursor = 0;

				dgb_printf_safe("[app_task_menu] m->parent item cursor=%d\r\n", item_cursor);
				dgb_printf_safe("[app_task_menu] m->parent item name %s\r\n", m->item ? m->item : "NULL");

				fun_run = 0;

				/* 显示当前菜单 */
				menu_show(&m_ext);
			}
		}
	}
}

/**
 * @brief 密码管理
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 密码验证、密码修改
 */
static void app_task_pass_man(void *pvParameters)
{
#define PASS1_ADDR_IN_EEPROM	0
#define PASS_LEN				6

#define PASS_MODIFY_STA_NONE	 0
#define PASS_MODIFY_STA_OLD_PASS 1
#define PASS_MODIFY_STA_NEW_PASS 2

#define	XOR_KEY					 0x88
	
	oled_t	 oled;
	BaseType_t xReturn;
	int32_t rt;

	char 	 key_val=0;
	char 	 key_buf[PASS_LEN]={0};
	uint32_t key_cnt=0;

const 
	char 	 pass_auth_default[PASS_LEN]={'8','8','8','8','8','8'};
	char 	 pass_auth_eeprom[PASS_LEN]={0};

	char 	 pass_old[PASS_LEN]={0};
	char 	 pass_new[PASS_LEN]={0};
	char	 enc_buf[PASS_LEN]={0};
	char	 dec_buf[PASS_LEN]={0};
	
	uint8_t  chn_tbl[16]={0};
	uint32_t i=0;
	
	beep_t	 beep;

	const uint8_t x_start=66;	
	const uint8_t y_start=4;		
	uint8_t	 x=x_start,y=y_start;
	
	uint32_t pass_modify_sta = PASS_MODIFY_STA_OLD_PASS;

	dgb_printf_safe("[app_task_pass_man] create success\r\n");

	for (;;)
	{
		/* 等待矩阵键盘消息 */
		xReturn = xQueueReceive(g_queue_kbd,&key_val,portMAX_DELAY);

		if(xReturn!=pdPASS)
			continue;

		/* 清零统计系统无操作计数值 */
		taskENTER_CRITICAL();
		g_system_no_opreation_cnt = 0;
		taskEXIT_CRITICAL();			
		
		/* 复位密码操作内容 */
		if(key_val == 'C')
		{
			x=x_start;
			y=y_start;
			key_cnt=0;

			memset(key_buf, 0,sizeof key_buf);
			memset(pass_old,0,sizeof pass_old);	
			memset(pass_new,0,sizeof pass_new);	

			pass_modify_sta = PASS_MODIFY_STA_OLD_PASS;

			continue;
		}

		/* 密码认证测试 */
		if(g_pass_man_what == FLAG_PASS_MAN_AUTH)
		{
			/* 没有密码，无效删除操作 */
			if(key_val == '*' && key_cnt==0)
				continue;

			/* 删除上一格密码 */
			if(key_val == '*' && key_cnt)
			{
				key_cnt--;
				key_buf[key_cnt]=0;

				x-=8;
				oled.ctrl = OLED_CTRL_SHOW_STRING;
				oled.x = x;
				oled.y = y;
				oled.font_size = 16;

				oled.str ="-";
				xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
									&oled,		   /* 发送的消息内容 */
									100);		   /* 等待时间 100 Tick */
				if (xReturn != pdPASS)
					dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);

				continue;				
			}

			if(key_cnt<6)
			{
				/* 显示数字 */
				key_buf[key_cnt] = key_val;
				
				oled.ctrl = OLED_CTRL_SHOW_STRING;
				oled.x = x;
				oled.y = y;
				oled.font_size = 16;

				oled.str =(uint8_t *) &key_buf[key_cnt];
				xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
									&oled,		   /* 发送的消息内容 */
									100);		   /* 等待时间 100 Tick */
				if (xReturn != pdPASS)
					dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);

				vTaskDelay(100);

				/* 显示* */
				oled.str = "*";
				xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
									&oled,		   /* 发送的消息内容 */
									100);		   /* 等待时间 100 Tick */
				if (xReturn != pdPASS)
					dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);

				x+=8;
				key_cnt++;
			}
			else
			{
				if(key_val == '#')
				{
					dgb_printf_safe("[app_task_pass_man] auth key buf is %6s\r\n", key_buf);		
					
					/* 读取eeprom存储的密码 */
					at24c02_read(PASS1_ADDR_IN_EEPROM,(uint8_t *)pass_auth_eeprom,PASS_LEN);
					
					/* 异或解密 */
					xor_decryption(XOR_KEY,pass_auth_eeprom,dec_buf,PASS_LEN);

					/* 匹配eeprom存储的密码 */
					rt = memcmp(dec_buf,key_buf,PASS_LEN);
					if(rt)
					{
						dgb_printf_safe("[app_task_pass_man] password auth by eeprom fail\r\n");
						
						/* 匹配默认密码 */
						rt=memcmp(pass_auth_default,key_buf,PASS_LEN);
						if(rt)
							dgb_printf_safe("[app_task_pass_man] password auth by defalut fail too\r\n");
					}
					
					/* 密码匹配成功 */
					if(rt==0)
					{
						dgb_printf_safe("[app_task_pass_man] password auth success\r\n");

						/* 设置要显示的图片-表情（成功） */
						oled.x=64;
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

						/* 嘀一声示意:第一次 */
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

						vTaskDelay(100);

						/* 嘀一声示意:第二次 */
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


					}
					else
					{
						/* 设置要显示的图片-表情（失败） */
						oled.x=64;
						oled.y=0;
						oled.pic_width=64;
						oled.pic_height=8;
						oled.ctrl=OLED_CTRL_SHOW_PICTURE;
						oled.pic=pic_face_fail_icon;

						xReturn = xQueueSend( 	g_queue_oled,	/* 消息队列的句柄 */
												&oled,			/* 发送的消息内容 */
												100);			/* 等待时间 100 Tick */
						if(xReturn != pdPASS)
							dgb_printf_safe("[app_task_pass_man] xQueueSend oled picture error code is %d\r\n",xReturn);

						/* 长鸣1秒示意 */
						beep.sta = 1;
						beep.duration = 1000;

						xReturn = xQueueSend(g_queue_beep, /* 消息队列的句柄 */
											&beep,		   /* 发送的消息内容 */
											100);		   /* 等待时间 100 Tick */				
						if (xReturn != pdPASS)
							dgb_printf_safe("[app_task_pass_man] xQueueSend beep error code is %d\r\n", xReturn);	

						/* [可选]阻塞等待信号量，用于确保任务完成对beep的控制 */
						xReturn = xSemaphoreTake(g_sem_beep, portMAX_DELAY);
						if (xReturn != pdPASS)
							dgb_printf_safe("[app_task_pass_man] xSemaphoreTake g_sem_beep error code is %d\r\n", xReturn);	

					}

					/* 延时一会 */
					vTaskDelay(2000);

					/* 恢复初值 */
					x=x_start;
					key_cnt=0;
					memset(key_buf,0,sizeof key_buf);	

					/* 清空右半部分区域 */
					oled.ctrl = OLED_CTRL_CLEAR_REGION;
					oled.x = 64;
					oled.y = 0;
					oled.clr_region_width=64;
					oled.clr_region_height=8;
					xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
										&oled,		   /* 发送的消息内容 */
										100);		   /* 等待时间 100 Tick */
					if (xReturn != pdPASS)
						dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);


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
							dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);
					}

					/* 清空* */
					oled.ctrl = OLED_CTRL_SHOW_STRING;
					oled.x = x_start;
					oled.y = 4;
					oled.str = "------";
					oled.font_size = 16;

					xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
										&oled,		   /* 发送的消息内容 */
										100);		   /* 等待时间 100 Tick */
					if (xReturn != pdPASS)
						dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);
				}
			}
		}

		/* 密码修改测试 */
		if(g_pass_man_what == FLAG_PASS_MAN_MODIFY)
		{
			switch(pass_modify_sta)
			{
				case PASS_MODIFY_STA_OLD_PASS:
				{
					/* 没有密码，无效删除操作 */
					if(key_val == '*' && key_cnt==0)
						continue;

					/* 删除上一格密码 */
					if(key_val == '*' && key_cnt)
					{
						key_cnt--;
						key_buf[key_cnt]=0;

						x-=8;
						oled.ctrl = OLED_CTRL_SHOW_STRING;
						oled.x = x;
						oled.y = 2;
						oled.font_size = 16;

						oled.str ="-";
						xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
											&oled,		   /* 发送的消息内容 */
											100);		   /* 等待时间 100 Tick */
						if (xReturn != pdPASS)
							dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);

						continue;				
					}

					if(key_cnt<6)
					{
						/* 显示数字 */
						key_buf[key_cnt] = key_val;

						oled.ctrl = OLED_CTRL_SHOW_STRING;
						oled.x = x;
						oled.y = 2;
						oled.font_size = 16;

						oled.str =(uint8_t *) &key_buf[key_cnt];
						xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
											&oled,		   /* 发送的消息内容 */
											100);		   /* 等待时间 100 Tick */
						if (xReturn != pdPASS)
							dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);

						vTaskDelay(100);

						oled.str = "*";
						xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
											&oled,		   /* 发送的消息内容 */
											100);		   /* 等待时间 100 Tick */
						if (xReturn != pdPASS)
							dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);

						x+=8;
						key_cnt++;
					}
					else
					{
						if(key_val == '#')
						{
							pass_modify_sta=PASS_MODIFY_STA_NEW_PASS;
							key_cnt=0;
							memcpy(pass_old,key_buf,PASS_LEN);
							memset(key_buf,0,sizeof key_buf);
							x=x_start;
							y=6;
						}
					}	
				}break;

				case PASS_MODIFY_STA_NEW_PASS:
				{

					/* 没有密码，无效删除操作 */
					if(key_val == '*' && key_cnt==0)
						continue;

					/* 删除上一格密码 */
					if(key_val == '*' && key_cnt)
					{
						key_cnt--;
						key_buf[key_cnt]=0;

						x-=8;
						oled.ctrl = OLED_CTRL_SHOW_STRING;
						oled.x = x;
						oled.y = 6;
						oled.font_size = 16;

						oled.str ="-";
						xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
											&oled,		   /* 发送的消息内容 */
											100);		   /* 等待时间 100 Tick */
						if (xReturn != pdPASS)
							dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);

						continue;				
					}


					if(key_cnt<6)
					{
						/* 显示'*' */
						key_buf[key_cnt] = key_val;

						oled.ctrl = OLED_CTRL_SHOW_STRING;
						oled.x = x;
						oled.y = 6;
						oled.font_size = 16;

						oled.str =(uint8_t *) &key_buf[key_cnt];
						xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
											&oled,		   /* 发送的消息内容 */
											100);		   /* 等待时间 100 Tick */
						if (xReturn != pdPASS)
							dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);

						vTaskDelay(100);

						oled.str = "*";
						xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
											&oled,		   /* 发送的消息内容 */
											100);		   /* 等待时间 100 Tick */
						if (xReturn != pdPASS)
							dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);

						x+=8;
						key_cnt++;
					}
					else
					{
						if(key_val == '#')
						{
							/* 保存新密码 */
							memcpy(pass_new,key_buf,PASS_LEN);

							/* 读取eeprom存储的密码 */
							at24c02_read(PASS1_ADDR_IN_EEPROM,(uint8_t *)pass_auth_eeprom,PASS_LEN);
							dgb_printf_safe("[app_task_pass_man] password eeprom is %6s\r\n",pass_auth_eeprom);
							
							/* 异或解密 */							
							xor_decryption(XOR_KEY,pass_auth_eeprom,dec_buf,PASS_LEN);
							
							/* 匹配eeprom存储的密码 */
							rt = memcmp(dec_buf,pass_old,PASS_LEN);
							if(rt)
							{
								dgb_printf_safe("[app_task_pass_man] password old is %6s\r\n",pass_old);
								dgb_printf_safe("[app_task_pass_man] password auth by eeprom fail\r\n");
								
								/* 匹配默认密码 */
								rt=memcmp(pass_auth_default,pass_old,PASS_LEN);
								dgb_printf_safe("[app_task_pass_man] password default is %6s\r\n",pass_auth_default);
								if(rt)
								{
									dgb_printf_safe("[app_task_pass_man] password auth by defalut fail too\r\n");
								}
									
							}
							
							/* 密码匹配成功 */
							if(rt==0)
							{
								
								dgb_printf_safe("[app_task_pass_man] password auth success\r\n");
								
								/* 进入临界区，可靠地向eeprom异或加密写入新密码 */
								taskENTER_CRITICAL();
								
								xor_encryption(XOR_KEY,pass_new,enc_buf,PASS_LEN);
								
								rt = at24c02_write(PASS1_ADDR_IN_EEPROM,(uint8_t *)enc_buf,PASS_LEN);
								
								taskEXIT_CRITICAL();

								if(rt == 0)
									dgb_printf_safe("[app_task_pass_man] password new is %6s,write into eeprom success\r\n",pass_new);
								else
									dgb_printf_safe("[app_task_pass_man] password new is %6s,write into eeprom fail,error code is %d\r\n",pass_new,rt);

								/* 设置要显示的图片-表情（失败） */
								oled.x=64;
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
								
								
								/* 嘀一声示意:第一次 */
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

								vTaskDelay(100);

								/* 嘀一声示意:第二次 */
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
							}
							else
							{
								/* 设置要显示的图片-表情（失败） */
								oled.x=64;
								oled.y=0;
								oled.pic_width=64;
								oled.pic_height=8;
								oled.ctrl=OLED_CTRL_SHOW_PICTURE;
								oled.pic=pic_face_fail_icon;

								xReturn = xQueueSend( 	g_queue_oled,	/* 消息队列的句柄 */
														&oled,			/* 发送的消息内容 */
														100);			/* 等待时间 100 Tick */
								if(xReturn != pdPASS)
									dgb_printf_safe("[app_task_pass_man] xQueueSend oled picture error code is %d\r\n",xReturn);


								/* 长鸣1秒示意 */
								beep.sta = 1;
								beep.duration = 1000;

								xReturn = xQueueSend(g_queue_beep, /* 消息队列的句柄 */
													&beep,		   /* 发送的消息内容 */
													100);		   /* 等待时间 100 Tick */				
								if (xReturn != pdPASS)
									dgb_printf_safe("[app_task_pass_man] xQueueSend beep error code is %d\r\n", xReturn);	

								/* [可选]阻塞等待信号量，用于确保任务完成对beep的控制 */
								xReturn = xSemaphoreTake(g_sem_beep, portMAX_DELAY);
								if (xReturn != pdPASS)
									dgb_printf_safe("[app_task_pass_man] xSemaphoreTake g_sem_beep error code is %d\r\n", xReturn);											


							}

							/* 延时一会 */
							vTaskDelay(2000);	


							/* 清空右半部分区域 */
							oled.ctrl = OLED_CTRL_CLEAR_REGION;
							oled.x = 64;
							oled.y = 0;
							oled.clr_region_width=64;
							oled.clr_region_height=8;
							xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
												&oled,		   /* 发送的消息内容 */
												100);		   /* 等待时间 100 Tick */
							if (xReturn != pdPASS)
								dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);


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

							/* 恢复初值 */
							pass_modify_sta=PASS_MODIFY_STA_OLD_PASS;
							key_cnt=0;
							memset(key_buf, 0,sizeof key_buf);
							memset(pass_old,0,sizeof pass_old);	
							memset(pass_new,0,sizeof pass_new);	

							x=x_start;
						}

					}

				}break;

				default:break;
			}
		}
	}
}

/**
 * @brief 系统复位任务
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 关键调用NVIC_SystemReset函数
 */
static void app_task_system_reset(void *pvParameters)
{
	uint8_t x = 0;
	beep_t beep;
	oled_t oled;
	BaseType_t xReturn = pdFALSE;
	EventBits_t EventValue;
	uint32_t i = 0;

	dgb_printf_safe("[app_task_system_reset] create success\r\n");

	for (;;)
	{
		/* 等待事件组中的相应事件位，或同步 */
		EventValue = xEventGroupWaitBits((EventGroupHandle_t)g_event_group,
										 (EventBits_t)EVENT_GROUP_SYSTEM_RESET,
										 (BaseType_t)pdTRUE,
										 (BaseType_t)pdFALSE,
										 (TickType_t)portMAX_DELAY);

		if (EventValue & EVENT_GROUP_SYSTEM_RESET)
		{
			/* 挂起涉及用到OLED的任务 */
			//vTaskSuspend(app_task_key_handle);
			vTaskSuspend(app_task_mpu6050_handle);
			vTaskSuspend(app_task_dht_handle);
			vTaskSuspend(app_task_rtc_handle);

			if (g_gesture_valid)
				vTaskSuspend(app_task_gesture_handle);

			dgb_printf_safe("[app_task_system_reset] xQueueSend oled clear\r\n");

			/* OLED清屏 */
			oled.ctrl = OLED_CTRL_CLEAR;
			xQueueSend(g_queue_oled, &oled, 100);

			/* 显示"正在执行复位" */
			oled.ctrl = OLED_CTRL_SHOW_CHINESE;
			oled.y = 2;
			
			for (x = 16, i = 16; i <= 21; i++, x += 16)
			{
				oled.x = x;
				oled.chinese = i;
				xQueueSend(g_queue_oled, &oled, 100);
			}

			x = 0;
			oled.ctrl = OLED_CTRL_SHOW_STRING;
			oled.y = 6;
			oled.font_size = 16;

			while (1)
			{
				/* 显示进度 */
				oled.x = x;
				oled.str = ">";
				xQueueSend(g_queue_oled, &oled, 100);

				vTaskDelay(100);

				x += 8;

				if (x >= 120)
				{
					/* 嘀一声示意 */
					beep.sta = 1;
					beep.duration = 100;

					xQueueSend(g_queue_beep, /* 消息队列的句柄 */
							   &beep,		 /* 发送的消息内容 */
							   100);		 /* 等待时间 100 Tick */

					/* [可选]阻塞等待信号量，用于确保任务完成对beep的控制 */
					xReturn = xSemaphoreTake(g_sem_beep, portMAX_DELAY);
					if (xReturn != pdPASS)
						dgb_printf_safe("[app_task_system_reset] xSemaphoreTake g_sem_beep error code is %d\r\n", xReturn);

					/* OLED清屏 */
					oled.ctrl = OLED_CTRL_CLEAR;
					xQueueSend(g_queue_oled, &oled, 100);

					/* 系统复位 */
					NVIC_SystemReset();
				}
			}
		}
	}
}
/**
 * @brief 软件定时器任务
 * @param pvParameters:创建任务时传递的参数
 * @retval 无
 * @details 倒数管理、独立看门狗喂狗
 */
static void soft_timer_callback(TimerHandle_t pxTimer)
{
static 
	uint32_t count_backwards_sta=0;
	oled_t oled;
	BaseType_t xReturn;
	uint8_t buf[16] = {0};
	int32_t timer_id = (int32_t)pvTimerGetTimerID(pxTimer);

	/* 倒数状态成立 且 统计系统无操作计数值为0 或 g_system_no_opreation_cnt达到熄屏时间值 */
	if (count_backwards_sta && (g_system_no_opreation_cnt == 0 || g_system_no_opreation_cnt >= SCREEN_OFF_TIME))
	{
		/* 清空记录倒数状态 */
		count_backwards_sta = 0;

		/* oled显示字符串 */
		/* 清空右上角的倒数值*/
		oled.ctrl = OLED_CTRL_SHOW_STRING;
		oled.x = 120;
		oled.y = 0;

		memset(buf, 0, sizeof buf);
		sprintf((char *)buf, "%d", SCREEN_OFF_TIME - g_system_no_opreation_cnt);

		oled.str = " ";
		oled.font_size = 12;

		xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
							 &oled,		   /* 发送的消息内容 */
							 100);		   /* 等待时间 100 Tick */
		if (xReturn != pdPASS)
			dgb_printf_safe("[soft_timer_callback id=%d] xQueueSend oled string error code is %d\r\n", timer_id, xReturn);
	}

	/* 开始倒数,3 2 1 */
	if ((g_system_no_opreation_cnt >= SCREEN_OFF_TIME - 3) && (g_system_no_opreation_cnt < SCREEN_OFF_TIME))
	{
		dgb_printf_safe("[soft_timer_callback id=%d] g_system_no_opreation_cnt is %d\r\n", timer_id, g_system_no_opreation_cnt);

		/* 记录倒数状态 */
		count_backwards_sta=1;

		/* oled显示字符串 */
		oled.ctrl = OLED_CTRL_SHOW_STRING;
		oled.x = 120;
		oled.y = 0;

		memset(buf, 0, sizeof buf);
		sprintf((char *)buf, "%d", SCREEN_OFF_TIME - g_system_no_opreation_cnt);

		oled.str = buf;
		oled.font_size = 12;

		xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
							 &oled,		   /* 发送的消息内容 */
							 100);		   /* 等待时间 100 Tick */
		if (xReturn != pdPASS)
			dgb_printf_safe("[soft_timer_callback id=%d] xQueueSend oled string error code is %d\r\n", timer_id, xReturn);
	}

	/* g_system_no_opreation_cnt达到熄屏时间值，则执行OLED熄屏*/
	if (g_oled_display_flag && g_system_no_opreation_cnt >= SCREEN_OFF_TIME)
	{
		g_oled_display_flag = 0;

		/* OLED熄屏控制 */
		oled.ctrl = OLED_CTRL_DISPLAY_OFF;

		xReturn = xQueueSend(g_queue_oled, /* 消息队列的句柄 */
							 &oled,		   /* 发送的消息内容 */
							 100);		   /* 等待时间 100 Tick */
		if (xReturn != pdPASS)
			dgb_printf_safe("[soft_timer_callback id=%d] xQueueSend oled error code is %d\r\n", timer_id, xReturn);
	}

	/* 进入临界区，FreeRTOS同时会关闭中断（UCOS的OS_CRITICAL_ENTER则停止任务调度） */
	taskENTER_CRITICAL();

#if IWDG_ENABLE
	/* 喂狗，刷新自身计数值 */
	IWDG_ReloadCounter();
#endif
	/* 统计系统无操作计数值自加1 */
	g_system_no_opreation_cnt++;

	/* 退出临界区，FreeRTOS同时会打开中断（UCOS的OS_CRITICAL_EXTI则恢复任务调度） */
	taskEXIT_CRITICAL();
}

/*-----------------------------------------------------------*/

void vApplicationMallocFailedHook(void)
{
	/* vApplicationMallocFailedHook() will only be called if
	configUSE_MALLOC_FAILED_HOOK is set to 1 in FreeRTOSConfig.h.  It is a hook
	function that will get called if a call to pvPortMalloc() fails.
	pvPortMalloc() is called internally by the kernel whenever a task, queue,
	timer or semaphore is created.  It is also called by various parts of the
	demo application.  If heap_1.c or heap_2.c are used, then the size of the
	heap available to pvPortMalloc() is defined by configTOTAL_HEAP_SIZE in
	FreeRTOSConfig.h, and the xPortGetFreeHeapSize() API function can be used
	to query the size of free heap space that remains (although it does not
	provide information on how the remaining heap might be fragmented). */
	taskDISABLE_INTERRUPTS();
	printf("[vApplicationMallocFailedHook] ....\r\n");
	for (;;)
		;
}
/*-----------------------------------------------------------*/

void vApplicationIdleHook(void)
{
	/* vApplicationIdleHook() will only be called if configUSE_IDLE_HOOK is set
	to 1 in FreeRTOSConfig.h.  It will be called on each iteration of the idle
	task.  It is essential that code added to this hook function never attempts
	to block in any way (for example, call xQueueReceive() with a block time
	specified, or call vTaskDelay()).  If the application makes use of the
	vTaskDelete() API functi    on (as this demo application does) then it is also
	important that vApplicationIdleHook() is permitted to return to its calling
	function, because it is the responsibility of the idle task to clean up
	memory allocated by the kernel to any task that has since been deleted. */

	/* 进入睡眠模式 */
	if(g_cpu_operating_mode == CPU_SLEEP)
		__wfi();
}
/*-----------------------------------------------------------*/

void vApplicationStackOverflowHook(TaskHandle_t pxTask, char *pcTaskName)
{
	(void)pcTaskName;
	(void)pxTask;

	/* Run time stack overflow checking is performed if
	configCHECK_FOR_STACK_OVERFLOW is defined to 1 or 2.  This hook
	function is called if a stack overflow is detected. */
	taskDISABLE_INTERRUPTS();
	printf("[vApplicationStackOverflowHook] %s is StackOverflow\r\n", pcTaskName);
	for (;;);
}

void vApplicationTickHook(void)
{
}
