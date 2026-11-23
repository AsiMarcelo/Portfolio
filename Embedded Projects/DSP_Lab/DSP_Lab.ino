#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SH110X.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1
Adafruit_SH1106G display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//Defining the pins
const int buttPin = 2;
float speed = 2.00; 
const float SPEED_START = 2.0;
const float SPEED_STEP  = 0.13;      // added on every frame
const float SPEED_MAX   = 3.5;
const int joy_x = A0;
const int joy_y = A1;

//Defining variables
bool game = false;
float offset, maxAngle, angle; //para sa bounce detection
//const int pad = 5; //padding from the OLED edges measured in pixels 
float x = 64;
float y = 32;
float vx = 0.9;
float vy = 1.8; 
int score = 0;

//Ball
const int r = 2;                    // ball radius
const float X_MIN = r;
const float X_MAX = 127 - r;
const float Y_MIN = r;

//Classes: entity-character, enemy, bullet for a block-style table tennis game
class entity_character{
  public:
    const int y_position = 59;
    int health = 3;
    int position = 60; 
    int width = 19;
    int height = 3;
};

void showMenu();
void deathScreen();
void centerText(const char *text, int yPosition);
void displayScore();
entity_character player;

void setup() {
  // put your setup code here, to run once:
  display.begin();
  Serial.begin(9600);
  pinMode(buttPin, INPUT_PULLUP);
  pinMode(joy_y, INPUT);
  pinMode(joy_x, INPUT);
  delay(3000);
}

void loop() {
  // put your main code here, to run repeatedly:
  bool gameStart = digitalRead(buttPin);
  if (gameStart == 0) {game = true;}
  if (game != false){
    display.fillCircle((int)x, (int)y, r , SH110X_WHITE);
    display.fillRect(player.position, player.y_position, player.width, player.height, SH110X_WHITE);
    displayScore();
    display.display();
    float oldSpeed = speed;
    speed = min(speed + SPEED_STEP, SPEED_MAX);
    x = x + vx; y = y + vy;
    float k = speed / oldSpeed;      // tiny factor, about 1.0066 at the start
    vx *= k;
    vy *= k;
    if (x > X_MAX) { x = 2 * X_MAX - x; vx = -vx; }
    if (x < X_MIN) { x = 2 * X_MIN - x; vx = -vx; }
    if (y < Y_MIN) { y = 2 * Y_MIN - y; vy = -vy; }
    if (vy > 0 && y + r >= player.y_position && y - r <= player.y_position + player.height
        && x >= player.position - r
        && x <= player.position + player.width + r) {
      y = player.y_position - r;
      score++;
      collisionCheck(x);
    }
    if (y > 63 +r) { player.health--; x=64; y=32; delay(300); vx=.9; vy=1.8; speed = SPEED_START; player.position = 60;}

    if(player.health < 1){
      deathScreen();
      game = false;
    }
    int x_val = analogRead(joy_x);
    Serial.println(x_val);
    if (x_val > 520){
      player.position += 3;
    }
    if (x_val < 490){
      player.position -=3;
    }
    player.position = constrain(player.position, 0, 128 - player.width);
    delay(30);
    display.clearDisplay();
  }
  else{
    showMenu();
  }
}

void showMenu(){
  int start = digitalRead(buttPin);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SH110X_WHITE);
  centerText("Want to", 6);
  centerText("play?",24);
  display.setCursor(0, 28);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  centerText("Click the joystick to", 48);
  centerText("play", 55);
  display.display();
}

void deathScreen(){
  display.clearDisplay();
  display.setCursor(26, 28);
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.println("You have died");
  display.setCursor(40, 50);
  display.setTextColor(SH110X_WHITE);
  display.print("Score: ");
  display.print(score);
  display.display();
  delay(3000);
  player.health = 3; player.position = 22; score = 0;
}

void collisionCheck(float ballX){
  offset = (ballX - (player.position + player.width / 2.0)) / (player.width / 2.0);
  offset = constrain(offset, -1.0, 1.0);
  maxAngle = 60 * PI / 180;
  angle = offset * maxAngle;
  vx = (speed * sin(angle));
  vy = (-speed * cos(angle));
}

void displayScore(){
  display.setTextSize(1);
  display.setTextColor(SH110X_WHITE);
  display.setCursor(0, 0);
  display.print("Score: ");
  display.print(score);
}

void centerText(const char *text, int yPosition) {
  int16_t x1, y1;
  uint16_t w, h;

  // Calculate pixel bounds of the string
  display.getTextBounds(text, 0, 0, &x1, &y1, &w, &h);

  // Center X = (Screen Width - Text Width) / 2
  int xPosition = (display.width() - w) / 2;

  display.setCursor(xPosition, yPosition);
  display.print(text);
}