#include <Arduino.h>

// prototype
// void updateShiftRegister();
void shiftRegisterOn(void);
void shiftRegisterOff(void);
void shiftRegisterIn(uint8_t input);

// 21, 22, 23 // example
// 21, 27, 12 // esp32
//  8,   ,     // uno
//  1,   ,      // esp8266

// // for ESP32 Microcontrollers
// uint8_t latchPin = 21;
// uint8_t clockPin = 13;
// uint8_t dataPin = 12;

// for ESP8266 Microcontrollers
uint8_t latchPin = 5; //1
uint8_t clockPin = 4; // 2
uint8_t dataPin = 2;  // 3

uint8_t number[10] = {
    0b00111111, // "0"
    0b00000110, // "1"
    0b01011011, // "2"
    0b01001111, // "3"
    0b01100110, // "4"
    0b01101101, // "5"
    0b01111101, // "6"
    0b00000111, // "7"
    0b01111111, // "8"
    0b01101111  // "9"
};

void setup()
{
  pinMode(latchPin, OUTPUT);
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);

  Serial.begin(9600);
}

void loop()
{
  for (uint8_t i = 0; i < 10; i++)
  {
    shiftRegisterIn(number[i]);
    delay(1000);
  }
  shiftRegisterOff();
  delay(1000);
  shiftRegisterOn();
  delay(1000);
  shiftRegisterOff();
}

// void updateShiftRegister()
// {
//   digitalWrite(latchPin, LOW);
//   shiftOut(dataPin, clockPin, LSBFIRST, leds);
//   digitalWrite(latchPin, HIGH);
// }

void shiftRegisterIn(uint8_t input)
{
  digitalWrite(latchPin, LOW);
  shiftOut(dataPin, clockPin, MSBFIRST, 0x00);
  digitalWrite(latchPin, HIGH);
  for (uint8_t i = 0; i < 4; i++)
  {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, input);
    digitalWrite(latchPin, HIGH);
  }
}

void shiftRegisterOn(void)
{
  for (uint8_t i = 0; i < 5; i++)
  {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 0xff);
    digitalWrite(latchPin, HIGH);
  }
}
void shiftRegisterOff(void)
{
  for (uint8_t i = 0; i < 5; i++)
  {
    digitalWrite(latchPin, LOW);
    shiftOut(dataPin, clockPin, MSBFIRST, 0x0);
    digitalWrite(latchPin, HIGH);
  }
}