#include <Arduino.h>
#include <Constants.h>
#include <Data.h>
#include <WiFiConnect.h>
#include <BME280.h>
#include <TVOC.h>
#include <Scanner.h>
#include <Ntp.h>
#include <Wire.h>
#include <Ticker.h>

boolean status = true;

void update()
{
  const int luxReading = analogRead(LUX_PIN);
  const int TVOCReading = TVOCread();
  lux.add((float)luxReading * 0.64453125);
  lux30.add((float)luxReading * 0.64453125);
  BME280read();

  Serial.print(timeStr + String(" Temp ") + insideTemp + String(" OutTemp ") + outsideTemp + " Hum " + insideHum + " Press " + insidePres);
  Serial.print(String(" CO2 ") + co2Value + " TVOC " + TVOCReading);
  Serial.println(String(" Backlit ") + backlit + " Lux " + luxReading);
}

Ticker scannerTicker(scanner, INTERVAL_UPDATE_MS, 0, MILLIS);
Ticker updateTicker(update, INTERVAL_UPDATE_MS, 0, MILLIS);
Ticker ntpTiker(updateTime, INTERVAL_UPDATE_MS, 0, MILLIS);
Ticker sendDataTicker(sendDataDomoticz, INTERVAL_DATA_SEND_MS, 0, MILLIS);
Ticker updateTempTicker(updateTemp, INTERVAL_DATA_GET_MS, 0, MILLIS);

void initDevice(const String deviceName, const uint8_t displayLine, boolean (*init)())
{
  Serial.print(deviceName + "... ");
  if (init())
  {
    //lcdPrint(F("OK"));
    Serial.println(F("OK"));
  }
  else
  {
    //lcdPrint(F("ERROR"));
    Serial.println(F("ERROR"));
    status = false;
  }
}

void setup()
{
  Serial.begin(76800U);
  Serial.print("setup");
  digitalWrite(LED_BUILTIN, HIGH);
  while (!Serial)
    ;

  pinMode(LUX_PIN, INPUT);
  Wire.begin(21U, 22U);

  scannerTicker.start();
  updateTicker.start();
  ntpTiker.start();
  sendDataTicker.start();
  updateTempTicker.start();

  // initDevice(String("S8"), 0U, Co2init);
  initDevice(String("BME280"), 1U, BME280init);
  initDevice(String("CCS811"), 2U, TVOCinit);
  initDevice(String("WiFi"), 3U, WiFiconnect);
  initDevice(String("WiFi OK, NTP"), 3U, ntpInit);

  tvocValue.add(0.0F);
  updateTemp();
  digitalWrite(LED_BUILTIN, LOW);
  Serial.print(" end");
}

void loop()
{
  scannerTicker.update();
  updateTicker.update();
  sendDataTicker.update();
  updateTempTicker.update();
  ntpTiker.update();
}
