#include "IRremote.h"
#include <Servo.h>
Servo servo1; // Define our Servo
Servo servo2; // Define our Servo
int pinA = 2;
int pinB = 3;
int pinC = 4;
int pinD = 5;
int pinE = 6;
int pinF = 7;
int pinG = 8;
int D1 = 9;
int D2 = 10;
int D3 = 11;
int D4 = 12;
int C = 13;
int S1 = 10;
int S2 = 11;
int receiver = 13; // Signal Pin of IR receiver to Arduino Digital Pin 11
long Previous;
int Speed;  
/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);     // create instance of 'irrecv'
decode_results results;      // create instance of 'decode_results'
 
void setup()   /*----( SETUP: RUNS ONCE )----*/
{
  Speed = 3;
  servo1.attach(S1);
  servo2.attach(S2);
  pinMode(pinA, OUTPUT);     
  pinMode(pinB, OUTPUT);     
  pinMode(pinC, OUTPUT);     
  pinMode(pinD, OUTPUT);     
  pinMode(pinE, OUTPUT);     
  pinMode(pinF, OUTPUT);     
  pinMode(pinG, OUTPUT);
  pinMode(D1, OUTPUT);
  //pinMode(D2, OUTPUT);
  //pinMode(D3, OUTPUT);
  pinMode(D4, OUTPUT);
  pinMode(C, OUTPUT);     
  
  Serial.begin(9600);
  Serial.println("IR Receiver Button Decode"); 
  irrecv.enableIRIn(); // Start the receiver
    
 
}/*--(end setup )---*/
 
 
void loop()   /*----( LOOP: RUNS CONSTANTLY )----*/
{
  if (irrecv.decode(&results)) // have we received an IR signal? 
  {
    translateIR(); 
    irrecv.resume(); // receive the next value
  }  
}/* --(end main loop )-- */
 
/*-----( Function )-----*/
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
  case 16753245: Serial.println(" Power");off(); break;
  case 16769565: Serial.println(" Menu");    break;
  case 16720605: Serial.println(" Test");    break;
  case 16761405: Serial.println(" Back");   break;
  case 16769055: Serial.println(" Left"); turnServoLeft(); break;
  case 16712445: Serial.println(" Up"); turnServoUp(); break;
  case 16748655: Serial.println(" Right"); turnServoRight(); break;
  case 16750695: Serial.println(" Down"); turnServoDown(); break;
  case 16754775: Serial.println(" Play"); break;
  case 16756815: Serial.println(" C"); break;  
  case 16738455: Serial.println(" 0");zero();setServo(90);    break;
  case 16724175: Serial.println(" 1");one();setServo(40);    break;
  case 16718055: Serial.println(" 2");two();setServo(52);    break;
  case 16743045: Serial.println(" 3");three();setServo(64);    break;
  case 16716015: Serial.println(" 4");four();setServo(76);    break;
  case 16726215: Serial.println(" 5");five();setServo(90);    break;
  case 16734885: Serial.println(" 6");six();setServo(104);    break;
  case 16728765: Serial.println(" 7");seven();setServo(116);    break;
  case 16730805: Serial.println(" 8");eight();setServo(128);    break;
  case 16732845: Serial.println(" 9");nine(); setServo(140);   break;
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
  delay(50); // Do not get immediate repeat
} //END translateIR
void setServo(int a){
  servo1.write(a);
  servo2.write(a);
}
void turnServoLeft(){
  int a = servo1.read();
  int newA = a-10;
  if(newA >=40){
    servo1.write(newA);
  }else{
    servo1.write(40);
  }  
}
void turnServoRight(){
  int a = servo1.read();
  int newA = a+10;
  if(newA <= 140){
    servo1.write(newA);
  }else{
    servo1.write(140);
  } 
}
void turnServoUp(){
  int a = servo2.read();
  int newA = a+10;
  if(newA <= 140){
    servo2.write(newA);
  }else{
    servo2.write(140);
  } 
}
void turnServoDown(){
  int a = servo2.read();
  int newA = a-10;
  if(newA >= 40){
    servo2.write(newA);
  }else{
    servo2.write(40);
  } 
}

//Digits
void digitSetUp(){
  digitalWrite(D1,LOW);
  digitalWrite(D2,LOW); 
  digitalWrite(D3,LOW); 
  digitalWrite(D4,LOW); 
}
void all(){
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);
  digitalWrite(C,HIGH);  
}
void off(){
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, LOW);
  digitalWrite(C,LOW);  
}
void zero(){
  //0
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, LOW);
}
void one(){
  //1
  digitalWrite(pinA, LOW);   
  digitalWrite(pinB, HIGH);  
  digitalWrite(pinC, HIGH);  
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, LOW);  
}
void two(){  
  //2
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, LOW);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, LOW);  
  digitalWrite(pinG, HIGH);   
}
void three(){
  //3
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, LOW);
  digitalWrite(pinF, LOW);
  digitalWrite(pinG, HIGH);
}
void four(){
  //4
  digitalWrite(pinA, LOW);
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);  
}
void five(){
  //5
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, LOW);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);   
}
void six(){
  //6
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, LOW);  
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);  
}
void seven(){
  //7
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, LOW);   
  digitalWrite(pinF, LOW);   
  digitalWrite(pinG, LOW); 
}
void eight(){
  //8
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, HIGH);   
  digitalWrite(pinE, HIGH);   
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH); 
}
void nine(){
  //9
  digitalWrite(pinA, HIGH);   
  digitalWrite(pinB, HIGH);   
  digitalWrite(pinC, HIGH);   
  digitalWrite(pinD, LOW);   
  digitalWrite(pinE, LOW);
  digitalWrite(pinF, HIGH);   
  digitalWrite(pinG, HIGH);
}
