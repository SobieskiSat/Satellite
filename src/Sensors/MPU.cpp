#include "Sensors.h"
#include "Arduino.h"
#include <SparkFunMPU9250-DMP.h>
#include "../src/config.h"

using namespace SobieskiSat;

MPU::MPU() : Sensor() { ID = 'M'; }

bool MPU::begin()
{
	Status = STA_DURINGINIT;
	fileName = "MPU9250.txt";
	minDelay = 0;
	setUpdateDelay(UPD_MPU);
	
	if (imu.begin() == INV_SUCCESS)
	{
		imu.dmpBegin(DMP_FEATURE_6X_LP_QUAT | DMP_FEATURE_GYRO_CAL, 10);
		
		Status = STA_INITIALIZED;
	}
	else Status = STA_MISSING;
	
	return (Status == STA_INITIALIZED);
}

bool MPU::update()
{
	if (timeForUpdate() && imu.fifoAvailable())
	{	
		if (imu.dmpUpdateFifo() == INV_SUCCESS)
		{
			imu.update();
			imu.computeEulerAngles();
			Quat[0] = imu.calcQuat(imu.qw);
			Quat[1] = imu.calcQuat(imu.qx);
			Quat[2] = imu.calcQuat(imu.qy);
			Quat[3] = imu.calcQuat(imu.qz);
			Gyro[0] = imu.roll;
			Gyro[1] = imu.pitch;
			Gyro[2] = imu.yaw;
			Accel[0] = imu.calcAccel(imu.ax);
			Accel[1] = imu.calcAccel(imu.ay);
			Accel[2] = imu.calcAccel(imu.az);
			Mag[0] = imu.calcMag(imu.mx);
			Mag[1] = imu.calcMag(imu.my);
			Mag[2] = imu.calcMag(imu.mz);
			
		
			SDbuffer += String(Gyro[0], PREC_MPU) + " " + String(Gyro[1], PREC_MPU) + " " + String(Gyro[2], PREC_MPU) + " ";
			SDbuffer += String(Accel[0], PREC_MPU) + " " + String(Accel[1], PREC_MPU) + " " + String(Accel[2], PREC_MPU) + " ";
			SDbuffer += String(Mag[0], PREC_MPU) + " " + String(Mag[1], PREC_MPU) + " " + String(Mag[2], PREC_MPU) + " ";
			SDbuffer += String(Quat[0], PREC_MPU) + " " + String(Quat[1], PREC_MPU) + " " + String(Quat[2], PREC_MPU) + " " + String(Quat[3], PREC_MPU) + " @" + String(millis());
			SDbuffer += "\r\n";
			
			successUpdateFinish();
			return true;
		}
	}
	return false;
}

String MPU::listReadings()
{
	return "Angles: " + String(Gyro[0], PREC_MPU) + " " + String(Gyro[1], PREC_MPU) + " " + String(Gyro[2], PREC_MPU)
    + " Accels: " + String(Accel[0], PREC_MPU) + " " + String(Accel[1], PREC_MPU) + " " + String(Accel[2], PREC_MPU)
	+ " Mag: " + String(Mag[0], PREC_MPU) + " " + String(Mag[1], PREC_MPU) + " " + String(Mag[2], PREC_MPU)
	+ " Quat: " + String(Quat[0], PREC_MPU) + " " + String(Quat[1], PREC_MPU) + " " + String(Quat[2], PREC_MPU) + " " + String(Quat[3], PREC_MPU);
}