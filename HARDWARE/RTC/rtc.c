#include "includes.h"


static EXTI_InitTypeDef	EXTI_InitStructure;
static NVIC_InitTypeDef	NVIC_InitStructure;

static RTC_TimeTypeDef  RTC_TimeStructure;
static RTC_DateTypeDef  RTC_DateStructure;
static RTC_InitTypeDef  RTC_InitStructure;
static RTC_AlarmTypeDef RTC_AlarmStructure;

void rtc_init(void)
{
	
	/* Enable the PWR clock��ʹ�ܵ�Դ����ʱ�� */
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
	
	/* Allow access to RTC���������RTC */
	PWR_BackupAccessCmd(ENABLE);

#if RTC_CLK_SRC_SEL == RTC_CLK_SRC_IS_LSE
	//��LSE��ʱ��
	RCC_LSEConfig(RCC_LSE_ON);

	//���LSE��ʱ���Ƿ����
	while(RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET);
	
	//ΪRTCѡ��LSE��Ϊʱ��Դ
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
	
#endif

#if RTC_CLK_SRC_SEL == RTC_CLK_SRC_IS_LSI	
	RCC_LSICmd(ENABLE);
	
	/* Wait till LSI is ready */  
	while(RCC_GetFlagStatus(RCC_FLAG_LSIRDY) == RESET)
	{
	}
	
	/* Select the RTC Clock Source */
	RCC_RTCCLKConfig(RCC_RTCCLKSource_LSI);
#endif
	
	/* Enable the RTC Clock��ʹ��RTC��Ӳ��ʱ�� */
	RCC_RTCCLKCmd(ENABLE);
	
	/* Wait for RTC APB registers synchronisation���ȴ�����RTC��ؼĴ������� */
	RTC_WaitForSynchro();
	
#if RTC_CLK_SRC_SEL == RTC_CLK_SRC_IS_LSE
	/* Configure the RTC data register and RTC prescaler������RTC���ݼĴ�����RTC�ķ�Ƶֵ */
	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;				//�첽��Ƶϵ��
	RTC_InitStructure.RTC_SynchPrediv = 0xFF;				//ͬ����Ƶϵ��
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;	//24Сʱ��ʽ
	RTC_Init(&RTC_InitStructure);
#endif

#if RTC_CLK_SRC_SEL == RTC_CLK_SRC_IS_LSI	
	/* Configure the RTC data register and RTC prescaler������RTC���ݼĴ�����RTC�ķ�Ƶֵ */
	RTC_InitStructure.RTC_AsynchPrediv = 0x7F;				//�첽��Ƶϵ��
	RTC_InitStructure.RTC_SynchPrediv = 0xF9;				//ͬ����Ƶϵ��
	RTC_InitStructure.RTC_HourFormat = RTC_HourFormat_24;	//24Сʱ��ʽ
	RTC_Init(&RTC_InitStructure);

#endif
	
	//��������
	RTC_DateStructure.RTC_Year = 0x21;				//2021��
	RTC_DateStructure.RTC_Month = RTC_Month_July;	//7�·�
	RTC_DateStructure.RTC_Date = 0x21;				//��21��/21��/21��
	RTC_DateStructure.RTC_WeekDay = RTC_Weekday_Wednesday;//������
	RTC_SetDate(RTC_Format_BCD, &RTC_DateStructure);


	//����ʱ�� 17:14:50
	RTC_TimeStructure.RTC_H12     = RTC_H12_PM;
	RTC_TimeStructure.RTC_Hours   = 0x17;
	RTC_TimeStructure.RTC_Minutes = 0x14;
	RTC_TimeStructure.RTC_Seconds = 0x00; 
	RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructure);  
	
	RTC_WakeUpCmd(DISABLE);
	
	//����ʱ��Դ��Ӳ��ʱ��Ƶ��Ϊ1Hz
	RTC_WakeUpClockConfig(RTC_WakeUpClock_CK_SPRE_16bits);
	
	//ֻ����һ�μ���
	RTC_SetWakeUpCounter(0);
	
	RTC_WakeUpCmd(ENABLE);

	//�����жϵĴ�����ʽ�������ж�
	RTC_ITConfig(RTC_IT_WUT, ENABLE);
	
	RTC_ClearFlag(RTC_FLAG_WUTF);
	
	EXTI_ClearITPendingBit(EXTI_Line22);
	EXTI_InitStructure.EXTI_Line = EXTI_Line22;			//
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;//RTC�ֲ�涨
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);

	//���ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = RTC_WKUP_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 5;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);
}


