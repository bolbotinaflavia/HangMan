#include "stm32f4xx.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "function.h"

#define RS 1    /* BIT0 mask for reg select */
#define EN 2    /* BIT1 mask for E */
#define RW 0x40 

void afis_mesaj_final(int ok){
	LCD_command(0x01);
	if(ok){
		LCD_data('A');
		LCD_data('I');
		LCD_data(' ');
		LCD_data('G');
		LCD_data('H');
		LCD_data('I');
		LCD_data('C');
		LCD_data('I');
		LCD_data('T');
		LCD_data('!');
		LCD_data(':');
		LCD_data(')');
	}
	else{
		LCD_data('G');
		LCD_data('A');
		LCD_data('M');
		LCD_data('E');
		LCD_data(' ');
		LCD_data('O');
		LCD_data('V');
		LCD_data('E');
		LCD_data('R');
		LCD_data(':');
		LCD_data('(');
	}
	delayMs(100);
}
void init_7seg(void){
	SPI1_init2();
	SPI1_write2(~0x7D); //afis 6 
	SPI1_write2(1 << 0);
}
void SPI1_write2(unsigned char data) {
    while (!(SPI1->SR & 2)) {}      /* wait until Transfer buffer Empty */
    GPIOA->BSRR = 0x02000000;       /* assert slave select */
    SPI1->DR = data;                /* write data */
    while (SPI1->SR & 0x80) {}      /* wait for transmission done */
    GPIOA->BSRR = 0x00000200;       /* deassert slave select */
}
void SPI1_init2(void) {
    RCC->AHB1ENR |= 1;              /* enable GPIOA clock */
    RCC->APB2ENR |= 0x1000;         /* enable SPI1 clock */

    /* PORTA 5, 7 for SPI1 MOSI and SCLK */
    GPIOA->MODER &= ~0x0000CC00;    /* clear pin mode */
    GPIOA->MODER |=  0x00008800;    /* set pin alternate mode */
    GPIOA->AFR[0] &= ~0xF0F00000;   /* clear alt mode */
    GPIOA->AFR[0] |=  0x50500000;   /* set alt mode SPI1 */

    /* PA9 as GPIO output for SPI slave select */
    GPIOA->MODER &= ~0x000C0000;    /* clear pin mode */
    GPIOA->MODER |=  0x00040000;    /* set pin output mode */

    SPI1->CR1 = 0x31F;
    SPI1->CR2 = 0;
    SPI1->CR1 |= 0x40;              /* enable SPI1 module */
}

