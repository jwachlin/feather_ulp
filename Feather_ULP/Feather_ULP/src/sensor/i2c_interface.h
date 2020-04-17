/*
 * i2c_interface.h
 *
 * Created: 4/10/2020 7:34:41 PM
 *  Author: Jake
 */ 


#ifndef I2C_INTERFACE_H_
#define I2C_INTERFACE_H_

void i2c_interface_init(void);
void i2c_interface_disable(void);
void i2c_write_bytes(uint8_t * data, uint8_t data_length, uint8_t i2c_address, uint8_t reg);
void i2c_read_bytes(uint8_t * data, uint8_t data_length, uint8_t i2c_address, uint8_t reg);

#endif /* I2C_INTERFACE_H_ */