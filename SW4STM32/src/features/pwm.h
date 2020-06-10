#ifndef FEATURES_PWM_H_
#define FEATURES_PWM_H_

void applyPWM(float duty_0, float duty_1)
{
	if (duty_0 < 0)
	{
		TIM1->CCR2 = 0;
		TIM1->CCR1 = convert(duty_0 * -39.99f);
	}
	else
	{
		TIM1->CCR1 = 0;
		TIM1->CCR2 = convert(duty_0 *  39.99f);
	}

	if (duty_1 < 0)
	{
		TIM1->CCR3 = 0;
		TIM1->CCR4 = convert(duty_1 * -39.99f);
	}
	else
	{
		TIM1->CCR4 = 0;
		TIM1->CCR3 = convert(duty_1 *  39.99f);
	}
}

void initPWM(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;

	GPIOC->AFR[0] |= (0b0010 << GPIO_AFRL_AFRL0_Pos);	// PC0: alternate function 2
	GPIOC->MODER |= (0b10 << GPIO_MODER_MODER0_Pos);	// PC0: alternate function mode
	GPIOC->AFR[0] |= (0b0010 << GPIO_AFRL_AFRL1_Pos);	// PC1: alternate function 2
	GPIOC->MODER |= (0b10 << GPIO_MODER_MODER1_Pos);	// PC1: alternate function mode
	GPIOC->AFR[0] |= (0b0010 << GPIO_AFRL_AFRL2_Pos);	// PC2: alternate function 2
	GPIOC->MODER |= (0b10 << GPIO_MODER_MODER2_Pos);	// PC2: alternate function mode
	GPIOC->AFR[0] |= (0b0010 << GPIO_AFRL_AFRL3_Pos);	// PC3: alternate function 2
	GPIOC->MODER |= (0b10 << GPIO_MODER_MODER3_Pos);	// PC3: alternate function mode

	RCC->APB2ENR |= RCC_APB2ENR_TIM1EN;					// enable clock

	TIM1->CCER |= TIM_CCER_CC1E;						// enable output
	TIM1->CCMR1 |= (0b110 << TIM_CCMR1_OC1M_Pos);		// PWM mode
	TIM1->CCMR1 |= TIM_CCMR1_OC1PE;						// enable preload

	TIM1->CCER |= TIM_CCER_CC2E;						// enable output
	TIM1->CCMR1 |= (0b110 << TIM_CCMR1_OC2M_Pos);		// PWM mode
	TIM1->CCMR1 |= TIM_CCMR1_OC2PE;						// enable preload

	TIM1->CCER |= TIM_CCER_CC3E;						// enable output
	TIM1->CCMR2 |= (0b110 << TIM_CCMR2_OC3M_Pos);		// PWM mode
	TIM1->CCMR2 |= TIM_CCMR2_OC3PE;						// enable preload

	TIM1->CCER |= TIM_CCER_CC4E;						// enable output
	TIM1->CCMR2 |= (0b110 << TIM_CCMR2_OC4M_Pos);		// PWM mode
	TIM1->CCMR2 |= TIM_CCMR2_OC4PE;						// enable preload

	TIM1->PSC = 0;										// prescaler
	TIM1->ARR = 3999;									// reset
	TIM1->CR1 |= TIM_CR1_CEN;							// enable timer
	TIM1->BDTR |= TIM_BDTR_MOE;							// main output enable
}

#endif
