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
	phi = (1 - ALPHA / F_LOOP) * (phi + GYR_GAIN / F_LOOP * (gyr_x - GYR_X_OFFSET)) + ALPHA / F_LOOP * ACC_GAIN * (acc_z - ACC_Z_OFFSET);
	float phi_delta = GYR_GAIN / F_LOOP * (gyr_x - GYR_X_OFFSET);

	static float phi_offset;
	float phi_error = phi + phi_offset;

	static float phi_error_sum;
	phi_error_sum = phi_error_sum + phi_error;
	if (phi_error_sum < -VEL_MAX * F_LOOP / I_PHI) phi_error_sum = -VEL_MAX * F_LOOP / I_PHI;
	if (phi_error_sum >  VEL_MAX * F_LOOP / I_PHI) phi_error_sum =  VEL_MAX * F_LOOP / I_PHI;

	float vel = P_PHI * phi_error + I_PHI / F_LOOP * phi_error_sum + D_PHI * F_LOOP * phi_delta;
	if (vel < -VEL_MAX) vel = -VEL_MAX;
	if (vel >  VEL_MAX) vel =  VEL_MAX;

	static float psi_error;
	psi_error = psi_error + GYR_GAIN / F_LOOP * (gyr_y - GYR_Y_OFFSET) + REMOTE_0_GAIN / F_LOOP * remote_0;
	float psi_delta = GYR_GAIN / F_LOOP * (gyr_y - GYR_Y_OFFSET);

	static float psi_error_sum;
	psi_error_sum = psi_error_sum + psi_error;
	if (psi_error_sum < -VEL_MAX * F_LOOP / I_PSI) psi_error_sum = -VEL_MAX * F_LOOP / I_PSI;
	if (psi_error_sum >  VEL_MAX * F_LOOP / I_PSI) psi_error_sum =  VEL_MAX * F_LOOP / I_PSI;

	float rot = P_PSI * psi_error + I_PSI / F_LOOP * psi_error_sum + D_PSI * F_LOOP * psi_delta;

	if (rot < 0)
	{
		if (vel < 0)
		{
			if (-rot - vel > VEL_MAX) rot = -VEL_MAX - vel;
		}
		else
		{
			if (-rot + vel > VEL_MAX) rot = -VEL_MAX + vel;
		}
	}
	else
	{
		if (vel < 0)
		{
			if (rot - vel > VEL_MAX) rot = VEL_MAX + vel;
		}
		else
		{
			if (rot + vel > VEL_MAX) rot = VEL_MAX - vel;
		}
	}

	setDAC(convert(vel - rot), convert(vel + rot));

	if (phi < -PHI_CRIT || phi > PHI_CRIT) goto stop;

	float vel_error = vel - REMOTE_1_GAIN * remote_1;

	static float vel_error_sum;
	vel_error_sum = vel_error_sum + vel_error;
	if (vel_error_sum < -PHI_MAX * F_LOOP / I_VEL) vel_error_sum = -PHI_MAX * F_LOOP / I_VEL;
	if (vel_error_sum >  PHI_MAX * F_LOOP / I_VEL) vel_error_sum =  PHI_MAX * F_LOOP / I_VEL;

	phi_offset = P_VEL * vel_error + I_VEL / F_LOOP * vel_error_sum;
	if (phi_offset < -PHI_MAX) phi_offset = -PHI_MAX;
	if (phi_offset >  PHI_MAX) phi_offset =  PHI_MAX;

	return -1;

	stop:
	setDAC(0, 0);
	setLED_A(0);
	setLED_B(0);

	phi = 0;
	phi_error_sum = 0;
	psi_error = 0;
	psi_error_sum = 0;
	vel_error_sum = 0;
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
		if (acc_z - ACC_Z_OFFSET < 0)
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
