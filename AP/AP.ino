/*
  Coded by Falcon Lee falcon.lee1011@gmail.com
  Jul.1 2018

  This code is licensed under the Apache License, Version 2.0
  for more information, please see LICENSE.md
*/

#include <SoftwareSerial.h>
#include <AFMotor.h>
#include <Servo.h>

#define servo1 10

#define rPin A15
#define gPin 53
#define bPin 51

SoftwareSerial esp(A8, A9);

const int rctUPin = 28;
const int rctDPin = 26;
const int rctLPin = 24;
const int rctRPin = 22;


void staLed(int rv = 1, int gv = 1, int bv = 1, int dt = 100, int tt = 4);
void set_Led(int rv = 1, int gv = 1, int bv = 1);

int ct, lt;
String rv;
String rvC = "sss";
int dd = 5;
int lrDeg = 90;
int rctL, rctR, rctU, rctD = 1;

boolean debug = false;
boolean rctDisabled = true;

AF_DCMotor motor1(1);
AF_DCMotor motor2(2);

Servo servo_01;

void setup() {
  esp.begin(9600);

  if(debug)Serial.begin(9600);

  //push();
  //delay(1000);
  //push();

  pinMode(13, OUTPUT);
  pinMode(rPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(bPin, OUTPUT);
  pinMode(rctUPin, INPUT);
  pinMode(rctDPin, INPUT);
  pinMode(rctLPin, INPUT);
  pinMode(rctRPin, INPUT);
  digitalWrite(rPin, 0);
  digitalWrite(gPin, 0);
  digitalWrite(bPin, 0);
  //digitalWrite(53, 0);

  setAPs();
  digitalWrite(13, 0);

  servo_01.attach(servo1);
  servo_01.write(90);
  delay(1000);
  servo_01.detach();

  left();
  if(debug)Serial.println("left");
  delay(750);
  intalize();
  right();
  if(debug)Serial.println("right");
  delay(750);
  intalize();
  up();
  if(debug)Serial.println("up");
  delay(500);
  intalize();
  down();
  if(debug)Serial.println("down");
  delay(500);
  intalize();

  motor1.setSpeed(255);
  motor1.run(RELEASE);
  motor2.setSpeed(255);
  motor2.run(RELEASE);

  if(debug)Serial.println("Ready!");
  set_Led(0, 0, 1);
  while (!esp.find("Hello!"));
  digitalWrite(13, 1);

  //digitalWrite(53, 1);
  if(debug)Serial.println("Connected!");
  set_Led(0, 1, 0);
}

void loop() {

  getData();

  get_Rct();
  if(debug)Serial.println(String(rctL) + " " + String(rctR) + " " + String(rctU) + " " + String(rctD) + " ");

  while (rvC.indexOf("+IPD,0,2:5") > 0) {
    getData();
    //digitalWrite(13, 1);
    push();
    if(debug)Serial.println("Shoot");
    delay(dd);
    rvC = "";
  }

  while (rvC.indexOf("+IPD,0,2:1") > 0) {
    getData();
    get_Rct();
    //rctU = digitalRead(rctUPin);
    if(debug)Serial.println("up");
    if(debug)Serial.println(String(rctL) + " " + String(rctR) + " " + String(rctU) + " " + String(rctD) + " ");
    if(rctU == 1 or rctDisabled == true)up();
    //digitalWrite(3, 1);
    delay(dd);
    //rvC = "";
  }

  while (rvC.indexOf("+IPD,0,2:2") > 0) {
    getData();
    get_Rct();
    //rctD = digitalRead(rctDPin);
    if(debug)Serial.println("down");
    if(debug)Serial.println(String(rctL) + " " + String(rctR) + " " + String(rctU) + " " + String(rctD) + " ");
    if(rctD == 1 or rctDisabled == true)down();
    //digitalWrite(2, 1);
    delay(dd);
    //rvC = "";
  }

  while (rvC.indexOf("+IPD,0,2:3") > 0) {
    getData();
    get_Rct();
    //rctR = digitalRead(rctRPin);
    if(debug)Serial.println("right");
    if(debug)Serial.println(String(rctL) + " " + String(rctR) + " " + String(rctU) + " " + String(rctD) + " ");
    if(rctR == 1 or rctDisabled == true)right();
    //digitalWrite(5, 1);
    delay(dd);
    //rvC = "";
  }

  while (rvC.indexOf("+IPD,0,2:4") > 0) {
    getData();
    get_Rct();
    //rctL = digitalRead(rctLPin);
    if(debug)Serial.println("left");
    if(debug)Serial.println(String(rctL) + " " + String(rctR) + " " + String(rctU) + " " + String(rctD) + " ");
    if(rctL == 1 or rctDisabled == true)left();
    //digitalWrite(4, 1);
    delay(dd);
    //rvC = "";
  }

  while (rvC.indexOf("+IPD,0,2:9") > 0) {
    getData();
    intalize();
    //digitalWrite(13, 1);
    //delay(400);
    rvC = "";
  }

  set_Led(0, 1, 0);
  //intalize();
}

void getData(){
  while (esp.available()) {
    rvC = esp.readStringUntil('>');
    set_Led(0, 1, 1);
  }
  if(debug)Serial.println("Recieved :" + rvC);
}

int get_Rct(){
  rctL = digitalRead(rctLPin);
  rctR = digitalRead(rctRPin);
  rctU = digitalRead(rctUPin);
  rctD = digitalRead(rctDPin);
}

void setAPs() {
  /*
    esp.println("AT+RST");
    delay(500);
    while(!esp.find("OK"));

    esp.println("AT+CWJAP=\"ZeroPoint0\",\"nonetno$hit\"");
    while(!esp.find("OK"));
  */

  esp.println("AT+CIPMUX=1");
  while (!esp.find("OK"));
  if(debug)Serial.println("CIPMUX set to 1.");

  esp.println("AT+CIPSERVER=1,80");
  while (!esp.find("OK"));
  if(debug)Serial.println("port 80 opened.");
  esp.println("AT+CIPSTART=0,\"UDP\",\"0.0.0.0\",80,80,0");
  delay(20);
  if(debug)Serial.println("UDP server online.");

}

void intalize() {
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}

void down() {
  motor1.setSpeed(255);
  motor1.run(FORWARD);
}

void up() {
  motor1.setSpeed(255);
  motor1.run(BACKWARD);
}

void left() {
  motor2.setSpeed(255);
  motor2.run(FORWARD);
}

void right() {
  motor2.setSpeed(255);
  motor2.run(BACKWARD);
}

void push() {
  servo_01.attach(servo1);
  delay(100);
  servo_01.write(130);
  delay(500);
  servo_01.write(90);
  delay(500);
  servo_01.detach();
}

void staLed(int rv = 1, int gv = 1, int bv = 1, int dt = 100, int tt = 4) {
  int ii = 0;
  while (ii <= tt) {
    digitalWrite(rPin, rv);
    digitalWrite(gPin, gv);
    digitalWrite(bPin, bv);
    delay(dt);
    digitalWrite(rPin, 0);
    digitalWrite(gPin, 0);
    digitalWrite(bPin, 0);
    delay(dt);
    ii += 1;
  }
}


void set_Led(int rv = 1, int gv = 1, int bv = 1) {
  digitalWrite(rPin, rv);
  digitalWrite(gPin, gv);
  digitalWrite(bPin, bv);
}
