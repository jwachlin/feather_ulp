/*
 * adc_interface.c
 *
 * Created: 4/18/2020 2:14:57 PM
 *  Author: Jake
 */ 

 #include "asf.h"

 #include "adc_interface.h"

 static struct adc_module module;

 void adc_interface_init(void)
 {
	struct adc_config config;
	adc_get_config_defaults(&config);
	config.clock_prescaler = ADC_CLOCK_PRESCALER_DIV256;	// Runs off 4MHz clock
	config.positive_input = ADC_POSITIVE_INPUT_PIN0;
	config.reference = ADC_REFERENCE_AREFA;
	config.reference_compensation_enable = false;
	config.positive_input_sequence_mask_enable = (1ul << 0) | (1ul << 2) | (1ul << 3) | (1ul << 4) | (1ul << 5) | (1ul << 10);

	adc_init(&module, ADC, &config);
	adc_enable(&module);
 }

 uint16_t adc_interface_read_pin(ioport_pin_t pin)
 {
	// TODO
 }