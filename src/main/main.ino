#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32, verstehen nicht wieso 0x3C klappt, aber 0x3D nicht
#define OLED_RESET     4 // Reset pin # (or -1 if sharing Arduino reset pin) verstehe nicht so ganz was das macht
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void setup() {
  Serial.begin(9600);
  Serial.println("I'm here");
  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(2);
  display.setTextWrap(true);
}

void loop() {

  int total_seconds = millis()/1000;

  // timer für die aktuelle Session
  display.clearDisplay();
  display.setCursor(18,0);
  display.print("Session: ");
  display.setCursor(18,16);
  int timevalue[3] = {0 ,0, 0};
  determine_time(total_seconds, timevalue);
  display_time(timevalue);
  
  // total 8-h timer
  display.setCursor(18,32);
  display.print("Total: ");
  display.setCursor(18,48);
  int timevalue2[3] = {0 ,0, 0};
  determine_time(28800 - total_seconds, timevalue2);
  display_time(timevalue2);
  display.display();
}

// helper functions
void determine_time(int total_seconds, int *a) {
  int time_now = total_seconds;
  a[2] = time_now % 60;
  a[1] = (time_now / 60) % 60;
  a[0] = (time_now / 60 / 60) % 60;

}

void display_time(int a[3]) {
  Serial.println(a[0]);
  Serial.println(a[1]);
  Serial.println(a[2]);
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
