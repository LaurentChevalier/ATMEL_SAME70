/*
 * Main.c
 *
 * Created: 10/11/2017 11:00:00
 *  Author: CHEVALIER Laurent
 */ 


#include "asf.h"
#include "stdio_serial.h"
#include "conf_board.h"
#include "led.h"
#include "usart.h"
#include "DS3231m.h"


/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
extern "C" {
#endif
/**INDENT-ON**/
/// @endcond

/** TWI Bus Clock 400kHz */
#define TWIHS_CLK     400000

#define STRING_EOL    "\r"
#define STRING_HEADER "--TWIHS MASTER Example --\r\n" \
		"-- "BOARD_NAME" --\r\n" \
		"-- Compiled: "__DATE__" "__TIME__" --"STRING_EOL
		
		
/** Global timestamp in milliseconds since start of application */
volatile uint32_t g_ul_ms_ticks = 0;

/**
 *  \brief Handler for System Tick interrupt.
 *
 *  Process System Tick Event
 *  increments the timestamp counter.
 */
void SysTick_Handler(void)
{
	g_ul_ms_ticks++;
}

/**
 *  \brief Configure the Console UART.
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

//call this routine to initialize all peripherals
void init_devices(void)
{
	twihs_options_t opt;
	/* Initialize the SAM system */
	sysclk_init();

	/* Initialize the board */
	board_init();

	/* Turn off LEDs */
	LED_Off(LED0);

	/* Initialize the console UART */
	configure_console();

	/* Output example information */
	puts(STRING_HEADER);

	/* Configure systick for 1 ms */
	puts("Configure system tick to get 1ms tick period.\r");
	if (SysTick_Config(sysclk_get_cpu_hz() / 1000)) {
		puts("-E- Systick configuration error\r");
		while (1) {
			/* Capture error */
		}
	}

	/* Enable the peripheral clock for TWI */
	pmc_enable_periph_clk(ID_TWIHS0);

	/* Configure the options of TWI driver */
	opt.master_clk = sysclk_get_peripheral_hz();
	opt.speed      = TWIHS_CLK;

	if (twihs_master_init(TWIHS0, &opt) != TWIHS_SUCCESS) {
		puts("-E-\tTWI master initialization failed.\r");
		while (1) {
			/* Capture error */
		}
	}
}



/**
 * \brief Application entry point for TWI EEPROM example.
 *
 * \return Unused (ANSI-C compatibility).
 */
int main(void)
{
	uint32_t i;
	unsigned char errorStatus, data, totalChar,receivebyte,option;
	init_devices();
	
	printf("\n DS3231 RTC TEST ON SAME70 X-Plained");
	while(1){
	printf("\ nPress key");
	
	//option=USART_GetChar(USART1);
	usart_getchar(USART1,receivebyte);
	printf(receivebyte);
	
	printf("\n> 1 : Display RTC Date");
	printf("\n");
	printf("\n> 2 : Display RTC Time");
	printf("\n");
	printf("\n> 3 : Update RTC Date");
	printf("\n");
	printf("\n> 4 : Update RTC Time");
	printf("\n");
	printf("\n> Select Option (1-4): ");
	
	//option = USART_GetChar(USART1);
	usart_getchar(USART1,receivebyte);
	printf(receivebyte);
	option=atoi(receivebyte);
	errorStatus = 0;
	totalChar = 0;
	
	switch (option)
	{
		case 1: 
		printf("\n> 1 : Display RTC Date");
		RTC_displayDate();
		break;
		case 2: 
		printf("\n> 2 : Display RTC Time");
		RTC_displayTime();
		break;
		case 3:
		printf("\n> 3 : Update RTC Date");
		RTC_updateDate();
		break;
		case 4:
		printf("\n> 4 : Update RTC Time");
		RTC_updateTime();
		break;
		default:
		printf("\n Invalid option!");
		
	}
	
	printf("\n Finished..");
	}
	
return 0;

}



	


/// @cond 0
/**INDENT-OFF**/
#ifdef __cplusplus
}
#endif
/**INDENT-ON**/
/// @endcond
