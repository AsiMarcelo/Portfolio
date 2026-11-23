#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "Hero2.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Input pins
int pin2 = 2;
int pin3 = 3;


void setup() {
  display.begin();
  Serial.begin(9600);
  pinMode(pin2, INPUT);
  pinMode(pin3, INPUT);
}


void loop() {
  if (digitalRead(pin2) == HIGH and digitalRead(pin3) == LOW){
    display.clearDisplay();
    display.drawBitmap(32, 0, epd_bitmap_Superman, 60, 60, SH110X_WHITE);
    display.display();
    Serial.println(pin2);
  }
  else if (digitalRead(pin2) == LOW and digitalRead(pin3) == HIGH){
    display.clearDisplay();
    display.drawBitmap(32, 0, epd_bitmap_Batman, 60, 60, SH110X_WHITE);
    display.display();
    Serial.println(pin3);
  }
  else{
  display.clearDisplay();
  display.drawBitmap(32, 0, epd_bitmap_World_s_Finest, 60, 60, SH110X_WHITE);
  display.display();
  }
  
}
