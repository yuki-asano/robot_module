#include <Encoder.h>

class EncoderPololu : public Encoder {
 public:
  float totalCPR;             // CPR: Counts per Revolution
  volatile long encoderCount; // volatile variable could be changed in MsTimer
  
 EncoderPololu(uint8_t pin1, uint8_t pin2, int encoderCPR, float gearRatio) :
  Encoder(pin1, pin2)
    {
      totalCPR = gearRatio * encoderCPR;
      encoderCount = 0;
    }
  
  ~EncoderPololu() {};

  void updateEncoderCount() {
    encoderCount = this->read();
  }

  void resetEncoderCount() {
    encoderCount = this->readAndReset();
  }

  void writeEncoderCount(long encoderCount) {
    this->write(encoderCount);
  }
  
  void overwriteMotorAngle(float angle) {
    encoderCount = angle / 360.0 * totalCPR;
    this->write(encoderCount);
  }

  float getEncoderCount() {
    updateEncoderCount();
    return encoderCount;
  }

  float getMotorAngle() {
    float angle = 0;
    updateEncoderCount();
    return angle =  encoderCount * 360 / totalCPR;
    // Example:
    // 4.4(gear) * 48(CPR) = 211.2(total CPR) -> counts per revolution
    // 360 [deg] : 211.2 = x [deg] : encoderCount
    // x = encoderCount * 360 / 211.2
  }

};
