#include <Arduino.h>
#include <LCD_4_5_Digits.h>

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
uint8_t latchPin = 5; // 1
uint8_t clockPin = 4; // 2
uint8_t dataPin = 2;  // 3

LCD_4_5_Digits lcd(latchPin, clockPin, dataPin);

void setup()
{
  Serial.begin(9600);
  lcd.init();
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