#include <Servo.h>
#include <SPI.h>
#include <Ethernet.h>

int REW_RELAY = 7;
int FEED_RELAY = 8;
int FIRE_BUTTON = 6;
int LEFT_BUTTON = 11;
int RIGHT_BUTTON = 12;
int UP_BUTTON = 2;
int DOWN_BUTTON = 3;
Servo HORIZONTAL_SERVO;
Servo VERTICAL_SERVO;
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
EthernetServer server(80);
EthernetClient client;

String command = "";

void setup() {
  Serial.begin(115200);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  pinMode(REW_RELAY, OUTPUT);
  pinMode(FEED_RELAY, OUTPUT);
  pinMode(FIRE_BUTTON, INPUT_PULLUP);
  pinMode(LEFT_BUTTON, INPUT_PULLUP);
  pinMode(RIGHT_BUTTON, INPUT_PULLUP);
  pinMode(UP_BUTTON, INPUT_PULLUP);
  pinMode(DOWN_BUTTON, INPUT_PULLUP);
  HORIZONTAL_SERVO.attach(9);
  VERTICAL_SERVO.attach(5);

  Ethernet.begin(mac);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());

  int currentAngle = HORIZONTAL_SERVO.read();
  Serial.println(currentAngle);
}

void loop() {
  client = server.available();
  if (client) {
    boolean currentLineIsBlank = true;
    String readString = String(100);
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (readString.length() < 100) {
          readString += c;
        }
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) {
          if (readString.indexOf("key=test") != -1) {
            if (readString.indexOf("command=fire") != -1) {
              burstFire();
              sendResponse("Command:fire");
              break;
            } else if (readString.indexOf("command=left") != -1) {
              turnLeft();
              sendResponse("Command:left");
            }
            else if (readString.indexOf("command=right") != -1) {
              turnRight();
              sendResponse("Command:right");
            } else {
              sendResponse("CommandNotFound");
              break;
            }
          } else {
            sendResponse("AccessDenied");
            break;
          }
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
  }
  if (digitalRead(FIRE_BUTTON) == LOW) {
    Serial.println("fire");
    burstFire();
  }
  if (digitalRead(LEFT_BUTTON) == LOW) {
    Serial.println("left");
    turnLeft();
  }
  if (digitalRead(RIGHT_BUTTON) == LOW) {
    Serial.println("right");
    turnRight();
  }
  if (digitalRead(UP_BUTTON) == LOW) {
    Serial.println("up");
    turnUp();
  }
  if (digitalRead(DOWN_BUTTON) == LOW) {
    Serial.println("down");
    turnDown();
  }
}

void sendResponse(String response) {
  Serial.println("sendResponse:" + response);
  client.println("HTTP/1.1 200 OK");
  client.println("Access-Control-Allow-Origin: *");
  //client.println("Access-Control-Allow-Headers: *");
  client.println("Content-Type: text/plain");
  client.println("Connection: close");
  client.println();
  client.println(response);
  delay(100);
  client.stop();
}

void burstFire() {
  Serial.println("burstFire");
  digitalWrite(REW_RELAY, HIGH);
  delay(1000);
  digitalWrite(FEED_RELAY, HIGH);
  delay(1000);
  digitalWrite(FEED_RELAY, LOW);
  digitalWrite(REW_RELAY, LOW);
}

void turnLeft() {
  int currentAngle = HORIZONTAL_SERVO.read();
  Serial.println("turnLeft");
  Serial.println(currentAngle);
  //HORIZONTAL_SERVO.write(145);
  HORIZONTAL_SERVO.write(145);
  delay(1000);
}

void turnRight() {
  int currentAngle = HORIZONTAL_SERVO.read();
  Serial.println("turnRight");
  Serial.println(currentAngle);
  //HORIZONTAL_SERVO.write(45);
  HORIZONTAL_SERVO.write(50);
  delay(1000);
}

void turnDown() {
  int currentAngle = VERTICAL_SERVO.read();
  Serial.println("turnDown");
  Serial.println(currentAngle);
  //HORIZONTAL_SERVO.write(145);
  VERTICAL_SERVO.write(120);
  delay(1000);
}

void turnUp() {
  int currentAngle = VERTICAL_SERVO.read();
  Serial.println("turnUp");
  Serial.println(currentAngle);
  //HORIZONTAL_SERVO.write(45);
  VERTICAL_SERVO.write(50);
  delay(1000);
}

