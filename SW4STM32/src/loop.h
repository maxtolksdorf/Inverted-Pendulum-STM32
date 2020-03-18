#ifndef LOOP_H_
#define LOOP_H_

void TIM3_IRQHandler(void)		// control loop based on the timer update interrupt
{
	readBTN();					// checks if the blue button is pressed
	readPWM();					// determines connection status and reads the PWM signal inputs of the remote control
	readIMU();					// burst reads the necessary gyroscope and accelerometer registers of the IMU

	loopPID();					// handles the state machine, complementary filter and PID control

	applyDAC();					// applies the motor voltages
	applyLED();					// switches the LEDs according to the current state of the control loop

//	timestamp++;				// increases the timestamp value
//	looptime = (int) TIM3->CNT;	// snapshots the current loop time

	TIM3->SR &= ~TIM_SR_UIF;	// clears the interrupt pending flag
}

#endif
