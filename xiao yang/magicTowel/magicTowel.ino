#include <FastLED.h>
#include <SoftwareSerial.h>
#include <DFRobotDFPlayerMini.h>
#include <Pixy2.h>

#define NUM_LEDS 185
#define DATA_PIN 7

SoftwareSerial dfpS(10, 11);
DFRobotDFPlayerMini dfp;

int x = 0;

CRGB leds[NUM_LEDS];
Pixy2 pixy;

void setup() {
  Serial.begin(9600);
  LEDS.addLeds<WS2812,DATA_PIN,RGB>(leds,NUM_LEDS);
  LEDS.setBrightness(50);
  pixy.init();
  
  dfpS.begin(9600);
  dfp.begin(dfpS);
  dfp.volume(30);
  dfp.play(3);
  delay(900);
}

void loop() {
  
  pixy.ccc.getBlocks();
  Serial.print(pixy.ccc.blocks[0].m_x);
  Serial.print(" ");
  Serial.println(pixy.ccc.blocks[0].m_y);
  
  if(pixy.ccc.numBlocks){
    if((120 >= pixy.ccc.blocks[0].m_x) or (pixy.ccc.blocks[0].m_x >= 200) or 
      (120 >= pixy.ccc.blocks[0].m_y) or (pixy.ccc.blocks[0].m_y >= 200)){
        while(x <= 3){
          Serial.println('1');
          ledStart();
          dfp.play(2);
          x += 1;
        }
        x = 0;
      }
      
    else{
      for(int i = (NUM_LEDS)-1; i >= 0; i--){
        Serial.println('0');
        leds[i] = CHSV(64, 255, 255);
        FastLED.show();
       }
      
    }
  }
  else{
    Serial.println('2');
    for(int i = (NUM_LEDS)-1; i >= 0; i--){
      leds[i] = CHSV(105, 255, 255);
      FastLED.show();
    }
  }
  
}

void fadeall() { for(int i = 0; i < NUM_LEDS; i++) { leds[i].nscale8(250); } }

void ledStart() {
  Serial.println('3');
  static uint8_t hue = 0;
  for(int i = 0; i < NUM_LEDS; i++) {
    leds[i] = CHSV(hue++, 255, 255);
    FastLED.show(); 
    fadeall();
    delay(1);
  }
}

