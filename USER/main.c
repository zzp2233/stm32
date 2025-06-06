/****************************************************************
*��    ��:FreeRTOS�ۺϴ���ʵ��
*��    ��:������
*��������:2021/07/21
*֪ ʶ ��:
	1.����ͼ��Logo�������ַ���ʾ
	2.���񴴽���ɾ�������𡢻ָ�
	3.�ź���
		��1����Ϣ���з�����ɺ����ź���֪ͨȷ����ɡ�
	4.������
		��1�����񻥳����printf	
	5.��Ϣ����
		��1������1������Ϣ���д�������RTC������ʱ��
			.����RTC����:DATE SET-2021-7-21-3#		
			.����RTCʱ��:TIME SET-10-30-45#		
		��2��LED��״̬��Ϣ���д���	
		��3��������������Ϣ���д���
		��4��OLED��������ơ�ͼ����ʾ
	6.�¼���־��
		��1����¼�����ⲿ�ж��¼���־
		��2����¼RTC�����ж��¼���־
		��3����¼ϵͳ��λ�¼���־
		��4����¼����ʶ��ģ���ж��¼���־
		��5����¼��ʪ�ȡ�RTC��MPU6050��OLED���¼���־			
	7.�ٽ���
		��1���ؼ�����������ı���
		��2�����Ź�ι��		
	8.�����ܷɲ�׽
		��1����׽HardFault_Handler
			������HardFault����LED�Ƶ�ȫ����ȫ������˸��֪������ӡ���ջ��Ϣ
		��2����׽��������쳣�ж�
			��������������쳣�жϣ���ӡ���Ӧ���쳣�жϺ�
	9.�����ʱ��
		��1�������ʱ���Ĵ���������
		��2�������ʱ���Ļص�����ʵ�ֶ������Ź�ι������	
	10.���������Ӻ���
		��1��һ�����������������CPU����˯��ģʽ�����͹���

*˵  ��:		
	��ǰ����淶���ṹ������������ʵ����ģ�黯��̣�һ���������һ��Ӳ����
	
*�޸�����:2021/07/22
	.���ӳ���S1����ʵ��ϵͳ��λ������S4����ʵ��OLED����/��
	.����OLED_SAFE����룬ʵ��OLED��ȫ�������
	.����mpu6050̧�ֻ���
	
*�޸�����:2021/07/23
	.OLED���ɵ�����������й���
	.֧�����ֲ�ͬ��I2C�ӿڵ�OLED��������oled.h�ļ������ú�OLED_I2C_PORT

*�޸�����:2021/07/24
	.���Ӳ˵����ܣ�ʹ����˫�������Ѷ��������ʺϻ����Ϻõ�ͬѧ�Ķ���
	.ɾ��ԭ�а�����ʾͼƬ

*�޸�����:2021/07/25
	.��������ʶ����(����ǰ�����ģ�飬�Զ�ʶ���ģ�鲢��ʼ��)
	.���Ӻ����ƣ�������Ϩ��״̬ʱ��ʼ����
	.�����Զ�Ϩ�����ܣ���ͨ��̧�ֻ򳤰�S4��������Ļ��
	
*�޸�����:2021/08/03
	.���ӿɵ�����Ϊת�̣����˵���ѡ��
	.��ǿ��˫������˵��������ͬ�����˵����ܣ�����˲˵���չ����
    .�����˴���1/����3���������˵���ѡ������ƣ�
	.������λ�����ɰ���1���Ϊ����3��ʵ��
	.����1������2֧�ֵĳ�������Ϊ����ѡ��˵���
*�޸�����:2021/08/17
	.�ر���ʱ��Ƭ����
	.�����˾������,ͬʱҪ���������̵���������ʪ��ģ��ĳ�ͻ
	.���������������棬����֤���롢�޸�����
	.�����˹��Ĺ����ܹ�����˯��ģʽ��ֹͣģʽ������ģʽ	
	
*�޸�����:2023/05/11
	.�Ż�OLED���ײ���룬�Ż�����
	.������̧���л�OLED����ʾ����0��~ 180��� 180��~ 0��
	.��������ԭ�������Ĵ洢�޸�Ϊ���Ĵ洢�������㷨Ϊ���ӽ����㷨
	.����author����һ����Զ���ʾ̫���˶���
	
*�޸�����:2023/09/17
	.HardFault_Handler�쳣�жϴ�������˴�ӡջ��Ϣ��R0~R3��R12��LR��PC�������ڲ��Ҵ���HardFault_Handler�Ĵ���
	.startup_stm32f40_41xxx.s�ļ��������˵���exceptions_catch���������ڲ�����������쳣�ж�

[����1]
	.��ǰĬ�������˿��Ź����룬�ɺ�IWDG_ENABLEʹ�ܡ�
	 ԭ�����ֹͣģʽ��û�б�Ҫʹ�ÿ��Ź������ӹ��ġ�
[����2]	 
	1��˯��ģʽ
		����˯��ģʽ��оƬ�����������ģʽ������͡�ͬʱ��Ӱ�����й��ܵ���ת��
	2��ֹͣģʽ
	.����ֹͣģʽ��оƬ������һ�ν��͡�
	.����ֹͣģʽ��OSֹͣ���У�mpu6050���޷����ڻ�ȡŷ���ǡ�
	 �ָ�������ģʽ��mpu6050�����³�ʼ��
	.���°���4/�����¼��������½�������ģʽ
	3������ģʽ
	.�������ģʽ��оƬ���Ľ�����͡�
	.���°���1����������ִ�У���ͬ����λЧ��
	.��ʹ�������ӣ����������ӻ��ѡ����ǰ���1���ѻ������ӻ���ֻ��2ѡ1��оƬ�����ġ�
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


/* �������ź������ */
SemaphoreHandle_t g_mutex_printf;
SemaphoreHandle_t g_mutex_oled;

/* �¼���־���� */
EventGroupHandle_t g_event_group;

/* ��Ϣ���о�� */
QueueHandle_t g_queue_usart;
QueueHandle_t g_queue_led;
QueueHandle_t g_queue_beep;
QueueHandle_t g_queue_oled;
QueueHandle_t g_queue_kbd;

/* ��ֵ�ź��� */
SemaphoreHandle_t g_sem_led;
SemaphoreHandle_t g_sem_beep;

/* �����ʱ����� */
static TimerHandle_t soft_timer_Handle = NULL;

/* ȫ�ֱ��� */
volatile uint32_t g_oled_display_flag = 1;
volatile uint32_t g_dht_get_what = FLAG_DHT_GET_NONE;
volatile uint32_t g_rtc_get_what = FLAG_RTC_GET_NONE;
volatile uint32_t g_mpu6050_get_what = FLAG_MPU6050_GET_NONE;
volatile uint32_t g_gesture_display_flag = FLAG_GESTURE_DISPLAY_OFF; // ��¼����ʶ��ģ���Ƿ���ʾ����ͼƬ
volatile uint32_t g_gesture_valid = 0;								 // ��¼����ʶ��ģ���Ƿ����ӳɹ�
volatile uint32_t g_system_no_opreation_cnt = 0;
volatile uint32_t g_pass_man_what = FLAG_PASS_MAN_NONE;
volatile uint32_t g_mpu6050_run_flag=1;
volatile cpu_operating_mode_t g_cpu_operating_mode=CPU_RUN;

/* �����ʱ�� */
static void soft_timer_callback(TimerHandle_t pxTimer);

/* ���� */ 
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


/* OLED�������߶ȷ�װ */
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

	/* ��ȡ�����ź��� */
	xSemaphoreTake(g_mutex_printf, portMAX_DELAY);

	vprintf(format, args);

	/* �ͷŻ����ź��� */
	xSemaphoreGive(g_mutex_printf);

	va_end(args);
#else
	(void)0;
#endif
}
/**
 * @brief ������
 * @param void:���봫�����
 * @retval ��
 */
int main(void)
{
	/* ����ϵͳ�ж����ȼ�����4 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	/* ϵͳ��ʱ���ж�Ƶ��ΪconfigTICK_RATE_HZ */
	SysTick_Config(SystemCoreClock/configTICK_RATE_HZ);	

	/* ��ʼ������1 */
	uart_init(115200);

	/* ���� app_task_init���� */
	xTaskCreate((TaskFunction_t)app_task_init,			/* ������ں��� */
				(const char *)"app_task_init",			/* �������� */
				(uint16_t)512,							/* ����ջ��С */
				(void *)NULL,							/* ������ں������� */
				(UBaseType_t)5,							/* ��������ȼ� */
				(TaskHandle_t *)&app_task_init_handle); /* ������ƿ�ָ�� */

	/* ����������� */
	vTaskStartScheduler();

	while (1);
}

/* �����б� */
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
 * @brief ���г�ʼ������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 */
