/******************** (C) COPYRIGHT 2015 FTC ***************************
 * ����		 ��FTC
 * �ļ���  ��FTC_Motor.cpp
 * ����    �����������غ���
**********************************************************************************/
#include "FTC_Motor.h"

FTC_Motor motor;

void FTC_Motor::writeMotor(uint16_t throttle, int32_t pidTermRoll, int32_t pidTermPitch, int32_t pidTermYaw)
{
	//to do
	// motorPWM[0] = throttle + 0.866 * pidTermRoll - 0.5 * pidTermRoll - pidTermYaw;
	// motorPWM[1] = throttle + 0.000 * pidTermRoll - 1.0 * pidTermRoll + pidTermYaw;
	// motorPWM[2] = throttle - 0.866 * pidTermRoll - 0.5 * pidTermRoll - pidTermYaw;
	// motorPWM[3] = throttle - 0.866 * pidTermRoll + 0.5 * pidTermRoll + pidTermYaw;
	// motorPWM[4] = throttle + 0.000 * pidTermRoll + 1.0 * pidTermRoll - pidTermYaw;
	// motorPWM[5] = throttle + 0.866 * pidTermRoll + 0.5 * pidTermRoll + pidTermYaw;
	
	motorPWM[0] = 0.1667 * throttle +  0.2887 * pidTermRoll + -0.1667 * pidTermRoll + -0.1667 * pidTermYaw;
	motorPWM[1] = 0.1667 * throttle +  0.0000 * pidTermRoll + -0.3333 * pidTermRoll +  0.1667 * pidTermYaw;
	motorPWM[2] = 0.1667 * throttle + -0.2887 * pidTermRoll + -0.1667 * pidTermRoll + -0.1667 * pidTermYaw;
	motorPWM[3] = 0.1667 * throttle + -0.2887 * pidTermRoll +  0.1667 * pidTermRoll +  0.1667 * pidTermYaw;
	motorPWM[4] = 0.1667 * throttle +  0.0000 * pidTermRoll +  0.3333 * pidTermRoll + -0.1667 * pidTermYaw;
	motorPWM[5] = 0.1667 * throttle +  0.2887 * pidTermRoll +  0.1667 * pidTermRoll +  0.1667 * pidTermYaw;

	int16_t maxMotor = motorPWM[0];
	for (u8 i = 1; i < MAXMOTORS; i++)
	{
		if (motorPWM[i] > maxMotor)
					maxMotor = motorPWM[i];
	}
	
	for (u8 i = 0; i < MAXMOTORS; i++) 
	{
		if (maxMotor > MAXTHROTTLE)    
			motorPWM[i] -= maxMotor - MAXTHROTTLE;	
		//���Ƶ��PWM����С�����ֵ
		motorPWM[i] = constrain_uint16(motorPWM[i], MINTHROTTLE, MAXTHROTTLE);
	}

	//���δ�������򽫵���������Ϊ���
	if(!ftc.f.ARMED)	
		ResetPWM();

	if(!ftc.f.ALTHOLD && rc.rawData[THROTTLE] < RC_MINCHECK)
		ResetPWM();

	//д����PWM
	pwm.SetPwm(motorPWM);
	
}

void FTC_Motor::getPWM(int16_t* pwm)
{
	*(pwm) = motorPWM[0];
	*(pwm+1) = motorPWM[1];
	*(pwm+2) = motorPWM[2];
	*(pwm+3) = motorPWM[3];
	*(pwm+4) = motorPWM[4];
	*(pwm+5) = motorPWM[5];	
}

void FTC_Motor::ResetPWM(void)
{
	for(u8 i=0; i< MAXMOTORS ; i++)
		motorPWM[i] = 1000;
}

/******************* (C) COPYRIGHT 2015 FTC *****END OF FILE************/
