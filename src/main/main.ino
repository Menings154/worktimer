#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32, verstehen nicht wieso 0x3C klappt, aber 0x3D nicht
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin) verstehe nicht so ganz was das macht
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

int buttonApin = 14;
int buttonBpin = 15;

// counter for the different functionalities
long old_millis_session = 0;
long old_millis_total = 0;

int timevalue[3] = {0 ,0, 0}; // array for session
int timevalue2[3] = {0 ,0, 0}; // array for total

long total_millis;
int total_seconds_session;
int total_seconds_total;

long temp = 0;
int phase = 0;

void setup() {
  display.display();
  delay(2000); // Pause for 2 seconds
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setTextWrap(true);

  // here comes the code for the button
  pinMode(buttonApin, INPUT_PULLUP);
  pinMode(buttonBpin, INPUT_PULLUP);
}

void loop() {
  total_millis = millis();
  total_seconds_session = (total_millis - old_millis_session) / 1000;
  total_seconds_total = (total_millis - old_millis_total) / 1000;

  switch (phase){
    case 0:
      normal_mode(total_seconds_session, total_seconds_total);
      break;
      
    case 1:
      button_A_pressed_mode(&old_millis_session, &phase);
      break;
      
    case 2:
      button_B_pressed_mode(&old_millis_session, &old_millis_total, &phase);
      break;
  }
}
void normal_mode(int total_seconds_session, int total_seconds_total){
  // timer für die aktuelle Session
  display.clearDisplay();
  display.setCursor(18,0);
  display.print("Session: ");
  display.setCursor(18,16);
  determine_time(total_seconds_session, timevalue);
  display_time(timevalue);
  
  // total 8-h timer
  display.setCursor(18,32);
  display.print("Total: ");
  display.setCursor(18,48);
  determine_time(28800 - total_seconds_total, timevalue2);
  display_time(timevalue2);
  display.display();

  if (digitalRead(buttonApin) == LOW) {
    temp = millis();
    delay(1000);
    phase = 1;
  }

  if (digitalRead(buttonBpin) == LOW) {
    delay(1000);
    phase = 2;
  }
}

void button_A_pressed_mode(long *old_millis_session, int *phase){
  if (digitalRead(buttonApin) == LOW) {
    delay(2000);
    *old_millis_session = millis();
    *phase = 0;
    old_millis_total += (millis()-temp);
  }
}

void button_B_pressed_mode(long *old_millis_session, long *old_millis_total, int *phase){
  if (digitalRead(buttonBpin) == LOW) {
    delay(2000);
    *old_millis_session = millis();
    *old_millis_total = millis();
    *phase = 0;
  }
}

// helper functions
void determine_time(int total_seconds, int *a) {
  int time_now = total_seconds;
  a[2] = time_now % 60;
  a[1] = (time_now / 60) % 60;
  a[0] = (time_now / 60 / 60) % 60;
}

void display_time(int a[3]) {
  if (a[0] < 10){
    display.print(0);
  }
  display.print(a[0]);
  display.print(":");
  if (a[1] < 10){
    display.print(0);
  }
  display.print(a[1]);
  display.print(":");
  if (a[2] < 10){
    display.print(0);
  }
  display.print(a[2]);
}