static void app_task_init(void *pvParameters)
{
	uint32_t i = 0;
	menu_ext_t menu_ext;

	printf("[app_task_init] create success\r\n");

	/* �����������ź��� */
	g_mutex_printf 	= xSemaphoreCreateMutex();
	g_mutex_oled 	= xSemaphoreCreateMutex();

	/* ������ֵ���ź��� */
	g_sem_led  = xSemaphoreCreateBinary();
	g_sem_beep = xSemaphoreCreateBinary();

	/* �����¼���־�� */
	g_event_group = xEventGroupCreate();

	/* ������Ϣ���� */
	g_queue_usart 	= xQueueCreate(QUEUE_USART_LEN, sizeof(struct usart_packet_t));
	g_queue_led  	= xQueueCreate(QUEUE_LED_LEN, 	sizeof(uint8_t));
	g_queue_beep  	= xQueueCreate(QUEUE_BEEP_LEN, 	sizeof(beep_t));
	g_queue_oled 	= xQueueCreate(QUEUE_OLED_LEN, 	sizeof(oled_t));
	g_queue_kbd 	= xQueueCreate(QUEUE_KBD_LEN, 	sizeof(uint8_t));

	/* ��ʼ��OLED */
	OLED_Init();
	
	/* ������ʾ����ת180 */
	OLED_Set_Direction(180);
	
	OLED_Clear();

#if 1	
	/* ��ʾlogo */
	OLED_DrawBMP(0,0,128,8,(uint8_t *)pic_logo);	
																	
	/* ����2�� */
	vTaskDelay(2000);

	OLED_Clear();	
	
	/* ��ʾ��Ƕ��ʽʵʱϵͳ�� */
	OLED_ShowCHinese(8,0,0);
	OLED_ShowCHinese(24,0,1);
	OLED_ShowCHinese(40,0,2);	
	OLED_ShowCHinese(56,0,3);	
	OLED_ShowCHinese(72,0,4);		
	OLED_ShowCHinese(88,0,5);		
	OLED_ShowCHinese(104,0,6);
	
	/* ��ʾ���ۺ�ʵ���� */
	OLED_ShowCHinese(32,3,7);
	OLED_ShowCHinese(48,3,8);
	OLED_ShowCHinese(64,3,9);	
	OLED_ShowCHinese(80,3,10);	

	/* ��ʾ����Ƕ�������� */
	OLED_ShowCHinese(0,6,11);
	OLED_ShowCHinese(16,6,12);
	OLED_ShowCHinese(82,6,13);	
	OLED_ShowCHinese(98,6,14);	
	OLED_ShowCHinese(112,6,15);
	
	/* ����1�� */
	vTaskDelay(1000);
#endif

	/* led��ʼ�� */
	led_init();

	/* �����Ƴ�ʼ�� */
	led_breath_init();

	/* beep��ʼ�� */
	beep_init();

	/* ������ʼ�� */
	key_init();

	/* ble��ʼ�� */
	ble_init(9600);

	/* ��ʪ��ģ���ʼ�� */
	dht11_init();

	/* adc��ʼ�� */
	adc_init();

	/* rtc��ʼ�� */
	rtc_init();
	
	/* ���ӳ�ʼ�� */
	rtc_alarm_init();
	

	/* PAJ7620U2����ʶ�𴫸������ӵ������壬����ʼ���ɹ���Ŵ�������ʶ������ */
	if (paj7620u2_init())
	{
		printf("[app_task_init] paj7620u2 init success\r\n");

		/* ��¼����ʶ��ģ����Ч�ԣ�1-������Ч 0-������Ч */
		g_gesture_valid = 1;

		/* ��ʾ����ʶ��ģ��ͼ�� */
		OLED_DrawBMP(0, 0, 64, 8, (uint8_t *)pic_gesture_icon);

		/* ��ʾ"���ӳɹ�" */
		OLED_ShowCHinese(64, 4, 22);
		OLED_ShowCHinese(90, 4, 23);
		OLED_ShowCHinese(64, 6, 30);
		OLED_ShowCHinese(90, 6, 31);

		/* ����2�� */
		vTaskDelay(2000);

		/* ����app_task_gesture���� */
		xTaskCreate((TaskFunction_t)app_task_gesture,		   /* ������ں��� */
					(const char *)"app_task_gesture",		   /* �������� */
					(uint16_t)512,							   /* ����ջ��С */
					(void *)NULL,							   /* ������ں������� */
					(UBaseType_t)5,							   /* ��������ȼ� */
					(TaskHandle_t *)&app_task_gesture_handle); /* ������ƿ�ָ�� */
	}

	/* mpu6050��ʼ�� */
	MPU_Init();

	mpu_dmp_init();

	/* eeprom��ʼ�� */
	at24c02_init();

#if IWDG_ENABLE
	/* �������Ź���ʼ�� */
	iwdg_init();
#endif

	/* ������̳�ʼ�� */
	kbd_init();

	/* �����õ������� */
	taskENTER_CRITICAL();
	i = 0;
	while (task_tbl[i].pxTaskCode)
	{
		xTaskCreate(task_tbl[i].pxTaskCode,		/* ������ں��� */
					task_tbl[i].pcName,			/* �������� */
					task_tbl[i].usStackDepth,	/* ����ջ��С */
					task_tbl[i].pvParameters,	/* ������ں������� */
					task_tbl[i].uxPriority,		/* ��������ȼ� */
					task_tbl[i].pxCreatedTask); /* ������ƿ�ָ�� */
		i++;
	}
	taskEXIT_CRITICAL();

	/* �������������ʱ�� */
	soft_timer_Handle = xTimerCreate((const char *)"AutoReloadTimer",
									 (TickType_t)1000,	  /* ��ʱ������ 1000(tick) */
									 (UBaseType_t)pdTRUE, /* ����ģʽ */
									 (void *)1,			  /* Ϊÿ����ʱ������һ��������ΨһID */
									 (TimerCallbackFunction_t)soft_timer_callback);
	/* �������������ʱ�� */
	xTimerStart(soft_timer_Handle, 0);
	
	/* ��ʾ�������� */								 
	OLED_Clear();	
									 
	for(i=0; i<16; i++)
	{
		OLED_ShowChar(i*8,3,'>',16);
		vTaskDelay(50);
	}									 
									 
	OLED_Clear();

	/* ��ʾ���˵� */
	menu_ext.menu = menu_main_tbl[0];
	menu_show(&menu_ext);

	/* ɾ���������� */
	vTaskDelete(NULL);

	printf("[app_task_init] nerver run here\r\n");
}
/**
  * @brief mpu6050����
  * @param pvParameters:��������ʱ���ݵĲ���
  * @retval ��
  */
static void app_task_mpu6050(void *pvParameters)
{
	uint8_t led_sta;
	uint8_t buf[32];
	uint32_t i;
	float pitch, roll, yaw; /* ŷ���� */
	BaseType_t xReturn = pdFALSE;
	oled_t oled;
	uint32_t roll_oled_direction_sta=0;

	dgb_printf_safe("[app_task_mpu6050] create success\r\n");
	

	for (;;)
	{
		vTaskDelay(200);

		/* ���⵱ǰ������;���𣬵���ʱ������ */
		if(g_cpu_operating_mode == CPU_STOP)
			continue;
		
		/* mpu6050Ҫ��ʱ�������Ի�ȡ��ʱ��100ms~200ms���ȡһ������ */
		if (0 == mpu_dmp_get_data(&pitch, &roll, &yaw))
		{
			if (g_mpu6050_get_what == FLAG_MPU6050_GET_EULER)
			{
				memset(buf, 0, sizeof buf);
				sprintf((char *)buf, "p%03d r%03d y%03d", (int32_t)pitch, (int32_t)roll, (int32_t)yaw);
					
				dgb_printf_safe("[app_task_mpu6050] %s\r\n",buf);	
				
				buf[16]=0;
				
				i = strlen((char *)buf);

				/* ���δ��ʾ���� */
				for (; i < 16; i++)
					buf[i] = ' ';

				oled.ctrl = OLED_CTRL_SHOW_STRING;
				oled.x = 0;
				oled.y = 5;
				oled.str = buf;
				oled.font_size = 12;

				xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
									 &oled,		   /* ���͵���Ϣ���� */
									 100);		   /* �ȴ�ʱ�� 100 Tick */
				if (xReturn != pdPASS)
					dgb_printf_safe("[app_task_mpu6050] xQueueSend oled string error code is %d\r\n", xReturn);
			}
			

			/* ����״̬��˼�룺��OLED������״̬�£�̧����ת��Ļ */
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
						
						/* ������Ļ�����0�㵽180�� */
						if(OLED_Get_Direction()==0)
							oled.direction=180;
						
						/* ������Ļ�����180�㵽0�� */
						if(OLED_Get_Direction()==180)
							oled.direction=0;						

						xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
											 &oled,		   /* ���͵���Ϣ���� */
											 100);		   /* �ȴ�ʱ�� 100 Tick */
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

			/* ͨ����յLED�Ʊ�ʾ����� */
			if (roll < 5)
				led_sta = 0x70;
			else if (roll < 15)
				led_sta = 0x71;
			else if (roll < 30)
				led_sta = 0x73;
			else
				led_sta = 0x77;

			xReturn = xQueueSend(g_queue_led, /* ��Ϣ���еľ�� */
								 &led_sta,	  /* ���͵���Ϣ���� */
								 100);		  /* �ȴ�ʱ�� 100 Tick */
			if (xReturn != pdPASS)
			{
				dgb_printf_safe("[app_task_mpu6050] xQueueSend led_sta error code is %d\r\n", xReturn);
				continue;
			}

			/* �����ȴ��ź���������ȷ��������ɶԵƵĿ��� */
			xReturn = xSemaphoreTake(g_sem_led, portMAX_DELAY);

			if (xReturn != pdPASS)
			{
				dgb_printf_safe("[app_task_mpu6050] xSemaphoreTake g_sem_led error code is %d\r\n", xReturn);
				continue;
			}

			/* ��OLED��Ϩ��״̬�£�̧�ֻ�����Ļ */
			if (roll > 15 && !g_oled_display_flag)
			{
				/* �����ٽ��� */
				taskENTER_CRITICAL();

				g_oled_display_flag = 1;

				/* ����ͳ��ϵͳ�޲�������ֵ */
				g_system_no_opreation_cnt = 0;

				/* �˳��ٽ��� */
				taskEXIT_CRITICAL();

				dgb_printf_safe("[app_task_mpu6050] roll > 15 , oled on\r\n");

				/* ����OLED�� */
				oled.ctrl = OLED_CTRL_DISPLAY_ON;

				xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
									 &oled,		   /* ���͵���Ϣ���� */
									 100);		   /* �ȴ�ʱ�� 100 Tick */
				if (xReturn != pdPASS)
					dgb_printf_safe("[app_task_mpu6050] xQueueSend oled error code is %d\r\n", xReturn);
				
				/* ���⻽�Ѻ��л���Ļ��ʾ���� */
				roll_oled_direction_sta=2;
			}
			

		}
	}
}