void afis_7seg(int incercari){
	
		switch(incercari){
			case 5:{
				SPI1_write2(~0x6D); //afis 5
				SPI1_write2(1 << 0);
				break;
				}	
			case 4:{
				SPI1_write2(~0x66); //afis 4
				SPI1_write2(1 << 0);
				break;
				}	
			case 3:{
				SPI1_write2(~0x4F); //afis 3
				SPI1_write2(1 << 0);
				break;
				}	
			case 2:{
				SPI1_write2(~0x5B); //afis 2
				SPI1_write2(1 << 0);
				break;
				}	
			case 1:{
				SPI1_write2(~0x06); //afis 1
				SPI1_write2(1 << 0);
				break;
				}	
			case 0:{
				SPI1_write2(~0x3F); //afis 0
				SPI1_write2(1 << 0);
				break;
				}	
		}
	}
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
		//LCD_command(0xC0); //Force cursor to beginning of 2nd line
		//LCD_command(0x38); //Force cursor to beginning of 1st line
}
void LCD_command_noPoll(unsigned char command) {
    GPIOB->BSRR = (RS | RW) << 16;  /* RS = 0, R/W = 0 */
    GPIOC->ODR = command;           /* put command on data bus */
    GPIOB->BSRR = EN;               /* pulse E high */
    delayMs(0);
    GPIOB->BSRR = EN << 16;         /* clear E */
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

/* 16 MHz SYSCLK */
void delayMs(int n) {
    int i;
    for (; n > 0; n--)
        for (i = 0; i < 3195; i++) ;
}


char* alege_cuvant_aleatoriu(char **cuvinte,int *index1) {
    //int index = rand() % (sizeof(cuvinte) / sizeof(cuvinte[0]));  // Genereaza un index aleatoriu	
		int index = rand() % 10;  // Genereaza un index aleatoriu
    return cuvinte[index];
}

char* init_hangman(char **cuvinte,Litera *litere){

	char *cuvant = alege_cuvant_aleatoriu(cuvinte,0);  // Alege un cuvânt aleatoriu
    int lungime = strlen(cuvant);
    
    // Afi?eaza liniu?ele corespunzatoare lungimii cuvântului
    for (int i = 0; i < lungime; i++) {
      //  LCD_data('_');
				LCD_data(cuvant[i]);
    }
		return cuvant;
}

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

//litere
void init_litere(Litera *litere) {
	
    char litera = 'A';
    for (int i = 0; i < 26; i++) {
        litere[i].litera= litera;
		    litere[i].contor=0;
				litera++;
    }
		delayMs(100);
}

int find_letter(char x,char *cur_word,Litera *litere){
	unsigned int i;
	int ok=0;
	for(i=0;i<strlen(cur_word);i++){
		if(cur_word[i]==x){
			 ok=1;
			 change_contor(x,litere);
		}
	}
	if(ok==1)
		return 1;
	else
		return 0;
}


		

//initializare si citire switch uri

void init_switch() {
    // Enable GPIO clock for the port connected to the switches (GPIOC)
    RCC->AHB1ENR |= RCC_AHB1ENR_GPIOCEN;

    // Configure pins for input mode with pull-up resistors
    GPIOC->MODER &= ~(GPIO_MODER_MODE8_Msk | GPIO_MODER_MODE9_Msk | GPIO_MODER_MODE10_Msk); // Clear mode bits for PC8, PC9, and PC10
    GPIOC->PUPDR |= GPIO_PUPDR_PUPD8_0 | GPIO_PUPDR_PUPD9_0 | GPIO_PUPDR_PUPD10_0; // Enable pull-up for PC8, PC9, and PC10
}

uint8_t read_switch() {
    uint8_t switchesState = (GPIOC->IDR >> 8) & 0x07; // Read PC8, PC9, and PC10
    
    // Invert logic if switches are active low
    //switchesState = ~switchesState; // If switches are active high, you can remove this line
    
    return switchesState;
}

int return_contor(char x,Litera *litere){
		for(int i=0;i<26;i++){
			if(litere[i].litera==x)
				return litere[i].contor;
		}
		return 0;
}
void change_contor(char x,Litera *litere){
		for(int i=0;i<26;i++){
			if(litere[i].litera==x)
				litere[i].contor=1;
		}
}

void show_word(char* word,Litera *litere){
	 int lungime = strlen(word);
	//	LCD_command(0x38);
	  
    // Afi?eaza liniutele corespunzatoare lungimii cuvântului
    for (int i = 0; i < lungime; i++) {
			
      if(return_contor(word[i],litere)!=0)
				 LCD_data(word[i]);
			else
				LCD_data('_');
				
    }
		LCD_command(0xC0);
}

int word(char *word,Litera *litere){
	int lungime = strlen(word);
	int ok=0;
	for (int i = 0; i < lungime; i++) 
			
      if(return_contor(word[i],litere)!=0)
				ok++;
	if(lungime==ok)
		return 1;
	else
		return 0;
}

void choose_letter(Litera *litere,char *cuvant){
	int tries;
	tries=6;
	int ok;
	ok=0;
	LCD_command(0xC0);
  // RCC->AHB1ENR |=  2;             /* enable GPIOB clock */
    RCC->AHB1ENR |=  4;             /* enable GPIOC clock */

   // GPIOB->MODER &= ~0x0000ff00;    /* clear pin mode */
    GPIOB->MODER |=  0x00005500;    /* set pins to output mode */
   // GPIOC->MODER &= ~0x00FF0000;    /* clear pin mode */

   
        
    int i = 0;
		
		LCD_data(litere[i].litera);
    while (1) {
				//conditie game over
			if(tries==0){
				
				afis_mesaj_final(0);
				break;
			}
			//conditie castigat
			if(word(cuvant,litere)){
				afis_mesaj_final(1);
				break;
			}
			
			
			if (GPIOC->IDR&0x0200) {
						if(find_letter(litere[i].litera,cuvant,litere)){
								LCD_command(0x01);
								show_word(cuvant,litere);
							}
						else{
							tries=tries-1;
							afis_7seg(tries);
							delayMs(100);
							//break;
						}
						LCD_command(0xC0);
           // break;
        }
        if (GPIOC->IDR&0x0100) {
            if (i + 1 >= 26)
                i = 0;
            else
                i++;
						
						LCD_command(0xC0);
						delayMs(100);
						LCD_data(litere[i].litera);
        }

        

        if (GPIOC->IDR&0x0400) {
            if (i - 1 <=-1)
                i = 25;
            else
                i--;
						
					LCD_command(0xC0);
						delayMs(100);
						LCD_data(litere[i].litera);
					
        }
    }
}
//}