#include "CurieIMU.h"
#include "BMI160.h"

///// INITIALIZE //////

// delay (microseconds) to prevent overloading the XBee connection
int delayTime(1000);

// the 6 values of the IMU
// in case of CurieIMU library, this has to be int32_t, but the compiler complains
// it should be int instead of int32_t, even though these types have the same precision and range.
int ax, ay, az;
int gx, gy, gz;

// bit startpoint for parsing the serial data
uint8_t start = 255;

// MSB/LSB variables for encapsulation
uint8_t msbax, lsbax, msbay, lsbay, msbaz, lsbaz;
uint8_t msbgx, lsbgx, msbgy, lsbgy, msbgz, lsbgz;

void setup()
{
    Serial1.begin(115200); // initialize Serial communication
    while (!Serial1);    // wait for the serial port to open

    CurieIMU.begin();

    // sampling rates
    CurieIMU.setAccelerometerRate(1600);
    CurieIMU.setGyroRate(3200);

    // ranges
    CurieIMU.setGyroRange(2000);
  CurieIMU.setAccelerometerRange(2);

    CurieIMU.noGyroOffset();
    CurieIMU.noAccelerometerOffset();
}

void loop()
{
    // read raw accel/gyro measurements from the sensor
    CurieIMU.readMotionSensor(ax, ay, az, gx, gy, gz);

    // convert sensor values from int to uint range (0...65535)
    ax += 32768;
    ay += 32768;
    az += 32768;

    gx += 32768;
    gy += 32768;
    gz += 32768;

    // cut off highest values (2^16-1, reserved for the "start" bytes)
    if (ax == 65535) ax = 65534;
    if (ay == 65535) ay = 65534;
    if (az == 65535) az = 65534;

    if (gx == 65535) gx = 65534;
    if (gy == 65535) gy = 65534;
    if (gz == 65535) gz = 65534;

    // send int16_t as 2 int8_ts for transmission and inhibit consecutive 255 bytes
    msbax = (uint8_t)(ax >> 8); lsbax = (uint8_t)(ax & 255);
    msbay = (uint8_t)(ay >> 8); lsbay = (uint8_t)(ay & 255);
    if(lsbax == 255 && msbay == 255) {
        lsbax = 254;
    }
    msbaz = (uint8_t)(az >> 8); lsbaz = (uint8_t)(az & 255);
    if(lsbay == 255 && msbaz == 255) {
        lsbay = 254;
    }
    msbgx = (uint8_t)(gx >> 8); lsbgx = (uint8_t)(gx & 255);
    if(lsbaz == 255 && msbgx == 255) {
        lsbaz = 254;
    }
    msbgy = (uint8_t)(gy >> 8); lsbgy = (uint8_t)(gy & 255);
    if(lsbgx == 255 && msbgy == 255) {
        lsbgx = 254;
    }
    msbgz = (uint8_t)(gz >> 8); lsbgz = (uint8_t)(gz & 255);
    if(lsbgy == 255 && msbgz == 255) {
        lsbgy = 254;
    }
    if(lsbgz == 255) {
        lsbgz = 254;
    }

    // transmit values
    Serial1.write(start); Serial1.write(start);
    Serial1.write(msbax); Serial1.write(lsbax);
    Serial1.write(msbay); Serial1.write(lsbay);
    Serial1.write(msbaz); Serial1.write(lsbaz);
    Serial1.write(msbgx); Serial1.write(lsbgx);
    Serial1.write(msbgy); Serial1.write(lsbgy);
    Serial1.write(msbgz); Serial1.write(lsbgz);

    // prevent overloading the XBee connection
    Serial1.flush();
    delayMicroseconds(delayTime);
}
