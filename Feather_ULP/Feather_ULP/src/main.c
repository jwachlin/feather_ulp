/**
 * \file main.c
 *
 * \brief Ultra low power development board example
 *
 */

#include <asf.h>

#include "sensor/i2c_interface.h"
#include "sensor/lis3dh.h"
#include "sensor/bmp280.h"
#include "lib/rtc_interface.h"
#include "lib/adc_interface.h"
#include "lib/pwm_interface.h"

static inline void flash_led(void)
{
	pwm_write(160);
	delay_us(50);
	pwm_write(0);
}

int main (void)
{
	int32_t i;

	system_init();
	delay_init();
	system_interrupt_enable_global();

	// Set up to performance level 0, buck converter
	// Clock must be <12MHz, other clocks must meet specs in datasheet
	system_switch_performance_level(SYSTEM_PERFORMANCE_LEVEL_0);
	system_performance_level_disable(); // Lock into PL0

	struct system_voltage_regulator_config vreg_config;
	system_voltage_regulator_get_config_defaults(&vreg_config);
	vreg_config.regulator_sel = SYSTEM_VOLTAGE_REGULATOR_BUCK;
	vreg_config.low_power_efficiency = SYSTEM_VOLTAGE_REGULATOR_LOW_POWER_EFFICIENCY_HIGHTEST; // Higher efficiency, runs 2.5-3.6V only
	vreg_config.run_in_standby_pl0 = true;
	system_voltage_regulator_set_config(&vreg_config);

	bod33_disable();
	bod12_disable();
	rtc_init();
	adc_interface_init();
	pwm_interface_init();

	// Delay in standby for a bit to ideally let voltage rise better
	rtc_standby_delay(5000);

	i2c_interface_init();
	init_lis3dh();
	init_bmp280();

	for(i=0;i<3;i++)
	{
		rtc_idle_delay(200);
		flash_led();
	}
	
	for(;;)
	{	
		uint32_t delay_time = 5000;
		if((i+1) % 3 == 0)
		{
			rtc_standby_delay(delay_time);
		}
		else if((i+2) % 3 == 0)
		{
			rtc_idle_delay(delay_time);
		}
		else
		{
			delay_ms(delay_time);
		}
		lis3dh_data_g_t accel_data;
		read_lis3dh_g(&accel_data);

		bmp280_meas_t baro_data;
		read_bmp280(&baro_data);

		uint16_t adc_value = adc_interface_read_pin(A0_PIN);

		flash_led();

		i++;
	}
}
