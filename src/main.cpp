#include <Arduino.h>
#include <Constants.h>
#include <Data.h>
#include <WiFi.h>
#include <Co2.h>
#include <LCD.h>
#include <BME280.h>
#include <TVOC.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Scanner.h>
#include <Wire.h>

uint32_t previousDataSendMillis = 0U;
boolean status = true;

void update()
{
  const int luxReading = analogRead(LUX_PIN);
  const int TVOCReading = TVOCread();
  lux.add((float)luxReading);
  lux30.add((float)luxReading);
  BME280read();
  Co2read();
  lcdUpdateData();

  const uint32_t currentMillis = millis();
  if (currentMillis - previousDataSendMillis >= INTERVAL_DATA_SEND_MS)
  {
    previousDataSendMillis = currentMillis;
    sendDataDomoticz();
  }

  Serial.print(String("Temp ") + dispTemp + " Hum " + dispHum + " Press " + dispPres);
  Serial.println(String(" CO2 ") + co2Value + " TVOC " + TVOCReading + " Lux " + luxReading);
  ESP.wdtFeed();
}

void initDevice(const String deviceName, const uint8_t displayLine, boolean (*init)())
{
  lcdSetCursor(0U, displayLine);
  lcdPrint(deviceName + "... ");
  Serial.print(deviceName + "... ");
  if (init())
  {
    lcdPrint(F("OK"));
    Serial.println(F("OK"));
  }
  else
  {
    lcdPrint(F("ERROR"));
    Serial.println(F("ERROR"));
    status = false;
  }
}

void setup()
{

  Serial.begin(9600U);
  Serial.setTimeout(2000);
  Serial.setDebugOutput(true);
  digitalWrite(LED_BUILTIN, HIGH);
  while (!Serial)
    ;

  pinMode(LUX_PIN, INPUT);
  pinMode(BACKLIGHT_PIN, OUTPUT);

  Serial.print("Startup reason: ");
  Serial.println(ESP.getResetReason());
  lcdInit();

  initDevice(String("S8"), 0U, Co2init);
  initDevice(String("BME280"), 1U, BME280init);
  initDevice(String("CCS811"), 2U, TVOCinit);
  initDevice(String("WiFi"), 3U, WiFiconnect);

  if (status)
  {
    tvocValue.add(0.0F);
    ESP.wdtDisable();
    ESP.wdtEnable(60 * WDTO_1S);
    lcdClear();
  }
  else
  {
    Serial.println(F("Self-check failed"));
    while (true)
      delay(100);
  }
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  // scanner();
  update();
  delay(1000);
}
