//I2C lib included
#include <Wire.h>
//define the address and important numbers
#define complementryRatio 0.9
#define PI 3.14
#define IMU_Address 0x68
#define PWR_MGMT 0x6B
#define GYRO_CONFIG 0x1B
#define ACCEL_CONFIG 0x1C
#define ACCEL_XOUT_H 0x3B
#define GYRO_ZOUT_H 0x47
#define samples 3000

//the variable used to calcaulte the accelometer and gyroscope variables
int16_t acc_X = 0, acc_Z = 0 ,acc_Y = 0 ,gyro_Z = 0;
int16_t gyro_off_Z=0, acc_off_X = 0, acc_off_Y = 0, acc_off_Z = 0;

//the variable used to know when the config is finished
int i_Gyro = 0;
int i_accel = 0;

//the variable used to calculte the yaw angle
double yaw=0;
double yaw_gyro = 0;
double FinalYaw =0;
double t_old = 0;
double dt;


void setup() {
  Serial.begin(9600);
  //set the arduino as master
   Wire.begin();
   //start the config
   Initializtion_IMU();
  Serial.print("gyroscope and accelometer will be calliprated");
  //start the calibration
  calli_ACCEL();
  calli_GYRO();
  Serial.print("successfully done");


}

void loop() {
  //get the raw value of accell and gyro
 MPU_readAcc_XYZ();
 MPU_readGyro_z();
 //convert it to the true value
 acc_X = (1.0*acc_X*4*9.81)/32786;
 acc_Y = (1.0*acc_Y*4*9.81)/32786;
 acc_Z = (1.0*acc_Z*4*9.81)/32786;
 gyro_Z = (1.0*gyro_Z*2000)/32786;
 //get the angle 
 get_yaw();
}

void get_yaw (){
  //get the angle between the X vector and the vector Z+Y
yaw = atan2(sqrt(acc_Y*acc_Y+acc_Z*acc_Z),acc_X)*180/PI;
//get the delta time
dt = millis()-t_old;
//get the yaw angle from the gyroscope
yaw_gyro = (gyro_Z *  dt)/1000;
//assign new value to the old t
t_old = dt;
//use complementry factor to get the final yaw
FinalYaw = (1-complementryRatio)*yaw + complementryRatio*yaw_gyro;
//print it
Serial.print ("the yaw angle = ");
Serial.println(FinalYaw);
}

void MPU_readAcc_XYZ(){
Wire.beginTransmission(IMU_Address);
Wire.write(ACCEL_XOUT_H);
Wire.endTransmission();
//wait untill there are six byte available
Wire.requestFrom(IMU_Address, 6);
while (Wire.available()<6);
//read the first byte then shift it by 8 bit to the right and read the last byte
acc_X = Wire.read()<<8 | Wire.read();
acc_Y = Wire.read()<<8 | Wire.read();
acc_Z = Wire.read()<<8 | Wire.read();
//if the calibration finished remove its average from the reading sample 
if(i_accel == 3000){
  acc_X -= acc_off_X;
  acc_Y -= acc_off_Y;
  acc_Z -= acc_off_Z;
}
}

//getting the value of the Gyro by request from the address of the register
void MPU_readGyro_z(){
Wire.beginTransmission(IMU_Address);
Wire.write(GYRO_ZOUT_H);
Wire.endTransmission();

Wire.requestFrom(IMU_Address, 2);
//wait untill there are two byte available
while (Wire.available()<2);
//read the first byte then shift it by 8 bit to the right and read the last byte
gyro_Z = Wire.read()<<8 | Wire.read();
//if the calibration finished remove its average from the reading sample 
if(i_Gyro == 3000){
  gyro_Z -= gyro_off_Z;
}

}

//IMU config , Gyro config, accelo config
//IMU set on zero to power it 
//Gyro set on 18 for the MAX scale
//Accel set on 8 for scale of 4g
void Initializtion_IMU(){
  Wire.beginTransmission(IMU_Address); 
  Wire.write(PWR_MGMT); 
  Wire.write(0x00); 
  Wire.endTransmission();
  Wire.beginTransmission(IMU_Address); 
  Wire.write(GYRO_CONFIG); 
  Wire.write(0x18); 
  Wire.endTransmission();
  Wire.beginTransmission(IMU_Address); 
  Wire.write(ACCEL_CONFIG); 
  Wire.write(0x08); 
  Wire.endTransmission();  
}
//calibration of the accelometer by get the average large number of sample 
void calli_ACCEL(){
for (int i_Accel ; i_Accel<samples; i_Accel++){
  MPU_readAcc_XYZ();
  acc_off_X+= acc_X;
  acc_off_Y+= acc_Y;
  acc_off_Z+= acc_Z;
}
  acc_off_X/= samples;
  acc_off_Y/= samples;
  acc_off_Z/= samples;
}

//calibration of the gyroscope by get the average large number of sample 
void calli_GYRO(){
for(int i_Gyro = 0; i_Gyro< samples; i_Gyro++){
  MPU_readGyro_z();
  gyro_off_Z += gyro_Z;
}
  gyro_off_Z /= samples;
}
