#include <Arduino.h>
#include <LCD_4_5_Digits.h>
#include <esp8266_gpio_mapping.h>
#include <DS3231.h>
#include <Wire.h>

// 0x57
// 0x68

// prototype
// void updateShiftRegister();

// 21, 22, 23 // example
// 21, 27, 12 // esp32
//  8,   ,     // uno
//  1,   ,      // esp8266

// // for ESP32 Microcontrollers
// uint8_t latchPin = 21;
// uint8_t clockPin = 13;
// uint8_t dataPin = 12;

// for ESP8266 Microcontrollers
// uint8_t latchPin = 5; // 1
// uint8_t clockPin = 4; // 2
// uint8_t dataPin = 2;  // 3

uint8_t latchPin = D8; // 1
uint8_t clockPin = D6; // 2
uint8_t dataPin = D7;  // 3

LCD_4_5_Digits lcd(latchPin, clockPin, dataPin);
DS3231 real_time_clock;

bool h12Flag;
bool pmFlag;

void setup()
{
  Serial.begin(9600);
  lcd.init();
  Wire.begin();

  delay(5000);
  Serial.print(real_time_clock.getHour(h12Flag, pmFlag), DEC); // 24-hr
  Serial.print(":");
  Serial.print(real_time_clock.getMinute(), DEC);
  Serial.print(":");
  Serial.println(real_time_clock.getSecond(), DEC);

  delay(10000);

  Serial.print(real_time_clock.getHour(h12Flag, pmFlag), DEC); // 24-hr
  Serial.print(":");
  Serial.print(real_time_clock.getMinute(), DEC);
  Serial.print(":");
  Serial.println(real_time_clock.getSecond(), DEC);

  delay(10000);
}

int16_t max_value = 20010;
int16_t min_value = -20010;

void loop()
{
  for (int16_t i = max_value; i >= min_value; i--)
  {
    lcd.set_value(i);
    Serial.print("Counter - = ");
    Serial.println(i);
    delay(100);
  }
  for (int16_t i = min_value; i <= max_value; i++)
  {
    lcd.set_value(i);
    Serial.print("Counter + = ");
    Serial.println(i);
    delay(100);
  }
}