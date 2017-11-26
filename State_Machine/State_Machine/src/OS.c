/*
 * OS.c
 *
 * Created: 21/11/2017 11:33:52
 *  Author: 101007423
 */ 

#include "OS.h"
#include "Menu.h"
#include "main.h"
#include "pgmspace.h"
#include <asf.h>

#define  MAXCALLBACKCHRONO 255

//Variables pour CallBack Chrono
void (*MaCB[MAXCALLBACKCHRONO])(void);
unsigned int TempsCB[MAXCALLBACKCHRONO];
volatile unsigned int TickCB[MAXCALLBACKCHRONO];

//Variables pour CallBack Button (Appui sur un bouton)
void(*MaCBButton)(volatile unsigned char*);

// PROTOTYPE FONCTIONS INTERNES
void ENABLE_INTERRUPT_BP(void);

//Variables pour la machine d'�tats
unsigned char state;  // holds the current state, according to "menu.h"

// ****************  OSInitialiser ******************************
// Initialise l'OS: initialise les variables et d�marre le Timer
// ****************************************************************
void OSInitialiser(void)
{
	unsigned char i;
	//Initialisation pour variables CallBack Chrono
	for (i=0; i<MAXCALLBACKCHRONO; i++)
	{
		MaCB[i] = 0;
		TempsCB[i] = 0;
	}

	//Initialisation pour variable Button
	MaCBButton = 0;
	
}

// ****************  EnregistrerFonctionDeRappel ******************************
// Sauve l'adresse de la fonction � rappeler. Lorsque le nombre d'interruptions
// aura atteint temps millisecondes, le syst�me rappellera la fonction
// *************************************************************************
unsigned char OSEnregistrerCB_TIMER(void(*ptFonction)(void), unsigned int tps)
{
	unsigned int i=1; //Attention ,on commence � 1 (IDCB = 0 --> callback non enregistr�e)
	while (MaCB[i]!=0 && i<MAXCALLBACKCHRONO) i++;
	//S'il reste de la place on enregistre et on retourne 0 (op�ration r�ussie)
	if (i<MAXCALLBACKCHRONO)
	{
		MaCB[i] = ptFonction;
		TempsCB[i] = tps;
		TickCB[i] = 0; //Initialiser le compteur � 0;
		return i; // ID du call back
	}
	else return 255; //Il n'y a plus de place pour enregistrer un callback
}


// ****************  Retirer fonction de rappel ******************************
// Lib�re l'emplacement de la callback
// *************************************************************************
void OSRetirerCB_TIMER(unsigned char IDCB)
{
	if ((IDCB >=0) && IDCB<MAXCALLBACKCHRONO)
	{
		MaCB[IDCB] = 0;
		TempsCB[IDCB] = 0;
	}
}

// ****************  Boucle principale de l'OS ******************************
// Boucle infinie qui attend des �v�nement li�s aux interruptions pour
// appeler les fonctions enregistr�es
// *************************************************************************
void OSStart()
{
	// Pour CALLBACKS
	unsigned char idx;
	
	// Pour STATES MACHINE
	unsigned char nextstate;
	PGM_P statetext; // cha�ne de texte sauvegard� dans la m�moire programme
	char (*pStateFunc)(char); // pointeur d'une fonction
	unsigned char input = NULL;
	unsigned char i, j;
	
	// Initialisation des variables qui concernent la machine d'�tats
	state = NULL;
	nextstate = FCT_MENU;
	statetext = NULL;
	pStateFunc = NULL;

	//Cr�ation, configuration et d�marrage de Timer pour g�n�rer une interruption toutes les mS
	//TIMER1_Init_1ms();
	
	// Configuration de l'usart
	//USART_Init_19200();
	
	// Initialisation des interruptions, on autorise toutes les interruptions
	// Pour les interruptions particuli�res, voir chaque fonction
	
	printf("Demarrage OS \n");
	
	

	//Boucle principale de l'OS d'o� on ne sort jamais
	while(1)
	{
		// Check les conditions pour rappeler les fonctions li�es au temps
		for (idx = 1; idx < MAXCALLBACKCHRONO; idx++)
		{
			if (MaCB[idx]) //Si on a l'adresse d'une fonction CB � cet index
			//Si on est arriv� au nombre de mS demand�, on appelle la fonction
			if (TickCB[idx] >= TempsCB[idx])
			{
				TickCB[idx] = 0;
				MaCB[idx]();  //Rappel de la fonction enregistr�e!
			}
		}
		
		// Check si r�ception USART
		/*if (USARTReception)
		{
			USARTReception = 0;
			if (MaCBUSART) MaCBUSART(bufRS232); //Rappel de la fonction enregistr�e!
		}*/
		
		// State Machine
		if (statetext)
		{
			//Afficher_LCD(statetext);
			//lcd_clrscr(); lcd_puts(strcpy_P(Message_LCD, statetext));
			//Usart_Tx_String(strcpy_P(Message_LCD, statetext));
			statetext = NULL;
		}

		// Read buttons
		//input = Button;
		//if(Button !=NONE)Previous_Button=input;
		//Button = NONE;
		
		// When in this state, we must call the state function
		if (pStateFunc)
		{
			nextstate = pStateFunc(input);
		}
		else
		{
			if (input != NULL)
			{
				nextstate = StateMachine(state, input);
				//printf("Appui sur bouton \n");
			}
		}
		if (nextstate != state)  // il y a changement d'�tat
		{
			printf("Changement d'etat \n");
			state = nextstate; // l'�tat est maintenant le nouveau �tat de la s�quence d�finie dans main.h
			for (i=0; (j=pgm_read_byte(&Menu_State[i].state)); i++)
			{
				if (j == state)
				{
					statetext =  (PGM_P) pgm_read_word(&Menu_State[i].pText);
					pStateFunc = (PGM_VOID_P) pgm_read_word(&Menu_State[i].pFunc);
					break;
				}
			}
		}
	}
}

unsigned char StateMachine(char state, unsigned char stimuli)
{
	printf("Dans la fonction state machine \n");
	unsigned char nextstate = state;    // Default stay in same state
	unsigned char i, j;
	for (i=0; ( j=pgm_read_byte(&menu_nextstate[i].state) ); i++ )
	{
		if ( j == state &&
		pgm_read_byte(&menu_nextstate[i].input) == stimuli)
		{
			// This is the one!
			printf("This is the one! \n");
			nextstate = pgm_read_byte(&menu_nextstate[i].nextstate);
			break;
		}
	}
	return nextstate;
}

// Routines INTERRUPTIONS

// ISR ...

//Interruption TIMER1
//ISR(TIMER1_OVF_vect)
//{
	//// R�initialisation TIMER1
	//// POUR 1 MHz
	////TCNT1H = 0xFC;
	////TCNT1L = 0x17;
	//// POUR 8 MHz
	//TCNT1H = 0xE0;
	//TCNT1L = 0xC0;
	//// Incr�menter tous les ticks
	//unsigned char i;
	//for (i = 0; i < MAXCALLBACKCHRONO; i++) TickCB[i]++;
//}