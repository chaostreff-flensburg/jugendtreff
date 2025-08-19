#define SERVOMIN 150 // This is the 'minimum' pulse length count (out of 4096)
#define SERVOMAX  4096 // This is the 'maximum' pulse length count (out of 4096)
#define USMIN  150 // This is the rounded 'minimum' microsecond length based on the minimum pulse of 150
#define USMAX  600 // This is the rounded 'maximum' microsecond length based on the maximum pulse of 600
#define SERVO_FREQ 50 // Analog servos run at ~50 Hz updates

#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

class Actuator {
public:
  Actuator(Adafruit_PWMServoDriver &newPwm, int newChannel = 0, int newMinPwm = 200, int newMaxPwm = 400, int newSpeed = 50, int newInterval = 30):
    pwm(newPwm),
    channel(newChannel),
    minPwm(newMinPwm),
    maxPwm(newMaxPwm),
    posSet(0),
    posCur(0),
    speed(newSpeed),
    updateIntervalMs(newInterval),
    timeNextUpdateMs(0)
  {
    if (speed < 0) speed *= -1;
    if (updateIntervalMs < 0) updateIntervalMs *= -1;
    posSet = posCur = (minPwm + maxPwm)/2;
  }

  void setPosition(int newPos) {posSet = newPos;} // set position in range 0..1000
  void setPosition(int newPos, int newSpeed) {posSet = newPos; speed = newSpeed;} // set position in range 0..1000 with new speed

  void update() {
    // call this function as often as you like but at least every updateIntervalMs
    if(millis() > timeNextUpdateMs) {
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

Actuator servo1(pwm, 15, 200, 400, 150); // horizontal eye (min is leftmost position (looking to the left))
Actuator servo2(pwm, 14, 200, 400, 150); // vertical eye (min is lowest position (looking down))
Actuator servo3(pwm, 13, 200, 400, 150); // low lid of right eye (min is maximum closed position (highest))
Actuator servo4(pwm, 12, 200, 400, 150); // high lid of right eye (min is maximum closed position (lowest))

const int potPin3 = A3;
const int potPin2 = A2;
const int potPin1 = A1;
const int potPin0 = A0;

void setup() {
  Serial.begin(9600);
  Serial.setTimeout(5);
  Serial.println("Start Servos");
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(50);
  for (int i=1; i<=15; i+=2) {
    pwm.setPWM(i, 0, 300);
    delay(200);
  }

  pinMode(potPin3, INPUT);
  pinMode(potPin2, INPUT);
  pinMode(potPin1, INPUT);
  pinMode(potPin0, INPUT);
}

void loop() {
  
  Serial.println(analogRead(potPin3));
  /*
  Serial.println(potentiometerToServo(potPin2));
  Serial.println(potentiometerToServo(potPin1));
  Serial.println(potentiometerToServo(potPin0));
  Serial.println("====");
  */
  
  
  servo1.update();
  servo2.update();
  servo3.update();
  servo4.update();

  servo1.setPosition(potentiometerToServo(A0));
  servo2.setPosition(potentiometerToServo(A1));
  servo3.setPosition(potentiometerToServo(A2));
  servo4.setPosition(potentiometerToServo(A3));
  Serial.println(analogRead(potPin0));
  Serial.println(analogRead(potPin1));
  Serial.println(analogRead(potPin2));
  Serial.println(analogRead(potPin3));
  Serial.println("-----------");
  /*
  int servoNumber = Serial.parseInt();
  if (servoNumber != 0) {
    int setPoint = Serial.parseInt();
    if (setPoint != 0) {
      Serial.print(servoNumber);
      Serial.print(" - ");
      Serial.println(setPoint);
      switch(servoNumber) {
      case 1:
        servo1.setPosition(setPoint);
        break;
      case 2:
        servo2.setPosition(setPoint);
        break;
      case 3:
        servo3.setPosition(setPoint);
        break;
      case 4:
        servo4.setPosition(setPoint);
        break;
      default:
        break;
      }
    }
  }
  */
}

int potentiometerToServo(int potentiometerPin){
  return map(analogRead(potentiometerPin), 0, 1023, 0, 1000); // SERVOMIN, SERVOMAX
}
