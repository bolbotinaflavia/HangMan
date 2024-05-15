#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include "function.h"
#include <stdlib.h>
Litera litere[26];
	char litera_curenta = 'A'; 
	
char *cuvinte[] = {"EXEMPLU", "HANGMAN", "JOC", "PLACA", "NUCLEO"};
int main(void) {
    /* initialize LCD controller */
    LCD_init();
		init_hangman(cuvinte);
	  init_litere(litere);
		choose_letter(litere);
	//init_switch();

	
	/*while(1) {
        
        LCD_data('_');
        LCD_data('_');
        LCD_data('_');
        LCD_data('_');
        LCD_data('_');
				LCD_data(' ');
				LCD_data(' ');
				LCD_data(' ');
				LCD_data(' ');
				LCD_data(' ');
				LCD_data(' ');
				LCD_data(' ');
				LCD_data(' ');
				LCD_data(' ');
				LCD_data(' ');
        delayMs(1000);
        LCD_command(1);
        delayMs(500);
    }*/
		
		//LED_switch();
	//RCC->AHB1ENR |= 4; 
		/* enable GPIOC clock / RCC->AHB1ENR |= 1; /
enable GPIOA clock */
	//GPIOA->MODER &= ~0x00000C00; /* clear pin mode */ 
	//GPIOA->MODER |= 0x00000400; /* set pin to output mode */
	//GPIOC->MODER &= ~0x0C000000; /* clear pin mode to input mode */
	//while(1) {
		//if (GPIOC->IDR & 0x2000) /* if PC13 is high */
			//GPIOA->BSRR = 0x00200000; /* turn off green LED */ 
		//else
			//GPIOA->BSRR = 0x00000020;  
}