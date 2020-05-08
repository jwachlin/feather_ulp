/*
 * opt3004.h
 *
 * Created: 5/7/2020 8:55:14 PM
 *  Author: Jake
 */ 


#ifndef OPT3004_H_
#define OPT3004_H_

#include <stdint.h>

#define OPT3004_CONVERSION_TIME_MS		(1000)

#define OPT3004_I2C_ADDRESS				(0x44) //ADDR to GND

// NOTE: All registers are 16 bits long
#define OPT3004_REG_RESULT				(0x00)
#define			OPT3004_RESULT_EXPONENT_MASK		(0xF000)	// lux = 0.01 * 2^exponent * mantissa
#define			OPT3004_RESULT_MANTISSA_MASK		(0x0FFF)
#define OPT3004_REG_CONFIG				(0x01)
#define		OPT3004_CONFIG_AUTO_FSR					(0x000F << 12)
#define		OPT3004_CONFIG_CONV_TIME_800MS			(0x0001 << 11)
#define		OPT3004_CONFIG_CONV_SHUTDOWN			(0x0000 << 9)
#define		OPT3004_CONFIG_CONV_SINGLE_SHOT			(0x0001 << 9)
#define		OPT3004_CONFIG_CONV_CONTINUOUS			(0x0002 << 9)
#define OPT3004_REG_LOW_LIMIT			(0x02)
#define OPT3004_REG_HIGH_LIMIT			(0x03)
#define OPT3004_REG_MANU_ID				(0x7E)
#define		OPT3004_MANU_ID							(0x5449)
#define OPT3004_REG_DEV_ID				(0x7F)
#define		OPT3004_DEV_ID							(0x3001)

void init_opt3004(void);
void sleep_opt3004(void);
uint32_t get_opt3004_ambient_light_level_lux(void);

#endif /* OPT3004_H_ */