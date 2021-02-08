#include <Arduino.h>
#include <Constants.h>
#include <Data.h>
#include <WiFi.h>
#include <Co2.h>
#include <LCD.h>
#include <BME280.h>
#include <TVOC.h>
#include <PM.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Scanner.h>
#include <Wire.h>
#include <Ticker.h>

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

  Serial.print(String("Temp ") + insideTemp + " Hum " + insideHum + " Press " + insidePres);
  Serial.print(String(" CO2 ") + co2Value + " TVOC " + TVOCReading + " PM10 " + PM10Reading + " PM25 " + PM25Reading);
  Serial.println(String(" Backlit ") + backlit + " Lux " + luxReading);
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

Ticker scannerTicker(scanner, 1000, 0, MILLIS);
Ticker updateTicker(update, 1000, 0, MILLIS);
Ticker sendDataTicker(sendDataDomoticz, INTERVAL_DATA_SEND_MS, 0, MILLIS);
Ticker PMTicker(PMread, 10000, 0, MILLIS);

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
  scannerTicker.start();
  updateTicker.start();
  sendDataTicker.start();
  PMTicker.start();
  initDevice(String("S8"), 0U, Co2init);
  initDevice(String("BME280"), 1U, BME280init);
  initDevice(String("CCS811"), 2U, TVOCinit);
  initDevice(String("PM"), 3U, PMinit);
  initDevice(String("WiFi"), 3U, WiFiconnect);
 

  if (status)
  {
    lcdClear();
  }
  else
  {
    Serial.println(F("Self-check failed"));
    //while (true)
    //  delay(100);
  }
  tvocValue.add(0.0F);
  ESP.wdtDisable();
  ESP.wdtEnable(60 * WDTO_1S);
  lcdClear();
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  scannerTicker.update();
  updateTicker.update();
  sendDataTicker.update();
  PMTicker.update();
}
