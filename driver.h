#ifndef DRIVERS_H
#define DRIVERS_H

#include "types.h"

void USART2_init(void);
int USART2_write(int c);
int USART2_read(void);

void SPI1_init(void);
void SPI1_write(U8);

void InitPeriodicTimer(U16 timeInMs);
void StopPeriodicTimer(void);

/* Lucrarea 3*/
void SystemTickTimerInit(U32 STRVR, U16 STCTRL);
void SystemTickTimerStop(void);
void TIM3_Cfg(U16 PSC, U16 ARR);
void TIM3_Stop(void);
void TIM3_CH1_Compare_Cfg(U16 TIM3_CH1_PSC, U16 TIM3_CH1_ARR, U16 TIM3_CH1_CCMR1, U16 TIM3_CH1_CCR1);
void TIM8_CH3_Capture_Cfg(U16 TIM8_CH3_PSC, U16 TIM8_CH3_CCMR2);
void TIM8_Stop(void);
void InitTIM8_CH1_PWM(void);
void StopTIM8_CH1_PWM(void);
void SetTIM8_CH1_PWM_DutyCycle(U8 DC);

/* Lucrarea 4 */
void ADC_Configure(U32 MODER, U32 ADC_CR1, U32 ADC_CR2);
U16 ADC_SampleChannel(U8 channel);
void ADC_Stop(void);
void DAC_Configure(U16 Init_Hold);
void DAC_Stop(void);

/* lucrarea 5 */
void SetupIOInterrupt(U32 MODERB, U32 MODERC, U32 PUPDR, U16 EXTI_FTSR, U16 EXTI_RSTR);
void EnableIRQForIO(U8 pin);
void DisableIRQForIO(U8 pin);

#endif