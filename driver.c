/* DRIVER C*/

#include "driver.h"

/* TIM 2 as a general timer for cyclic messages */
void InitPeriodicTimer(U16 timeInMs)
{
  /* setup TIM2 */
  RCC->APB1ENR |= 1;        /* enable TIM2 clock */
  TIM2->PSC = 16000 - 1;    /* divided by 16000  to generate an up timer at 1kHz or 1ms*/
  TIM2->ARR = timeInMs - 1; /* divided by TimeInMs to generate the diseired timeout */
  TIM2->CR1 = 1;            /* enable counter */

  TIM2->DIER |= 1;           /* enable UIE */
  NVIC_EnableIRQ(TIM2_IRQn); /* enable interrupt in NVIC */

  /* setup main board LED to togle each 500 ms for periodic messages */
  RCC->AHB1ENR |= 1; /* enable GPIOA clock */

  GPIOA->MODER &= ~0x00000C00; /* clear the led */
  GPIOA->MODER |= 0x00000400;  /* set the mod as output */
}

void StopPeriodicTimer(void)
{
  RCC->APB1ENR &= ~1;         /* disable TIM2 clock */
  TIM2->CR1 = 0;              /* disable counter */
  TIM2->DIER &= ~1;           /* disable UIE */
  NVIC_DisableIRQ(TIM2_IRQn); /* disable interrupt in NVIC */
}

/* configure SPI1 and the associated GPIO pins */
void SPI1_init(void)
{
  RCC->AHB1ENR |= 1;      /* enable GPIOA clock */
  RCC->APB2ENR |= 0x1000; /* enable SPI1 clock */

  /* PORTA 5, 7 for SPI1 MOSI and SCLK */
  GPIOA->MODER &= ~0x0000CC00;  /* clear pin mode */
  GPIOA->MODER |= 0x00008800;   /* set pin alternate mode */
  GPIOA->AFR[0] &= ~0xF0F00000; /* clear alt mode */
  GPIOA->AFR[0] |= 0x50500000;  /* set alt mode SPI1 */

  /* PA9 as GPIO output for SPI slave select */
  GPIOA->MODER &= ~0x000C0000; /* clear pin mode */
  GPIOA->MODER |= 0x00040000;  /* set pin output mode */

  SPI1->CR1 = 0x31F;
  SPI1->CR2 = 0;
  SPI1->CR1 |= 0x40; /* enable SPI1 module */
}

/* This function enables slave select, writes one byte to SPI1, */
/* wait for transmit complete and deassert slave select. */
void SPI1_write(U8 data)
{
  while (!(SPI1->SR & 2))
  {
  }                         /* wait until Transfer buffer Empty */
  GPIOA->BSRR = 0x02000000; /* assert slave select */
  SPI1->DR = data;          /* write data */
  while (SPI1->SR & 0x80)
  {
  }                         /* wait for transmission done */
  GPIOA->BSRR = 0x00000200; /* deassert slave select */
}

/* Lucrarea 3 */
void SystemTickTimerInit(U32 STRVR, U16 STCTRL)
{
  /* setup LED1 to toggle */
  RCC->AHB1ENR |= 2; /* enable GPIOB clock */

  GPIOB->MODER &= ~0x00000C00; /* Reset GPIOB PB5  */
	GPIOB->MODER |= 0x00000400;	 /* Set GPIOB PB5  as ouput */
  __disable_irq();
  /* Configure SysTick */
  SysTick->LOAD = STRVR - 1; /*reload with number of clocks per second */
  SysTick->VAL = 0;
  SysTick->CTRL = STCTRL; /* enable it, no interrupt, use system clock */

  SysTick->CTRL |= 2; /* enable interrupt */
  __enable_irq();
}

void SystemTickTimerStop(void)
{
	SysTick->CTRL = 0;
}

