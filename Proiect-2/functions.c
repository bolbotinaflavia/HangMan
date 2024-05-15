#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "function.h"

#define RS 1    /* BIT0 mask for reg select */
#define EN 2    /* BIT1 mask for E */

/* configure SPI1 and the associated GPIO pins */
void LCD_init(void) {
    RCC->AHB1ENR |= 1;              /* enable GPIOA clock */
    RCC->AHB1ENR |= 4;              /* enable GPIOC clock */
    RCC->APB2ENR |= 0x1000;         /* enable SPI1 clock */

    /* PORTA 5, 7 for SPI1 MOSI and SCLK */
    GPIOA->MODER &= ~0x0000CC00;    /* clear pin mode */
    GPIOA->MODER |=  0x00008800;    /* set pin alternate mode */
    GPIOA->AFR[0] &= ~0xF0F00000;   /* clear alt mode */
    GPIOA->AFR[0] |=  0x50500000;   /* set alt mode SPI1 */

    /* PA12 as GPIO output for SPI slave select */
    GPIOA->MODER &= ~0x03000000;    /* clear pin mode */
    GPIOA->MODER |=  0x01000000;    /* set pin output mode */

    /* initialize SPI1 module */
    SPI1->CR1 = 0x31F;
    SPI1->CR2 = 0;
    SPI1->CR1 |= 0x40;              /* enable SPI1 module */

    /* LCD controller reset sequence */
    //delayMs(20);
    LCD_nibble_write(0x30, 0);
    //delayMs(5);
    LCD_nibble_write(0x30, 0);
    //delayMs(1);
    LCD_nibble_write(0x30, 0);
    //delayMs(1);
    LCD_nibble_write(0x20, 0);  /* use 4-bit data mode */
    //delayMs(1);
    LCD_command(0x28);          /* set 4-bit data, 2-line, 5x7 font */
    LCD_command(0x06);          /* move cursor right */
    LCD_command(0x01);          /* clear screen, move cursor to home */
    LCD_command(0x0F);          /* turn on display, cursor blinking */
}

void LCD_nibble_write(char data, unsigned char control) {
    data &= 0xF0;       /* clear lower nibble for control */
    control &= 0x0F;    /* clear upper nibble for data */
    SPI1_write (data | control);           /* RS = 0, R/W = 0 */
    SPI1_write (data | control | EN);      /* pulse E */
    delayMs(0);
    SPI1_write (data);
}

void LCD_command(unsigned char command) {
    LCD_nibble_write(command & 0xF0, 0);    /* upper nibble first */
    LCD_nibble_write(command << 4, 0);      /* then lower nibble */

    if (command < 4)
        delayMs(2);         /* command 1 and 2 needs up to 1.64ms */
    else
        delayMs(1);         /* all others 40 us */
}

void LCD_data(char data) {
    LCD_nibble_write(data & 0xF0, RS);      /* upper nibble first */
    LCD_nibble_write(data << 4, RS);        /* then lower nibble */

    //delayMs(1);
}

/* This function enables slave select, writes one byte to SPI1, */
/* wait for transmit complete and deassert slave select. */
void SPI1_write(unsigned char data) {
    while (!(SPI1->SR & 2)) {}      /* wait until Transfer buffer Empty */
    GPIOA->BSRR = 0x10000000;       /* assert slave select */
    SPI1->DR = data;                /* write data */
    while (SPI1->SR & 0x80) {}      /* wait for transmission done */
    GPIOA->BSRR = 0x00001000;       /* deassert slave select */
}
//void SPI1_write(unsigned char data) {
  //  while (!(SPI1->SR & 2)) {}      /* wait until Transfer buffer Empty */
    //GPIOA->BSRR = 0x00100000;       /* assert slave select */
    //SPI1->DR = data;                /* write data */
    //while (SPI1->SR & 0x80) {}      /* wait for transmission done */
    //GPIOA->BSRR = 0x00000010;       /* deassert slave select */
