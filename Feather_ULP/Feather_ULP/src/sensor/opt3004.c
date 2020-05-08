/*
 * opt3004.c
 *
 * Created: 5/7/2020 8:55:03 PM
 *  Author: Jake
 */ 

#include "asf.h"

#include <string.h>

#include "i2c_interface.h"

#include "opt3004.h"

static uint16_t unpack_reg(uint8_t * bytes)
{
	uint16_t val = (((uint16_t) *(bytes)) << 8);
	val += (((uint16_t) *(bytes+1)) << 0);
	return val;
}

static void pack_reg(uint16_t val, uint8_t * bytes)
{
	*(bytes) = ((val & 0xFF00) >> 8);
	*(bytes+1) = (val & 0x00FF);
}

void init_opt3004(void)
{
	uint8_t me_raw[2] = {0};
	
	i2c_read_bytes(me_raw, 2, OPT3004_I2C_ADDRESS, OPT3004_REG_DEV_ID);

	uint16_t me = unpack_reg(me_raw);
	if(me != OPT3004_DEV_ID)
	{
		return;
	}

	// Note: power (about 4uA) can be reduced by operating in single shot mode only occasionally
	uint16_t config = OPT3004_CONFIG_AUTO_FSR | OPT3004_CONFIG_CONV_TIME_800MS | OPT3004_CONFIG_CONV_CONTINUOUS;
	uint8_t config_raw[2];
	pack_reg(config,config_raw);
	i2c_write_bytes(config_raw,2,OPT3004_I2C_ADDRESS, OPT3004_REG_CONFIG);
}

void sleep_opt3004(void)
{
	uint16_t config = OPT3004_CONFIG_CONV_SHUTDOWN;
	uint8_t config_raw[2];
	pack_reg(config,config_raw);
	i2c_write_bytes(config_raw,2,OPT3004_I2C_ADDRESS, OPT3004_REG_CONFIG);
}

uint32_t get_opt3004_ambient_light_level_lux(void)
{
	uint8_t light_raw[2] = {0};
	i2c_read_bytes(light_raw, 2, OPT3004_I2C_ADDRESS, OPT3004_REG_RESULT);
	uint16_t light_reg = unpack_reg(light_raw);

	uint32_t exponent = (light_reg & OPT3004_RESULT_EXPONENT_MASK) >> 12;
	uint32_t mantissa = (light_reg & OPT3004_RESULT_MANTISSA_MASK);

	// lux = 0.01 * 2^exponent * mantissa
	// Note: 2^exponent is shifting "right" by that amount
	uint32_t lux = ((1ul << exponent) * mantissa) / 100;
	return lux;
}