#include <stm32f3xx.h>					// STM32F303RE manufacturer dependencies

#define F_LOOP			2000			// control loop frequency
#define ALPHA			0.0001f			// parameter of the complementary filter
#define ACC_CONV		0.003497057f	// conversion factor of the accelerometers
#define GYR_CONV		0.007629395f	// conversion factor of the gyroscopes
#define REMOTE_0_CONV	7.5f			// conversion factor of the remote control inputs
#define REMOTE_1_CONV	15.0f			// conversion factor of the remote control inputs
#define MAX_TILT		1000000			// maximum allowed tilt angle in a static position
#define CRITICAL_TILT	30				// maximum allowed tilt angle before the control loop is shut off

#define P_VEL 0.0005f					// PID values of the velocity controller
#define I_VEL 0.025f					// PID values of the velocity controller
#define D_VEL 0							// PID values of the velocity controller

#define P_PHI 500						// PID values of the tilt angle controller
#define I_PHI 2500						// PID values of the tilt angle controller
#define D_PHI 10						// PID values of the tilt angle controller

#define P_PSI 250						// PID values of the turn angle controller
#define I_PSI 1000						// PID values of the turn angle controller
#define D_PSI 5							// PID values of the turn angle controller

#include "system.h"						// CPU overclock and timer update interrupt to periodically run the control loop

#include "features/btn.h"				// button to activate the machine
#include "features/dac.h"				// DAC outputs to drive the motor voltages
#include "features/imu.h"				// I2C communication with the IMU
#include "features/pwm.h"				// PWM signal inputs to read the remote control
#include "features/pid.h"				// state machine, complementary filter and PID control
#include "features/led.h"				// LEDs to visualize the current state of the control loop
//#include "features/com.h"				// COM port to log values using a terminal

#include "loop.h"						// control loop based on timer update interrupt

int main(void)							// entry point, initializes the system and all peripherals
{
	initSYS();							// CPU overclock
	initBTN();							// button to activate the machine
//	initCOM();							// COM port to log values using a terminal
	initDAC();							// DAC outputs to drive the motor voltages
	initIMU();							// I2C communication with the IMU
	initLED();							// LEDs to visualize the current state of the control loop
	initPWM();							// PWM signal inputs to read the remote control
	initIRQ();							// timer update interrupt to periodically run the control loop

	while (1)
	{
		__WFI();						// do nothing and wait for the timer update interrupt
	}
}