void TIM3_Cfg(U16 TIM3_PSC, U16 TIM3_ARR)
{
	__disable_irq();
	/* setup main board LED to toggle */
	RCC->AHB1ENR |= 1; /* enable GPIOA clock */

	// configure TIM3 to wrap around at 40ms
	RCC->APB1ENR |= 2;	  /* enable TIM3 clock */
	TIM3->PSC = TIM3_PSC - 1;  
	TIM3->ARR = TIM3_ARR - 1; 
	TIM3->CNT = 0;		  /* clear timer counter */
	TIM3->CR1 = 1;		  /* enable TIM3 */

	TIM3->DIER |= 1;		   /* enable UIE */
	NVIC_EnableIRQ(TIM3_IRQn); /* enable interrupt in NVIC */
	__enable_irq();
}

void TIM3_Stop(void)
{
	TIM3->CR1 = 0;				/* disable counter */
	TIM3->DIER &= ~1;			/* disable UIE */
	NVIC_DisableIRQ(TIM3_IRQn); /* disable interrupt in NVIC */
}

void TIM3_CH1_Compare_Cfg(U16 TIM3_CH1_PSC, U16 TIM3_CH1_ARR, U16 TIM3_CH1_CCMR1, U16 TIM3_CH1_CCR1)
{
	// configure PB4 as output to drive the LED
	RCC->AHB1ENR |= 2;			 /* enable GPIOB clock */
	GPIOB->MODER &= ~0x00000300; /* clear pin mode */
	GPIOB->MODER |= 0x00000200;	 /* set pin to alternate function */
	GPIOB->AFR[0] &= 0x000F0000; /* clear pin AF bits */
	GPIOB->AFR[0] |= 0x00020000; /* set pin to AF2 for TIM3 CH1 */

	// configure TIM3
	// and toggle CH1 output when match
	RCC->APB1ENR |= 2;	  /* enable TIM3 clock */
	TIM3->PSC = TIM3_CH1_PSC - 1;  /* TODO: */
	TIM3->ARR = TIM3_CH1_ARR - 1; /*TODO:  */
	TIM3->CCMR1 = TIM3_CH1_CCMR1;	  /* TODO:  set output to toggle when match */
	TIM3->CCR1 = TIM3_CH1_CCR1;		  /* TODO: set match value */
	TIM3->CCER |= 1;	  /* enable CH1 compare mode */
	TIM3->CNT = 0;		  /* clear counter */
	TIM3->CR1 = 1;		  /* enable TIM3 */
}

void TIM8_CH3_Capture_Cfg(U16 TIM8_CH3_PSC, U16 TIM8_CH3_CCMR2)
{
	__disable_irq();
	// configure PC8 as input of TIM3 CH 1
	RCC->AHB1ENR |= 4;			  /* enable GPIOC clock */
	GPIOC->MODER &= ~0x00030000;  /* clear pin mode */
	GPIOC->MODER |= 0x00020000;	  /* set pin to alternate function */
	GPIOC->AFR[1] &= ~0x0000000F; /* clear pin AF bits */
	GPIOC->AFR[1] |= 0x00000003;  /* set pin to AF2 for TIM8 CH3 */

	// configure TIm8 CH 3 to inpute capture
	RCC->APB2ENR |= 2;	   /* enable TIM8 clock */
	TIM8->PSC = TIM8_CH3_PSC - 1; /* TODO: */
	TIM8->CCMR2 = TIM8_CH3_CCMR2;	   /* TODO:  set  CH3 to capture at every edge*/
	TIM8->CCER = 0x0100;   /* enable CH3 capture rising edge */
	TIM8->CR1 = 1;		   /* enable TIM8 */

	TIM8->DIER |= 8;			  /* enable CC3IE */
	NVIC_EnableIRQ(TIM8_CC_IRQn); /* enable interrupt in NVIC */
	__enable_irq();
}

