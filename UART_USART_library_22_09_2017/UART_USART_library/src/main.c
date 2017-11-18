/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include <gpio.h>
#include <board.h>
#include "lib/serial_mdw.h"
#include "string.h"
extern char message[];

static void configure_uart(void)
{
	const sam_usart_opt_t usart_console_settings = {
		115200ul,
		US_MR_CHRL_8_BIT,
		US_MR_PAR_NO,
		US_MR_NBSTOP_1_BIT,
		US_MR_CHMODE_NORMAL,
		/* This field is only used in IrDA mode. */
		0
	};


	/* Configure USART in RS232 mode. */
	serial_mdw_init(UART0, &usart_console_settings);
	serial_mdw_init(UART1, &usart_console_settings);
	serial_mdw_init(UART2, &usart_console_settings);
	serial_mdw_init(UART3, &usart_console_settings);
	serial_mdw_init(UART4, &usart_console_settings);
	
	serial_mdw_init(USART0, &usart_console_settings);
	serial_mdw_init(USART1, &usart_console_settings);
	serial_mdw_init(USART2, &usart_console_settings);

	/* Disable all the interrupts. */
	//usart_disable_interrupt(UART0, 0xffffffff);
	usart_disable_interrupt(UART0, US_IER_RXRDY);
	usart_disable_interrupt(UART1, 0xffffffff);
	usart_disable_interrupt(UART2, 0xffffffff);
	usart_disable_interrupt(UART3, 0xffffffff);
	usart_disable_interrupt(UART4, 0xffffffff);
	
	usart_disable_interrupt(USART0, 0xffffffff);
	usart_disable_interrupt(USART1, 0xffffffff);
	usart_disable_interrupt(USART2, 0xffffffff);


	/* Enable TX & RX function. */
	usart_enable_tx(UART0);
	usart_enable_rx(UART0);
	usart_enable_tx(UART1);
	usart_enable_rx(UART1);
	usart_enable_tx(UART2);
	usart_enable_rx(UART2);
	usart_enable_tx(UART3);
	usart_enable_rx(UART3);
	usart_enable_tx(UART4);
	usart_enable_rx(UART4);
	
	usart_enable_tx(USART0);
	usart_enable_rx(USART0);
	usart_enable_tx(USART1);
	usart_enable_rx(USART1);
	usart_enable_tx(USART2);
	usart_enable_rx(USART2);

	/* Configure and enable interrupt of USART. */
	NVIC_ClearPendingIRQ(UART0_IRQn);
	NVIC_EnableIRQ(UART0_IRQn);
	uart_enable_interrupt(UART0, UART_IER_RXRDY);
	NVIC_ClearPendingIRQ(UART1_IRQn);
	NVIC_EnableIRQ(UART1_IRQn);
	uart_enable_interrupt(UART1, UART_IER_RXRDY);
	NVIC_ClearPendingIRQ(UART2_IRQn);
	NVIC_EnableIRQ(UART2_IRQn);
	uart_enable_interrupt(UART2, UART_IER_RXRDY);
	NVIC_ClearPendingIRQ(UART3_IRQn);
	NVIC_EnableIRQ(UART3_IRQn);
	uart_enable_interrupt(UART3, UART_IER_RXRDY);
	NVIC_ClearPendingIRQ(UART4_IRQn);
	NVIC_EnableIRQ(UART4_IRQn);
	uart_enable_interrupt(UART4, UART_IER_RXRDY);
	
	NVIC_ClearPendingIRQ(USART0_IRQn);
	NVIC_EnableIRQ(USART0_IRQn);
	usart_enable_interrupt(USART0, US_IER_RXRDY);
	NVIC_ClearPendingIRQ(USART1_IRQn);
	NVIC_EnableIRQ(USART1_IRQn);
	usart_enable_interrupt(USART1, US_IER_RXRDY);
	NVIC_ClearPendingIRQ(USART2_IRQn);
	NVIC_EnableIRQ(USART2_IRQn);
	usart_enable_interrupt(USART2, US_IER_RXRDY);
}

int main (void)
{
	/* Initialize the SAM system. */
	sysclk_init();
	board_init();

	/* Configure USART-USART */
	configure_uart();
	
	while (1)
	{
		//serial_mdw_putchar(UART4, 'h');
		strcpy(message,"HELLO WORLD FROM UART0\n");
		serial_mdw_sendData(UART0, message, strlen(message));
		strcpy(message,"HELLO WORLD FROM UART1\n");
		serial_mdw_sendData(UART1, message, strlen(message));
		strcpy(message,"HELLO WORLD FROM UART2\n");
		serial_mdw_sendData(UART2, message, strlen(message));
		strcpy(message,"HELLO WORLD FROM UART3\n");
		serial_mdw_sendData(UART3, message, strlen(message));
		strcpy(message,"HELLO WORLD FROM UART4\n");
		serial_mdw_sendData(UART4, message, strlen(message));
		
		strcpy(message,"HELLO WORLD FROM USART0\n");
		serial_mdw_sendData(USART0, message, strlen(message)); //serial_mdw_sendData(USART X, "HELLO WORLD fROM USART X ", length of message)
		strcpy(message,"HELLO WORLD FROM USART1\n");
		serial_mdw_sendData(USART1, message, strlen(message)); //Problem with this usart
		strcpy(message,"HELLO WORLD FROM USART2\n");
		serial_mdw_sendData(USART2, message, strlen(message));
		gpio_toggle_pin(PIO_PC8_IDX);
	
		delay_ms(50);
	}
}
