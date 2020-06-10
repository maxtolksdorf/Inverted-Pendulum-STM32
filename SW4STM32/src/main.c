#include <stm32f3xx.h>

#include "system.h"

#include "features/led.h"
#include "features/esc.h"
#include "features/imu.h"
#include "features/pwm.h"
#include "features/pid.h"
#include "features/com.h"

int main(void)
{
	initSystem();

//	initCOM();
	initLED();
	initESC();
	initIMU();
	initPWM();

	NVIC_EnableIRQ(TIM3_IRQn);
	while (1) __WFI();
}

void TIM3_IRQHandler(void)
{
	readESC();
	readIMU();
	loopPID();
//	sendCOM();

	TIM3->SR &= ~TIM_SR_UIF;
}
