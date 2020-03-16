#ifndef FEATURES_PID_H_
#define FEATURES_PID_H_

int state;

void loopPID(void)
{
	static int		sign;
	static float	phi;
	float			phi_offset;
	float			phi_error;
	static float	phi_error_i;
	float			phi_error_d;
	static float	phi_error_prev;
	static float	psi_error;
	static float	psi_error_i;
	float			psi_error_d;
	static float	psi_error_prev;
	float			rot;
	float			vel;
	float			vel_offset;
	static float	vel_error;
	static float	vel_error_i;
	static float	vel_error_d;
	static float	vel_error_prev;

	if (state == 2)
	{
		phi 			= (1.0f - ALPHA) * (phi + GYR_CONV / F_LOOP * gyr_x) + ALPHA * ACC_CONV * acc_z;
		phi_offset		= P_VEL * vel_error + I_VEL / F_LOOP * vel_error_i + D_VEL * F_LOOP * vel_error_d;
		phi_error		= phi + phi_offset;
		phi_error_i		= phi_error + phi_error_i;
		phi_error_d		= phi_error - phi_error_prev;
		phi_error_prev	= phi_error;

		psi_error		= psi_error + GYR_CONV / F_LOOP * gyr_y + REMOTE_0_CONV / F_LOOP * remote_0;
		psi_error_i		= psi_error + psi_error_i;
		psi_error_d		= psi_error - psi_error_prev;
		psi_error_prev	= psi_error;

		rot				= P_PSI * psi_error + I_PSI / F_LOOP * psi_error_i + D_PSI * F_LOOP * psi_error_d;
		vel				= P_PHI * phi_error + I_PHI / F_LOOP * phi_error_i + D_PHI * F_LOOP * phi_error_d;
		motor_0			= convert(vel - rot);
		motor_1			= convert(vel + rot);

		vel_offset		= REMOTE_1_CONV * remote_1;
		vel_error		= vel + vel_offset;
		vel_error_i		= vel_error + vel_error_i;
		vel_error_d		= vel_error - vel_error_prev;
		vel_error_prev	= vel_error;

		if (vel_error_i < -MAX_TILT) { vel_error_i = -MAX_TILT;	}
		if (vel_error_i >  MAX_TILT) { vel_error_i =  MAX_TILT; }

		if (phi > CRITICAL_TILT || phi < -CRITICAL_TILT)
		{
			state			= 0;

			sign			= 0;
			phi				= 0;
			phi_error_i		= 0;
			phi_error_prev	= 0;
			psi_error		= 0;
			psi_error_i		= 0;
			psi_error_prev	= 0;
			vel_error		= 0;
			vel_error_i		= 0;
			vel_error_d		= 0;
			vel_error_prev	= 0;

			motor_0			= 0;
			motor_1			= 0;
		}
	}
	else
	{
		if (button == 1)
		{
			state = 1;
		}

		if (state == 1)
		{
			if (acc_z < 0)
			{
				if (sign == 1)
				{
					state = 2;
				}

				sign = -1;
			}
			else
			{
				if (sign == -1)
				{
					state = 2;
				}

				sign = 1;
			}
		}
	}
}

#endif
