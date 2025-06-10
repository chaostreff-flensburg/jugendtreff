#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

class Actuator {
public:
  Actuator(Adafruit_PWMServoDriver &newPwm, int newChannel = 0, int newMinPwm = 200, int newMaxPwm = 400, int newSpeed = 10, int newInterval = 30)
    : pwm(newPwm),
      channel(newChannel),
      minPwm(newMinPwm),
      maxPwm(newMaxPwm),
      speed(newSpeed),
      updateIntervalMs(newInterval),
      timeNextUpdateMs(0) {
    if (speed < 0) speed *= -1;
    if (updateIntervalMs < 0) updateIntervalMs *= -1;
    setPosition(500);
    posCur = posSet;
    update();
  }

  // set position in range 0..1000
  void setPosition(int newPos) {
    posSet = newPos > 1000 ? 1000 : newPos < 0 ? 0
                                               : newPos;
    update();
  }

  // set position in range 0..1000 with new speed
  void setPosition(int newPos, int newSpeed) {
    speed = newSpeed < 0 ? 0 : newSpeed;
    setPosition(newPos);
  }

  // initialize timing of the aActuator class
  void resetTime() {
    timeNextUpdateMs = millis();
  }

  // call this method as often as you like but at least every updateIntervalMs
  void update() {
    if (millis() > timeNextUpdateMs) {
      timeNextUpdateMs += updateIntervalMs;
      if (posCur < posSet) {
        // increasing position
        if (posSet - posCur < speed) {
          // by remaining bit
          posCur = posSet;
        } else {
          // by speed
          posCur += speed;
        }
      }
      if (posCur > posSet) {
        // decreasing position
        if (posCur - posSet < speed) {
          // by remaining bit
          posCur = posSet;
        } else {
          // by speed
          posCur -= speed;
        }
      }
      pwm.setPWM(channel, 0, map(posCur, 0, 1000, minPwm, maxPwm));
    }
  }

  // returns `true` if current position is at set position
  bool isTargetReached() {
    update();
    return posCur == posSet;
  }

protected:
  Adafruit_PWMServoDriver &pwm;
  int channel;
  int minPwm;
  int maxPwm;
  int posSet;
  int posCur;
  int speed;
  unsigned updateIntervalMs;
  unsigned long timeNextUpdateMs;
};


Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();


Actuator shoulderPan(pwm, 12, 650, 2350, 5);
Actuator shoulderTilt(pwm, 13, 650, 2350, 5);
Actuator ellbow(pwm, 14, 2350, 650, 5);
Actuator grip(pwm, 15, 2000, 1100, 20);


void reachTargets() {
  while (!shoulderPan.isTargetReached()
         || !shoulderTilt.isTargetReached()
         || !ellbow.isTargetReached()
         || !grip.isTargetReached()) {
    shoulderPan.update();
    shoulderTilt.update();
    ellbow.update();
    grip.update();
    delay(1);
  }
}

void myDelay(int ms) {
  auto endMillis = millis() + ms;
  while (millis() < endMillis) {
    shoulderPan.update();
    shoulderTilt.update();
    ellbow.update();
    grip.update();
    delay(1);
  };
}


void setup() {
  Serial.begin(9600);
  Serial.setTimeout(5);
  Serial.println("Servo Arm sequencer starting...");

  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  // 244 Hz for T = 4.096 ms, so that pwm.setPWM essentially takes uS as parameters
  //  most servos handle this just fine
  pwm.setPWMFreq(244);  //50);
  shoulderPan.resetTime();
  shoulderTilt.resetTime();
  ellbow.resetTime();
  grip.resetTime();
}

void loop() {
  grip.setPosition(1000);
  shoulderPan.setPosition(0);
shoulderTilt.setPosition(200);
ellbow.setPosition(600);
reachTargets();
shoulderTilt.setPosition(0);
reachTargets();
  grip.setPosition(250);
reachTargets();

  myDelay(1000);
   shoulderTilt.setPosition(400);
  ellbow.setPosition(400);
  
  reachTargets();
  shoulderPan.setPosition(700);
  reachTargets();
  
  
  reachTargets();

  myDelay(1000);
  grip.setPosition(600);
  reachTargets();

  

}
