#include <Arduino.h>
#include <Constants.h>
#include <Data.h>
#include <LCD.h>
#include <Adafruit_ST7735.h>
#include <WiFiFunctions.h>
#include <BME280.h>
#include <TVOC.h>
#include <CO2.h>
#include <Scanner.h>
#include <Ntp.h>
#include <Wire.h>
#include <Ticker.h>

boolean status = true;

void update()
{
  ESP.wdtFeed();
  ESP.wdtDisable();
  const int luxReading = analogRead(LUX_PIN);
  // const int TVOCReading = TVOCread();
  const int TVOCReading = 0;
  lux.add((float)luxReading * 0.64453125);
  lux30.add((float)luxReading * 0.64453125);
  co2read();
  BME280read();

  Serial.print(timeStr + String(" Temp ") + insideTemp + String(" OutTemp ") + outsideTemp + " Hum " + insideHum + " Press " + insidePres);
  Serial.print(String(" CO2 ") + co2Value + " TVOC " + TVOCReading);
  Serial.println(String(" Backlit ") + backlit + " Lux " + luxReading);

  textToLcd(0, 0, String(insideTemp, 2) + "C " + String(insideHum, 2) + "%");
  textToLcd(0, 1, "Outside: " + String(outsideTemp, 1) + "C ");
  textToLcd(0, 2, String(insidePres) + "hPa");
  textToLcd(0, 3, "CO " + String(co2Value) + "ppm");
}

Ticker scannerTicker(scanner, INTERVAL_UPDATE_MS, 0, MILLIS);
Ticker updateTicker(update, INTERVAL_UPDATE_MS, 0, MILLIS);
Ticker ntpTiker(updateTime, INTERVAL_UPDATE_MS, 0, MILLIS);
Ticker sendDataTicker(sendDataDomoticz, INTERVAL_DATA_SEND_MS, 0, MILLIS);
Ticker updateTempTicker(updateTemp, INTERVAL_DATA_GET_MS, 0, MILLIS);

void initDevice(const String deviceName, const uint8_t displayLine, const boolean watchdogDisable, boolean (*init)())
{
  ESP.wdtFeed();
  if (watchdogDisable) {
    ESP.wdtEnable(0U);
  }
  Serial.print(deviceName + "... ");
  if (deviceName != "LCD")
  {
    textToLcd(0, displayLine, deviceName, false);
  }
  if (init())
  {
    if (deviceName == "LCD") {
      textToLcd(0, displayLine, "LCD");
    }
    textToLcd(10, displayLine, " OK", false);
    Serial.println(F("OK"));
  }
  else
  {
    textToLcd(10, displayLine, " ERROR", false);
    Serial.println(F("ERROR"));
    status = false;
  }
  ESP.wdtFeed();
}

void initDevice(const String deviceName, const uint8_t displayLine, boolean (*init)()) {
  initDevice(deviceName, displayLine, false, init);
}

void setup()
{
  Serial.begin(76800U);
  digitalWrite(LED_BUILTIN, HIGH);
  while (!Serial)
    ;

  Serial.printf("\n\nSdk version: %s\n", ESP.getSdkVersion());
  Serial.printf("Core Version: %s\n", ESP.getCoreVersion().c_str());
  Serial.printf("Boot Version: %u\n", ESP.getBootVersion());
  Serial.printf("Boot Mode: %u\n", ESP.getBootMode());
  Serial.printf("CPU Frequency: %u MHz\n", ESP.getCpuFreqMHz());
  Serial.printf("Reset reason: %s\n", ESP.getResetReason().c_str());

  pinMode(LUX_PIN, INPUT);
  ESP.wdtDisable();
  Wire.begin(SDA, SCL);

  scannerTicker.start();
  updateTicker.start();
  ntpTiker.start();
  sendDataTicker.start();
  updateTempTicker.start();

  initDevice(String("LCD"), 0U, initLcd);
  initDevice(String("MH-Z19B"), 1U, coinit);
  initDevice(String("BME280"), 2U, BME280init);
  // initDevice(String("CCS811"), 3U, TVOCinit);
  initDevice(String("WiFi"), 3U, WiFiconnect);
  // initDevice(String("NTP"), 5U, ntpInit);

  updateTemp();
  clearLcd();
  digitalWrite(LED_BUILTIN, LOW);
  Serial.println("setup end");
}

void loop()
{
  // scannerTicker.update();
  updateTicker.update();
  sendDataTicker.update();
  updateTempTicker.update();
  // ntpTiker.update();
}
