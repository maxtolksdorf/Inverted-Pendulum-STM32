#ifndef SYSTEM_H_
#define SYSTEM_H_

#define F_LOOP 4000

int convert(float x)
{
	if (x < 0) return (int) (x - 0.5f);
	else       return (int) (x + 0.5f);
}

int readButton(void)
{
	return ((~GPIOC->IDR & GPIO_IDR_13) >> 13);
}

void initSystem(void)
{
	FLASH->ACR |= FLASH_ACR_LATENCY_1;			// set wait states for flash
	RCC->CR |= RCC_CR_HSEON;					// enable on board quartz (HSE)
	while (!(RCC->CR & RCC_CR_HSERDY));			// wait until HSE is ready
	RCC->CFGR |= RCC_CFGR_PLLSRC_HSE_PREDIV;	// set HSE as PLL source
	RCC->CFGR |= RCC_CFGR_PLLMUL9;				// set PLL multiplier
	RCC->CFGR |= RCC_CFGR_PPRE1_DIV2;			// set prescaler for APB1 to not exceed the maximum supported frequency of 36 MHz
	RCC->CR |= RCC_CR_PLLON;					// enable PLL
	while (!(RCC->CR & RCC_CR_PLLRDY));			// wait until PLL is ready
	RCC->CFGR |= RCC_CFGR_SW_PLL;				// set PLL as CPU clock source

	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;			// enable clock
	TIM3->PSC = 71;								// prescale timer step to 1 us
	TIM3->ARR = 1000000 / F_LOOP - 1; 			// set timer count reset to match desired loop frequency
	TIM3->DIER |= TIM_DIER_UIE;					// enable update interrupt
	TIM3->CR1 |= TIM_CR1_CEN; 					// enable timer

	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;			// enable clock
}

#endif
