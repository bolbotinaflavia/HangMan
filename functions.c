#include "stm32f4xx.h"
	#include <stdio.h>
	#include <string.h>
	#include "function.h"
	
	void load_word(char word[16]){
	
		__ASM  {
			mov EAX,word
			
	}
	
	}