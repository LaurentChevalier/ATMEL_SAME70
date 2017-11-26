/*
 * Menu.h
 *
 * Created: 21/11/2017 11:33:03
 *  Author: 101007423
 */ 


#ifndef MENU_H_
#define MENU_H_

#include "main.h"
#include "pgmspace.h"
#ifndef PGM_P
#define PGM_P const rom char *
#endif

typedef struct PROGMEM
{
	unsigned char state;
	unsigned char input;
	unsigned char nextstate;
} MENU_NEXTSTATE;

typedef struct PROGMEM
{
	unsigned char state;
	PGM_P pText;
	char (*pFunc)(char input);
} MENU_STATE;

// Menu text
//const char Txt_START[] PROGMEM 		=	"BONJOUR!";
//const char Txt_RELAY[] PROGMEM		= 	"RELAIS ON/OFF ";
//const char Txt_SWITCH_LED[] PROGMEM =	"CLIGNOT. ON/OFF";

const MENU_NEXTSTATE menu_nextstate[] PROGMEM = {
	//  STATE                       INPUT       NEXT STATE
	// MENUS

	//{ST_MENU,                   BOUTON_1,		FCT_AUTO},
	//{ST_MENU,                   BOUTON_2,		FCT_MANU},
	//{ST_MENU,                   BOUTON_3,		FCT_PARAMETERS},
	//
	//{ST_AUTO,                   BOUTON_B,		FCT_MENU},
	//{ST_AUTO,					BOUTON_A,		FCT_AUTO_CONSIGNE_CHANGE},
	
	
	{0,                        0,          0},
};

const MENU_STATE Menu_State[] PROGMEM = {
	//  STATE                               STATE TEXT                  STATE_FUNC

	// Menu principal
	{FCT_MENU,                          NULL,   		            Fct_menu},
	{ST_MENU,                           NULL,   		            NULL},

	
	{0,                                 NULL,                       NULL},
	
};


#endif /* MENU_H_ */