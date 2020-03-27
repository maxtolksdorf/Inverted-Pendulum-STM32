#ifndef FEATURES_PWM_H_
#define FEATURES_PWM_H_

int connection;
int remote_0;
int remote_1;

void readPWM(void)
{
	static int init;
	static int delay;
	static int offset;

	if (TIM2->CNT > 16384)
	{
		delay = 0;
		connection = 0;
		remote_0 = 0;
		remote_1 = 0;
	}
	else
	{
		if (delay < F_LOOP)
		{
			if (init == 0)
			{
				offset = (int) TIM2->CCR1;
			}

			delay++;
		}
		else
		{
			if (TIM2->CNT > 8192)
			{
				init = 1;
				connection = 1;
				remote_0 = ((int) TIM2->CCR1 - offset) / 10;
				remote_1 = ((int) TIM2->CCR2 - (int) TIM2->CCR1 - offset) / 10;
			}
		}
	}
}

void initPWM(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;					// enable clock
	GPIOA->AFR[0] |= (0b0001 << GPIO_AFRL_AFRL0_Pos);	// PA0: alternate function 1
	GPIOA->MODER |= (0b10 << GPIO_MODER_MODER0_Pos);	// PA0: alternate function mode
	GPIOA->AFR[0] |= (0b0001 << GPIO_AFRL_AFRL1_Pos);	// PA1: alternate function 1
	GPIOA->MODER |= (0b10 << GPIO_MODER_MODER1_Pos);	// PA1: alternate function mode

	RCC->APB1ENR |= RCC_APB1ENR_TIM2EN;					// enable clock
	TIM2->SMCR |= (0b101 << TIM_SMCR_TS_Pos);			// timer input 1
	TIM2->SMCR |= (0b0100 << TIM_SMCR_SMS_Pos);			// rising edge resets the timer

	TIM2->CCMR1 |= (0b10 << TIM_CCMR1_CC1S_Pos);		// capture 1 on timer input 2
	TIM2->CCER |= (0b0 << TIM_CCER_CC1P_Pos);			// capture rising edge
	TIM2->CCER |= TIM_CCER_CC1E;						// enable capture

	TIM2->CCMR1 |= (0b01 << TIM_CCMR1_CC2S_Pos);		// capture 2 on timer input 2
	TIM2->CCER |= (0b1 << TIM_CCER_CC2P_Pos);			// capture falling edge
	TIM2->CCER |= TIM_CCER_CC2E;						// enable capture

	TIM2->PSC = 71;									// prescale timer step to 1 us
	TIM2->ARR = 0xffff;								// set timer count to maximum
	TIM2->CR1 |= TIM_CR1_CEN;							// enable timer
}

#endif
