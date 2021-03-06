#include <SparkFunMPU9250-DMP.h>

// These are the ROS headers for getting ROS Client API's.
#include <ros.h>

//Header for Vector3 ROS message
#include <sensor_msgs/Imu.h>

MPU9250_DMP imu;

//Creating handlers of Node, IMU message, quaternion and ROS publisher.
ros::NodeHandle nh;
//Creating orientation message header
sensor_msgs::Imu imuRawData;

//Creating ROS publisher object for IMU orientation
ros::Publisher imu_pub("imu", &imuRawData);

void setup()
{
  nh.initNode();
  nh.advertise(imu_pub);
  // Serial.begin(115200);

  // Call imu.begin() to verify communication with and
  // initialize the MPU-9250 to it's default values.
  // Most functions return an error code - INV_SUCCESS (0)
  // indicates the IMU was present and successfully set up
  if (imu.begin() != INV_SUCCESS)
  {
    while (1)
    {
      Serial.println("Unable to communicate with MPU-9250");
      Serial.println("Check connections, and try again.");
      Serial.println();
      delay(5000);
    }
  }

  // Use setSensors to turn on or off MPU-9250 sensors.
  // Any of the following defines can be combined:
  // INV_XYZ_GYRO, INV_XYZ_ACCEL, INV_XYZ_COMPASS,
  // INV_X_GYRO, INV_Y_GYRO, or INV_Z_GYRO
  // Enable all sensors:
  imu.setSensors(INV_XYZ_GYRO | INV_XYZ_ACCEL | INV_XYZ_COMPASS);

  // Use setGyroFSR() and setAccelFSR() to configure the
  // gyroscope and accelerometer full scale ranges.
  // Gyro options are +/- 250, 500, 1000, or 2000 dps
  imu.setGyroFSR(2000); // Set gyro to 2000 dps
  // Accel options are +/- 2, 4, 8, or 16 g
  imu.setAccelFSR(2); // Set accel to +/-2g
  // Note: the MPU-9250's magnetometer FSR is set at
  // +/- 4912 uT (micro-tesla's)

  // setLPF() can be used to set the digital low-pass filter
  // of the accelerometer and gyroscope.
  // Can be any of the following: 188, 98, 42, 20, 10, 5
  // (values are in Hz).
  imu.setLPF(5); // Set LPF corner frequency to 5Hz

  // The sample rate of the accel/gyro can be set using
  // setSampleRate. Acceptable values range from 4Hz to 1kHz
  imu.setSampleRate(10); // Set sample rate to 10Hz

  // Likewise, the compass (magnetometer) sample rate can be
  // set using the setCompassSampleRate() function.
  // This value can range between: 1-100Hz
  imu.setCompassSampleRate(10); // Set mag rate to 10Hz
  nh.loginfo("IMU Ready");

  for (byte i = 0 ; i < 9 ; i++)
  {
    imuRawData.orientation_covariance[i] = 0;
    imuRawData.angular_velocity_covariance[i] = 0;
    imuRawData.linear_acceleration_covariance[i] = 0;
  }
  imuRawData.orientation_covariance[0] = -1;
}

void loop()
{
  // dataReady() checks to see if new accel/gyro data
  // is available. It will return a boolean true or false
  // (New magnetometer data cannot be checked, as the library
  //  runs that sensor in single-conversion mode.)
  if ( imu.dataReady() )
  {
    // Call update() to update the imu objects sensor data.
    // You can specify which sensors to update by combining
    // UPDATE_ACCEL, UPDATE_GYRO, UPDATE_COMPASS, and/or
    // UPDATE_TEMPERATURE.
    // (The update function defaults to accel, gyro, compass,
    //  so you don't have to specify these values.)
    imu.update(UPDATE_ACCEL | UPDATE_GYRO | UPDATE_COMPASS);
    printIMUData();
  }
}

void printIMUData(void)
{
  // After calling update() the ax, ay, az, gx, gy, gz, mx,
  // my, mz, time, and/or temerature class variables are all
  // updated. Access them by placing the object. in front:

  // Use the calcAccel, calcGyro, and calcMag functions to
  // convert the raw sensor readings (signed 16-bit values)
  // to their respective units.

  float accelX = imu.calcAccel(imu.ax);
  float accelY = imu.calcAccel(imu.ay);
  float accelZ = imu.calcAccel(imu.az);
  float gyroX = imu.calcGyro(imu.gx);
  float gyroY = imu.calcGyro(imu.gy);
  float gyroZ = imu.calcGyro(imu.gz);
  float magX = imu.calcMag(imu.mx);
  float magY = imu.calcMag(imu.my);
  float magZ = imu.calcMag(imu.mz);
  //  Serial.println("Accel: " + String(accelX) + ", " +
  //              String(accelY) + ", " + String(accelZ) + " g");
  //  Serial.println("Gyro: " + String(gyroX) + ", " +
  //              String(gyroY) + ", " + String(gyroZ) + " dps");
  //  Serial.println("Mag: " + String(magX) + ", " +
  //                 String(magY) + ", " + String(magZ) + " uT");
  //  Serial.println("Time: " + String(imu.time) + " ms");
  //Serial.println();
  imuRawData.header.stamp = nh.now();

  imuRawData.linear_acceleration.x = accelX;
  imuRawData.linear_acceleration.y = accelY;
  imuRawData.linear_acceleration.z = accelZ;

  imuRawData.angular_velocity.x = gyroX;
  imuRawData.angular_velocity.y = gyroY;
  imuRawData.angular_velocity.z = gyroZ;

  imuRawData.orientation.x = 0;
  imuRawData.orientation.y = 0;
  imuRawData.orientation.z = 0;
  imuRawData.orientation.w = 0;
  imu_pub.publish(&imuRawData);
  nh.spinOnce();
}
