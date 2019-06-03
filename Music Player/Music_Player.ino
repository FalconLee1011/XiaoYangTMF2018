/*
  Coded by Falcon Lee falcon.lee1011@gmail.com 
  Jul.1 2018

  This code is licensed under the Apache License, Version 2.0
  for more information, please see LICENSE.md
*/

#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>

SoftwareSerial esp(10, 11);
SoftwareSerial dfpSoftS(19, 18); // RX, TX
DFRobotDFPlayerMini dfp;

String ssid = "XiaoYang";
String pass = "maker7788";

String rvc = "aaa";

boolean debugOpt = true;

void setup(){

  pinMode(13, OUTPUT);
  digitalWrite(13, 0);

  if(debugOpt)Serial.begin(9600);

  esp.begin(9600);
  //esp.println("AT+CWJAP=\"" + ssid + "\",\"" + pass + "\"");
  //while(!esp.find("OK"))
  esp.println("AT+CIPMUX=1");
  while(!esp.find("OK"));
  esp.println("AT+CIPSERVER=1,80");
  while(!esp.find("OK"));
  //esp.println("AT+CIPSTO=7000");
  //while(!esp.find("OK"));
  esp.println("AT+CIPSTART=0,\"UDP\",\"0.0.0.0\",80,80,0");

  delay(20);


  dfpSoftS.begin(9600);
  dfp.begin(dfpSoftS);
  dfp.volume(30);
  dfp.play(3);
  delay(900);

  digitalWrite(13, 1);
  esp.println("AT+CIPSTART=0,\"UDP\",\"0.0.0.0\",80,80,0");
}

void loop() {
  while (esp.available()) {
    rvc = esp.readStringUntil('>');
    if(debugOpt)Serial.println("Recieved = " + rvc);
  }


  if(rvc.indexOf("+IPD,0,5:play") > 0){
    if(debugOpt)Serial.println("line");
    dfp.play(2);
    delay(900);
  }

  else if(rvc.indexOf("+IPD,0,6:playW") > 0){
    if(debugOpt)Serial.println("Win");
    dfp.play(1);
    delay(900);
  }

  else if(rvc.indexOf("+IPD,0,4:hit") > 0){
    if(debugOpt)Serial.println("hit");
    dfp.play(3);
    delay(900);
  }
  else if(rvc.indexOf("+IPD,0,6:play0") > 0){
    if(debugOpt)Serial.println("000");
    dfp.play(4);
    delay(900);
  }

  rvc = "";
}
