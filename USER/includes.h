/* 作者:粤嵌.温子祺 */
#ifndef __INCLUDES_H__
#define __INCLUDES_H__



/* 标准C库*/
#include <stdio.h>	
#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <ctype.h>

/* FreeRTOS */
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
#include "stack_macros.h"
#include "semphr.h"
#include "event_groups.h"

/* 外设相关 */
#include "sys.h"
#include "global.h"
#include "delay.h"
#include "usart.h"
#include "led.h"
#include "dht11.h"
#include "key.h"
#include "adc.h"
#include "mpu6050.h"
#include "inv_mpu.h"
#include "inv_mpu_dmp_motion_driver.h"
#include "dmpKey.h"
#include "dmpmap.h"
#include "rtc.h"
#include "iwdg.h" 
#include "beep.h"
#include "oled.h"
#include "oledfont.h"  	
#include "bmp.h"
#include "paj7620u2_iic.h"
#include "paj7620u2.h"
#include "paj7620u2_cfg.h"
#include "ble.h"
#include "keyboard.h"
#include "eeprom.h"

/* 类型 */
typedef struct __oled_t
{

#define OLED_CTRL_DISPLAY_ON        0x01
#define OLED_CTRL_DISPLAY_OFF       0x02
#define OLED_CTRL_INIT              0x03
#define OLED_CTRL_CLEAR             0x04
#define OLED_CTRL_CLEAR_REGION      0x05
#define OLED_CTRL_SHOW_STRING       0x06
#define OLED_CTRL_SHOW_CHINESE      0x07
#define OLED_CTRL_SHOW_PICTURE      0x08
	
#define OLED_CTRL_DISPLAY_DIRECTION     0x09

	uint8_t ctrl;
	uint8_t x;
	uint8_t y;

	uint8_t *str;
	uint8_t font_size;
    uint8_t chinese;

	uint8_t clr_region_width;
	uint8_t clr_region_height;	
	
	const uint8_t *pic;
	uint8_t pic_width;
	uint8_t pic_height;
	
	uint8_t direction;
	
	
}oled_t;

typedef enum __key_fn_t
{
	KEY_NONE=0,	
	KEY_UP,
	KEY_DOWN,
	KEY_ENTER,
	KEY_BACK,
}key_fn_t;

typedef enum __cpu_operating_mode_t
{
	CPU_RUN=0,	
	CPU_SLEEP,
	CPU_STOP,
	CPU_STANDBY,
}cpu_operating_mode_t;

typedef struct __beep_t
{
	uint32_t sta;				//1-工作 0-停止
	uint32_t duration;			//持续时间，单位毫秒
}beep_t;

typedef struct __task_t
{
	TaskFunction_t pxTaskCode;
	const char * const pcName;		
	const configSTACK_DEPTH_TYPE usStackDepth;
	void * const pvParameters;
	UBaseType_t uxPriority;
	TaskHandle_t * const pxCreatedTask;
}task_t;

#include "menu.h"

/* 宏定义 */
#define EVENT_GROUP_KEY1_DOWN		0x01
#define EVENT_GROUP_KEY2_DOWN		0x02
#define EVENT_GROUP_KEY3_DOWN		0x04
#define EVENT_GROUP_KEY4_DOWN		0x08
#define EVENT_GROUP_KEYALL_DOWN		0x0F

#define EVENT_GROUP_RTC_WAKEUP		0x10
#define EVENT_GROUP_RTC_ALARM		0x20

#define EVENT_GROUP_OLED_ON			0x100
#define EVENT_GROUP_OLED_OFF		0x200

#define EVENT_GROUP_SYSTEM_RESET	0x1000


#define EVENT_GROUP_FN_KEY_UP		0x10000
#define EVENT_GROUP_FN_KEY_DOWN		0x20000
#define EVENT_GROUP_FN_KEY_ENTER	0x40000
#define EVENT_GROUP_FN_KEY_BACK		0x80000

#define EVENT_GROUP_GESTURE			0x100000


#define QUEUE_USART_LEN    			4   	/* 队列的长度，最大可包含多少个消息 */
#define QUEUE_LED_LEN    			4   	/* 队列的长度，最大可包含多少个消息 */
#define QUEUE_BEEP_LEN    			4   	/* 队列的长度，最大可包含多少个消息 */
#define QUEUE_OLED_LEN    			16   	/* 队列的长度，最大可包含多少个消息 */
#define QUEUE_KBD_LEN    			4   	/* 队列的长度，最大可包含多少个消息 */


#define FLAG_DHT_GET_NONE			0
#define FLAG_DHT_GET_TEMP			1
#define FLAG_DHT_GET_HUMI			2

#define FLAG_RTC_GET_NONE			0
#define FLAG_RTC_GET_DATE			1
#define FLAG_RTC_GET_TIME			2

#define FLAG_MPU6050_GET_NONE		0
#define FLAG_MPU6050_GET_EULER		1

#define FLAG_GESTURE_DISPLAY_OFF	0
#define FLAG_GESTURE_DISPLAY_ON		1

#define FLAG_PASS_MAN_NONE			0
#define FLAG_PASS_MAN_AUTH			1
#define FLAG_PASS_MAN_MODIFY		2

#define ARRAY_SIZE(x)	(sizeof(x) / sizeof(x[0]))

#define SCREEN_OFF_TIME				30



/* 互斥型信号量句柄 */
extern SemaphoreHandle_t g_mutex_printf;
extern SemaphoreHandle_t g_mutex_oled;

/* 事件标志组句柄 */
extern EventGroupHandle_t g_event_group;	

/* 消息队列句柄 */
extern QueueHandle_t 	g_queue_usart;
extern QueueHandle_t 	g_queue_led;
extern QueueHandle_t 	g_queue_beep;
extern QueueHandle_t 	g_queue_oled;
extern QueueHandle_t 	g_queue_kbd;

/* 计数型信号量 */
extern SemaphoreHandle_t g_sem_led;
extern SemaphoreHandle_t g_sem_beep;

/* 任务句柄 */ 
extern TaskHandle_t app_task_init_handle		;
extern TaskHandle_t app_task_mpu6050_handle 	;
extern TaskHandle_t app_task_key_handle			;
extern TaskHandle_t app_task_dht_handle 		;
extern TaskHandle_t app_task_usart_handle 		;
extern TaskHandle_t app_task_rtc_handle 		;
extern TaskHandle_t app_task_rtc_alarm_handle 	;
extern TaskHandle_t app_task_led_handle 		;
extern TaskHandle_t app_task_beep_handle 		;
extern TaskHandle_t app_task_gesture_handle 	;
extern TaskHandle_t app_task_oled_handle		;
extern TaskHandle_t app_task_oled_animation_handle;
extern TaskHandle_t app_task_adc_handle			;
extern TaskHandle_t app_task_menu_handle		;
extern TaskHandle_t app_task_system_reset_handle;
extern TaskHandle_t app_task_led_breath_handle	;
extern TaskHandle_t app_task_kbd_handle			;




/* 变量 */
extern volatile uint32_t g_oled_display_flag;
extern volatile uint32_t g_dht_get_what;
extern volatile uint32_t g_rtc_get_what;
extern volatile uint32_t g_mpu6050_get_what;
extern volatile uint32_t g_gesture_display_flag;	
extern volatile uint32_t g_gesture_valid;								
extern volatile uint32_t g_system_no_opreation_cnt;		
extern volatile uint32_t g_pass_man_what;
extern volatile cpu_operating_mode_t g_cpu_operating_mode;

/* 函数 */
extern void dgb_printf_safe(const char *format, ...);

#endif