/**
 * @brief �����������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 */
void app_task_kbd(void* pvParameters)
{
	char key_val='N';
	beep_t beep;
	BaseType_t xReturn;
	dgb_printf_safe("[app_task_kbd] create success\r\n");
	
	while(1)
	{
		/* ��ȡ������̰���ֵ */
		key_val=kbd_read();

		if(key_val != 'N')
		{
			dgb_printf_safe("[app_task_kbd] kbd press %c \r\n",key_val);

			xReturn = xQueueSend(g_queue_kbd, 	/* ��Ϣ���еľ�� */
								&key_val,		/* ���͵���Ϣ���� */
								100);		   	/* �ȴ�ʱ�� 100 Tick */
			if (xReturn != pdPASS)
				dgb_printf_safe("[app_task_kbd] xQueueSend kbd error code is %d\r\n", xReturn);	

			/* ��һ��ʾ�� */
			beep.sta = 1;
			beep.duration = 20;

			xReturn = xQueueSend(g_queue_beep, /* ��Ϣ���еľ�� */
								&beep,		   /* ���͵���Ϣ���� */
								100);		   /* �ȴ�ʱ�� 100 Tick */				
			if (xReturn != pdPASS)
				dgb_printf_safe("[app_task_kbd] xQueueSend beep error code is %d\r\n", xReturn);

			/* [��ѡ]�����ȴ��ź���������ȷ��������ɶ�beep�Ŀ��� */
			xReturn = xSemaphoreTake(g_sem_beep, portMAX_DELAY);
			if (xReturn != pdPASS)
				dgb_printf_safe("[app_task_system_reset] xSemaphoreTake g_sem_beep error code is %d\r\n", xReturn);					

		}	
	}
}

/**
 * @brief ��������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
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
		/* �ȴ��¼����е���Ӧ�¼�λ����ͬ�� */
		EventValue = xEventGroupWaitBits((EventGroupHandle_t)g_event_group,
										 (EventBits_t)EVENT_GROUP_KEYALL_DOWN,
										 (BaseType_t)pdTRUE,
										 (BaseType_t)pdFALSE,
										 (TickType_t)portMAX_DELAY);

		/* ��ʱ���� */
		vTaskDelay(20);

		/* ����ͳ��ϵͳ�޲�������ֵ */
		taskENTER_CRITICAL();
		g_system_no_opreation_cnt = 0;
		taskEXIT_CRITICAL();

		if (EventValue & EVENT_GROUP_KEY1_DOWN)
		{
			/* ��ֹEXTI0�����ж� */
			NVIC_DisableIRQ(EXTI0_IRQn);

			/* ȷ���ǰ��� */
			if (PAin(0) == 0)
			{
				t = 0;

				/* �������� */
				while (PAin(0) == 0)
				{
					vTaskDelay(20);

					if (t++ >= 100)
						break;
				}

				if (t >= 100)
				{
					dgb_printf_safe("[app_task_key] S1 Press Continue\r\n");

					/* ʶ�𵽳�������˵�ѡ��0.25�������л� */
					while (PAin(0) == 0)
					{
						/* ����KEY_UP�����¼� */
						xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_UP);	

						vTaskDelay(250);
					}

				}
				else
				{
					dgb_printf_safe("[app_task_key] S1 Press\r\n");

					/* ����KEY_UP�����¼� */
					xReturn = xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_UP);
					if (xReturn != pdPASS)
						dgb_printf_safe("[app_task_key] xEventGroupSetBits EVENT_GROUP_FN_KEY_UP error code is %d\r\n", xReturn);	
				}
			}

			/* ����EXTI0�����ж� */
			NVIC_EnableIRQ(EXTI0_IRQn);
		}

		if (EventValue & EVENT_GROUP_KEY2_DOWN)
		{
			/* ��ֹEXTI2�����ж� */
			NVIC_DisableIRQ(EXTI2_IRQn);

			if (PEin(2) == 0)
			{
				dgb_printf_safe("[app_task_key] S2 Press\r\n");

				/* �������� */
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

					/* ʶ�𵽳�������˵�ѡ��0.25�������л� */
					while (PEin(2) == 0)
					{
						/* ����KEY_DOWN�����¼� */
						xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_DOWN);	

						vTaskDelay(250);
					}
				}

				/* ����KEY_DOWN�����¼� */
				xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_DOWN);
			}

			/* ����EXTI2�����ж� */
			NVIC_EnableIRQ(EXTI2_IRQn);
		}

		if (EventValue & EVENT_GROUP_KEY3_DOWN)
		{
			/* ��ֹEXTI3�����ж� */
			NVIC_DisableIRQ(EXTI3_IRQn);

			if (PEin(3) == 0)
			{
				dgb_printf_safe("[app_task_key] S3 Press\r\n");

				/* �������� */
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

					/* ����ϵͳ��λ�¼� */
					xReturn = xEventGroupSetBits(g_event_group, EVENT_GROUP_SYSTEM_RESET);
					if (xReturn != pdPASS)
						dgb_printf_safe("[app_task_key] xEventGroupSetBits EVENT_GROUP_SYSTEM_RESET error code is %d\r\n", xReturn);
				}
				else
				{
					dgb_printf_safe("[app_task_key] S3 Press\r\n");					

					/* ����KEY_ENTER�����¼� */
					xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_ENTER);
				}
			}

			/* ����EXTI3�����ж� */
			NVIC_EnableIRQ(EXTI3_IRQn);
		}

		if (EventValue & EVENT_GROUP_KEY4_DOWN)
		{
			/* ��ֹEXTI4�����ж� */
			NVIC_DisableIRQ(EXTI4_IRQn);

			if (PEin(4) == 0)
			{

				t = 0;
				/* �������������ƶ�OLED�������� */
				while (PEin(4) == 0)
				{
					vTaskDelay(20);

					if (t++ >= 100)
						break;
				}

				if (t >= 100)
				{

					dgb_printf_safe("[app_task_key] S4 Press Continue\r\n");

					/* �����ٽ��� */
					taskENTER_CRITICAL();

					g_oled_display_flag = !g_oled_display_flag;

					/* �˳��ٽ��� */
					taskEXIT_CRITICAL();
				
					/* OLED��/�������� */
					oled.ctrl = g_oled_display_flag ? OLED_CTRL_DISPLAY_ON : OLED_CTRL_DISPLAY_OFF;

					xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
										 &oled,		   /* ���͵���Ϣ���� */
										 100);		   /* �ȴ�ʱ�� 100 Tick */
					if (xReturn != pdPASS)
						dgb_printf_safe("[app_task_key] xQueueSend oled error code is %d\r\n", xReturn);
								
				}
				else
				{
					dgb_printf_safe("[app_task_key] S4 Press\r\n");

					/* ����KEY_BACK�����¼� */
					xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_BACK);
				}
			}

			/* ����EXTI4�����ж� */
			NVIC_EnableIRQ(EXTI4_IRQn);
		}
	}
}
/**
 * @brief adc����
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details ͨ���ɵ������ȡ�ĵ�ѹֵ������ת�̲˵��Ŀ���
 */
static void app_task_adc(void *pvParameters)
{
	int32_t adc_vol_last = 0;
	int32_t adc_vol = 0;
	int32_t result;

	dgb_printf_safe("[app_task_adc] create success and suspend self \r\n");

	/* ��ȡ��ǰ��ѹ��ֵ */
	adc_vol = adc_voltage_get();
	adc_vol_last = adc_vol;

	for (;;)
	{
		/* ��ȡ��ѹֵ */
		adc_vol = adc_voltage_get();

		result = adc_vol - adc_vol_last;

		if (result > 200 || result < -200)
		{
			/* ����ͳ��ϵͳ�޲�������ֵ */
			taskENTER_CRITICAL();
			g_system_no_opreation_cnt = 0;
			taskEXIT_CRITICAL();

			/* ����KEY_DOWN/KEY_UP�����¼� */
			xEventGroupSetBits(g_event_group, result > 0 ? EVENT_GROUP_FN_KEY_DOWN : EVENT_GROUP_FN_KEY_UP);

			dgb_printf_safe("[app_task_adc] adc_vol_last=%d adc_vol=%d\r\n", adc_vol_last, adc_vol);

			adc_vol_last = adc_vol;
		}

		vTaskDelay(100);
	}
}
/**
 * @brief ����ʶ������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details ͨ����ȡ����ֵ�������˵��Ŀ���
 */
