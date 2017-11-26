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
#include "pgmspace.h"
#include "main.h"
#include "OS.h"
#include "same70_xplained.h"


// CONSTANTES
// VARIABLES
unsigned char IDCB_Switch_Led;	// mémorisation de l'identificateur créé lors de l'enregistrement de la Callback Switch_Led


void wait_100x_ms(int ms) { uint8_t i; for (i=0;i<ms;i++) delay_ms(100); }

/**
* \brief Configure UART console.
*/
static void configure_console(void)
{
const usart_serial_options_t uart_serial_options = {
.baudrate = CONF_UART_BAUDRATE,
#ifdef CONF_UART_CHAR_LENGTH
.charlength = CONF_UART_CHAR_LENGTH,
#endif
.paritytype = CONF_UART_PARITY,
#ifdef CONF_UART_STOP_BITS
.stopbits = CONF_UART_STOP_BITS,
#endif
};

/* Configure console UART. */
sysclk_enable_peripheral_clock(CONSOLE_UART_ID);
stdio_serial_init(CONF_UART, &uart_serial_options);
}

//****************** FONCTION PRINCIPALE *****************
int main (void)
{
	
	//Initialisation Carte
	sysclk_init();
	board_init();
	configure_console();
	
	printf("\n Bienvenue");
	gpio_set_pin_high(PIO_PC8_IDX);
	// Initialisation de l'OS (appel des Callbacks)
	OSInitialiser();
	printf("\n Bienvenue");
	gpio_set_pin_high(PIO_PC8_IDX);

	// Initialisation des Callbacks
 	//IDCB_Switch_Led = OSEnregistrerCB_TIMER(Switch_Led, 500);
		
	OSStart();

 	//N'arrive jamais ici
	return 0;
}


char Fct_menu(char input)
{
			 printf("\n Menu");
			
			 return ST_MENU;
}