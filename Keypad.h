#ifndef KEYPAD1_H
#define KEYPAD1_H

#include <Keypad.h>

#include "LCD.h"

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
    Serial.println(key);
    if (key || key == '0') return key;
  }
}

/**
 * Get a key input to represent that quantity. Numbers 0 through 9 only.
 * 1-9 are for the quantity but 0 is for moving into read card mode where users can sscan to see their balance
 */
uint8_t get_quantity_input()
{
  while (true)
  {
    char key = listen_for_keypress();
    if (isdigit(key)) return static_cast<uint8_t>(key - '0');
  }
}

/**
 * Accept key input until the # is pressed. This will be the price.
 * Display on the LCD the formatted current price input as it's being typed.
 * @param quantity The amount that will be purchased at the typed price
 */
float handle_price_input(uint8_t quantity)
{
    char price_str[8] = ""; // Up to 7 digits plus a null terminator
    uint8_t price_len = 0;
    display_price(quantity, 0);

    while (true)
    {
      char c = listen_for_keypress();
      
      if (c == '*' && price_len > 0)
      {
        price_str[--price_len] = '\0'; // Remove last character and add null terminator
        display_price(quantity, price_len == 0 ? 0 : atol(price_str));
        continue;
      }

      if (c == '#') break;
      
      if (isdigit(c) && price_len < 7) {
        price_str[price_len++] = c;
        price_str[price_len] = '\0';
        display_price(quantity, atol(price_str));
      }
    }

    return atol(price_str) / 100.00;
}

#endif