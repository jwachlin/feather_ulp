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

int main (void)
{
	system_init();
	delay_init();

	delay_ms(50);

	// Set up to performance level 0, buck converter
	// Clock must be <12MHz, other clocks must meet specs in datasheet
	system_switch_performance_level(SYSTEM_PERFORMANCE_LEVEL_0); 

	struct system_voltage_regulator_config vreg_config;
	system_voltage_regulator_get_config_defaults(&vreg_config);
	vreg_config.regulator_sel = SYSTEM_VOLTAGE_REGULATOR_BUCK;
	vreg_config.low_power_efficiency = SYSTEM_VOLTAGE_REGULATOR_LOW_POWER_EFFICIENCY_HIGHTEST; // Higher efficiency, runs 2.5-3.6V only
	system_voltage_regulator_set_config(&vreg_config);

	i2c_interface_init();
	init_lis3dh();
	init_bmp280();

	/* Insert application code here, after the board has been initialized. */
	int32_t i;
	for(;;)
	{
		delay_ms(1000);
		lis3dh_data_g_t accel_data;
		read_lis3dh_g(&accel_data);

		bmp280_meas_t baro_data;
		read_bmp280(&baro_data);

		i++;
	}
}
