#include <stm32f3xx.h>					// STM32F303RE manufacturer dependencies

#define F_LOOP			4000			// control loop frequency
#define VEL_MAX			2047			// maximum velocity value
#define ACC_Z_OFFSET	1000			// offset of the IMU
#define GYR_X_OFFSET	100				// offset of the IMU
#define GYR_Y_OFFSET	65				// offset of the IMU
#define ACC_GAIN		0.003497057f	// gain of the accelerometers
#define GYR_GAIN		0.007629395f	// gain of the gyroscopes
#define REMOTE_0_GAIN	4.0f			// gain of the remote control inputs
#define REMOTE_1_GAIN	16.0f			// gain of the remote control inputs
#define PHI_MAX			7.5f			// maximum allowed tilt angle in a static position
#define PHI_CRIT		15.0f			// maximum allowed tilt angle before the control loop is shut off
#define ALPHA			0.4f			// parameter of the complementary filter

#define P_VEL			0.0004f			// P value of the velocity controller
#define I_VEL			0.04f			// I value of the velocity controller

#define P_PHI			400.0f			// P value of the tilt angle controller
#define I_PHI			800.0f			// I value of the tilt angle controller
#define D_PHI			8.0f			// D value of the tilt angle controller

#define P_PSI			200.0f			// P value of the turn angle controller
#define I_PSI			400.0f			// I value of the turn angle controller
#define D_PSI			4.0f			// D value of the turn angle controller

int acc_z;								// IMU input
int gyr_x;								// IMU input
int gyr_y;								// IMU input
int remote_0;							// remote input
int remote_1;							// remote input

#include "features/com.h"				// COM port to log values using a terminal
#include "features/dac.h"				// DAC outputs to drive the motor voltages
#include "features/btn.h"				// button to activate the machine
#include "features/led.h"				// LEDs to visualize the current state of the control loop
#include "features/pwm.h"				// PWM signal inputs to read the remote control
#include "features/imu.h"				// I2C communication with the IMU
#include "features/sys.h"				// CPU overclock and timer update interrupt to periodically run the control loop

#include "pid.h"						// state machine, complementary filter and PID control
#include "loop.h"						// control loop based on timer update interrupt

int main(void)							// entry point, initializes the system and all peripherals
{
//	initCOM();							// COM port to log values using a terminal
	initDAC();							// DAC outputs to drive the motor voltages
	initBTN();							// button to activate the machine
	initLED();							// LEDs to visualize the current state of the control loop
	initPWM();							// PWM signal inputs to read the remote control
	initIMU();							// I2C communication with the IMU
	initSYS();							// CPU overclock and timer update interrupt to periodically run the control loop

	while (1) __WFI();					// do nothing and wait for the timer update interrupt
}
