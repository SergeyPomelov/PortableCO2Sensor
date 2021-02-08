#include <Arduino.h>
#include <Constants.h>
#include <Data.h>
#include <Adafruit_BME280.h>

Adafruit_BME280 bme;

boolean BME280init()
{
  boolean result = bme.begin(BME280_ADDRESS_ALTERNATE, &Wire);
  bme.setSampling(Adafruit_BME280::MODE_FORCED,
                  Adafruit_BME280::SAMPLING_X1, // temperature
                  Adafruit_BME280::SAMPLING_X1, // pressure
                  Adafruit_BME280::SAMPLING_X1, // humidity
                  Adafruit_BME280::FILTER_OFF);
  return result;
}

void BME280read()
{
  bme.takeForcedMeasurement();
  insideTemp = bme.readTemperature();
  insideHum = bme.readHumidity();
  insidePres = (uint16_t)(bme.readPressure() / 100.0F);
}
