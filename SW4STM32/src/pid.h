#ifndef PID_H_
#define PID_H_

int convert(float x)
{
	if (x < 0) return (int) (x - 0.5f);
	else       return (int) (x + 0.5f);
}

int runPID(void)
{
	static float phi;
	phi = (1.0f - ALPHA) * (phi + GYR_CONV / F_LOOP * gyr_x) + ALPHA * ACC_CONV * acc_z;

	float phi_error;
	static float phi_offset;
	phi_error = phi + phi_offset;

	static float phi_error_i;
	phi_error_i = phi_error + phi_error_i;
	if (phi_error_i < -2048 * F_LOOP / I_PHI) phi_error_i = -2048 * F_LOOP / I_PHI;
	if (phi_error_i >  2048 * F_LOOP / I_PHI) phi_error_i =  2048 * F_LOOP / I_PHI;

	float phi_error_d;
	static float phi_error_old;
	phi_error_d = phi_error - phi_error_old;
	phi_error_old = phi_error;

	float vel;
	vel = P_PHI * phi_error + I_PHI / F_LOOP * phi_error_i + D_PHI * F_LOOP * phi_error_d;
	if (vel < -2048) vel = -2048;
	if (vel >  2048) vel =  2048;

	static float psi_error;
	psi_error = psi_error + GYR_CONV / F_LOOP * gyr_y + REMOTE_0_CONV / F_LOOP * remote_0;

	static float psi_error_i;
	psi_error_i = psi_error + psi_error_i;
	if (psi_error_i < -2048 * F_LOOP / I_PSI) psi_error_i = -2048 * F_LOOP / I_PSI;
	if (psi_error_i >  2048 * F_LOOP / I_PSI) psi_error_i =  2048 * F_LOOP / I_PSI;

	float psi_error_d;
	static float psi_error_old;
	psi_error_d = psi_error - psi_error_old;
	psi_error_old = psi_error;

	float rot;
	rot = P_PSI * psi_error + I_PSI / F_LOOP * psi_error_i + D_PSI * F_LOOP * psi_error_d;
	if (vel < 0)
	{
		if (rot < -2048 - vel) rot = -2048 - vel;
		if (rot >  2048 + vel) rot =  2048 + vel;
	}
	else
	{
		if (rot < -2048 + vel) rot = -2048 + vel;
		if (rot >  2048 - vel) rot =  2048 - vel;
	}

	setDAC(convert(vel - rot), convert(vel + rot));

	if (phi < -PHI_CRIT || phi > PHI_CRIT) goto stop;

	vel = P_PHI * phi_error + I_PHI / F_LOOP * phi_error_i;
	if (vel < -2048) vel = -2048;
	if (vel >  2048) vel =  2048;

	float vel_error;
	vel_error = vel - REMOTE_1_CONV * remote_1;

	static float vel_error_i;
	vel_error_i = vel_error + vel_error_i;
	if (vel_error_i < -PHI_MAX * F_LOOP / I_VEL) vel_error_i = -PHI_MAX * F_LOOP / I_VEL;
	if (vel_error_i >  PHI_MAX * F_LOOP / I_VEL) vel_error_i =  PHI_MAX * F_LOOP / I_VEL;

	phi_offset = P_VEL * vel_error + I_VEL / F_LOOP * vel_error_i;
	if (phi_offset < -PHI_MAX) phi_offset = -PHI_MAX;
	if (phi_offset >  PHI_MAX) phi_offset =  PHI_MAX;

	return -1;

	stop:
	setDAC(0, 0);
	setLED_A(0);
	setLED_B(0);

	phi = 0;
	phi_error_i = 0;
	phi_error_old = 0;
	psi_error = 0;
	psi_error_i = 0;
	psi_error_old = 0;
	vel_error_i = 0;
	phi_offset = 0;

	return 0;
}

void loopPID(void)
{
	static int state;
	static int sign;

	if (state < 0)
	{
		state = runPID();
	}
	else if (state > 0)
	{
		if (acc_z < 0)
		{
			if (sign > 0) {goto start;}
			sign = -1;
		}
		else
		{
			if (sign < 0) {goto start;}
			sign = 1;
		}

		return;

		start:
		setLED_B(1);
		state = -1;
	}
	else
	{
		state = readBTN();

		if (state)
		{
			setLED_A(1);
			sign = 0;
		}
	}
}

#endif
