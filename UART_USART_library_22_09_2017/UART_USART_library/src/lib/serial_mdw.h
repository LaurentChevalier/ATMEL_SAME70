/************************************************************************
Title:    Serial Middleware Library
Author:   Julien Delvaux
Software: Atmel Studio 7
Hardware: SAME70Q21
License:  GNU General Public License 3
Usage:    see Doxygen manual

LICENSE:
	Copyright (C) 2015 Julien Delvaux

	This program is free software; you can redistribute it and/or modify
	it under the terms of the GNU General Public License as published by
	the Free Software Foundation; either version 3 of the License, or
	any later version.

	This program is distributed in the hope that it will be useful,
	but WITHOUT ANY WARRANTY; without even the implied warranty of
	MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
	GNU General Public License for more details.

    
************************************************************************/

/** 
 *  @defgroup Serial Middleware Library
 *  @code #include <serial_mdw.h> @endcode
 * 
 *  @brief
 *
 *
 *  @author Julien Delvaux <delvaux.ju@gmail.com>
 */


#ifndef SERIAL_MDW_H_
#define SERIAL_MDW_H_

#include "compiler.h"
#include "sysclk.h"
#include "uart.h"
#include "usart.h"
#include "status_codes.h"

/** Input parameters when initializing RS232 and similar modes. */
typedef struct uart_rs232_options {
	/** Set baud rate of the USART (unused in slave modes). */
	uint32_t baudrate;

	/** Number of bits to transmit as a character (5-bit to 9-bit). */
	uint32_t charlength;

	/**
	 * Parity type: USART_PMODE_DISABLED_gc, USART_PMODE_EVEN_gc,
	 * USART_PMODE_ODD_gc.
	 */
	uint32_t paritytype;

	/** 1, 1.5 or 2 stop bits. */
	uint32_t stopbits;

} usart_rs232_options_t;

typedef usart_rs232_options_t usart_serial_options_t;

typedef Usart *usart_if;


/** 
 * \brief Initializes the Usart in master mode.
 *
 * \param p_usart  Base address of the USART instance.
 * \param opt      Options needed to set up RS232 communication (see
 * \ref usart_options_t).
 */
extern void serial_mdw_init(usart_if p_usart,usart_serial_options_t *opt) ;

extern int serial_mdw_putchar(usart_if p_usart, const uint8_t c);

extern void serial_mdw_sendData(usart_if p_usart,const uint8_t *p_buff, uint32_t ulsize);

uint32_t UART_IsRxReady(Uart *uart);

//uint8_t UART_GetChar(Uart *uart);

//void UART_ReceiveBuffer(Uart *uart, uint8_t *pBuffer, uint32_t BuffLen);


#endif /* SERIAL_MDW_H_ */