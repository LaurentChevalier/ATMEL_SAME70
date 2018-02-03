/**
 * \file
 *
 * \brief Httpd server.
 *
 * Copyright (c) 2013-2015 Atmel Corporation. All rights reserved.
 *
 * \asf_license_start
 *
 * \page License
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 * 1. Redistributions of source code must retain the above copyright notice,
 *    this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 *
 * 3. The name of Atmel may not be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * 4. This software may only be redistributed and used in connection with an
 *    Atmel microcontroller product.
 *
 * THIS SOFTWARE IS PROVIDED BY ATMEL "AS IS" AND ANY EXPRESS OR IMPLIED
 * WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NON-INFRINGEMENT ARE
 * EXPRESSLY AND SPECIFICALLY DISCLAIMED. IN NO EVENT SHALL ATMEL BE LIABLE FOR
 * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
 * STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 * ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 *
 * \asf_license_stop
 *
 */

#include "string.h"
#include "lwip/debug.h"
#include "lwip/stats.h"
#include "lwip/tcp.h"
#include "lwipopts.h"
#include "httpd.h"
#include "fs.h"

#define COUNTER_DISP_INDEX      119

/* This is the data for the actual web page. */
static char indexdata[] =
"HTTP/1.1 200 OK\r\n\
Content-type: text/html\r\n\
\r\n\<html> <head><title>lwIP test page</title></head> <body> Small test page </body> </html>";


u8_t   data[100];



///**
//* This is the callback function that is called when
//* a connection has been accepted.
//*/
//static err_t
//httpd_accept(void *arg, struct tcp_pcb *pcb, err_t err)
//{
	//arg = arg;
//
	//if (err != ERR_OK)
	//return err;
//
	///* Set up the function httpd_recv() to be called when data
	//arrives. */
	//tcp_recv(pcb, httpd_recv);
	//return ERR_OK;
//}



static void client_close(struct tcp_pcb *pcb)
{
	tcp_arg(pcb, NULL);
	tcp_sent(pcb, NULL);
	tcp_close(pcb);

	printf("\nclient_close(): Closing...\n");
}

static err_t client_sent(void *arg, struct tcp_pcb *pcb, u16_t len)
{
	LWIP_UNUSED_ARG(arg);

	printf("\nclient_sent(): Number of bytes ACK'ed is %d\n", len);

	//client_close(pcb);


	return ERR_OK;
}

/**
* This is the callback function that is called
* when a TCP segment has arrived in the connection.
*/
static err_t
httpd_recv(void *arg, struct tcp_pcb *pcb, struct pbuf *p, err_t err)
{
	char rq[9];//
	arg = arg;
	char datas[] = "\nReçu";

	printf("CB recv %x, %x, %x, %x\n\r", arg, pcb, p, err);
	if (err != ERR_OK)
	return err;

	/* If we got a NULL pbuf in p, the remote end has closed
	the connection. */
	if (p != NULL) {

		/* The payload pointer in the pbuf contains the data
		in the TCP segment. */
		//rq = p->payload;
		strcpy(rq,p->payload);
		//printf(rq);
		/* Check if the request was an HTTP "GET /\r\n".
		or HTTP "GET / HTTP/1.1\r\n" */
		if (rq[0] == 'G' && rq[1] == 'E' && rq[2] == 'T' &&
		rq[3] == ' ' && rq[4] == '/' &&
		((rq[ 5] == '\r' && rq[ 6] == '\n') ||
		(rq[14] == '\r' && rq[15] == '\n')) ) {
			/* Send the web page to the remote host. A zero
			in the last argument means that the data should
			not be copied into internal buffers. */

			//tcp_write(pcb, indexdata, sizeof(indexdata), 0);
		}
		tcp_sent(pcb, client_sent);
		tcp_write(pcb, rq, sizeof(rq), 1);
		printf(datas);
		/* Free the pbuf. */
		pbuf_free(p);
	}

	/* Close the connection. */
	//tcp_close(pcb);

	return ERR_OK;
}

/**
* @brief Function called when TCP connection established
* @param tpcb: pointer on the connection contol block
* @param err: when connection correctly established err should be ERR_OK
* @retval err_t: returned error
*/
static err_t
callback_Connected(void *arg, struct tcp_pcb *tpcb, err_t err)

{
	printf("\n Callback connected");
	char datas[] = "$0071Hello!#";
	
	LWIP_UNUSED_ARG(arg);
	
	if (err != ERR_OK)
	printf("\nclient_connected(): err argument not set to ERR_OK, but is value is %d\n", err);
	
	else
	{
		tcp_sent(tpcb, client_sent);
		tcp_write(tpcb, datas, sizeof(datas), 1);
		tcp_recv(tpcb, httpd_recv);
	}
	
	return err;
	
	
}




/**
* The initialization function.
*/
err_t httpd_init(void)
{
	struct ip_addr dest;
	IP4_ADDR(&dest, 192, 168, 1, 23);
	struct tcp_pcb *pcb;
	err_t  err;
	/* Create a new TCP PCB. */
	pcb = tcp_new();

	if (pcb == NULL) {
		//printf("F: Fail to create PCB\n\r");
		return ERR_BUF;
	}
	
	tcp_bind(pcb, IP_ADDR_ANY, 7000); //client port for outcoming connection

	/*--------------------------
	*	For SERVER MODE ONLY:
	--Modified by Laurent Chevalier 2017_10_02 */

	///* Bind the PCB to TCP port 80. */
	//err = tcp_bind(pcb, NULL, 80);
	//
	//if (err != ERR_OK) {
	//printf("E: tcp_bind %x\n\r", err);
	//return err;
	//}
	//
	/* Change TCP state to LISTEN. */
	//pcb = tcp_listen(pcb);
	//
	//if (pcb == NULL) {
	//printf("E: tcp_listen\n\r");
	//return ERR_BUF;
	//}
//
	///* Set up httpd_accet() function to be called
	//when a new connection arrives. */
	//tcp_accept(pcb, httpd_accept);
	
	/*--------------------------
	*	For CLIENT MODE ONLY:
	-Added by Laurent Chevalier 2017_10_02 */

	/* Connect the PCB to remote host. */
	err = tcp_connect(pcb, &dest,9996,callback_Connected);

	if (err != ERR_OK) {
		//printf("E: tcp_connect %x\n\r", err);
		return err;

	}
	return ERR_OK;
	
	
}