void rtc_alarm_init(void)
{
	/* Enable RTC Alarm A Interrupt������RTC��A���Ӵ����ж� */
	RTC_ITConfig(RTC_IT_ALRA, ENABLE);
	
	/* ��ձ�־λ */
	RTC_ClearFlag(RTC_FLAG_ALRAF);

	/*ʹ���ⲿ�жϿ�����17���ж�*/
	EXTI_ClearITPendingBit(EXTI_Line17);
	EXTI_InitStructure.EXTI_Line = EXTI_Line17;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	
	/*ʹ�����ӵ��ж� */
	NVIC_InitStructure.NVIC_IRQChannel = RTC_Alarm_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);


	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
}

void rtc_alarm_set(RTC_AlarmTypeDef rtc_alarm_structure)
{
	/* �ر����ӣ������رգ��������Ӵ������ж���BUG��������ô���ã�ֻҪ��00�룬�򴥷��ж�*/
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);
	
	/* Configure the RTC Alarm A register������RTC��A���� */
	RTC_SetAlarm(RTC_Format_BCD, RTC_Alarm_A, &rtc_alarm_structure);
	
	/* Enable the alarm ����RTC������A����*/
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
}


/* second��Χ:1~59 */
void rtc_alarm_set_in_second(uint32_t second)
{

	/* �ر����ӣ������رգ��������Ӵ������ж���BUG��������ô���ã�ֻҪ��00�룬�򴥷��ж�*/
	RTC_AlarmCmd(RTC_Alarm_A, DISABLE);

	RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructure);  
	
	
	RTC_AlarmStructure.RTC_AlarmTime.RTC_H12   = RTC_TimeStructure.RTC_H12;

	if(RTC_TimeStructure.RTC_Seconds+second>=59)
	{
		RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = RTC_TimeStructure.RTC_Seconds+second-59;
		RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = RTC_TimeStructure.RTC_Minutes+1;

	}
	else
	{
		RTC_AlarmStructure.RTC_AlarmTime.RTC_Seconds = RTC_TimeStructure.RTC_Seconds+second;
		RTC_AlarmStructure.RTC_AlarmTime.RTC_Minutes = RTC_TimeStructure.RTC_Minutes;
	}

	RTC_AlarmStructure.RTC_AlarmTime.RTC_Hours = RTC_TimeStructure.RTC_Hours;
	
	RTC_AlarmStructure.RTC_AlarmMask = RTC_AlarmMask_DateWeekDay;

	/* Configure the RTC Alarm A register������RTC��A���� */
	RTC_SetAlarm(RTC_Format_BIN, RTC_Alarm_A, &RTC_AlarmStructure);
	
	/* Enable the alarm ����RTC������A����*/
	RTC_AlarmCmd(RTC_Alarm_A, ENABLE);
}

void  RTC_WKUP_IRQHandler(void)
{
	uint32_t ulReturn;

	BaseType_t  xHigherPriorityTaskWoken = pdFALSE;	

	/* �����ٽ�Σ��ٽ�ο���Ƕ�� */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();	
	
	//����־λ
	if(RTC_GetITStatus(RTC_IT_WUT) == SET)
	{
		
		//�����¼���־��
		xEventGroupSetBitsFromISR(g_event_group,EVENT_GROUP_RTC_WAKEUP,&xHigherPriorityTaskWoken);	
		
		//��ձ�־λ	
		RTC_ClearITPendingBit(RTC_IT_WUT);
		
		EXTI_ClearITPendingBit(EXTI_Line22);
	}

	/* ��������Ϣ������������ȼ����ڵ�ǰ���е��������˳��жϺ��������������л���ִ��ǰ��;
	   ����ȴ���һ��ʱ�ӽ��ĲŽ��������л�
	*/
	if(xHigherPriorityTaskWoken)
	{
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}	
	
	/* �˳��ٽ�� */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );			
}


void RTC_Alarm_IRQHandler(void)
{
	uint32_t ulReturn;

	BaseType_t  xHigherPriorityTaskWoken = pdFALSE;	

	/* �����ٽ�Σ��ٽ�ο���Ƕ�� */
	ulReturn = taskENTER_CRITICAL_FROM_ISR();	
		
	if(RTC_GetITStatus(RTC_IT_ALRA) == SET)
	{
		//�����¼���־��
		if(g_cpu_operating_mode != CPU_STOP)
			xEventGroupSetBitsFromISR(g_event_group,EVENT_GROUP_RTC_ALARM,&xHigherPriorityTaskWoken);			
		
		RTC_ClearITPendingBit(RTC_IT_ALRA);
		EXTI_ClearITPendingBit(EXTI_Line17);
	} 

	/* ��������Ϣ������������ȼ����ڵ�ǰ���е��������˳��жϺ��������������л���ִ��ǰ��;
	   ����ȴ���һ��ʱ�ӽ��ĲŽ��������л�
	*/
	if(xHigherPriorityTaskWoken)
	{
		portYIELD_FROM_ISR( xHigherPriorityTaskWoken );
	}	
	
	/* �˳��ٽ�� */
	taskEXIT_CRITICAL_FROM_ISR( ulReturn );	

}
