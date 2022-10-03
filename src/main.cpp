#include <Arduino.h>
#include <LCD_4_5_Digits.h>
#include <esp8266_gpio_mapping.h>
// #include <DS3231.h>
// #include <Wire.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include "../login.h"

// 0x57
// 0x68

// prototype
void setup_wifi(void);
void callback(String topic, byte *message, uint32_t length);
void reconnect(void);
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

const char *ssid = SSID;
const char *password = SSID_PASSWORD;
const char *mqtt_server = MQTT_SERVER;

WiFiClient phaseThreeClient;
PubSubClient client(phaseThreeClient);

void setup()
{
  // Serial.begin(9600);
  Serial.println("let's start");
  lcd.init();
  lcd.set_value(10801);
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop()
{
  if (!client.connected())
  {
    reconnect();
  }
  if (!client.loop())
    client.connect("phaseThreeClient");
  // uint16_t light_sensor_value = analogRead(A0);
  // lcd.set_value(light_sensor_value);
  delay(500);
}

void setup_wifi(void)
{
  delay(10);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    // Serial.print(".");
    delay(500);
  }
}

void callback(String topic, byte *message, uint32_t length)
{
  String messageTemp;

  for (uint32_t i = 0; i < length; i++)
  {
    messageTemp += (char)message[i];
  }

  if (topic == "energy_meter/data_6")
  {
    float powerMeterPhaseThreeFloat = messageTemp.toFloat();
    uint16_t powerMeterPhaseThreeInt = uint16_t(powerMeterPhaseThreeFloat * 1000);
    lcd.set_value(powerMeterPhaseThreeInt);
  }
}

void reconnect(void)
{
  while (!client.connected())
  {
    if (client.connect("phaseThreeClient"))
    {
      client.subscribe("energy_meter/data_6");
    }
    else
    {
      // Serial.println("reconnect, moment please");
      delay(5000);
    }
  }
}