static void app_task_gesture(void *pvParameters)
{
	EventBits_t EventValue;
	BaseType_t xReturn;
	oled_t oled;
	uint16_t gesture;
	uint32_t i = 0;

	/* ����ͼƬ�б�NULL��ʾ����ʾ������ */
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

	/* ��ǰ����ͼƬ��ʾ��λ�á���С */
	oled.x = 30;
	oled.y = 0;
	oled.pic_width = 64;
	oled.pic_height = 8;

	dgb_printf_safe("[app_task_gesture] create success\r\n");
	dgb_printf_safe("[app_task_gesture] pic_gesture_icon_tbl array size is %d \r\n", ARRAY_SIZE(pic_gesture_icon_tbl));

	for (;;)
	{
		/* �ȴ��¼����е���Ӧ�¼�λ����ͬ�� */
		EventValue = xEventGroupWaitBits((EventGroupHandle_t)g_event_group,
										 (EventBits_t)EVENT_GROUP_GESTURE,
										 (BaseType_t)pdTRUE,
										 (BaseType_t)pdFALSE,
										 (TickType_t)portMAX_DELAY);

		if (!(EventValue & EVENT_GROUP_GESTURE))
			continue;

		/* ����ͳ��ϵͳ�޲�������ֵ */
		taskENTER_CRITICAL();
		g_system_no_opreation_cnt = 0;
		taskEXIT_CRITICAL();

		/* ��ȡ���� */
		gesture = gesture_get();

		/* ��ȡ����ͼƬ */
		for (i = 0; i < ARRAY_SIZE(pic_gesture_icon_tbl); i++)
		{
			if (gesture & (1 << i))
				break;
		}

		/* ��ʾ����ͼƬ */
		if ((g_gesture_display_flag == FLAG_GESTURE_DISPLAY_ON) && pic_gesture_icon_tbl[i])
		{
			dgb_printf_safe("[app_task_gesture] gesture is %d \r\n", i);

			oled.ctrl = OLED_CTRL_SHOW_PICTURE;

			oled.pic = pic_gesture_icon_tbl[i];

			xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
								 &oled,		   /* ���͵���Ϣ���� */
								 100);		   /* �ȴ�ʱ�� 100 Tick */
			if (xReturn != pdPASS)
				dgb_printf_safe("[app_task_gesture] xQueueSend oled picture error code is %d\r\n", xReturn);
		}

		/* �������ƣ�ִ�ж�Ӧ�İ������� */
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
				/* ��ʾ��ʱ��ͼ����Զ��˳� */
				vTaskDelay(1000);
				g_gesture_display_flag = FLAG_GESTURE_DISPLAY_OFF;
			}

			/* ������ʾ����ʱ��ֻ����ʱ�봥�������˳� */
			xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_BACK);
		}
		break;

		default:
			break;
		}
	}
}
/**
 * @brief ��ʪ������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details �����񱻴�����Ĭ��Ϊ����״̬��
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

			xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
								 &oled,		   /* ���͵���Ϣ���� */
								 100);		   /* �ȴ�ʱ�� 100 Tick */
			if (xReturn != pdPASS)
				dgb_printf_safe("[app_task_dht] xQueueSend oled string error code is %d\r\n", xReturn);

			dgb_printf_safe("[app_task_dht] %s\r\n", buf);
		}

		vTaskDelay(6000);
	}
}
/**
 * @brief ��������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details ͨ��������Ϣ���л�ȡ��Ӧ�����ݣ�������������ʱ�䡢���ӡ��˵�����
 */
static void app_task_usart(void *pvParameters)
{
	struct usart_packet_t usart_packet = {0};
	oled_t oled;

	/* RTC_AlarmStructure����Ϊ��̬ */
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
		xReturn = xQueueReceive(g_queue_usart,	/* ��Ϣ���еľ�� */
								&usart_packet,	/* �õ�����Ϣ���� */
								portMAX_DELAY); /* �ȴ�ʱ��һֱ�� */
		if (xReturn != pdPASS)
		{
			dgb_printf_safe("[app_task_usart] xQueueReceive usart_packet error code is %d\r\n", xReturn);
			continue;
		}

		/* ����ͳ��ϵͳ�޲�������ֵ */
		taskENTER_CRITICAL();
		g_system_no_opreation_cnt = 0;
		taskEXIT_CRITICAL();


		/* �������� */
		if (strstr((char *)usart_packet.rx_buf, "DATE SET"))
		{
			/* �ԵȺŷָ��ַ��� */
			p = strtok((char *)usart_packet.rx_buf, "-");
			if (p == NULL)
				goto err_analyse_packet;

			//��ȡ��
			p = strtok(NULL, "-");
			if (p == NULL)
				goto err_analyse_packet;

			//2021-2000=21
			i = atoi(p) - 2000;

			//ת��Ϊ16���� 21 ->0x21
			i = (i / 10) * 16 + i % 10;
			RTC_DateStructure.RTC_Year = i;

			//��ȡ��
			p = strtok(NULL, "-");
			if (p == NULL)
				goto err_analyse_packet;

			i = atoi(p);
			//ת��Ϊ16����
			i = (i / 10) * 16 + i % 10;
			RTC_DateStructure.RTC_Month = i;

			//��ȡ��
			p = strtok(NULL, "-");
			if (p == NULL)
				goto err_analyse_packet;

			i = atoi(p);
			//ת��Ϊ16����
			i = (i / 10) * 16 + i % 10;
			RTC_DateStructure.RTC_Date = i;

			//��ȡ����
			p = strtok(NULL, "-");
			if (p == NULL)
				goto err_analyse_packet;

			i = atoi(p);
			//ת��Ϊ16����
			i = (i / 10) * 16 + i % 10;
			RTC_DateStructure.RTC_WeekDay = i;

			//��������
			RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);
		}
		/* ����ʱ�� */
		else if (strstr((char *)usart_packet.rx_buf, "TIME SET"))
		{
			/* �ԵȺŷָ��ַ��� */
			p = strtok((char *)usart_packet.rx_buf, "-");
			if (p == NULL)
				goto err_analyse_packet;

			/* ��ȡʱ */
			p = strtok(NULL, "-");
			if (p == NULL)
				goto err_analyse_packet;

			i = atoi(p);

			/* ͨ��ʱ���ж���AM����PM */
			if (i < 12)
				RTC_TimeStructure.RTC_H12 = RTC_H12_AM;
			else
				RTC_TimeStructure.RTC_H12 = RTC_H12_PM;

			/* ת��ΪBCD���� */
			i = (i / 10) * 16 + i % 10;
			RTC_TimeStructure.RTC_Hours = i;

			/* ��ȡ�� */
			p = strtok(NULL, "-");
			if (p == NULL)
				goto err_analyse_packet;

			i = atoi(p);

			/* ת��ΪBCD���� */
			i = (i / 10) * 16 + i % 10;
			RTC_TimeStructure.RTC_Minutes = i;

			/* ��ȡ�� */
			p = strtok(NULL, "-");
			if (p == NULL)
				goto err_analyse_packet;

			i = atoi(p);

			/* ת��ΪBCD���� */
			i = (i / 10) * 16 + i % 10;
			RTC_TimeStructure.RTC_Seconds = i;

			/* ����RTCʱ�� */
			RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);

			dgb_printf_safe("[app_task_usart] rtc set time ok\r\n");
		}
		else if(strstr((char *)usart_packet.rx_buf,"ALARM SET"))
		{
			//�ԵȺŷָ��ַ���
			p=strtok((char *)usart_packet.rx_buf,"-");
			if (p == NULL)
				goto err_analyse_packet;			
			
			//��ȡʱ
			p=strtok(NULL,"-");
			if (p == NULL)
				goto err_analyse_packet;

			i = atoi(p);
			
			//ͨ��ʱ���ж���AM����PM
			if(i<12)
				RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_AM;
			else
				RTC_AlarmStructure.RTC_AlarmTime.RTC_H12     = RTC_H12_PM;
				
			//ת��ΪBCD����
			i= (i/10)*16+i%10;
			RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours   = i;
			
			//��ȡ��
			p=strtok(NULL,"-");
			if (p == NULL)
				goto err_analyse_packet;

			i = atoi(p);						
			//ת��ΪBCD����
			i= (i/10)*16+i%10;	
			RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = i;
			
			//��ȡ��
			p=strtok(NULL,"-");
			if (p == NULL)
				goto err_analyse_packet;

			i = atoi(p);	
			
			//ת��ΪBCD����
			i= (i/10)*16+i%10;					
			RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = i; 	
			
			//ÿ�����Ӷ�����Ч
			//��������/���ڣ�Ҳ��˵����ÿ�춼����Ч
			RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;				

			//��������
			rtc_alarm_set(RTC_AlarmStructure); 

			/* ����Ҫ��ʾ��ͼƬ-����ͼ�� */
			oled.x=112;
			oled.y=0;
			oled.pic_width=16;
			oled.pic_height=2;
			oled.ctrl=OLED_CTRL_SHOW_PICTURE;
			oled.pic=pic_alarm_icon;

			xReturn = xQueueSend( 	g_queue_oled,	/* ��Ϣ���еľ�� */
									&oled,			/* ���͵���Ϣ���� */
									100);			/* �ȴ�ʱ�� 100 Tick */
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
					/* ����KEY_UP�����¼� */
					xReturn = xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_UP);

					dgb_printf_safe("[app_task_usart] '^' trigger KEY_UP Event\r\n");
				}break;

				case 'v':
				{
					/* ����KEY_DOWN�����¼� */
					xReturn = xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_DOWN);

					dgb_printf_safe("[app_task_usart] 'v' trigger KEY_Down Event\r\n");

				}break;	

				case '<':
				{
					/* ����KEY_LEFT�����¼� */
					xReturn = xEventGroupSetBits(g_event_group, EVENT_GROUP_FN_KEY_BACK);

					dgb_printf_safe("[app_task_usart] '<' trigger KEY_BACK Event\r\n");
				}break;	

				case '>':
				{
					/* ����KEY_LEFT�����¼� */
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
 * @brief rtc����
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details ��ʾ������ʱ��
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
		/* �ȴ��¼����е���Ӧ�¼�λ����ͬ�� */
		EventValue = xEventGroupWaitBits((EventGroupHandle_t)g_event_group,
										 (EventBits_t)EVENT_GROUP_RTC_WAKEUP,
										 (BaseType_t)pdTRUE,
										 (BaseType_t)pdFALSE,
										 (TickType_t)portMAX_DELAY);

		if (EventValue & EVENT_GROUP_RTC_WAKEUP)
		{
			if (g_rtc_get_what == FLAG_RTC_GET_TIME)
			{
				/* RTC_GetTime����ȡʱ�� */
				RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure);

				memset(buf, 0, sizeof buf);

				/* ��ʽ���ַ��� */
				sprintf((char *)buf, "%02x:%02x:%02x", RTC_TimeStructure.RTC_Hours,
						RTC_TimeStructure.RTC_Minutes,
						RTC_TimeStructure.RTC_Seconds);

				oled.x = 32;
			}

			if (g_rtc_get_what == FLAG_RTC_GET_DATE)
			{
				/* RTC_GetTime����ȡ���� */
				RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure);

				memset(buf, 0, sizeof buf);

				/* ��ʽ���ַ��� */
				sprintf((char *)buf, "20%02x-%02x-%02x-%d", RTC_DateStructure.RTC_Year,
						RTC_DateStructure.RTC_Month,
						RTC_DateStructure.RTC_Date,
						RTC_DateStructure.RTC_WeekDay);
				oled.x = 15;
			}

			/* oled��ʾʱ�� */
			oled.ctrl = OLED_CTRL_SHOW_STRING;
			oled.y = 5;
			oled.str = buf;
			oled.font_size = 16;

			xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
								 &oled,		   /* ���͵���Ϣ���� */
								 100);		   /* �ȴ�ʱ�� 100 Tick */
			if (xReturn != pdPASS)
				dgb_printf_safe("[app_task_rtc] xQueueSend oled string error code is %d\r\n", xReturn);

			dgb_printf_safe("[app_task_rtc] %s\r\n", buf);
		}
	}
}

