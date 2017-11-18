/*
* DS3231m.c
*
* Created: 13/11/2017 13:58:08
*  Author: CHEVALIER Laurent
*/
#include "DS3231m.h"
#include "asf.h"
#include "stdio_serial.h"
#include "conf_board.h"
#include "usart.h"
#include "twihs.h"
#include "stdint.h"
#include "stdio.h"
#include "string.h"

char time_update[3];
char date_update[4];
char rtc_register[7];
char time[10]; 		//xxam:xx:xx;
char date[12];		//xx/xx/xxxx;
char day;

/** Data to be sent */
#define TEST_DATA_LENGTH  (sizeof(test_data_tx) / sizeof(uint8_t))

static const uint8_t test_data_tx[] = { 0,0,0,0,0,0,0 };

twihs_packet_t packet_tx = {.chip= DS3231m_ADDRESS, .addr[0]=RTC_REG_ADDR, .addr_length=RTC_REG_ADDR_LENGTH, .buffer=time_update, .length=TEST_DATA_LENGTH };
twihs_packet_t packet_rx = {.chip= DS3231m_ADDRESS, .addr[0]=RTC_REG_ADDR, .addr_length=RTC_REG_ADDR_LENGTH, .buffer=rtc_register, .length=TEST_DATA_LENGTH };


//***********************************************************************
//Function to read RTC registers and store them in buffer rtc_register[]
//***********************************************************************
void RTC_read(void)
{
	// read 7 bytes starting from register 0
	// sec, min, hour, day-of-week, date, month, year
	packet_rx.addr[0]     = 0x0;
	packet_rx.length	  = (sizeof(rtc_register) / sizeof(uint8_t));
	if (twihs_master_read(TWIHS0, &packet_rx) != TWIHS_SUCCESS) {
		puts("-E-\tTWI master read RTC packet failed.\r");
		
	}
	puts("\nRead RTC:\tOK!\r");
	//puts(packet_rx.buffer); Only for debug
	
	
}

//******************************************************************
//Function to form time string
//******************************************************************
void RTC_getTime(void)
{
	RTC_read();
	time[8] = 0x00;	  //NIL
	time[7] = (SECONDS & 0x0f) | 0x30;		//seconds(1's)
	time[6] = ((SECONDS & 0x70) >> 4) | 0x30;		//seconds(10's)
	time[5] = ':';
	
	time[4] = (MINUTES & 0x0f) | 0x30;
	time[3] = ((MINUTES & 0x70) >> 4) | 0x30;
	time[2] = ':';
	
	time[1] = (HOURS & 0x0f) | 0x30;
	time[0] = ((HOURS & 0x30) >> 4) | 0x30;
}

//******************************************************************
//Function to form date string
//******************************************************************
void RTC_getDate(void)
{
	RTC_read();
	date[11] = 0x00;
	date[10] = 0x00;
	date[9] = (YEAR & 0x0f) | 0x30;
	date[8] = ((YEAR & 0xf0) >> 4) | 0x30;
	date[7] = '0';
	date[6] = '2';
	date[5] = '/';
	date[4] = (MONTH & 0x0f) | 0x30;
	date[3] = ((MONTH & 0x10) >> 4) | 0x30;
	date[2] = '/';
	date[1] = (DATE & 0x0f) | 0x30;
	date[0] = ((DATE & 0x30) >> 4) | 0x30;
}

//******************************************************************
//Function to display time on UART
//******************************************************************
void RTC_displayTime(void)
{
	RTC_getTime();
	printf("\nTime:");
	printf(time);
}

//******************************************************************
//Function to display date on UART
//******************************************************************
void RTC_displayDate(void)
{
	RTC_getDate();
	
	printf("\nDate:");
	printf(date);
	RTC_displayDay();
}

//******************************************************************
//Function to get the string for day
//******************************************************************
void RTC_displayDay(void)
{
	printf("    Day: ");
	
	switch(DAY)
	{
		case 0:printf("Sunday");
		break;
		case 1:printf("Monday");
		break;
		case 2:printf("Tuesday");
		break;
		case 3:printf("Wednesday");
		break;
		case 4:printf("Thursday");
		break;
		case 5:printf("Friday");
		break;
		case 6:printf("Saturday");
		break;
		default:	printf("Unknown");
	}
}

//******************************************************************
//Function to update buffer rtc_register[] for next writing to RTC
//******************************************************************
void RTC_updateRegisters(void)
{
	SECONDS = ((time[6] & 0x07) << 4) | (time[7] & 0x0f);
	MINUTES = ((time[3] & 0x07) << 4) | (time[4] & 0x0f);
	HOURS = ((time[0] & 0x03) << 4) | (time[1] & 0x0f);
	DAY = date[10];
	DATE = ((date[0] & 0x03) << 4) | (date[1] & 0x0f);
	MONTH = ((date[3] & 0x01) << 4) | (date[4] & 0x0f);
	YEAR = ((date[8] & 0x0f) << 4) | (date[9] & 0x0f);
	
	time_update[0] = SECONDS;
	time_update[1] = MINUTES;
	time_update[2] = HOURS;
	
	date_update[0] = DAY;
	date_update[1] = DATE;
	date_update[2] = MONTH;
	date_update[3] = YEAR;
}

//******************************************************************
//Function to write new time in the RTC
//******************************************************************
unsigned char RTC_writeTime(void)
{	/**** Only For Debug ****
time[0]='0'; // HOURs
time[1]='8';
time[3]='5'; // MINUTES
time[4]='3';
time[6]='0'; // SECONDS
time[7]='0';
RTC_updateRegisters(); //UPDATE Buffers
***/
packet_tx.addr[0]   = 0x00;
packet_tx.buffer	= time_update;
packet_tx.length	= (sizeof(time_update) / sizeof(uint8_t));
if (twihs_master_write(TWIHS0, &packet_tx) != TWIHS_SUCCESS) {
	puts("-E-\tTWI master write Time packet failed.\r");
	return(1);
}
puts("\nWrite Time:\tOK!\n\r");
return(0);
}

