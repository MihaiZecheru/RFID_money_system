#ifndef KEYPAD1_H
#define KEYPAD1_H

#include <Keypad.h>

#include "OLED.h"

// COL2, ROW1, COL1, ROW4, COL3, ROW3, ROW2.
#define COL2 8
#define ROW1 7
#define COL1 6
#define ROW4 5
#define COL3 4
#define ROW3 3
#define ROW2 2

const byte ROW_COUNT = 4;
const byte COL_COUNT = 3;

char keymap[ROW_COUNT][COL_COUNT] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROW_COUNT] = {ROW1, ROW2, ROW3, ROW4};
byte colPins[COL_COUNT] = {COL1, COL2, COL3};

Keypad keypad = Keypad(makeKeymap(keymap), rowPins, colPins, ROW_COUNT, COL_COUNT);

/**
 * Wait for keypress from the keypad. Blocking function
 */
char listen_for_keypress()
{
  while (true)
  {
    char key = keypad.getKey();
    if (key) return key;
  }
}

uint8_t get_quantity_input()
{
  while (true)
  {
    char key = listen_for_keypress();
    if (key != '*' && key != '0' && key != '#')
    return static_cast<uint8_t>(key - '0');
  }
}

/**
 * Accept key input until the # is pressed.
 * Display on the OLED the formatted current price input as it's being typed.
 * @param quantity The amount that will be purchased at the typed price
 */
float handle_price_input(uint8_t quantity)
{
  String price_str = "";
  display_price(quantity, price_str.length() == 0 ? 0 : price_str.toInt());

  while (true)
  {
    char c = listen_for_keypress();
    Serial.print("Keypress: ");
    Serial.println(c);
    
    // Delete previous character
    if (c == '*' && price_str.length() > 0)
    {
      price_str = price_str.substring(0, price_str.length() - 1);
      display_price(quantity, price_str.length() == 0 ? 0 : price_str.toInt());
      continue;
    }

    if (c == '#') break;
    price_str += c;
    Serial.print("price_str: $");
    Serial.println(price_str);
    display_price(quantity, price_str.toInt());
  }
  
  return price_str.toInt();
}

#endif