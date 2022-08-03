#include <Arduino.h>
#include "MHZ19.h"
#include <SoftwareSerial.h>
#include <Data.h>

MHZ19 sensor;
SoftwareSerial co2Serial(D6, D7);

boolean coinit()
{
  co2Serial.begin(9600U);    // Uno example: Begin Stream with MHZ19 baudrate
  sensor.begin(co2Serial);
  sensor.autoCalibration(true, 24U * 7U);

  char myVersion[4];
  sensor.getVersion(myVersion);

  Serial.print("\nFirmware Version: ");
  for (byte i = 0; i < 4; i++)
  {
    Serial.print(myVersion[i]);
    if (i == 1)
      Serial.print(".");
  }
  Serial.println("");

  Serial.print("Range: ");
  Serial.println(sensor.getRange());
  Serial.print("Background CO2: ");
  Serial.println(sensor.getBackgroundCO2());
  Serial.print("Temperature Cal: ");
  Serial.println(sensor.getTempAdjustment());
  Serial.print("ABC Status: ");
  sensor.getABC() ? Serial.println("ON") : Serial.println("OFF");
  return sensor.getCO2() > 300 && sensor.getCO2() < 5000;
}

uint16_t co2read()
{
  const uint16_t currentCo2Value = sensor.getCO2();
  if (currentCo2Value <= 0 || sensor.getCO2() > 5000){
    co2Fails++;
  } else {
    co2Value = currentCo2Value;
  }
  if (co2Fails > 300) {
    Serial.print("Co2 failure");
    delay(100U);
    ESP.reset();
  }
  return currentCo2Value;
}