void TIM8_Stop(void)
{
	TIM8->CR1 = 0;				   /* disable counter */
	TIM8->DIER &= ~8;			   /* disable CC3IE */
	NVIC_DisableIRQ(TIM8_CC_IRQn); /* disable interrupt in NVIC */
}

void InitTIM8_CH1_PWM(void)
{
  RCC->AHB1ENR |= 1; /* enable GPIOA clock */

  // configure PA5 as TIM8_CH1 PWM output pin
  GPIOA->AFR[0] |= 0x00300000; /* setam functia alternativa tim8 Ch1N */
  GPIOA->MODER &= ~0x00000C00;
  GPIOA->MODER |= 0x00000800;

  /* setup TIM8_CH1 PWM */
  RCC->APB2ENR |= 2;     /* enable TIM8 clock */
  TIM8->PSC = 10 - 1;    /* divided by 10 */
  TIM8->ARR = 26667 - 1; /* divided by 26667 */
  TIM8->CNT = 0;
  TIM8->CCMR1 = 0x0068; /* PWM mode */
  TIM8->CCER = 4;       /* enable PWM Ch1N */
  TIM8->CCR1 = 90;      /* pulse width */
  TIM8->BDTR |= 0x8000; /* enable output */
  TIM8->CR1 = 1;        /* enable timer */
}
void StopTIM8_CH1_PWM(void)
{
  TIM8->CR1 = 0; /* disable timer */
}
void SetTIM8_CH1_PWM_DutyCycle(U8 DC)
{
  TIM8->CCR1 = 266 * DC;
}


/* Lucrarea 4*/
void ADC_Configure(U32 MODER, U32 ADC_CR1, U32 ADC_CR2)
{
  RCC->AHB1ENR |=  1;	            /* pornire semanl tact pentru GPIOA */
  GPIOC->MODER |=  MODER; ;    /* PA0-PA5 analog */

  /* configurare ADC1 */
  RCC->APB2ENR |= 0x00000100;     /* pornire semnal de tact pentru ADC1 */
  ADC1->CR1 = ADC_CR1;
  ADC1->CR2 = ADC_CR2;                
}

U16 ADC_SampleChannel(U8 channel)
{
  ADC1->CR1 &= ~0x0000001F;
  ADC1->CR1 |= (channel & 0x1F); /* conversie canal */
  ADC1->SQR3 = (channel & 0x1F);
  ADC1->SQR1 = 0;
  if ((channel & 0x1F) == 18)
  {
    ADC->CCR |= 0x800000;  // enable temp
    ADC->CCR &= ~0x400000; // disable vbate
  }
  ADC1->CR2 |= 1;

  ADC1->CR2 = 0x40000001; /* start conversie */

  while (!(ADC1->SR & 2))
  {
  }                   /* asteapta sa finalizeze */
  return ADC1->DR;    /* preia rezultat */
}

void ADC_Stop(void)
{
  ADC1->CR2 = 0;
}

void DAC_Configure(U16 Init_Hold)
{
  RCC->AHB1ENR |= 1;
  GPIOA->MODER |= 0x00000C00; /*PA5 output*/

  RCC->APB1ENR |= 1 << 29;
  DAC->CR |= 1;
  DAC->DHR12R1 = Init_Hold & 0x0FFF;
}

void DAC_Stop(void)
{
  DAC->CR = 0;
}

/* lucrarea 5*/

