/*
 * main.h
 *
 * Created: 21/11/2017 11:32:24
 *  Author: 101007423
 */ 


#ifndef MAIN_H_
#define MAIN_H_
// DEFINE

// Gestion registre par bit unique
#define TRUE				1
#define FALSE				0
#define NULL				0


// Liste des états
#define FCT_MENU					 10
#define ST_MENU						 11

enum {OFF, ON};

// DECLARATION DES FONCTIONS

// CALLBACKS
void Switch_Led(void);


// STATE MACHINE

char Fct_menu(char input);


#endif /* MAIN_H_ */