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
