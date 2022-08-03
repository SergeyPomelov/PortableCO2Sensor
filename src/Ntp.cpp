#include <Arduino.h>
#include <Constants.h>
#include <Data.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", UTC_OFFSET_SEC, 60000);

String updateTime()
{
  timeClient.update();
  timeStr = timeClient.getFormattedTime();
  // Serial.println(timeClient.getFormattedTime());
  return timeStr;
}

boolean ntpInit()
{
  timeClient.forceUpdate();
  return true;
}
