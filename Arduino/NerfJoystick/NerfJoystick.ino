#include "IRremote.h"
#include <Servo.h>
Servo servo1; // Define our Servo
Servo servo2; // Define our Servo

//Servo 1
int S1 = 10;
//Servo 2
int S2 = 11;
//Rev
int R = 9;
//Fire
int F = 12;
// Signal Pin of IR receiver
int C = 13;
int receiver = 13;
//Previous IR CODE
long Previous;
//Speed variables
int SpeedV;
int SpeedH;
//Patrol variables
bool Patrol = false;
bool PatrolRight = true;

//JOYSTIC
const int SW_pin = 2;
const int X_pin = 0;
const int Y_pin = 1;
/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'

//Fireing timing
unsigned long revMillis = 0;
unsigned long fireMillis = 0;
const long revInterval = 1500;
const long fireInterval = 1000;
//Angle Range
int MaxAngleV = 140;
int MinAngleV = 40;
int MaxAngleH = 140;
int MinAngleH = 40;

//Setup
void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  //Speed
  SpeedV = 10;
  SpeedH = 5;
  servo1.attach(S1);
  servo2.attach(S2);

  pinMode(R, OUTPUT);
  pinMode(F, OUTPUT);
  pinMode(C, OUTPUT);     
  
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode"); 
  irrecv.enableIRIn(); // Start the receiver
  Serial.println(revInterval); 
  Serial.println(revMillis); 
  //JOYSTIC
  pinMode(SW_pin, INPUT);
  digitalWrite(SW_pin, HIGH);
  
}/*--(end setup )---*/
 

void loop()   
{
  //Check fireing timeing
  checkFireing();
  //Check if Patrol Mode is active
  checkPatrol();
  //Joystick
  joyStickRead();
  //joyStickControl();
  joyStickControlResponsive();
  //joyStickControlExact();
  //IR DECODING
  if (irrecv.decode(&results)) // have we received an IR signal? 
  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  
}
 
/*-----( Function )-----*/
void joyStickRead(){
  Serial.print("Switch:  ");
  Serial.print(digitalRead(SW_pin));
  Serial.print("\n");
  Serial.print("X-axis: ");
  Serial.print(analogRead(X_pin));
  Serial.print("\n");
  Serial.print("Y-axis: ");
  Serial.println(analogRead(Y_pin));
  Serial.print("\n\n");
  delay(10);  
}
void joyStickControl(){
  int x = analogRead(X_pin);
  int y = analogRead(Y_pin);
  int sw = digitalRead(SW_pin);
  if(x > 530){
    turnServoRight();
  }
  if(x < 516){
    turnServoLeft();
  }
  if(y < 520){
    turnServoUp();
  }
  if(y > 540){
    turnServoDown();
  }
  if(sw == 0){
    fireBurst();
  }
}
void joyStickControlResponsive(){
  int x = analogRead(X_pin);
  int y = analogRead(Y_pin);
  int sw = digitalRead(SW_pin);
  int xSpeed = joyStickGetSpeed(x);
  int ySpeed = joyStickGetSpeed(y);
  if(x > 530){
    turnServoRightSpeed(xSpeed);
  }
  if(x < 516){
    turnServoLeftSpeed(xSpeed);
  }
  if(y < 520){
    turnServoUpSpeed(ySpeed);
  }
  if(y > 540){
    turnServoDownSpeed(ySpeed);
  }
  if(sw == 0){
    fireBurst();
  }
}
int joyStickGetSpeed(int output){
  int speed = 1;
  if((output >= 0 && output < 100) || (output >= 900 && output < 1050)){
    speed = 15;
  }else if((output >= 100 && output < 200) || (output >= 800 && output < 900)){
    speed = 10;
  }else if((output >= 200 && output < 300) || (output >= 700 && output < 800)){
    speed = 5;
  }else if((output >= 300 && output < 515) || (output >= 545 && output < 700)){
    speed = 1;
  }else {
    speed = 0;
  }
  return speed;
}
void joyStickControlExact(){
  int x = analogRead(X_pin);
  int y = analogRead(Y_pin);
  int sw = digitalRead(SW_pin);
  
  setServoX(joyStickGetAngle(MinAngleH,MaxAngleH,x));
  setServoY(joyStickGetAngle(MinAngleV,MaxAngleV,y));
  if(sw == 0){
    fireBurst();
  }

}
int joyStickGetAngle(int min, int max, int output){  
  int deltaA = 0;
  if(output != 0){
    deltaA = output/10;
  }
  int a = min + deltaA;
  if(a < min){
    a = min;
  }else if (a > max){
    a = max;
  }
  return a;
}


