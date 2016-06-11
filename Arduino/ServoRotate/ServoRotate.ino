#include <Servo.h>

Servo HORIZONTAL_SERVO;

void setup() {
  Serial.begin(115200);
  HORIZONTAL_SERVO.attach(9);
  HORIZONTAL_SERVO.write(90);
}

void loop() {
  delay(2000);
  HORIZONTAL_SERVO.write(170);
  Serial.println(HORIZONTAL_SERVO.read());
  delay(2000);
  HORIZONTAL_SERVO.write(10);
  Serial.println(HORIZONTAL_SERVO.read());
}

