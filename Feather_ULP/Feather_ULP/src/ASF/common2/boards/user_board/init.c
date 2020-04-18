/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

#if defined(__GNUC__)
void board_init(void) WEAK __attribute__((alias("system_board_init")));
#elif defined(__ICCARM__)
void board_init(void);
#  pragma weak board_init=system_board_init
#endif

void system_board_init(void)
{
	ioport_set_pin_mode(SDA_PIN, SDA_PIN_MUX);
	ioport_set_pin_mode(SCL_PIN, SCL_PIN_MUX);

	ioport_set_pin_mode(A0_PIN, A0_PIN_MUX);
	ioport_set_pin_mode(A1_PIN, A1_PIN_MUX);
	ioport_set_pin_mode(A2_PIN, A2_PIN_MUX);
	ioport_set_pin_mode(A3_PIN, A3_PIN_MUX);
	ioport_set_pin_mode(A4_PIN, A4_PIN_MUX);
	ioport_set_pin_mode(A5_PIN, A5_PIN_MUX);

	/*ioport_set_pin_mode(TX_PIN, TX_PIN_MUX);
	ioport_set_pin_mode(RX_PIN, RX_PIN_MUX);*/
}