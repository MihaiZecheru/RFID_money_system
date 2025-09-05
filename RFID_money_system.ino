#include <SPI.h>
#include <MFRC522.h>

#include "RFID.h"
#include "Transactions.h"
#include "Keypad.h"
#include "Buzzer.h"
#include "LCD.h"

// NOTE: names must be at most 6 characters

void setup()
{
  // Wire.begin();
  Serial.begin(9600);
  init_RFID_module();
  init_lcd_display();
}

void loop()
{
  write_starting_instructions();
  uint8_t quantity = get_quantity_input();
  if (quantity == 0)
  {
    // No quantity given; enter "read" mode
    // Users can scan their card to see their balance
    wait_for_card_and_show_user_his_balance();
  }
  else
  {
    // Quantity has been given. Get the price per item, multiply, then charge the card
    float price_per_item = handle_price_input(quantity);
    float total = price_per_item * quantity;
    wait_for_card_and_perform_transaction(total);
  }
}
