/*
 * rtc_interface.c
 *
 * Created: 4/16/2020 8:02:20 PM
 *  Author: Jake
 */ 

 #include "asf.h"

 #include "rtc_interface.h"

 static struct rtc_module module;

 static void rtc_callback(void)
 {
	// TODO do anything else?
	rtc_count_disable_callback(&module, RTC_COUNT_CALLBACK_COMPARE_0);
 }

 void rtc_init(void)
 {
	// Uses 1.024kHz from ULP32K clock
	struct rtc_count_config config;
	rtc_count_get_config_defaults(&config);
	config.prescaler = RTC_COUNT_PRESCALER_OFF; // 1.024kHz clock
	
	rtc_count_init(&module, RTC, &config);
	rtc_count_set_count(&module, 0);
	rtc_count_enable(&module);

	rtc_count_register_callback(&module, rtc_callback, RTC_COUNT_CALLBACK_COMPARE_0);
 }

 void rtc_idle_delay(uint32_t sleep_ms)
 {
	uint32_t current_count = rtc_count_get_count(&module);

	uint32_t wakeup_count = current_count + sleep_ms;
	rtc_count_set_compare(&module, wakeup_count, RTC_COUNT_COMPARE_0);
	rtc_count_enable_callback(&module, RTC_COUNT_CALLBACK_COMPARE_0);

	system_set_sleepmode(SYSTEM_SLEEPMODE_IDLE);
	system_sleep();
 }

 void rtc_standby_delay(uint32_t sleep_ms)
 {
	uint32_t current_count = rtc_count_get_count(&module);

	uint32_t wakeup_count = current_count + sleep_ms;
	rtc_count_set_compare(&module, wakeup_count, RTC_COUNT_COMPARE_0);
	rtc_count_enable_callback(&module, RTC_COUNT_CALLBACK_COMPARE_0);

	system_set_sleepmode(SYSTEM_SLEEPMODE_STANDBY);
	system_sleep();
 }