/**
 * @brief rtc����/��������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details �����¼������CPU������ֹͣģʽ����ִ��ι��
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
		/* �ȴ��¼����е���Ӧ�¼�λ����ͬ�� */
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

				//������ӣ�����Ч��:���Ʒ�������ƹ�
				for(j=0; j<6; j++)
				{
					for(i=0; i<4; i++)
					{
						/* led */
						if(i%2==0)led_sta=0x11;
						else 	  led_sta=0x10;
							
						xReturn = xQueueSend(g_queue_led, /* ��Ϣ���еľ�� */
											&led_sta,	  /* ���͵���Ϣ���� */
											100);		  /* �ȴ�ʱ�� 100 Tick */
						if (xReturn != pdPASS)
						{
							dgb_printf_safe("[app_task_rtc_alarm] xQueueSend led_sta error code is %d\r\n", xReturn);
							continue;
						}

						/* ������ */
						if(i%2==0)beep.sta=0x1;
						else 	  beep.sta=0x0;	
						beep.duration = 0;
						xQueueSend(g_queue_beep, /* ��Ϣ���еľ�� */
								&beep,		 	 /* ���͵���Ϣ���� */
								100);		 	 /* �ȴ�ʱ�� 100 Tick */

						/* �����ȴ��ź���������ȷ��������ɶԵƵĿ��� */
						xReturn = xSemaphoreTake(g_sem_led, portMAX_DELAY);

						if (xReturn != pdPASS)
							dgb_printf_safe("[app_task_rtc_alarm] xSemaphoreTake g_sem_led error code is %d\r\n", xReturn);

						/* [��ѡ]�����ȴ��ź���������ȷ��������ɶ�beep�Ŀ��� */
						xReturn = xSemaphoreTake(g_sem_beep, portMAX_DELAY);
						if (xReturn != pdPASS)
							dgb_printf_safe("[app_task_rtc_alarm] xSemaphoreTake g_sem_beep error code is %d\r\n", xReturn);	
						vTaskDelay(80);
					}	
					vTaskDelay(600);
				}

				/* ������Ͻǵ�����Сͼ�� */
				oled.ctrl = OLED_CTRL_CLEAR_REGION;
				oled.x = 112;
				oled.y = 0;
				oled.clr_region_width=16;
				oled.clr_region_height=2;
				xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
									&oled,		   /* ���͵���Ϣ���� */
									100);		   /* �ȴ�ʱ�� 100 Tick */
				if (xReturn != pdPASS)
					dgb_printf_safe("[app_task_rtc_alarm] xQueueSend oled string error code is %d\r\n", xReturn);

			}
		}
	}
}
/**
 * @brief led����
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details ������Ϣ����������led
 */
static void app_task_led(void *pvParameters)
{
	uint8_t led_sta = 0;
	BaseType_t xReturn = pdFALSE;

	dgb_printf_safe("[app_task_led] create success\r\n");

	for (;;)
	{
		xReturn = xQueueReceive(g_queue_led,	/* ��Ϣ���еľ�� */
								&led_sta,		/* �õ�����Ϣ���� */
								portMAX_DELAY); /* �ȴ�ʱ��һֱ�� */
		if (xReturn != pdPASS)
			continue;

		/* ��⵽����LED1 */
		if (led_sta & 0x10)
			PFout(9) = led_sta & 0x01 ? 0 : 1;

		/* ��⵽����LED2 */
		if (led_sta & 0x20)
			PFout(10) = led_sta & 0x02 ? 0 : 1;

		/* ��⵽����LED3 */
		if (led_sta & 0x40)
			PEout(13) = led_sta & 0x04 ? 0 : 1;

		/* ��⵽����LED4 */
		if (led_sta & 0x80)
			PEout(14) = led_sta & 0x08 ? 0 : 1;

		/* �ͷ��ź��������߶Է�����ǰled���������Ѿ���� */
		xSemaphoreGive(g_sem_led);
	}
}
/**
 * @brief ����������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details ����Ϩ��״̬�����������С��������
 */
static void app_task_led_breath(void *pvParameters)
{
	int8_t brightness = 0;

	dgb_printf_safe("[app_task_led_breath] create success and suspend self\r\n");

	vTaskSuspend(NULL);

	dgb_printf_safe("[app_task_led_breath] resume success\r\n");

	for (;;)
	{
		/* ���� */
		for (brightness = 0; brightness <= 100; brightness++)
		{
			led_breath_brightness(brightness);
			vTaskDelay(20);
		}

		/* ���� */
		for (brightness = 100; brightness >= 0; brightness--)
		{
			led_breath_brightness(brightness);
			vTaskDelay(20);
		}
	}
}
/**
 * @brief ����������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details ������Ϣ���п��Ʒ�������
 */
static void app_task_beep(void *pvParameters)
{
	beep_t beep;
	BaseType_t xReturn = pdFALSE;

	dgb_printf_safe("[app_task_beep] create success\r\n");

	for (;;)
	{
		xReturn = xQueueReceive(g_queue_beep,	/* ��Ϣ���еľ�� */
								&beep,			/* �õ�����Ϣ���� */
								portMAX_DELAY); /* �ȴ�ʱ��һֱ�� */
		if (xReturn != pdPASS)
			continue;

		dgb_printf_safe("[app_task_beep] beep.sta=%d beep.duration=%d\r\n", beep.sta, beep.duration);

		/* ���������Ƿ���Ҫ�������� */
		if (beep.duration)
		{
			BEEP(beep.sta);

			while (beep.duration--)
				vTaskDelay(1);

			/* ������״̬��ת */
			beep.sta ^= 1;
		}

		BEEP(beep.sta);

		/* �ͷ��ź��������߶Է�����ǰbeep���������Ѿ���� */
		xSemaphoreGive(g_sem_beep);
	}
}
/**
 * @brief oled����
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details ������Ϣ���п���oled��ʾ��
 */
