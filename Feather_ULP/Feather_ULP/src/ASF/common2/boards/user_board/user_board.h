/**
 * \file
 *
 * \brief User board definition template
 *
 */

 /* This file is intended to contain definitions and configuration details for
 * features and devices that are available on the board, e.g., frequency and
 * startup time for an external crystal, external memory devices, LED and USART
 * pins.
 */
/*
 * Support and FAQ: visit <a href="https://www.microchip.com/support/">Microchip Support</a>
 */

#ifndef USER_BOARD_H
#define USER_BOARD_H

#include <conf_board.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * \ingroup group_common_boards
 * \defgroup user_board_group User board
 *
 * @{
 */

void system_board_init(void);

/** Name string macro */
#define BOARD_NAME                "USER_BOARD"

#define SDA_PIN						IOPORT_CREATE_PIN(IOPORT_PORTA,22)
#define SDA_PIN_MUX					IOPORT_MODE_MUX_C

#define SCL_PIN						IOPORT_CREATE_PIN(IOPORT_PORTA,23)
#define SCL_PIN_MUX					IOPORT_MODE_MUX_C

#define A0_PIN						IOPORT_CREATE_PIN(IOPORT_PORTA, 2)
#define A0_PIN_MUX					IOPORT_MODE_MUX_B

#define A1_PIN						IOPORT_CREATE_PIN(IOPORT_PORTB, 8)
#define A1_PIN_MUX					IOPORT_MODE_MUX_B

#define A2_PIN						IOPORT_CREATE_PIN(IOPORT_PORTB, 9)
#define A2_PIN_MUX					IOPORT_MODE_MUX_B

#define A3_PIN						IOPORT_CREATE_PIN(IOPORT_PORTA, 4)
#define A3_PIN_MUX					IOPORT_MODE_MUX_B

#define A4_PIN						IOPORT_CREATE_PIN(IOPORT_PORTA, 5)
#define A4_PIN_MUX					IOPORT_MODE_MUX_B

#define A5_PIN						IOPORT_CREATE_PIN(IOPORT_PORTB, 2)
#define A5_PIN_MUX					IOPORT_MODE_MUX_B

#define D5_PIN						IOPORT_CREATE_PIN(IOPORT_PORTB, 3)

#define D6_PIN						IOPORT_CREATE_PIN(IOPORT_PORTB, 1)

#define D9_PIN						IOPORT_CREATE_PIN(IOPORT_PORTB, 0)

#define D10_PIN						IOPORT_CREATE_PIN(IOPORT_PORTB, 31)

#define D11_PIN						IOPORT_CREATE_PIN(IOPORT_PORTB, 30)

#define D12_PIN						IOPORT_CREATE_PIN(IOPORT_PORTA, 19)

#define D13_PIN						IOPORT_CREATE_PIN(IOPORT_PORTA, 17)

#define TX_PIN						IOPORT_CREATE_PIN(IOPORT_PORTB, 22)
#define TX_PIN_MUX					IOPORT_MODE_MUX_D

#define RX_PIN						IOPORT_CREATE_PIN(IOPORT_PORTB, 23)
#define RX_PIN_MUX					IOPORT_MODE_MUX_D

#define ACCEL_INT_PIN				IOPORT_CREATE_PIN(IOPORT_PORTB, 4)

// TODO add SPI

/** @} */

#ifdef __cplusplus
}
#endif

#endif // USER_BOARD_H
