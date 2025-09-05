#ifndef OLED_H
#define OLED_H

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "Buzzer.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

void init_oled_display()
{
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C))
  {
    Serial.println(F("SSD1306 allocation failed"));
    error_beep();
    while (true);
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE); // The only color is white
  display.display();
}

/**
 * Write the starting instructions to the OLED,
 * which prompt the user to type a quantity (1 through 9)
 */
void write_starting_instructions()
{
  display.clearDisplay();
  display.setCursor(0, 10);
  display.println(F("Enter"));
  // display.setCursor(10, 26);
  display.println(F("quantity"));
  display.display();
}

/**
 * Show the quantity, price, and total for the transaction on the OLED
 */
void display_price(uint8_t quantity, int price_per_unit_in_cents)
{
  Serial.print("Price per unit: ");
  Serial.println(price_per_unit_in_cents);
  display.clearDisplay();
  display.setCursor(0, 10);
  display.print(quantity);
  display.print(" x $");
  display.println(price_per_unit_in_cents / 100.00, 2); // .2f
  display.print("= $");
  display.print(quantity * price_per_unit_in_cents / 100.00, 2); // .2f
  display.display();
}

#endif