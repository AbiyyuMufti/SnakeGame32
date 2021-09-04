#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include "SnakeElement.h"

#define PIN_LEFT 2
#define PIN_RIGHT 4
#define PIN_DOWN 0
#define PIN_UP 17
#define PIN_STOP 5

Adafruit_SSD1306 display(128, 64, &Wire, -1);
SnakeElement mSnake(128 / 2 + 8 * 2, 64 / 2, 2);
SnakeDirection Direction = left;
Circle Food(0, 0, 2);


void createSnake(){
  mSnake.setPosition(128 / 2 + 8 * 2, 64 / 2);
  for (int i = 0; i < 4; i++) {
      mSnake.update(left);
      mSnake.elongate();
  }
  mSnake.draw();
}

void checkDirection()
{
  int right_stat = digitalRead(PIN_RIGHT);
  int left_stat = digitalRead(PIN_LEFT);
  int up_stat = digitalRead(PIN_UP);
  int down_stat = digitalRead(PIN_DOWN);
  int stop_stat = digitalRead(PIN_STOP);
  if (right_stat && Direction != left) {
    Serial.println("Right");
    Direction = right;
  }
  else if (left_stat && Direction != right) {
    Serial.println("Left");
      Direction = left;
  }
  else if (up_stat && Direction != down) {
    Serial.println("UP");
      Direction = up;
  }
  else if (down_stat && Direction != up) {
    Serial.println("Down");
      Direction = down;
  }
  else if (stop_stat)
  {
    Direction = stop;
  }
}

void setup() {
  Serial.begin(115200);
  pinMode(PIN_UP, INPUT_PULLDOWN);
  pinMode(PIN_LEFT, INPUT_PULLDOWN);
  pinMode(PIN_RIGHT, INPUT_PULLDOWN);
  pinMode(PIN_DOWN, INPUT_PULLDOWN);
  pinMode(PIN_STOP, INPUT_PULLDOWN);
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
      digitalWrite(13, HIGH);
      for (;;); // Don't proceed, loop forever
  }
  else {
      digitalWrite(13, LOW);
  }

  display.display();
  delay(1000); // Pause for 1 seconds
  display.clearDisplay();
  createSnake();
  Food.setX(random(124));
  Food.setY(random(60));
  display.display();
  Direction = stop;
}


void snake_routine(){
  checkDirection();
  mSnake.update(Direction);
  mSnake.draw();

  if(mSnake.check_collision()){
    mSnake.detachTail();
    createSnake();
    Direction = stop;
  }
}

void food_routine(){
  if (mSnake.eating(Food))
  {
    randomSeed(analogRead(35));
    int x = random(124); 
    randomSeed(analogRead(35));
    int y = random(60);
    Food.setX(x);
    Food.setY(y);
    mSnake.elongate();
  }
  else
  {
    Food.draw();
  }
}

void loop() {
  static long last = millis();
  if (millis() - last >= 100)
  {
    display.clearDisplay();
    snake_routine();
    food_routine();
    
    display.display();
    last = millis();
  }
}

