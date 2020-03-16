#ifndef FEATURES_BTN_H_
#define FEATURES_BTN_H_

int button;

void readBTN(void)
{
	button = ((~GPIOC->IDR & GPIO_IDR_13) >> 13);

}

void initBTN(void)
{
	RCC->AHBENR |= RCC_AHBENR_GPIOCEN;		// enable clock
}

#endif
