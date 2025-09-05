#ifndef TRANSACTIONS_H
#define TRANSACTIONS_H

#include "RFID.h"
#include "TokenMarketPrice.h"
#include "Buzzer.h"
#include "LCD.h"

/**
  * Attempt to purchase an item for <price>. Works if the card has sufficient funds
  * @note The card must be in the RFID reader when calling this function
  * @returns True if the card had sufficient funds and the item was bought; false otherwise
  */
bool buy_item(float* current_balance, float price)
{
  if (*current_balance > price)
  {
    *current_balance -= price;
    write_balance(*current_balance);
    return true;
  }
  
  return false;
}

/**
  * Used when the user trades in a token using the ATM (tokens are the plastic gold coins).
  * The value of the token fluctuates like a stock.
  * @note The card must be in the RFID reader when calling this function
  */
void trade_in_token(float* current_balance)
{
  *current_balance += TOKEN_MARKET_PRICE;
  write_balance(*current_balance);
}

void wait_for_card_and_perform_transaction(float total_due)
{
  // Wait for user to insert card once prompted
  show_checkout_screen(total_due);
  wait_for_new_rfid_card();

  // Read card
  String user_name = read_name();
  float current_balance = read_balance();

  // Handle transaction (automatically updates current_balance to contain new bal)
  bool success = buy_item(&current_balance, total_due);
  if (success)
  {
    show_thank_you_screen(user_name, current_balance);
    success_beep();
    rfid_cleanup();
    delay(5000); // Show thank you screen for a bit
  }
  else
  {
    show_insufficient_funds_screen(current_balance);
    error_beep();
    delay(5000);
  }
}

void wait_for_card_and_show_user_his_balance()
{
  // Wait for user to insert card once prompted
  show_insert_card_to_see_balance_screen();
  wait_for_new_rfid_card();

  // Read card & show user his balance
  String user_name = read_name();
  float current_balance = read_balance();
  show_user_his_balance(user_name, current_balance);
  delay(5000);
}

#endif