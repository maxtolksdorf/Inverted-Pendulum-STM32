#ifndef FEATURES_DAC_H_
#define FEATURES_DAC_H_

void setDAC(int motor_0, int motor_1)
{
	DAC->DHR12RD = (((motor_1 + VEL_MAX) << 16) | (motor_0 + VEL_MAX));
}

void initDAC(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN_Pos;				// enable clock
	GPIOA->MODER |= (0b11 << GPIO_MODER_MODER4_Pos);	// PA4: analog mode
	GPIOA->MODER |= (0b11 << GPIO_MODER_MODER5_Pos);	// PA5: analog mode

	RCC->APB1ENR |= RCC_APB1ENR_DAC1EN;					// enable clock
	DAC->CR |= DAC_CR_EN1;								// enable DAC 1
	DAC->CR |= DAC_CR_EN2;								// enable DAC 2

	setDAC(0, 0);
}

#endif
