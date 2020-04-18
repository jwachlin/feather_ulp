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

int main (void)
{
	system_init();
	delay_init();
	system_interrupt_enable_global();

	delay_ms(50);

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

	i2c_interface_init();
	init_lis3dh();
	init_bmp280();

	/* Insert application code here, after the board has been initialized. */
	int32_t i;
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

		i++;
	}
}