static void app_task_oled(void *pvParameters)
{
	oled_t oled;

	BaseType_t xReturn = pdFALSE;
	dgb_printf_safe("[app_task_oled] create success\r\n");

	for (;;)
	{
		xReturn = xQueueReceive(g_queue_oled,	/* ��Ϣ���еľ�� */
								&oled,			/* �õ�����Ϣ���� */
								portMAX_DELAY); /* �ȴ�ʱ��һֱ�� */
		if (xReturn != pdPASS)
			continue;

		switch (oled.ctrl)
		{
			case OLED_CTRL_DISPLAY_ON:
			{
				/* ���� */
				OLED_Display_On();

				/* ������ֹͣ���� */
				vTaskSuspend(app_task_led_breath_handle);
				led_breath_stop();
			}
			break;

			case OLED_CTRL_DISPLAY_OFF:
			{
				/* ���� */
				OLED_Display_Off();

				/* �����ƿ�ʼ���� */
				led_breath_run();
				vTaskResume(app_task_led_breath_handle);
			}
			break;
			
			case OLED_CTRL_DISPLAY_DIRECTION:
			{
				/* ������Ļ��ʾ���� */
				OLED_Set_Direction(oled.direction);
			}
			break;			
			

			case OLED_CTRL_CLEAR:
			{
				/* ���� */
				OLED_Clear();
			}
			break;

			case OLED_CTRL_CLEAR_REGION:
			{
				/* ���ָ����ʾ���� */
				OLED_ClearRegion(oled.x,
								oled.y,
								oled.x + oled.clr_region_width,
								oled.y + oled.clr_region_height);
			}
			break;		

			case OLED_CTRL_SHOW_STRING:
			{
				/* ��ʾ�ַ��� */
				OLED_ShowString(oled.x,
								oled.y,
								oled.str,
								oled.font_size);
			}
			break;

			case OLED_CTRL_SHOW_CHINESE:
			{

				/* ��ʾ���� */
				OLED_ShowCHinese(oled.x,
								oled.y,
								oled.chinese);
			}
			break;

			case OLED_CTRL_SHOW_PICTURE:
			{
				/* ��ʾͼƬ */
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
 * @brief oled��ʾ����
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details ����oled��ʾ����
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
			/* ����Ҫ��ʾ��ͼƬ */
			oled.x=0;
			oled.y=0;
			oled.pic_width=128;
			oled.pic_height=8;
			oled.ctrl=OLED_CTRL_SHOW_PICTURE;
			oled.pic=pic_astronaut_tbl[i];

			xReturn = xQueueSend( 	g_queue_oled,	/* ��Ϣ���еľ�� */
									&oled,			/* ���͵���Ϣ���� */
									100);			/* �ȴ�ʱ�� 100 Tick */
			if(xReturn != pdPASS)
				dgb_printf_safe("[app_task_oled_animation] xQueueSend oled picture error code is %d\r\n",xReturn);		


			vTaskDelay(100);
		
		}
	}
	
}
/**
 * @brief �˵���������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details ���հ����¼���ִ����Ӧ�˵���
 */
static void app_task_menu(void *pvParameters)
{
	uint32_t item_total;		//��¼��ǰ�˵��ж��ٸ���Ŀ
	uint32_t item_cursor = 0;	//��¼��ǰ�˵�ָ���ĸ���Ŀ
	uint32_t y = 0;
	uint32_t fun_run = 0;		//��¼�Ƿ�����Ŀ���ܺ�����ִ��

	EventBits_t EventValue;

	menu_ext_t m_ext;

	menu_t *m;

	/* ͨ���������ݻ�ȡ���˵�ҳָ�� */
	menu_t **m_main = (menu_t **)pvParameters;	

	/* ����m_extָ��ĵ�ǰ�˵�����ز��� */
	m_ext.menu = *m_main;						
	m_ext.key_fn = KEY_NONE;
	m_ext.item_cursor = item_cursor;
	m_ext.item_total = menu_item_total(m_ext.menu);

	/* ���ü�¼�˵�ָ��m */
	m = m_ext.menu;

	dgb_printf_safe("[app_task_menu] create success\r\n");

	for (;;)
	{
		/* �ȴ��¼����е���Ӧ�¼�λ����ͬ�� */
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

			/* ������Ŀ���ܺ��������У���ʾ��Ҫ���ز��ܽ�����Ŀѡ�� */
			if (fun_run)
			{
				dgb_printf_safe("[app_task_menu] menu fun is running,please press back\r\n");

				continue;
			}

			/* ����ϸ���Ŀ��� */
			item_cursor_show(5, y + item_cursor * 2, " ");

			/* ��Ŀ���λ��ֵ�Լ�1 */
			if (item_cursor)
			{
				item_cursor--;

				dgb_printf_safe("[app_task_menu] item_cursor-- is=%d\r\n", item_cursor);

				/* ��Ŀ�˵�ָ����һ���˵� */
				m--;
			}
			else
			{
				/* ��main�˵���ͬ�������˵� */
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

					/* ��ʾ�˵����� */
					menu_show(&m_ext);
				}
			}

			/* ��Ŀ���λ����ʾ > */
			item_cursor_show(5, y + item_cursor * 2, ">");

			dgb_printf_safe("[app_task_menu] item cursor=%d\r\n", item_cursor);
			dgb_printf_safe("[app_task_menu] item name %s\r\n", m->item);

			/* ���浱ǰ�������� */
			m_ext.key_fn = KEY_UP;

			/* ���浱ǰ��Ŀ���λ��ֵ */
			m_ext.item_cursor = item_cursor;
		}

		if (EventValue & EVENT_GROUP_FN_KEY_DOWN)
		{
			dgb_printf_safe("[app_task_menu] KEY_DOWN\r\n");

			/* ������Ŀ���ܺ��������У���ʾ��Ҫ���ز��ܽ�����Ŀѡ�� */
			if (fun_run)
			{
				dgb_printf_safe("[app_task_menu] menu fun is running,please press back\r\n");

				continue;
			}

			/* ��ȡ��ǰ�˵��ж��ٸ���Ŀ */
			item_total = menu_item_total(m_ext.menu);

			/* ���浱ǰ�˵��ж��ٸ���Ŀ */
			m_ext.item_total = item_total;

			dgb_printf_safe("[app_task_menu] item_total=%d\r\n", item_total);

			/* ����ϸ���Ŀ��� */
			item_cursor_show(5, y + item_cursor * 2, " ");

			if (item_cursor < (item_total - 1))
			{
				item_cursor++;

				dgb_printf_safe("[app_task_menu] item_cursor++ is=%d\r\n", item_cursor);

				/* ��Ŀ�˵�ָ����һ���˵� */
				m++;
			}
			else
			{
				/* ��main�˵���ͬ�����Ҳ�˵� */
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

					/* ��ʾ�˵����� */
					menu_show(&m_ext);
				}
			}

			/* ��Ŀ���λ����ʾ > */
			item_cursor_show(5, y + item_cursor * 2, ">");

			dgb_printf_safe("[app_task_menu] item cursor=%d\r\n", item_cursor);
			dgb_printf_safe("[app_task_menu] KEY_DOWN item name %s\r\n", m->item ? m->item : "NULL");

			/* ���浱ǰ�������� */
			m_ext.key_fn = KEY_DOWN;

			/* ���浱ǰ��Ŀ���λ��ֵ */
			m_ext.item_cursor = item_cursor;
		}

		if (EventValue & EVENT_GROUP_FN_KEY_ENTER)
		{
			m_ext.key_fn = KEY_ENTER;

			/* ������Ŀ���ܺ��������У���ʾ��Ҫ���ز��ܽ�����Ŀѡ�� */
			if (fun_run)
			{
				dgb_printf_safe("[app_task_menu] menu fun is running,please press back\r\n");

				continue;
			}

			dgb_printf_safe("[app_task_menu] KEY_ENTER item cursor=%d\r\n", item_cursor);
			dgb_printf_safe("[app_task_menu] KEY_ENTER item name %s\r\n", m->item ? m->item : "NULL");

			/* �Ӳ˵���Ч */
			if (m->child)
			{
				/* ָ���Ӳ˵� */
				m = m->child;

				/* ���浱ǰ�˵� */
				m_ext.menu = m;

				/* ��ʾ�˵����� */
				menu_show(&m_ext);

				/* ��λ���ֵ */
				item_cursor = 0;
			}

			/* ��û���Ӳ˵�,��ֱ��ִ�й��ܺ��� */
			if (!m->child && m->fun)
			{
				/* �������Ŀ���ܺ��������� */
				fun_run = 1;

				m->fun(&m_ext);
			}
		}

		if (EventValue & EVENT_GROUP_FN_KEY_BACK)
		{
			m_ext.key_fn = KEY_BACK;

			dgb_printf_safe("[app_task_menu] KEY_BACK item cursor=%d\r\n", item_cursor);
			dgb_printf_safe("[app_task_menu] KEY_BACK item name %s\r\n", m->item ? m->item : "NULL");

			/* ���Ӳ˵����ܺ�����Ч����ִ�У���Ҫ�ǹ����Ӧ���� */
			if (m->fun)
			{
				/* �������Ŀ���ܺ��������� */
				fun_run = 1;

				m->fun(&m_ext);
			}

			/* ���˵���Ч */
			if (m->parent)
			{
				/* ָ�򸸲˵� */
				m = m->parent;

				/* ���浱ǰ�˵� */
				m_ext.menu = m;

				/* ��λ���ֵ */
				item_cursor = 0;
				m_ext.item_cursor = 0;

				dgb_printf_safe("[app_task_menu] m->parent item cursor=%d\r\n", item_cursor);
				dgb_printf_safe("[app_task_menu] m->parent item name %s\r\n", m->item ? m->item : "NULL");

				fun_run = 0;

				/* ��ʾ��ǰ�˵� */
				menu_show(&m_ext);
			}
		}
	}
}

