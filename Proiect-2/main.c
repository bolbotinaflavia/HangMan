#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "function.h"
#include <stdlib.h>
#include <time.h>
	Litera litere[26];
	
	
char *cuvinte[10]={"HANGMAN","TRIMIS","JOC","EXEMPLU","NUCLEO","CUVANT","LABORATOR","PLACA","ARDUINO"};

char cuv_curent[16];
int main(void) {
		
	    /* initialize LCD controller */
	
	LCD_init();
	LCD_command(0x01);
	init_litere(litere);
	delayMs(100);
    
		init_7seg();
	srand(time(0));
	strcpy(cuv_curent,init_hangman(cuvinte,litere));

	LCD_command(0x01);
	show_word(cuv_curent,litere);
	delayMs(100);

	choose_letter(litere,cuv_curent);
	
}