/*
 * i2c_interface.c
 *
 * Created: 4/10/2020 7:34:53 PM
 *  Author: Jake
 */ 

 #include "asf.h"

 #include <string.h>

 #include "i2c_interface.h"

 static struct i2c_master_config config;
 static struct i2c_master_module module;

 void i2c_interface_init(void)
 {
	i2c_master_get_config_defaults(&config);
	config.start_hold_time = I2C_MASTER_START_HOLD_TIME_400NS_800NS;
	config.baud_rate = I2C_MASTER_BAUD_RATE_100KHZ;
	config.buffer_timeout = 10000;
	config.pinmux_pad0 = PINMUX_PA22C_SERCOM3_PAD0;
	config.pinmux_pad1 = PINMUX_PA23C_SERCOM3_PAD1;

	i2c_master_init(&module, SERCOM3, &config);

	i2c_master_enable(&module);
 }

 void i2c_interface_disable(void)
 {
	i2c_master_disable(&module);
 }

 void i2c_write_bytes(uint8_t * data, uint8_t data_length, uint8_t i2c_address, uint8_t reg)
 {
	uint8_t buffer[data_length+2];
	buffer[0] = reg;
	memcpy(buffer+1, data, data_length);
	
	struct i2c_master_packet packet;
	packet.address = i2c_address;
	packet.data = buffer;
	packet.data_length = data_length+1;
	packet.high_speed = false;
	packet.hs_master_code = 0;
	packet.ten_bit_address = 0;

	i2c_master_write_packet_wait(&module, &packet);
 }

 void i2c_read_bytes(uint8_t * data, uint8_t data_length, uint8_t i2c_address, uint8_t reg)
 {
	struct i2c_master_packet packet_setup;
	packet_setup.address = i2c_address;
	packet_setup.data = &reg;
	packet_setup.data_length = 1;
	packet_setup.high_speed = false;
	packet_setup.hs_master_code = 0;
	packet_setup.ten_bit_address = 0;

	i2c_master_write_packet_wait(&module, &packet_setup);

	struct i2c_master_packet packet_read;
	packet_read.address = i2c_address;
	packet_read.data = data;
	packet_read.data_length = data_length;
	packet_read.high_speed = false;
	packet_read.hs_master_code = 0;
	packet_read.ten_bit_address = 0;

	i2c_master_read_packet_wait(&module, &packet_read);
 }