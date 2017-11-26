/*
 * OS.h
 *
 * Created: 21/11/2017 11:33:34
 *  Author: 101007423
 */ 


#ifndef OS_H_
#define OS_H_
//**********************************************************
//********** FONCTIONS POUR LE MOTEUR OS *******************
//**********************************************************

//Initialisation OS
void OSInitialiser(void);

//Enregistrer des fonctions callback liées au temps
//Retourne un ID associé à l'enregistrement
unsigned char OSEnregistrerCB_TIMER(void(*ptFonction)(void), unsigned int temps);

//Retirer des fonctions callback liées au temps, prend l'ID du CallBack comme argument
void OSRetirerCB_TIMER(unsigned char IDCB);

//Démarrage de la boucle principale
void OSStart(void);

//Fonction qui recherche l'état suivant par rapport à l'état actuel et une nouvelle entrée (stimuli)
unsigned char StateMachine(char state, unsigned char stimuli);




#endif /* OS_H_ */