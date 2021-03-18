#include <Arduino.h>
#include <U8x8lib.h>
#include <Constants.h>
#include <Data.h>

U8X8_SH1106_128X64_NONAME_HW_I2C lcd(U8X8_PIN_NONE);

boolean lcdInit()
{
  lcd.setFont(u8x8_font_victoriamedium8_r);
  return lcd.begin();
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
  lcd.setCursor(0U, 0U);
  lcd.print("CO2 " + String(co2Value));
}
