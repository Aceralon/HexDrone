/******************** (C) COPYRIGHT 2015 FTC ***************************
 * ����		 ��FTC
 * �ļ���  ��FTC_FlyControl.cpp
 * ����    �����п���
**********************************************************************************/
#include "FTC_FlyControl.h"

FTC_FlyControl fc;

FTC_FlyControl::FTC_FlyControl()
{
	rollPitchRate = 150;
	yawRate = 50;
	
	altHoldDeadband = 100;
	
	//����PID����
	PID_Reset();
}

//����PID����
void FTC_FlyControl::PID_Reset(void)
{
	pid[PIDROLL].set_pid(0.15, 0.15, 0.02, 200);
	pid[PIDPITCH].set_pid(0.15, 0.15, 0.02, 200);
	pid[PIDYAW].set_pid(0.8, 0.45, 0, 200);
	pid[PIDANGLE].set_pid(5, 0, 0, 0);
	pid[PIDMAG].set_pid(2, 0, 0, 0);
 	pid[PIDVELZ].set_pid(1.5, 0.5, 0.002, 150);
 	pid[PIDALT].set_pid(1.2, 0, 0, 200);
}

//��������̬�⻷����
void FTC_FlyControl::Attitude_Outter_Loop(void)
{
	//to do
	Vector3f temp;
	temp = Vector3f(rc.Command[ROLL], rc.Command[PITCH], rc.Command[YAW]) - imu.angle;

	outterOut.x =  pid[PIDROLL].get_pid(temp.x, PID_OUTER_LOOP_TIME*1e-6);
	outterOut.y =  pid[PIDROLL].get_pid(temp.y, PID_OUTER_LOOP_TIME*1e-6);
	outterOut.z =  pid[PIDROLL].get_pid(temp.z, PID_OUTER_LOOP_TIME*1e-6);
}

//��������̬�ڻ�����
void FTC_FlyControl::Attitude_Inner_Loop(void)
{
	//to do
	RateError[ROLL] = outterOut.x  - imu.Gyro.x;
	RateError[PITCH] = outterOut.y - imu.Gyro.y;
	RateError[YAW]= outterOut.z - imu.Gyro.z;

	if(rc.Command[THROTTLE] < RC_MINCHECK)
	{
		pid[PIDROLL].reset_I();
		pid[PIDPITCH].reset_I();
		pid[PIDYAW].reset_I();
	}
	
	velPIDTerm.x = pid[PIDROLL].get_pid(RateError[ROLL], PID_INNER_LOOP_TIME * 1e-6);
	velPIDTerm.y = pid[PIDPITCH].get_pid(RateError[PITCH], PID_INNER_LOOP_TIME * 1e-6);
	velPIDTerm.z = pid[PIDYAW].get_pid(RateError[YAW], PID_INNER_LOOP_TIME * 1e-6);

	maxAngle = imu.angle.x > imu.angle.y ? imu.angle.x : imu.angle.y;

	motor.writeMotor(rc.Command[THROTTLE]/cosf(maxAngle), velPIDTerm.x, velPIDTerm.y, velPIDTerm.z);	
}	

//�������߶��⻷����
void FTC_FlyControl::Altitude_Outter_Loop(void)
{
	//to do
}

//�������߶��ڻ�����
void FTC_FlyControl::Altitude_Inner_Loop(void)
{
	//to do
}

void FTC_FlyControl::AltHoldReset(void)
{
	AltHold = nav.position.z;
}

/************************ (C) COPYRIGHT 2015 FTC *****END OF FILE**********************/
