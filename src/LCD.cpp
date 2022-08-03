#include <Arduino.h>
#include <Constants.h>
#include <Data.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd = LiquidCrystal_I2C(DISPLAY_ADDR, 20U, 4U);

void initLcd()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.setCursor(0U, 0U);
  analogWrite(BACKLIGHT_PIN, 255U);
}

void clearLcd()
{
  lcd.clear();
}

String addSpaces(String input, const uint8_t amount)
{
  String output = String(input.c_str());

  for (int i = 0; i < amount; i++)
  {
    output.concat(' ');
  }
  return output;
}

void textToLcd(const uint8_t row, const String str)
{
  uint8_t cols = 20U;
  String out = String(str.c_str());

  if (cols > str.length())
  {
    out = addSpaces(str, cols - str.length());
    // Serial.println("out1 " + out);
  }

  if (cols < str.length())
  {
    out = str.substring(0, cols);
    // Serial.println("out2 " + out);
  }

  lcd.setCursor(0U, row);
  // Serial.println("in " + str);

  lcd.print(out);
}
