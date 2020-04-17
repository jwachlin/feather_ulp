/*
 * rtc_interface.h
 *
 * Created: 4/16/2020 8:02:35 PM
 *  Author: Jake
 */ 


#ifndef RTC_INTERFACE_H_
#define RTC_INTERFACE_H_

void rtc_init(void);
void rtc_idle_delay(uint32_t sleep_ms);
void rtc_standby_delay(uint32_t sleep_ms);

#endif /* RTC_INTERFACE_H_ */