#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>
#include "Ap.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char sor []= "Sorry, wala tayong" 
            " \ngala at busy langg"
            " \n\nBawi ako sa susunod"
            " \nEnjoy your day =)";
int s1 = sizeof(sor)/sizeof(sor[0]);

void setup() {
  display.begin();

}

void loop() {
  display.clearDisplay();
  display.setCursor(28, 10);
  display.setTextColor(SH110X_WHITE);
  display.setTextSize(2);
  display.println("HAPPY \n  BIRTHDAY");
  display.setCursor(28, 50);
  display.println("APRIL!!");
  display.display();
  delay(3000);

  display.clearDisplay();
  display.setCursor(0, 0);
  display.setTextSize(1);
  for (int i = 0; i < s1; i++){
    display.print(sor[i]);
    display.display();
    delay(80);
  }
  display.clearDisplay();
  //display.println("\n");
  delay(500);
  display.drawBitmap(0, 0, epd_bitmap_HBD_A, 128, 64, SH110X_WHITE);
  display.display();
  delay(4500);
  display.clearDisplay();
  display.drawBitmap(32,0, epd_bitmap_balloon_and_bear, 64, 64, SH110X_WHITE);
  display.display();
  delay(3000);
}
