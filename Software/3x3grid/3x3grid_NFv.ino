/*
  Coded by Falcon Lee falcon.lee1011@gmail.com
  Jul.1 2018

  This code is licensed under the Apache License, Version 2.0
  for more information, please see LICENSE.md
*/

#include <SoftwareSerial.h>

#define rPin A15
#define gPin 53
#define bPin 51
#define mrstPin 12

void staLed(int rv = 1, int gv = 1, int bv = 1, int dt = 100, int tt = 4);

SoftwareSerial esp(A8, A9);

/*
  0 1 2
  3 4 5
  6 7 8

  00 01 02
  10 11 12
  20 21 22
*/

const int statuL = 53;

const int rlp[3][3] = {37, 33, 29,
                       25, 22, 26,
                       30, 34, 38
                       };

const int lp[3][3] = {39, 35, 31,
                      27, 23, 24,
                      28, 32, 36
                       };
/*

const int rlp[3][3] = {38, 34, 30,
                      26, 23, 25,
                      31, 35, 37
                      };

const int lp[3][3] = {36, 32, 28,
                       24, 22, 27,
                       29, 33, 39
                       };


const int tp[3][3] = {2, 3, 4,
                      5, 6, 7,
                      8, 9, 10
                      };
*/

const int tp[3][3] = {10, 9, 8,
                      7, 6, 5,
                      4, 3, 2
                      };

int val[3][3] = {1, 1, 1,
                 1, 1, 1,
                 1, 1, 1
                 };

int lineH, lineV, cunt = 0;

int score = 0;

int mrst = 1;

int stuL = 53;

boolean pL[8] = {false, false, false, false, false, false, false, false};

boolean played = false;

boolean debugOpt = false;


boolean LineRst = false;


boolean c[3][3] = {false, false, false,
                   false, false, false,
                   false, false, false
                  };

boolean netWorkyn = true;

//String ssid = "ZeroPoint11";
//String pass = "n9WnEQFc";

String ssid = "XiaoYang";
String pass = "maker7788";

String musIP = "192.168.4.2";

void setup() {
  esp.begin(9600);
  if (debugOpt)Serial.begin(9600);
  esp.setTimeout(20);

  pinMode(rPin, OUTPUT);
  pinMode(gPin, OUTPUT);
  pinMode(bPin, OUTPUT);
  pinMode(mrstPin, INPUT);
  digitalWrite(rPin, 0);
  digitalWrite(gPin, 0);
  digitalWrite(bPin, 0);

  if(debugOpt)bdLd();

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      pinMode(lp[i][j], OUTPUT);
      pinMode(rlp[i][j], OUTPUT);
      pinMode(tp[i][j], INPUT);
    }
  }

  ConnectToAp(ssid, pass);

  if (netWorkyn)ConnectToMusic(musIP);

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      digitalWrite(lp[i][j], 1);
      digitalWrite(rlp[i][j], 0);
    }
  }
  digitalWrite(stuL, 1);

  if (debugOpt)Serial.println("ready");
}

void loop() {

  for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++) {
      val[i][j] = digitalRead(tp[i][j]);
      //val[i][j] = digitalRead(tp[0][0]);
      if (val[i][j] == 0 && c[i][j] == false) {
        if (debugOpt)Serial.println("hit");
        digitalWrite(lp[i][j], 0);
        digitalWrite(rlp[i][j], 1);
        c[i][j] = true;
        score += 1;
        if (debugOpt)Serial.println("score");
        if(score == 9){
          if (netWorkyn)WWCDMusic();
        }
        else if (c[0][0] && c[0][1] && c[0][2]  && !pL[0]) {
          if (netWorkyn) playMusic();
          pL[0] = true;
        }

        else if (c[1][0] && c[1][1] && c[1][2] && !pL[1]) {
          if (netWorkyn) playMusic();
          pL[1] = true;
        }

        else if (c[2][0] && c[2][1] && c[2][2] && !pL[2]) {
          if (netWorkyn) playMusic();
          pL[2] = true;
        }

        else if (c[0][0] && c[1][0] && c[2][0] && !pL[3]) {
          if (netWorkyn) playMusic();
          pL[3] = true;
        }

        else if (c[0][1] && c[1][1] && c[2][1] && !pL[4]) {
          if (netWorkyn) playMusic();
          pL[4] = true;
        }

        else if (c[0][2] && c[1][2] && c[2][2] && !pL[5]) {
          if (netWorkyn) playMusic();
          pL[5] = true;
        }

        else if (c[0][0] && c[1][1] && c[2][2] && !pL[6]) {
          if (netWorkyn) playMusic();
          pL[6] = true;
        }

        else if (c[0][2] && c[1][1] && c[2][0] && !pL[7]) {
          if (netWorkyn) playMusic();
          pL[7] = true;
        }
        else {
          if (netWorkyn) {
            hitMuic();
          }
        }
      }
    }
  }
  /*
  if(LineRst){
    for(int iii = 0; iii <= 7; iii++){
      int xx = 0;
      if(pL[iii] == true){
        xx = xx + 1;
      }
      else{
        xx = 0;
      }
    }
  }
  */
  if(debugOpt) Serial.println("------------------------");
  if(debugOpt) Serial.println(String(c[0][0]) + "  " + String(c[0][1]) + "  " + String(c[0][2]));
  if(debugOpt) Serial.println(String(c[1][0]) + "  " + String(c[1][1]) + "  " + String(c[1][2]));
  if(debugOpt) Serial.println(String(c[2][0]) + "  " + String(c[2][1]) + "  " + String(c[2][2]));
  if(debugOpt) Serial.println("------------------------");


  /*
    00 01 02
    10 11 12
    20 21 22
  */

  mrst = digitalRead(mrstPin);

  if (esp.find("stop") or mrst == 1) {
    if (debugOpt)Serial.println("stopped");
    digitalWrite(stuL, 0);
    staLed(1, 0, 1, 50, 3);
    if (score == 0) {
      if (netWorkyn)fuckedUpMusic();
    }
    else if (score == 9) {
      if (netWorkyn)WWCDMusic();
    }

    if(mrst != 1)while (!esp.find("rest"));

    if (debugOpt)Serial.println("restting.");
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        digitalWrite(lp[i][j], 0);
        digitalWrite(rlp[i][j], 0);
        c[i][j] = false;
        if (debugOpt)Serial.print(".");
      }
    }
    staLed(1, 0, 1, 50, 10);
    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        digitalWrite(lp[i][j], 1);
        digitalWrite(rlp[i][j], 0);
        c[i][j] = false;
        if (debugOpt)Serial.print(".");
      }
    }
    for (int j = 0; j < 8; j++) {
      pL[j] = false;
    }
    played = false;
    score = 0;
    if (debugOpt)Serial.println("\nreset_done");
    digitalWrite(stuL, 1);
  }

  if (debugOpt)Serial.println("Loop done");
}