void SetupIOInterrupt(U32 MODERB, U32 MODERC, U32 PUPDR, U16 EXTI_FTSR, U16 EXTI_RTSR) {
   __disable_irq();                    /* global disable IRQs */

    RCC->AHB1ENR |= 4;	                /* enable GPIOC clock */
    RCC->AHB1ENR |= 2;                  /* enable GPIOB clock */
    RCC->APB2ENR |= 0x4000;             /* enable SYSCFG clock */

    /* configure LEDs */
    GPIOB->MODER &= ~0x0000ff00;    /* clear pin mode */
    GPIOB->MODER |=  0x00005500;    /* set pins to output mode */

	  GPIOC->MODER &= ~MODERC; /* Reset GPIOC PC[11..8]  for input mode */

	  GPIOB->MODER &= ~MODERB; /* Reset GPIOB PB[15..12]  */
	  GPIOB->PUPDR |= PUPDR;	 /* Enable pull-ups on GPIOB PB[15..12]  */
    
    SYSCFG->EXTICR[3] &= ~0xFFFF;       /* clear port selection for EXTI10-15 */
    SYSCFG->EXTICR[3] |= 0x1111;        /* select port B for EXTI10_15 */

    SYSCFG->EXTICR[2] &= ~0xFFFF;       /* clear port selection for EXTI9_5, 10_15 */
    SYSCFG->EXTICR[2] |= 0x2222;        /* select port C for EXTI9_5, 10_15 */
    
    EXTI->FTSR |= EXTI_FTSR;               /* select falling edge trigger for PB8-11*/
    EXTI->RTSR |= EXTI_RTSR;               /* select rising edge trigger for PC12-15*/

    NVIC_EnableIRQ(EXTI15_10_IRQn);     /* enable interrupt in NVIC */
    NVIC_EnableIRQ(EXTI9_5_IRQn);     /* enable interrupt in NVIC */

    __enable_irq();                     /* global enable IRQs */
}

void EnableIRQForIO(U8 pin) {
    EXTI->IMR = 0;
    EXTI->IMR |= (1<<pin);
}

void DisableIRQForIO(U8 pin) {
  EXTI->IMR &=~ (1<<pin);
	GPIOB->ODR = 0x00000000;
}

/*----------------------------------------------------------------------------
  Initialize UART pins, Baudrate
 *----------------------------------------------------------------------------*/
void USART2_init(void)
{
  RCC->AHB1ENR |= 1;       /* Enable GPIOA clock */
  RCC->APB1ENR |= 0x20000; /* Enable USART2 clock */

  /* Configure PA2 and PA3 for USART2_TX and USART2_RX */
  GPIOA->AFR[0] &= ~0xFF00;
  GPIOA->AFR[0] |= 0x7700; /* alt7 for USART2 */
  GPIOA->MODER &= ~0x00F0;
  GPIOA->MODER |= 0x00A0; /* enable alternate function for PA2 and PA3 */

  USART2->BRR = 0x008B;  /* 115200 baud @ 16 MHz */
  USART2->CR1 = 0x000C;  /* enable Tx/Rx, 8-bit data */
  USART2->CR2 = 0x0000;  /* 1 stop bit */
  USART2->CR3 = 0x0000;  /* no flow control */
  USART2->CR1 |= 0x2000; /* enable USART2*/
}

/* Write a character to USART2 */
int USART2_write(int ch)
{
  while (!(USART2->SR & 0x0080))
  {
  } // wait until Tx buffer empty
  USART2->DR = (ch & 0xFF);
  return ch;
}

/* Read a character from USART2 */
int USART2_read(void)
{
  while (!(USART2->SR & 0x0020))
  {
  } // wait until char arrives
  return USART2->DR;
}

/* The code below is the interface to the C standard I/O library.
 * All the I/O are directed to the console, which is UART2.
 */
// struct __FILE { int handle; };
FILE __stdin = {0};
FILE __stdout = {1};
FILE __stderr = {2};

/* Called by C library console/file input
 * This function echoes the character received.
 * If the character is '\r', it is substituted by '\n'.
 */
int fgetc(FILE *f)
{
  int c;

  c = USART2_read(); /* read the character from console */

  if (c == '\r')
  {                  /* if '\r', after it is echoed, a '\n' is appended*/
    USART2_write(c); /* echo */
                     // c = '\n';
  }

  USART2_write(c); /* echo */

  return c;
}

/* Called by C library console/file output */
int fputc(int c, FILE *f)
{
  return USART2_write(c); /* write the character to console */
}

