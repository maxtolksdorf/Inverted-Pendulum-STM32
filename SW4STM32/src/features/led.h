#ifndef FEATURES_LED_H_
#define FEATURES_LED_H_

void disableLED(int id)
{
	GPIOA->ODR &= ~(1 << id);
}

void enableLED(int id)
{
	GPIOA->ODR |= (1 << id);
}

void applyLED(void)
{
	if (state > 0) { enableLED(8); } else { disableLED(8); }
	if (state > 1) { enableLED(10); } else { disableLED(10); }
	if (connection == 1) { enableLED(9); } else { disableLED(9); }
}

void initLED(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;						// enable clock
	GPIOA->MODER |= (0b01 << GPIO_MODER_MODER8_Pos);		// PA8: output mode
	GPIOA->MODER |= (0b01 << GPIO_MODER_MODER9_Pos);		// PA9: output mode
	GPIOA->MODER |= (0b01 << GPIO_MODER_MODER10_Pos);		// PA10: output mode
}

#endif
