#include <avr/wdt.h>
#include <Arduino.h>
#include <Constants.h>
#include <Data.h>
#include <Co2.h>
#include <LCD.h>
#include <Scanner.h>
#include <Wire.h>
#include <Ticker.h>

boolean status = true;

void update()
{
  Co2read();
  lcdUpdateData();

  Serial.print(String(" CO2 ") + co2Value);
  wdt_reset();
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

Ticker scannerTicker(scanner, INTERVAL_UPDATE_MS, 0, MILLIS);
Ticker updateTicker(update, INTERVAL_UPDATE_MS, 0, MILLIS);

void setup()
{

  Serial.begin(76800U);
  digitalWrite(LED_BUILTIN, HIGH);
  while (!Serial)
    ;

  scannerTicker.start();
  updateTicker.start();

  initDevice(String("SSD1106"), 0U, lcdInit);
  initDevice(String("S8"), 0U, Co2init);

  if (status)
  {
    lcdClear();
  }
  else
  {
    Serial.println(F("Self-check failed"));
    while (true)
      delay(100);
  }
  wdt_enable(WDTO_4S);
  lcdClear();
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  scannerTicker.update();
  // updateTicker.update();
}
