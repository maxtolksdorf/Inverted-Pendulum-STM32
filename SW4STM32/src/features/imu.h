#ifndef FEATURES_IMU_H_
#define FEATURES_IMU_H_

int acc_z;
int gyr_x;
int gyr_y;

void wakeIMU(void)
{
	uint8_t i2c_data[2];
	i2c_data[0] = 0x6B;
	i2c_data[1] = 0x01;
	int i = 0;

	I2C1->CR2 = 0;
	I2C1->CR2 |= ((0x68 << 1) << I2C_CR2_SADD_Pos);
	I2C1->CR2 |= (2 << I2C_CR2_NBYTES_Pos);
	I2C1->CR2 |= I2C_CR2_START;

	do
	{
		if (I2C1->ISR & (I2C_ISR_NACKF | I2C_ISR_ARLO))
		{
			goto error;
		}

		if (I2C1->ISR & I2C_ISR_TXIS)
		{
			I2C1->TXDR = i2c_data[i];
			i++;
		}
	} while (!(I2C1->ISR & I2C_ISR_TC));

	I2C1->CR2 |= I2C_CR2_STOP;

	error:
	I2C1->CR1 &= ~I2C_CR1_PE;
	I2C1->CR1 |= I2C_CR1_PE;
}

void readIMU(void)
{
	uint8_t i2c_data[8];
	int i = 0;

	I2C1->CR2 = 0;
	I2C1->CR2 |= ((0x68 << 1) << I2C_CR2_SADD_Pos);
	I2C1->CR2 |= (1 << I2C_CR2_NBYTES_Pos);
	I2C1->CR2 |= I2C_CR2_START;

	do
	{
		if (I2C1->ISR & (I2C_ISR_NACKF | I2C_ISR_ARLO))
		{
			goto error;
		}

		if (I2C1->ISR & I2C_ISR_TXIS)
		{
			I2C1->TXDR = 0x3F;
		}
	} while (!(I2C1->ISR & I2C_ISR_TC));

	I2C1->CR2 = 0;
	I2C1->CR2 |= ((0x68 << 1) << I2C_CR2_SADD_Pos);
	I2C1->CR2 |= (8 << I2C_CR2_NBYTES_Pos);
	I2C1->CR2 |= I2C_CR2_RD_WRN;
	I2C1->CR2 |= I2C_CR2_START;

	do
	{
		if (I2C1->ISR & (I2C_ISR_NACKF | I2C_ISR_ARLO))
		{
			goto error;
		}

		if (I2C1->ISR & I2C_ISR_RXNE)
		{
			i2c_data[i] = (uint8_t) I2C1->RXDR;
			i++;
		}
	} while (!(I2C1->ISR & I2C_ISR_TC));

	I2C1->CR2 |= I2C_CR2_STOP;

	acc_z = (int16_t) ((i2c_data[0] << 8) | i2c_data[1]);
	gyr_x = (int16_t) ((i2c_data[4] << 8) | i2c_data[5]);
	gyr_y = (int16_t) ((i2c_data[6] << 8) | i2c_data[7]);

	error:
	I2C1->CR1 &= ~I2C_CR1_PE;
	I2C1->CR1 |= I2C_CR1_PE;
}

void initIMU(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOBEN;						// enable clock
	GPIOB->OTYPER |= GPIO_OTYPER_OT_8;						// PB8: open drain output
	GPIOB->AFR[1] |= (0b0100 << GPIO_AFRH_AFRH0_Pos);		// PB8: alternate function 4
	GPIOB->MODER |= (0b10 << GPIO_MODER_MODER8_Pos);		// PB8: alternate function mode
	GPIOB->OTYPER |= GPIO_OTYPER_OT_9;						// PB9: open drain output
	GPIOB->AFR[1] |= (0b0100 << GPIO_AFRH_AFRH1_Pos);		// PB9: alternate function 4
	GPIOB->MODER |= (0b10 << GPIO_MODER_MODER9_Pos);		// PB9: alternate function mode

	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;						// enable clock
	RCC->CFGR3 |= RCC_CFGR3_I2C1SW;							// use 72 MHz CPU clock as clock source for I2C
	I2C1->TIMINGR |= (8 << I2C_TIMINGR_PRESC_Pos);			// set timings for 400 kHz fast mode I2C
	I2C1->TIMINGR |= (9 << I2C_TIMINGR_SCLL_Pos);			// set timings for 400 kHz fast mode I2C
	I2C1->TIMINGR |= (3 << I2C_TIMINGR_SCLH_Pos);			// set timings for 400 kHz fast mode I2C
	I2C1->TIMINGR |= (1 << I2C_TIMINGR_SDADEL_Pos);			// set timings for 400 kHz fast mode I2C
	I2C1->TIMINGR |= (3 << I2C_TIMINGR_SCLDEL_Pos);			// set timings for 400 kHz fast mode I2C
	I2C1->CR1 |= I2C_CR1_PE;								// enable I2C

	do
	{
		wakeIMU();
		readIMU();
	} while (acc_z == 0);
}

#endif