void checkFireing(){
  unsigned long currentMillis = millis();
  if (currentMillis - revMillis >= revInterval && revMillis != 0) {
      Serial.println(revInterval);
      fire();
      fireMillis = millis();
      revMillis = 0;
  }else if (currentMillis - fireMillis >= fireInterval && fireMillis != 0) {
      fireStop();
      revStop();
      fireMillis = 0;
  }
}
void checkPatrol(){
  if(Patrol){
    Serial.println("Patroling Perimiter!");
    if(servo1.read() == MaxAngleH){
      PatrolRight = false;
    }else if(servo1.read() == MinAngleH){
      PatrolRight = true;
    }
    if(PatrolRight){
      turnServoRight();
      delay(100);
    }else{
      turnServoLeft();
      delay(100);
    }
  }
}
// takes action based on IR code received 
// describing Remote IR codes
void translateIR()   
{
  //Serial.println(results.value);
  long code = results.value;
  Serial.println(code);
  Serial.println(Previous);
  if(code == -1){
    Serial.println(" hej REPEAT");
    Serial.println(Previous);
    code = Previous;
  }
  switch(code) 
  { 
  case 16753245: Serial.println(" Power"); break;
  case 16769565: Serial.println(" Menu"); patrolToggle(); break;
  case 16720605: Serial.println(" Test"); fireToggle(); break;
  case 16761405: Serial.println(" Back"); revToggle();  break;
  case 16769055: Serial.println(" Left"); turnServoLeft(); break;
  case 16712445: Serial.println(" Up"); turnServoUp(); break;
  case 16748655: Serial.println(" Right"); turnServoRight(); break;
  case 16750695: Serial.println(" Down"); turnServoDown(); break;
  case 16754775: Serial.println(" Play"); fireBurst(); break;
  case 16756815: Serial.println(" C"); revStop();break;  
  case 16738455: Serial.println(" 0");setServo(90);    break;
  case 16724175: Serial.println(" 1");setServo(MinAngleH);    break;
  case 16718055: Serial.println(" 2");setServo(52);    break;
  case 16743045: Serial.println(" 3");setServo(64);    break;
  case 16716015: Serial.println(" 4");setServo(76);    break;
  case 16726215: Serial.println(" 5");setServo(90);    break;
  case 16734885: Serial.println(" 6");setServo(104);    break;
  case 16728765: Serial.println(" 7");setServo(116);    break;
  case 16730805: Serial.println(" 8");setServo(128);    break;
  case 16732845: Serial.println(" 9"); setServo(MaxAngleH);   break;
  case 4294967295: Serial.println(" REPEAT");break;
  
  default: 
    Serial.println(" other button   ");
  }
  //setPrevious
  if(results.value != 4294967295){
    Serial.println("setting prev");
    Previous = results.value;
    Serial.println(Previous);  
  }  
} //END translateIR
//PATROLING FUNCTIONS
void patrolToggle(){
  if(Patrol){
    Patrol = false;
  }else{
    Patrol = true;
  }
  delay(100);// Do not get immediate repeat
}
//FIRERING FUNCTIONS
void fireBurst(){
  rev();
  revMillis = millis(); 
}
void fire(){
  digitalWrite(F, HIGH);  
}
void fireStop(){  
  digitalWrite(F, LOW);
}
void fireToggle(){
  if(digitalRead(F) == LOW){
    fire();
  }else{
    fireStop();
  }
  delay(100); // Do not get immediate repeat
}
//REVING FUNCTIONS
void rev(){
  digitalWrite(R, HIGH);
}
void revStop(){
  digitalWrite(R,LOW);
}
void revToggle(){
  if(digitalRead(R) == LOW){
    rev();
  }else{
    revStop();
  }
  delay(100); // Do not get immediate repeat
}
//SERVO FUNCTIONS
void setServo(int a){
  servo1.write(a);
  servo2.write(a);
}
void setServoX(int a){
  servo1.write(a);
}
void setServoY(int a){
  servo2.write(a);
}
void turnServoLeft(){
  int a = servo1.read();
  int newA = a-SpeedH;
  if(newA >=MinAngleH){
    servo1.write(newA);
  }else{
    servo1.write(MinAngleH);
  }  
}
void turnServoRight(){
  int a = servo1.read();
  int newA = a+SpeedH;
  if(newA <= MaxAngleH){
    servo1.write(newA);
  }else{
    servo1.write(MaxAngleH);
  } 
}
void turnServoUp(){
  int a = servo2.read();
  int newA = a+SpeedV;
  if(newA <= MaxAngleV){
    servo2.write(newA);
  }else{
    servo2.write(MaxAngleV);
  } 
}
void turnServoDown(){
  int a = servo2.read();
  int newA = a-SpeedV;
  if(newA >= MinAngleV){
    servo2.write(newA);
  }else{
    servo2.write(MinAngleV);
  } 
}
//Turn servo with custom speed
void turnServoLeftSpeed(int Speed){
  int a = servo1.read();
  int newA = a-Speed;
  if(newA >=MinAngleH){
    servo1.write(newA);
  }else{
    servo1.write(MinAngleH);
  }  
}
void turnServoRightSpeed(int Speed){
  int a = servo1.read();
  int newA = a+Speed;
  if(newA <= MaxAngleH){
    servo1.write(newA);
  }else{
    servo1.write(MaxAngleH);
  } 
}
void turnServoUpSpeed(int Speed){
  int a = servo2.read();
  int newA = a+Speed;
  if(newA <= MaxAngleV){
    servo2.write(newA);
  }else{
    servo2.write(MaxAngleV);
  } 
}
void turnServoDownSpeed(int Speed){
  int a = servo2.read();
  int newA = a-Speed;
  if(newA >= MinAngleV){
    servo2.write(newA);
  }else{
    servo2.write(MinAngleV);
  } 
}

