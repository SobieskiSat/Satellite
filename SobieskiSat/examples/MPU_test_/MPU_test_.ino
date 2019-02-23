#include <SobieskiSat.h>

using namespace SobieskiSat;

MPU mpu;
int prec = 1;

void setup() {
  SerialUSB.begin(115200);

  mpu.begin();
  delay(1000);
  SerialUSB.println(mpu.Initialized);
}

void loop() {
  if (mpu.update())
  {
    SerialUSB.print("Angles: ");
    SerialUSB.print(String(mpu.Gyro[0], prec) + " " + String(mpu.Gyro[1], prec) + " " + String(mpu.Gyro[2], prec));
    SerialUSB.print(" Accels: ");
    SerialUSB.print(String(mpu.Accel[0], prec) + " " + String(mpu.Accel[1], prec) + " " + String(mpu.Accel[2], prec));
    SerialUSB.print(" Mag: ");
    SerialUSB.println(String(mpu.Mag[0], prec) + " " + String(mpu.Mag[1], prec) + " " + String(mpu.Mag[2], prec));
  }
}
