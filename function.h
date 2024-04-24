#ifndef FUNCTION_H
	#define FUNCTION_H

	#include "stm32f4xx.h"
	#include <stdio.h>
	#include <string.h>
	
	//registers+memory
	void load_word(char word[16]);
	void load_alphabet();
	
	//LCD
	void set_LCD();
	//random select from memory
	void write_LCD(char l);
	int game_over_finish();
	
	//7 segment
	void set_counter();
	void counter();
	void print_tries();
	
	//buttons
	void config_buttons();
	void choose_letter();
	
	
	#endif