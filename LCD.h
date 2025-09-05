#ifndef LCD_H
#define LCD_H

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include "Buzzer.h"

// The LCD I2C address is usually 0x27 or 0x3F
#define LCD_ADDRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

LiquidCrystal_I2C lcd(LCD_ADDRESS, LCD_COLUMNS, LCD_ROWS);

void init_lcd_display()
{
  lcd.init();
  lcd.backlight();
  lcd.clear();
}


/**
 * Write the starting instructions to the LCD,
 * which prompt the user to type a quantity (1 through 9)
 */
void write_starting_instructions()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Enter quantity"));
  lcd.setCursor(0, 1);
  lcd.print(F("between [1-9]"));
}

/**
 * Show the quantity, price, and total for the transaction on the LCD
 * @param quantity Quantity purchased
 * @param price_per_unit_in_cents Price in cents
 */
void display_price(uint8_t quantity, uint32_t price_per_unit_in_cents)
{
  lcd.clear();
  lcd.setCursor(0, 0);

  // Format: "3 x $1.23"
  lcd.print(quantity);
  lcd.print(F(" x $"));
  lcd.print(price_per_unit_in_cents / 100.0, 2);

  // Format: "= $3.69"
  lcd.setCursor(0, 1);
  lcd.print(F("  = $"));
  lcd.print((quantity * price_per_unit_in_cents) / 100.0, 2);
}

/**
 * The checkout screen shows the amount due (the price of everything)
 * and prompts the user to insert their card
 */
void show_checkout_screen(float total_due)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Amt due: $"));
  lcd.print(total_due, 2);
  lcd.setCursor(0, 1);
  lcd.print(F("Tap card to pay"));
}

/**
 * Thank the user for their purchase
 */
void show_thank_you_screen(String user_name, float remaining_balance)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Thank you "));
  lcd.print(user_name);
  lcd.setCursor(0, 1);
  lcd.print(F("Bal: $"));
  lcd.print(remaining_balance, 2);
}

/**
 * Show the user that they don't have enough money to finish the transaction
 */
void show_insufficient_funds_screen(float user_balance)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Card declined"));
  lcd.setCursor(0, 1);
  lcd.print(F("Bal: $"));
  lcd.print(user_balance, 2);
}

void show_user_his_balance(String user_name, float user_balance)
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Hello "));
  lcd.print(user_name);
  lcd.setCursor(0, 1);
  lcd.print(F("Funds: $"));
  lcd.print(user_balance, 2);
}

void show_insert_card_to_see_balance_screen()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("Tap card to view"));
  lcd.setCursor(0, 1);
  lcd.print(F("your balance"));
}

#endif