#include <Arduino.h>
#include <RunningMedian.h>

float dispTemp;
float dispHum;
uint16_t dispPres;
RunningMedian lux30(30U);
RunningMedian lux(5U);
uint16_t co2Value;
RunningMedian tvocValue(10U);