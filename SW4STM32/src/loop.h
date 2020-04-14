#ifndef LOOP_H_
#define LOOP_H_

void TIM3_IRQHandler(void)		// control loop based on the timer update interrupt
{
	readPWM();					// determines connection status and reads the PWM signal inputs of the remote control
	readIMU();					// burst reads the necessary gyroscope and accelerometer registers of the IMU
	loopPID();					// handles the state machine, complementary filter and PID control
//	sendCOM();					// sends values to the virtual COM port, can be read with a terminal program like putty

	TIM3->SR &= ~TIM_SR_UIF;	// clears the interrupt pending flag
}

#endif
