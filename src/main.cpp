#include <Arduino.h>
#include <LCD_4_5_Digits.h>

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>

#include <../secrets.h>

const char *ssid = SSID;
const char *password = PASSWORD;
const char *mqtt_server = MQTT_SERVER_IP;

// prototype
void callback(char *topic, byte *message, unsigned int length);
void setup_wifi();
void reconnect();

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;

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

  setup_wifi();
  client.setServer(mqtt_server, MQTT_PORT_NUMBER);
  client.setCallback(callback);
}

void loop()
{
  if (!client.connected())
    reconnect();

  client.loop();
}

//---------------
void callback(char *topic, byte *message, unsigned int length)
{
  Serial.print("Message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". Message: ");
  String messageTemp;

  for (int i = 0; i < length; i++)
  {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "stal/cv")
  {
    Serial.print("Changing output to ");
    Serial.println(messageTemp);
    lcd.all_on();
  }
}

void setup_wifi()
{
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266_LCD"))
    {
      Serial.println("connected");
      // Subscribe
      client.subscribe("stal/cv");
    }
    else
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//-------------------------------------------------------------
// int16_t max_value = 20020;
// int16_t min_value = -20020;

// void loop()
// {
//   for (uint16_t i = 0; i <= 3600; i++)
//   {
//     lcd.set_timer(i);
//     delay(500);
//   }
//   for (uint16_t i = 0; i >= 3600; i--)
//   {
//     lcd.set_timer(i);
//     delay(500);
//   }

//   for (int16_t i = min_value; i <= max_value; i++)
//   {
//     lcd.set_value(i);
//     Serial.print("Counter + = ");
//     Serial.println(i);
//     delay(100);
//   }
//   for (int16_t i = max_value; i >= min_value; i--)
//   {
//     lcd.set_value(i);
//     Serial.print("Counter - = ");
//     Serial.println(i);
//     delay(100);
//   }
// }
