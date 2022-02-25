#include <Arduino.h>
#include <Constants.h>
#include <Data.h>
#include "TZ.h"
#include <time.h>

#define NTP_SERVER "time.google.com"
#define TZ "<+07>-7"

tm timeinfo;
time_t now;
long unsigned lastNTPtime;
unsigned long lastEntryTime;

String getFormattedTime()
{
  time(&now);
  localtime_r(&now, &timeinfo);
  unsigned long hours = timeinfo.tm_hour;
  String hoursStr = hours < 10 ? "0" + String(hours) : String(hours);

  unsigned long minutes = timeinfo.tm_min;
  String minuteStr = minutes < 10 ? "0" + String(minutes) : String(minutes);

  return hoursStr + ((timeinfo.tm_sec % 2) ? ":" : " ") + minuteStr;
}

bool getNTPtime(unsigned sec)
{

  {
    uint32_t start = millis();
    do
    {
      time(&now);
      localtime_r(&now, &timeinfo);
      Serial.print(".");
      delay(200);
    } while (((millis() - start) <= (1000U * sec)) && (timeinfo.tm_year < (2016 - 1900)));
    if (timeinfo.tm_year <= (2016 - 1900))
      return false; // the NTP call was not successful
    Serial.print("now ");
    Serial.println(now);
    char time_output[30];
    strftime(time_output, 30, "%a  %d-%m-%y %T", localtime(&now));
    Serial.println(time_output);
    Serial.println();
  }
  return true;
}

boolean ntpInit()
{
  configTime(TZ_Asia_Novosibirsk, NTP_SERVER);
  boolean result = getNTPtime(10U);
  lastNTPtime = time(&now);
  lastEntryTime = millis();
  return result;
}

String updateTime()
{
  timeStr = getFormattedTime();
  return timeStr;
}
