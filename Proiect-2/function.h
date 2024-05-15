#ifndef FUNCTION_H
	#define FUNCTION_H

	#include "stm32f4xx.h"
	#include <stdio.h>
	#include <string.h>
	
	//registers+memory
	//void load_word(char word[16]);
	//void load_alphabet();
	
	//LCD
	//void set_LCD();
	//random select from memory
	//void write_LCD(char l);
	//int game_over_finish();
	
	//7 segment
	//void set_counter();
	//void counter();
	//void print_tries();
	
	//buttons
	//void config_buttons();
	//void choose_letter();
	
	typedef struct{
		char litera;
		int contor;
	}Litera;
	Litera litere[26];
	char litera_curenta = 'A'; 
	
	void delayMs(int n);
void LCD_nibble_write(char data, unsigned char control);
void LCD_command(unsigned char command);
void LCD_data(char data);
void LCD_init(void);
void SPI1_write(unsigned char data);
void LED_switch(void);
void init_hangman(char *cuvinte[]);
char* alege_cuvant_aleatoriu(char *cuvinte[]);
void delayMs(int n);
void SPI1_write(unsigned char data);
void SPI1_init(void);
void parse_alphabet(void);
	void init_litere(void);
	void alegere_litere(void);
	#endif