/*
 * pwm_interface.c
 *
 * Created: 4/25/2020 11:07:57 AM
 *  Author: Jake
 */ 

  #include "asf.h"

  #include "pwm_interface.h"

  static struct tc_module module;
  static struct tc_config config;

  void pwm_interface_init(void)
  {
	tc_get_config_defaults(&config);
	config.counter_size = TC_COUNTER_SIZE_8BIT;
	config.wave_generation = TC_WAVE_GENERATION_NORMAL_PWM;
	config.clock_source = GCLK_GENERATOR_0; // 4MHz
	config.clock_prescaler = TC_CLOCK_PRESCALER_DIV1; // 4 MHz for ~16kHz PWM freq

	config.counter_8_bit.compare_capture_channel[0] = 0; // start off
	config.counter_8_bit.value = 0;
	config.counter_8_bit.period = 255;
	config.pwm_channel[0].enabled = true;
	config.pwm_channel[0].pin_out = LED_PIN;
	config.pwm_channel[0].pin_mux = LED_PIN_PWM_PINMUX;

	tc_init(&module, TC3, &config);

	tc_enable(&module);
  }

  void pwm_write(uint32_t val)
  {
	tc_set_compare_value(&module, TC_COMPARE_CAPTURE_CHANNEL_0, val);
  }