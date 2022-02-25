#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>

U8X8_SSD1306_128X64_NONAME_HW_I2C display(U8X8_PIN_NONE);

void clearLcd()
{
  display.clear();
}

boolean initLcd() {
  boolean result = display.begin();
  display.setFont(u8x8_font_victoriabold8_r);
  display.fillDisplay();
  clearLcd();
  return result;
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

void textToLcd(int16_t x, int16_t y, const String str, boolean fullLine)
{
  uint8_t cols = 15U;
  String out = String(str.c_str());

  if (fullLine && cols > str.length())
  {
    out = addSpaces(str, cols - str.length());
    //Serial.println("out1 " + out);
  }

  if (cols < str.length())
  {
    out = str.substring(0, cols);
    //Serial.println("out2 " + out);
  }

  display.draw1x2String(x, y * 2, out.c_str());
}

void textToLcd(int16_t x, int16_t y, const String str)
{
  textToLcd(x, y, str, true);
}