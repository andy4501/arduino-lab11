#include <LWiFi.h>
#include <WiFiClient.h>
#include "MCS.h"
#include "DHT.h"
#define DHTPIN 2

// Assign AP ssid / password here
#define _SSID "AA202_2G"
#define _KEY  "iteaa202"
#define DHTTYPE DHT11
// Assign device id / key of your test device
MCSDevice mcs("DypR0ItS", "er0MUjDl8Jbqw3gY");

// Assign channel id
// The test device should have 2 channel
// the first channel should be "Controller" - "On/Off"
// the secord channel should be "Display" - "On/Off"

MCSDisplayFloat  humidity("H");
MCSDisplayFloat  temperature("T");

#define LED_PIN 7
DHT dht(DHTPIN, DHTTYPE);
void setup() {
  // setup Serial output at 9600
  Serial.begin(9600);
  Serial.println(F("DHTxx test!"));

  dht.begin();
  // setup LED/Button pin
  pinMode(LED_PIN, OUTPUT);

  // setup Wifi connection
  while (WL_CONNECTED != WiFi.status())
  {
    Serial.print("WiFi.begin(");
    Serial.print(_SSID);
    Serial.print(",");
    Serial.print(_KEY);
    Serial.println(")...");
    WiFi.begin(_SSID, _KEY);
  }
  Serial.println("WiFi connected !!");

  // setup MCS connection
 
  mcs.addChannel(humidity);
  mcs.addChannel(temperature);

  while (!mcs.connected())
  {
    Serial.println("MCS.connect()...");
    mcs.connect();
  }
  Serial.println("MCS connected !!");

  // read LED value from MCS server

}

void loop() {
  // call process() to allow background processing, add timeout to avoid high cpu usage
  Serial.print("process(");
  Serial.print(millis());
  Serial.println(")");
  mcs.process(100);

  // updated flag will be cleared in process(), user must check it after process() call.
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  

 if (!humidity.set(h))
    {
      Serial.print("Failed to update humidity");
      Serial.println(humidity.value());
    }
 if (!temperature.set(t))
    {
      Serial.print("Failed to update temperature");
      Serial.println(temperature.value());
    }
 delay(2000);
  // check if need to re-connect
  while (!mcs.connected())
  {
    Serial.println("re-connect to MCS...");
    mcs.connect();
    if (mcs.connected())
      Serial.println("MCS connected !!");
  }
}