//}
/* 16 MHz SYSCLK */
void delayMs(int n) {
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++) ;
}

//void LED_switch(void){
	//RCC->AHB1ENR |= 4; /* enable GPIOC clock / RCC->AHB1ENR |= 1; /
//enable GPIOA clock */
	//GPIOA->MODER &= ~0x00000C00; /* clear pin mode */ 
	//GPIOA->MODER |= 0x00000400; /* set pin to output mode */
	//GPIOC->MODER &= ~0x0C000000; /* clear pin mode to input mode */
	//while(1) {
		//if (GPIOC->IDR & 0x2000) /* if PC13 is high */
			//GPIOA->BSRR = 0x00200000; /* turn off green LED */ 
		//else
			//GPIOA->BSRR = 0x00000020;  
	//}
//}

char* alege_cuvant_aleatoriu(char *cuvinte[]) {
    //int index = rand() % (sizeof(cuvinte) / sizeof(cuvinte[0]));  // Genereaza un index aleatoriu
	int index = rand() % 5;
    return cuvinte[index];
}

void init_hangman(char *cuvinte[]){
	char *cuvant = alege_cuvant_aleatoriu(cuvinte);  // Alege un cuvânt aleatoriu
    int lungime = strlen(cuvant);
    
    // Afi?eaza liniu?ele corespunzatoare lungimii cuvântului
    for (int i = 0; i < lungime; i++) {
        //LCD_data('_');
				LCD_data(cuvant[i]);
    }
		//delayMs(1000);
				
				
      //  LCD_command(1);
	
	/*while(1) {
		
				//char vector[10];
        
        LCD_data('_');
        LCD_data('_');
        LCD_data('_');
        LCD_data('_');
        LCD_data('_');
				
				delayMs(1000);
				delayMs(1000);
				delayMs(1000);
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
}

/* p8_1.c: Using SPI1 to send A to Z characters
 *
 * SPI1 is configured as master with software slave select.
 * Clock rate is set to 1 MHz. Polarity/Phase are 0, 0
 *
 * PA7  MOSI
 * PA5  SCK
 * PA4  SS
 *
 * This program was tested with Keil uVision v5.24a with DFP v2.11.0
 */


/* enable SPI1 and associated GPIO pins */
void SPI1_init(void) {
    RCC->AHB1ENR |= 1;              /* enable GPIOA clock */
    RCC->APB2ENR |= 0x1000;         /* enable SPI1 clock */

    /* PORTA 5, 7 for SPI1 MOSI and SCLK */
    GPIOA->MODER &= ~0x0000CC00;    /* clear pin mode */
    GPIOA->MODER |=  0x00008800;    /* set pin alternate mode */
    GPIOA->AFR[0] &= ~0xF0F00000;   /* clear alt mode */
    GPIOA->AFR[0] |=  0x50500000;   /* set alt mode SPI1 */

    /* PORTA4 as GPIO output for SPI slave select */
    GPIOA->MODER &= ~0x00000300;    /* clear pin mode */
    GPIOA->MODER |=  0x00000100;    /* set pin output mode */
    
    SPI1->CR1 = 0x31C;		   /* set the Baud rate, 8-bit data frame */
    SPI1->CR2 = 0;
    SPI1->CR1 |= 0x40;              /* enable SPI1 module */
}

/* This function enables slave select, writes one byte to SPI1,
   wait for transmission complete and deassert slave select. */


/* 16 MHz SYSCLK */

void init_litere() {
    char litera = 'A';
    for (int i = 0; i < 26; i++) {
        litere[i].litera = litera++;
        litere[i].contor = 0;
    }
}

//void parse_alphabet(void){
	//char c;
	//int ok=1;
    
    //SPI1_init();
    
    /*while(1) {
        for (c = 'A'; c <= 'Z'; c++) {
            SPI1_write(c);      
            delayMs(100);
        }
    }*/
		
		
//}