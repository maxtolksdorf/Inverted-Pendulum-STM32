#ifndef FEATURES_LED_H_
#define FEATURES_LED_H_

void setLED_A(int enabled)
{
	if (enabled) GPIOA->ODR |=  (1 << 9);
	else         GPIOA->ODR &= ~(1 << 9);
}

void setLED_B(int enabled)
{
	if (enabled) GPIOA->ODR |=  (1 << 8);
	else         GPIOA->ODR &= ~(1 << 8);
}

void setLED_C(int enabled)
{
	if (enabled) GPIOA->ODR |=  (1 << 10);
	else         GPIOA->ODR &= ~(1 << 10);
}

void initLED(void)
{
	RCC->AHBENR  |= RCC_AHBENR_GPIOAEN;
	GPIOA->MODER |= (0b01 << GPIO_MODER_MODER8_Pos);
	GPIOA->MODER |= (0b01 << GPIO_MODER_MODER9_Pos);
	GPIOA->MODER |= (0b01 << GPIO_MODER_MODER10_Pos);
}

#endif