void ConnectToAp(String ssidd, String passs) {
  int xx = 0;
  esp.println("AT+CWJAP=\"" + ssidd + "\",\"" + passs + "\"");
  while (!esp.find("OK")) {
    xx += 1;
    delayMicroseconds(1);
    if(esp.find("OK"))break;
    if (xx > 300) {
      if (debugOpt)Serial.println("No_wifi");
      staLed(1, 1, 1, 100, 5);
      stuL = A15;
      netWorkyn = false;
      break;
    }
  }

  if (netWorkyn) {
    esp.println("AT+CIPMUX=1");
    while (!esp.find("OK"));
    esp.println("AT+CIPSERVER=1,80");
    while (!esp.find("OK"));
    esp.println("AT+CIPSTO=7000");
    while (!esp.find("OK"));
    esp.println("AT+CIPSTART=0,\"UDP\",\"0.0.0.0\",80,80,0");
    delay(20);
    if (debugOpt)Serial.println("udpServer_done");
  }

}

void ConnectToMusic(String musIPp) {
  int xx = 0;
  esp.println("AT+CIPSTART=1,\"UDP\",\"" + musIPp + "\"," + "80");
  delay(1000);
  /*
  while (!esp.find("OK")) {
    xx += 1;
    delayMicroseconds(1);
    if (xx > 300) {
      if (debugOpt)Serial.println("No_Music");
      staLed(0, 0, 1, 500, 2);
      stuL = 51;
      netWorkyn = false;
      break;
    }
  }
  */
}

void playMusic() {
  //udpCnn();
  esp.println("AT+CIPSEND=1,5");
  while (!esp.find(">"));
  esp.print("play>");
}

void WWCDMusic() {
  esp.println("AT+CIPSEND=1,6");
  while (!esp.find(">"));
  esp.print("playW>");
}

void fuckedUpMusic() {
  //udpCnn();
  esp.println("AT+CIPSEND=1,6");
  while (!esp.find(">"));
  esp.print("play0>");
}

void hitMuic() {
  //udpCnn();
  esp.println("AT+CIPSEND=1,4");
  while (!esp.find(">"));
  esp.print("hit>");
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

void rstt(){
  if (debugOpt)Serial.println("stopped");
    digitalWrite(stuL, 0);
    staLed(1, 0, 1, 50, 3);

    if (debugOpt)Serial.println("restting.");

    for (int i = 0; i < 3; i++) {
      for (int j = 0; j < 3; j++) {
        digitalWrite(lp[i][j], 1);
        digitalWrite(rlp[i][j], 0);
        c[i][j] = false;
        if (debugOpt)Serial.print(".");
      }
    }
    for (int j = 0; j < 8; j++) {
      pL[j] = false;
    }
    played = false;
    score = 0;
    staLed(1, 0, 1, 50, 10);
    if (debugOpt)Serial.println("\nreset_done");
    digitalWrite(stuL, 1);
}

void bdLd(){
  digitalWrite(rPin, 0);
  digitalWrite(gPin, 0);
  digitalWrite(bPin, 0);
  delay(100);
  digitalWrite(rPin, 1);
  delay(100);
  digitalWrite(gPin, 1);
  delay(100);
  digitalWrite(bPin, 1);
  delay(200);
  digitalWrite(rPin, 0);
  digitalWrite(gPin, 0);
  digitalWrite(bPin, 0);
}
