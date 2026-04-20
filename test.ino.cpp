#include <Arduino.h>
#line 1 "C:\\Users\\amanp\\Downloads\\test\\test.ino"
#define BLYNK_TEMPLATE_ID "TMPL3axb_KWcH"
#define BLYNK_TEMPLATE_NAME "Home Security and Real time Monitoring System"
#define BLYNK_AUTH_TOKEN "YbbonXdPGFejMUOeZc0mJM-emONe6Egr"

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <BlynkSimpleEsp8266.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1 // Reset pin is not used
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

char auth[] = BLYNK_AUTH_TOKEN;

char ssid[] = "Redmi";
char pass[] = "12345678";

// Sensor pins
#define PIR_PIN 12       // GPIO12 (D6)
#define VIBRATION_PIN 13 // GPIO13 (D7)
#define TOUCH_PIN 14     // GPIO14 (D5)
#define GAS_PIN A0       // Analog pin A0

BlynkTimer timer;

void sendSensor()
{
  // Read sensor states
  bool pirStatus = digitalRead(PIR_PIN);
  bool vibrationStatus = digitalRead(VIBRATION_PIN);
  bool touchStatus = digitalRead(TOUCH_PIN);
  int gasValue = analogRead(GAS_PIN); // Analog value from gas sensor

  // Display sensor statuses on OLED
  display.clearDisplay();
  display.setCursor(0, 0);
  display.print("Sensor Status:");
  display.setCursor(0, 10);
  display.print("PIR: ");
  display.print(pirStatus ? "Detected" : "Not detected");
  display.setCursor(0, 20);
  display.print("Vibration: ");
  display.print(vibrationStatus ? "Detected" : "Not detected");
  display.setCursor(0, 30);
  display.print("Touch: ");
  display.print(touchStatus ? "Touched" : "Not touched");
  display.setCursor(0, 40);
  display.print("Gas: ");
  display.print(gasValue);
  display.display();

  Blynk.virtualWrite(V0, pirStatus);
  Blynk.virtualWrite(V1, vibrationStatus);
  Blynk.virtualWrite(V2, touchStatus);
  Blynk.virtualWrite(V3, gasValue);
}

void setup()
{
  Serial.begin(115200); // Initialize serial communication for debugging
  Blynk.begin(auth, ssid, pass);

  // Initialize OLED
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Infinite loop
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.print("Sensor Status:");
  display.display();

  // Initialize sensor pins
  pinMode(PIR_PIN, INPUT);
  pinMode(VIBRATION_PIN, INPUT);
  pinMode(TOUCH_PIN, INPUT);

  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}

