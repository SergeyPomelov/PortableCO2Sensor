#include <Arduino.h>
#include <Constants.h>
#include <Data.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(DISPLAY_ADDR, 20U, 4U);

void lcdInit()
{
  lcd.init(SDA, SCL);
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0U, 0U);
  analogWrite(BACKLIGHT_PIN, 255U);
}

void lcdSetCursor(const uint8_t col, const uint8_t row)
{
  lcd.setCursor(col, row);
}

void lcdPrint(const String &s)
{
  lcd.print(s);
}

void lcdClear()
{
  lcd.clear();
}

void lcdUpdateData()
{
  uint32_t backlit = (uint32_t) lux30.getAverage(30) * 6U;
  lcd.setCursor(0U, 0U);
  lcd.print(String(dispTemp - 3.3F, 2U) + "C " + String(dispHum, 0U) + "% " + String(dispPres) + "hPa");
  lcd.setCursor(0U, 1U);
  lcd.print("CO2 " + String(co2Value) + "ppm " + (int)lux.getAverage() + "Lux  ");
  lcd.setCursor(0U, 2U);
  lcd.print("TVOC " + String((uint16_t)tvocValue.getMedian()) + "ppb     ");
  lcd.setCursor(0U, 3U);
  lcd.print("                   ");
  analogWrite(BACKLIGHT_PIN, backlit < 255U ? (int) backlit : 255U);
}
