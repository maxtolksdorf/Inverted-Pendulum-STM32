#ifndef FEATURES_COM_H_
#define FEATURES_COM_H_

int timestamp;
int tic;
int toc;

void sendByte(uint8_t byte)
{
	while (!(USART2->ISR & USART_ISR_TXE));

	USART2->TDR = byte;
}

void sendValueSigned(int value, int digits)
{
	uint8_t number[digits + 1];

	if (value < 0)
	{
		number[0] = '-';
		value *= -1;
	}
	else
	{
		number[0] = '+';
	}

	for (int i = digits; i > 0; i--)
	{
		number[i] = value % 10 + 48;
		value /= 10;
	}

	for (int i = 0; i < digits + 1; i++)
	{
		sendByte(number[i]);
	}

	sendByte(' ');
}

void sendValueUnsigned(int value, int digits)
{
	uint8_t number[digits];

	for (int i = digits; i > 0; i--)
	{
		number[i - 1] = value % 10 + 48;
		value /= 10;
	}

	for (int i = 0; i < digits; i++)
	{
		sendByte(number[i]);
	}

	sendByte(' ');
}

void sendCOM(void)
{
	sendValueUnsigned(tic, 3);
	sendValueUnsigned(toc, 3);
	sendValueSigned(remote_0, 2);
	sendValueSigned(remote_1, 2);
	sendValueSigned(convert(phi), 2);
	sendByte('\r');
	sendByte('\n');
}

void initCOM(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOAEN;					// enable clock
	GPIOA->AFR[0] |= (0b0111 << GPIO_AFRL_AFRL2_Pos);	// PA2: alternate function 7
	GPIOA->MODER |= (0b10 << GPIO_MODER_MODER2_Pos);	// PA2: alternate function mode
	GPIOA->AFR[0] |= (0b0111 << GPIO_AFRL_AFRL3_Pos);	// PA3: alternate function 7
	GPIOA->MODER |= (0b10 << GPIO_MODER_MODER3_Pos);	// PA3: alternate function mode

	RCC->APB1ENR |= RCC_APB1ENR_USART2EN;				// enable clock
	RCC->CFGR3 |= RCC_CFGR3_USART2SW_SYSCLK;			// use 72 MHz CPU clock as clock source for USART
	USART2->BRR = 36;									// set baud rate to the maximum supported value of 2000000
	USART2->CR1 |= USART_CR1_TE;						// enable transmit
	USART2->CR1 |= USART_CR1_UE;						// enable USART
}

#endif