/**
 * @brief �������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details ������֤�������޸�
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
		/* �ȴ����������Ϣ */
		xReturn = xQueueReceive(g_queue_kbd,&key_val,portMAX_DELAY);

		if(xReturn!=pdPASS)
			continue;

		/* ����ͳ��ϵͳ�޲�������ֵ */
		taskENTER_CRITICAL();
		g_system_no_opreation_cnt = 0;
		taskEXIT_CRITICAL();			
		
		/* ��λ����������� */
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

		/* ������֤���� */
		if(g_pass_man_what == FLAG_PASS_MAN_AUTH)
		{
			/* û�����룬��Чɾ������ */
			if(key_val == '*' && key_cnt==0)
				continue;

			/* ɾ����һ������ */
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
				xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
									&oled,		   /* ���͵���Ϣ���� */
									100);		   /* �ȴ�ʱ�� 100 Tick */
				if (xReturn != pdPASS)
					dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);

				continue;				
			}

			if(key_cnt<6)
			{
				/* ��ʾ���� */
				key_buf[key_cnt] = key_val;
				
				oled.ctrl = OLED_CTRL_SHOW_STRING;
				oled.x = x;
				oled.y = y;
				oled.font_size = 16;

				oled.str =(uint8_t *) &key_buf[key_cnt];
				xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
									&oled,		   /* ���͵���Ϣ���� */
									100);		   /* �ȴ�ʱ�� 100 Tick */
				if (xReturn != pdPASS)
					dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);

				vTaskDelay(100);

				/* ��ʾ* */
				oled.str = "*";
				xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
									&oled,		   /* ���͵���Ϣ���� */
									100);		   /* �ȴ�ʱ�� 100 Tick */
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
					
					/* ��ȡeeprom�洢������ */
					at24c02_read(PASS1_ADDR_IN_EEPROM,(uint8_t *)pass_auth_eeprom,PASS_LEN);
					
					/* ������ */
					xor_decryption(XOR_KEY,pass_auth_eeprom,dec_buf,PASS_LEN);

					/* ƥ��eeprom�洢������ */
					rt = memcmp(dec_buf,key_buf,PASS_LEN);
					if(rt)
					{
						dgb_printf_safe("[app_task_pass_man] password auth by eeprom fail\r\n");
						
						/* ƥ��Ĭ������ */
						rt=memcmp(pass_auth_default,key_buf,PASS_LEN);
						if(rt)
							dgb_printf_safe("[app_task_pass_man] password auth by defalut fail too\r\n");
					}
					
					/* ����ƥ��ɹ� */
					if(rt==0)
					{
						dgb_printf_safe("[app_task_pass_man] password auth success\r\n");

						/* ����Ҫ��ʾ��ͼƬ-���飨�ɹ��� */
						oled.x=64;
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

						/* ��һ��ʾ��:��һ�� */
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

						vTaskDelay(100);

						/* ��һ��ʾ��:�ڶ��� */
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


					}
					else
					{
						/* ����Ҫ��ʾ��ͼƬ-���飨ʧ�ܣ� */
						oled.x=64;
						oled.y=0;
						oled.pic_width=64;
						oled.pic_height=8;
						oled.ctrl=OLED_CTRL_SHOW_PICTURE;
						oled.pic=pic_face_fail_icon;

						xReturn = xQueueSend( 	g_queue_oled,	/* ��Ϣ���еľ�� */
												&oled,			/* ���͵���Ϣ���� */
												100);			/* �ȴ�ʱ�� 100 Tick */
						if(xReturn != pdPASS)
							dgb_printf_safe("[app_task_pass_man] xQueueSend oled picture error code is %d\r\n",xReturn);

						/* ����1��ʾ�� */
						beep.sta = 1;
						beep.duration = 1000;

						xReturn = xQueueSend(g_queue_beep, /* ��Ϣ���еľ�� */
											&beep,		   /* ���͵���Ϣ���� */
											100);		   /* �ȴ�ʱ�� 100 Tick */				
						if (xReturn != pdPASS)
							dgb_printf_safe("[app_task_pass_man] xQueueSend beep error code is %d\r\n", xReturn);	

						/* [��ѡ]�����ȴ��ź���������ȷ��������ɶ�beep�Ŀ��� */
						xReturn = xSemaphoreTake(g_sem_beep, portMAX_DELAY);
						if (xReturn != pdPASS)
							dgb_printf_safe("[app_task_pass_man] xSemaphoreTake g_sem_beep error code is %d\r\n", xReturn);	

					}

					/* ��ʱһ�� */
					vTaskDelay(2000);

					/* �ָ���ֵ */
					x=x_start;
					key_cnt=0;
					memset(key_buf,0,sizeof key_buf);	

					/* ����Ұ벿������ */
					oled.ctrl = OLED_CTRL_CLEAR_REGION;
					oled.x = 64;
					oled.y = 0;
					oled.clr_region_width=64;
					oled.clr_region_height=8;
					xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
										&oled,		   /* ���͵���Ϣ���� */
										100);		   /* �ȴ�ʱ�� 100 Tick */
					if (xReturn != pdPASS)
						dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);


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
							dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);
					}

					/* ���* */
					oled.ctrl = OLED_CTRL_SHOW_STRING;
					oled.x = x_start;
					oled.y = 4;
					oled.str = "------";
					oled.font_size = 16;

					xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
										&oled,		   /* ���͵���Ϣ���� */
										100);		   /* �ȴ�ʱ�� 100 Tick */
					if (xReturn != pdPASS)
						dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);
				}
			}
		}

		/* �����޸Ĳ��� */
		if(g_pass_man_what == FLAG_PASS_MAN_MODIFY)
		{
			switch(pass_modify_sta)
			{
				case PASS_MODIFY_STA_OLD_PASS:
				{
					/* û�����룬��Чɾ������ */
					if(key_val == '*' && key_cnt==0)
						continue;

					/* ɾ����һ������ */
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
						xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
											&oled,		   /* ���͵���Ϣ���� */
											100);		   /* �ȴ�ʱ�� 100 Tick */
						if (xReturn != pdPASS)
							dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);

						continue;				
					}

					if(key_cnt<6)
					{
						/* ��ʾ���� */
						key_buf[key_cnt] = key_val;

						oled.ctrl = OLED_CTRL_SHOW_STRING;
						oled.x = x;
						oled.y = 2;
						oled.font_size = 16;

						oled.str =(uint8_t *) &key_buf[key_cnt];
						xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
											&oled,		   /* ���͵���Ϣ���� */
											100);		   /* �ȴ�ʱ�� 100 Tick */
						if (xReturn != pdPASS)
							dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);

						vTaskDelay(100);

						oled.str = "*";
						xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
											&oled,		   /* ���͵���Ϣ���� */
											100);		   /* �ȴ�ʱ�� 100 Tick */
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

					/* û�����룬��Чɾ������ */
					if(key_val == '*' && key_cnt==0)
						continue;

					/* ɾ����һ������ */
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
						xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
											&oled,		   /* ���͵���Ϣ���� */
											100);		   /* �ȴ�ʱ�� 100 Tick */
						if (xReturn != pdPASS)
							dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);

						continue;				
					}


					if(key_cnt<6)
					{
						/* ��ʾ'*' */
						key_buf[key_cnt] = key_val;

						oled.ctrl = OLED_CTRL_SHOW_STRING;
						oled.x = x;
						oled.y = 6;
						oled.font_size = 16;

						oled.str =(uint8_t *) &key_buf[key_cnt];
						xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
											&oled,		   /* ���͵���Ϣ���� */
											100);		   /* �ȴ�ʱ�� 100 Tick */
						if (xReturn != pdPASS)
							dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);

						vTaskDelay(100);

						oled.str = "*";
						xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
											&oled,		   /* ���͵���Ϣ���� */
											100);		   /* �ȴ�ʱ�� 100 Tick */
						if (xReturn != pdPASS)
							dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);

						x+=8;
						key_cnt++;
					}
					else
					{
						if(key_val == '#')
						{
							/* ���������� */
							memcpy(pass_new,key_buf,PASS_LEN);

							/* ��ȡeeprom�洢������ */
							at24c02_read(PASS1_ADDR_IN_EEPROM,(uint8_t *)pass_auth_eeprom,PASS_LEN);
							dgb_printf_safe("[app_task_pass_man] password eeprom is %6s\r\n",pass_auth_eeprom);
							
							/* ������ */							
							xor_decryption(XOR_KEY,pass_auth_eeprom,dec_buf,PASS_LEN);
							
							/* ƥ��eeprom�洢������ */
							rt = memcmp(dec_buf,pass_old,PASS_LEN);
							if(rt)
							{
								dgb_printf_safe("[app_task_pass_man] password old is %6s\r\n",pass_old);
								dgb_printf_safe("[app_task_pass_man] password auth by eeprom fail\r\n");
								
								/* ƥ��Ĭ������ */
								rt=memcmp(pass_auth_default,pass_old,PASS_LEN);
								dgb_printf_safe("[app_task_pass_man] password default is %6s\r\n",pass_auth_default);
								if(rt)
								{
									dgb_printf_safe("[app_task_pass_man] password auth by defalut fail too\r\n");
								}
									
							}
							
							/* ����ƥ��ɹ� */
							if(rt==0)
							{
								
								dgb_printf_safe("[app_task_pass_man] password auth success\r\n");
								
								/* �����ٽ������ɿ�����eeprom������д�������� */
								taskENTER_CRITICAL();
								
								xor_encryption(XOR_KEY,pass_new,enc_buf,PASS_LEN);
								
								rt = at24c02_write(PASS1_ADDR_IN_EEPROM,(uint8_t *)enc_buf,PASS_LEN);
								
								taskEXIT_CRITICAL();

								if(rt == 0)
									dgb_printf_safe("[app_task_pass_man] password new is %6s,write into eeprom success\r\n",pass_new);
								else
									dgb_printf_safe("[app_task_pass_man] password new is %6s,write into eeprom fail,error code is %d\r\n",pass_new,rt);

								/* ����Ҫ��ʾ��ͼƬ-���飨ʧ�ܣ� */
								oled.x=64;
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
								
								
								/* ��һ��ʾ��:��һ�� */
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

								vTaskDelay(100);

								/* ��һ��ʾ��:�ڶ��� */
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
							}
							else
							{
								/* ����Ҫ��ʾ��ͼƬ-���飨ʧ�ܣ� */
								oled.x=64;
								oled.y=0;
								oled.pic_width=64;
								oled.pic_height=8;
								oled.ctrl=OLED_CTRL_SHOW_PICTURE;
								oled.pic=pic_face_fail_icon;

								xReturn = xQueueSend( 	g_queue_oled,	/* ��Ϣ���еľ�� */
														&oled,			/* ���͵���Ϣ���� */
														100);			/* �ȴ�ʱ�� 100 Tick */
								if(xReturn != pdPASS)
									dgb_printf_safe("[app_task_pass_man] xQueueSend oled picture error code is %d\r\n",xReturn);


								/* ����1��ʾ�� */
								beep.sta = 1;
								beep.duration = 1000;

								xReturn = xQueueSend(g_queue_beep, /* ��Ϣ���еľ�� */
													&beep,		   /* ���͵���Ϣ���� */
													100);		   /* �ȴ�ʱ�� 100 Tick */				
								if (xReturn != pdPASS)
									dgb_printf_safe("[app_task_pass_man] xQueueSend beep error code is %d\r\n", xReturn);	

								/* [��ѡ]�����ȴ��ź���������ȷ��������ɶ�beep�Ŀ��� */
								xReturn = xSemaphoreTake(g_sem_beep, portMAX_DELAY);
								if (xReturn != pdPASS)
									dgb_printf_safe("[app_task_pass_man] xSemaphoreTake g_sem_beep error code is %d\r\n", xReturn);											


							}

							/* ��ʱһ�� */
							vTaskDelay(2000);	


							/* ����Ұ벿������ */
							oled.ctrl = OLED_CTRL_CLEAR_REGION;
							oled.x = 64;
							oled.y = 0;
							oled.clr_region_width=64;
							oled.clr_region_height=8;
							xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
												&oled,		   /* ���͵���Ϣ���� */
												100);		   /* �ȴ�ʱ�� 100 Tick */
							if (xReturn != pdPASS)
								dgb_printf_safe("[app_task_pass_man] xQueueSend oled string error code is %d\r\n", xReturn);


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

							/* �ָ���ֵ */
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
 * @brief ϵͳ��λ����
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details �ؼ�����NVIC_SystemReset����
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
		/* �ȴ��¼����е���Ӧ�¼�λ����ͬ�� */
		EventValue = xEventGroupWaitBits((EventGroupHandle_t)g_event_group,
										 (EventBits_t)EVENT_GROUP_SYSTEM_RESET,
										 (BaseType_t)pdTRUE,
										 (BaseType_t)pdFALSE,
										 (TickType_t)portMAX_DELAY);

		if (EventValue & EVENT_GROUP_SYSTEM_RESET)
		{
			/* �����漰�õ�OLED������ */
			//vTaskSuspend(app_task_key_handle);
			vTaskSuspend(app_task_mpu6050_handle);
			vTaskSuspend(app_task_dht_handle);
			vTaskSuspend(app_task_rtc_handle);

			if (g_gesture_valid)
				vTaskSuspend(app_task_gesture_handle);

			dgb_printf_safe("[app_task_system_reset] xQueueSend oled clear\r\n");

			/* OLED���� */
			oled.ctrl = OLED_CTRL_CLEAR;
			xQueueSend(g_queue_oled, &oled, 100);

			/* ��ʾ"����ִ�и�λ" */
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
				/* ��ʾ���� */
				oled.x = x;
				oled.str = ">";
				xQueueSend(g_queue_oled, &oled, 100);

				vTaskDelay(100);

				x += 8;

				if (x >= 120)
				{
					/* ��һ��ʾ�� */
					beep.sta = 1;
					beep.duration = 100;

					xQueueSend(g_queue_beep, /* ��Ϣ���еľ�� */
							   &beep,		 /* ���͵���Ϣ���� */
							   100);		 /* �ȴ�ʱ�� 100 Tick */

					/* [��ѡ]�����ȴ��ź���������ȷ��������ɶ�beep�Ŀ��� */
					xReturn = xSemaphoreTake(g_sem_beep, portMAX_DELAY);
					if (xReturn != pdPASS)
						dgb_printf_safe("[app_task_system_reset] xSemaphoreTake g_sem_beep error code is %d\r\n", xReturn);

					/* OLED���� */
					oled.ctrl = OLED_CTRL_CLEAR;
					xQueueSend(g_queue_oled, &oled, 100);

					/* ϵͳ��λ */
					NVIC_SystemReset();
				}
			}
		}
	}
}
/**
 * @brief �����ʱ������
 * @param pvParameters:��������ʱ���ݵĲ���
 * @retval ��
 * @details ���������������Ź�ι��
 */