//******************************************************************
//Function to write new date in the RTC
//******************************************************************
unsigned char RTC_writeDate(void)
{
	/**** Only For Debug ****
	date[0]='1'; // DATE
	date[1]='4';
	date[3]='1'; // MONTH
	date[4]='1';
	date[8]='1'; // YEAR
	date[9]='7';
	date[10]='2'& 0x0f; // DAY
	RTC_updateRegisters(); // UPDATE Buffers
	***/
	packet_tx.addr[0]	= 0x03;
	packet_tx.buffer	= date_update;
	packet_tx.length	= (sizeof(date_update) / sizeof(uint8_t));
	
	if (twihs_master_write(TWIHS0, &packet_tx) != TWIHS_SUCCESS) {
		puts("-E-\tTWI master write Date packet failed.\r");
		return(1);
	}
	puts("\nWrite Date:\tOK!\n\r");
	return(0);
}

//******************************************************************
//Function to update RTC time by entering it at hyper terminal
//******************************************************************
void RTC_updateTime(void)
{
	unsigned char data;
	printf("\nEnter Time in 24h format(hh:mm:ss):");
	
	data = USART_GetChar(USART1);  	  	  				//receive hours
	//printf(data);
	if(data < 0x30 || data > 0x32)
	goto TIME_ERROR;
	
	time[0]= data;
	
	data = USART_GetChar(USART1);
	//printf(data);
	if(data < 0x30 || data > 0x39)
	goto TIME_ERROR;
	
	time[1]= data;
	
	printf(':');
	
	if(((time[1] & 0x0f) + ((time[0] & 0x03)*10)) > 23)
	goto TIME_ERROR;
	
	data = USART_GetChar(USART1);	 				   			  //receive minutes
	//printf(data);
	if(data < 0x30 || data > 0x35)
	goto TIME_ERROR;
	
	time[3]= data;
	
	data = USART_GetChar(USART1);
	//printf(data);
	if(data < 0x30 || data > 0x39)
	goto TIME_ERROR;
	
	time[4]= data;
	
	printf(':');
	
	data = USART_GetChar(USART1); 				   			  //receive seconds
	//printf(data);
	if(data < 0x30 || data > 0x35)
	goto TIME_ERROR;
	
	time[6]= data;
	
	data = USART_GetChar(USART1);
	//printf(data);
	if(data < 0x30 || data > 0x39)
	goto TIME_ERROR;
	
	time[7]= data;
	
	
	RTC_updateRegisters();
	data = RTC_writeTime();
	
	if(data == 0)
	{
		printf("\nTime Updated sucessfully..");
	}
	return;
	
	TIME_ERROR:

	printf("\nInvalid Entry..");
	return;
}


//******************************************************************
//Function to update RTC date by entering it at hyper terminal
//******************************************************************
void RTC_updateDate(void)
{
	unsigned char data;
	
	printf("\nEnter Date (dd/mm/yy):");
	
	data = USART_GetChar(USART1); 	   				  		//receive date
	//printf(data);
	if(data < 0x30 || data > 0x33)
	goto DATE_ERROR;
	
	date[0]= data;
	
	data = USART_GetChar(USART1);
	//printf(data);
	if(data < 0x30 || data > 0x39)
	goto DATE_ERROR;
	
	date[1]= data;
	
	if(((time[8] & 0x0f) + ((time[9] & 0x03)*10)) > 31)
	goto DATE_ERROR;
	
	data = USART_GetChar(USART1);	  			   			  //receive special char (/,-, etc.)
	//printf(data);
	if(data != '/' && data != '-' && data != ' ')
	goto DATE_ERROR;
	date[2] = data;
	
	
	data = USART_GetChar(USART1); 			   			  //receive month
	//printf(data);
	if(data < 0x30 || data > 0x31)
	goto DATE_ERROR;
	
	date[3]= data;
	
	data = USART_GetChar(USART1);
	//printf(data);
	if(data < 0x30 || data > 0x39)
	goto DATE_ERROR;
	
	date[4] = data;
	
	if(((date[4] & 0x0f) + ((date[3] & 0x03)*10)) > 12)
	goto DATE_ERROR;
	
	data = USART_GetChar(USART1);	 	  			   			  //receive special char (/,-, etc.)
	//printf(data);
	if(data != '/' && data != '-' && data != ' ')
	goto DATE_ERROR;
	date[5] = data;
	
	date[6] = '2'; 	   	   	  	  //year is 20xx
	date[7] = '0';
	
	data = USART_GetChar(USART1);	 			   			  //receive seconds
	//printf(data);
	if(data < 0x30 || data > 0x39)
	goto DATE_ERROR;
	
	date[8]= data;
	
	data = USART_GetChar(USART1);
	//printf(data);
	if(data < 0x30 || data > 0x39)
	goto DATE_ERROR;
	
	date[9]= data;
	
	
	printf("\nEnter Day (Sunday:0, Monday:1...) (0-6):");
	
	data = USART_GetChar(USART1);	 					   //receive Day of the week
	//printf(data);
	if(data < 0x30 || data > 0x36)
	goto DATE_ERROR;
	
	date[10] = data & 0x0f;
	
	RTC_updateRegisters();
	data = RTC_writeDate();
	
	if(data == 0)
	{
		printf("\nDate Updated sucessfully..");
	}
	return;
	
	DATE_ERROR:

	printf("\nInvalid Entry..");
	return;
	
}







