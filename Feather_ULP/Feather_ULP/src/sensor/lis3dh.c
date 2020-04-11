/*
 * lis3dh.c
 *
 * Created: 4/10/2020 7:33:37 PM
 *  Author: Jake
 */ 

 #include "asf.h"

 #include <string.h>

 #include "i2c_interface.h"

 #include "lis3dh.h"

 void init_lis3dh(void)
 {
	uint8_t me = 0;
	i2c_read_bytes(&me, 1, LIS3DH_ADDRESS, REG_WHO_AM_I);
	if(me != WHO_AM_I_VAL)
	{
		return;
	}

	// Set up sensors
	// 50Hz ODR, low power mode -> 6uA typical
	uint8_t data = (0x04 << 4) | (0x01 << 3) | (0x07 << 0);   // 50Hz, all axes enabled, low power mode
	i2c_write_bytes(&data, 1, LIS3DH_ADDRESS, REG_CTRL_REG1);

	data = (0x01 << 4); // +/-4g range
	i2c_write_bytes(&data, 1, LIS3DH_ADDRESS, REG_CTRL_REG4);
 }

 void read_lis3dh_raw(lis3dh_data_raw_t * data)
 {
	uint8_t buffer[6];
	i2c_read_bytes(buffer, 6, LIS3DH_ADDRESS, REG_OUT_X_L | 0x80); // 0x80 enables auto increment
	data->x = buffer[0];
	data->x |= ((uint16_t) buffer[1]) << 8;
	data->y = buffer[2];
	data->y |= ((uint16_t) buffer[3]) << 8;
	data->z = buffer[4];
	data->z |= ((uint16_t) buffer[5]) << 8;
 }

 void read_lis3dh_g(lis3dh_data_g_t * data)
 {
	lis3dh_data_raw_t raw;
	read_lis3dh_raw(&raw);
	// 8 bit mode, +/-4g range
	data->x = raw.x * 0.00012207;
	data->y = raw.y * 0.00012207;
	data->z = raw.z * 0.00012207;
 }