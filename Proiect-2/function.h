#ifndef FUNCTION_H
	#define FUNCTION_H

	#include "stm32f4xx.h"
	#include <stdio.h>
	#include <string.h>
	#define SW3_PIN 0 // PA0 (GPIOA Pin 0)
#define SW4_PIN 1 // PA1 (GPIOA Pin 1)
#define SW5_PIN 2 // PA2 (GPIOA Pin 2)
void LCD_command_noPoll(unsigned char command);
	
	
	typedef struct{
		char litera;
		int contor;
	}Litera;

	
	void show_word(char *word,Litera *litere);
	void SPI1_write2(unsigned char data);
	void SPI1_init2(void);
	void init_7seg(void);
	void afis_7seg(int incercari);
	void SPI1_write2(unsigned char data);
	void SPI1_init2(void);

	void choose_letter(Litera *Litere,char *cuvant);
	
	void delayMs(int n);
void LCD_nibble_write(char data, unsigned char control);
void LCD_command(unsigned char command);
	void afis_mesaj_final(int ok);
	void LCD_clear_second_line(unsigned char command);
	int word(char *word,Litera *litere);
void LCD_data(char data);
void LCD_init(void);
void SPI1_write(unsigned char data);
void LED_switch(void);
char* init_hangman(char **cuvinte,Litera *litere);
char* alege_cuvant_aleatoriu(char **cuvinte,int *index);
void delayMs(int n);
void SPI1_write(unsigned char data);
void SPI1_init(void);
void parse_alphabet(void);
	void init_litere(Litera *litere);
	void alegere_litere(void);
	int return_contor(char x,Litera *litere);
	void change_contor(char x,Litera *litere);
	
	//switch-uri
	void init_switch(void);
	uint8_t read_switch(void);
	
	//litere
		int find_letter(char x,char *cur_word,Litera *litere);
	
	#endif