static void soft_timer_callback(TimerHandle_t pxTimer)
{
static 
	uint32_t count_backwards_sta=0;
	oled_t oled;
	BaseType_t xReturn;
	uint8_t buf[16] = {0};
	int32_t timer_id = (int32_t)pvTimerGetTimerID(pxTimer);

	/* ����״̬���� �� ͳ��ϵͳ�޲�������ֵΪ0 �� g_system_no_opreation_cnt�ﵽϨ��ʱ��ֵ */
	if (count_backwards_sta && (g_system_no_opreation_cnt == 0 || g_system_no_opreation_cnt >= SCREEN_OFF_TIME))
	{
		/* ��ռ�¼����״̬ */
		count_backwards_sta = 0;

		/* oled��ʾ�ַ��� */
		/* ������Ͻǵĵ���ֵ*/
		oled.ctrl = OLED_CTRL_SHOW_STRING;
		oled.x = 120;
		oled.y = 0;

		memset(buf, 0, sizeof buf);
		sprintf((char *)buf, "%d", SCREEN_OFF_TIME - g_system_no_opreation_cnt);

		oled.str = " ";
		oled.font_size = 12;

		xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
							 &oled,		   /* ���͵���Ϣ���� */
							 100);		   /* �ȴ�ʱ�� 100 Tick */
		if (xReturn != pdPASS)
			dgb_printf_safe("[soft_timer_callback id=%d] xQueueSend oled string error code is %d\r\n", timer_id, xReturn);
	}

	/* ��ʼ����,3 2 1 */
	if ((g_system_no_opreation_cnt >= SCREEN_OFF_TIME - 3) && (g_system_no_opreation_cnt < SCREEN_OFF_TIME))
	{
		dgb_printf_safe("[soft_timer_callback id=%d] g_system_no_opreation_cnt is %d\r\n", timer_id, g_system_no_opreation_cnt);

		/* ��¼����״̬ */
		count_backwards_sta=1;

		/* oled��ʾ�ַ��� */
		oled.ctrl = OLED_CTRL_SHOW_STRING;
		oled.x = 120;
		oled.y = 0;

		memset(buf, 0, sizeof buf);
		sprintf((char *)buf, "%d", SCREEN_OFF_TIME - g_system_no_opreation_cnt);

		oled.str = buf;
		oled.font_size = 12;

		xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
							 &oled,		   /* ���͵���Ϣ���� */
							 100);		   /* �ȴ�ʱ�� 100 Tick */
		if (xReturn != pdPASS)
			dgb_printf_safe("[soft_timer_callback id=%d] xQueueSend oled string error code is %d\r\n", timer_id, xReturn);
	}

	/* g_system_no_opreation_cnt�ﵽϨ��ʱ��ֵ����ִ��OLEDϨ��*/
	if (g_oled_display_flag && g_system_no_opreation_cnt >= SCREEN_OFF_TIME)
	{
		g_oled_display_flag = 0;

		/* OLEDϨ������ */
		oled.ctrl = OLED_CTRL_DISPLAY_OFF;

		xReturn = xQueueSend(g_queue_oled, /* ��Ϣ���еľ�� */
							 &oled,		   /* ���͵���Ϣ���� */
							 100);		   /* �ȴ�ʱ�� 100 Tick */
		if (xReturn != pdPASS)
			dgb_printf_safe("[soft_timer_callback id=%d] xQueueSend oled error code is %d\r\n", timer_id, xReturn);
	}

	/* �����ٽ�����FreeRTOSͬʱ��ر��жϣ�UCOS��OS_CRITICAL_ENTER��ֹͣ������ȣ� */
	taskENTER_CRITICAL();

#if IWDG_ENABLE
	/* ι����ˢ���������ֵ */
	IWDG_ReloadCounter();
#endif
	/* ͳ��ϵͳ�޲�������ֵ�Լ�1 */
	g_system_no_opreation_cnt++;

	/* �˳��ٽ�����FreeRTOSͬʱ����жϣ�UCOS��OS_CRITICAL_EXTI��ָ�������ȣ� */
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

	/* ����˯��ģʽ */
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
