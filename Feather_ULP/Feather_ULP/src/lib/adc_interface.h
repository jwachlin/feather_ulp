/*
 * adc_interface.h
 *
 * Created: 4/18/2020 2:15:08 PM
 *  Author: Jake
 */ 


#ifndef ADC_INTERFACE_H_
#define ADC_INTERFACE_H_

void adc_interface_init(void);
uint16_t adc_interface_read_pin(ioport_pin_t pin);


#endif /* ADC_INTERFACE_H_ */