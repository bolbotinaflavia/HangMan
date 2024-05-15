#ifndef FUNCTION_H
	#define FUNCTION_H

	#include "stm32f4xx.h"
	#include <stdio.h>
	#include <string.h>
	#define SW3_PIN 0 // PA0 (GPIOA Pin 0)
#define SW4_PIN 1 // PA1 (GPIOA Pin 1)
#define SW5_PIN 2 // PA2 (GPIOA Pin 2)

	
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
	
	
	
	
	typedef struct{
		char litera;
		int contor;
	}Litera;
	//buttons
	//void config_buttons();
	void choose_letter(Litera Litere[]);
	
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
	void init_litere(Litera litere[]);
	void alegere_litere(void);
	
	//switch-uri
	void init_switch(void);
	uint8_t read_switch(void);
	
	//litere
		int find_letter(char x,char cur_word[]);
	
	#endif