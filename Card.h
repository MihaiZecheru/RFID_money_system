#ifndef CARD_H
#define CARD_H

#include "TokenMarketPrice.h"

class Card
{
private:
  String uid;
  String name;
  float balance;

public:
  Card(String uid, String name, float balance)
  {
    this->uid = uid;
    this->name = name;
    this->balance = balance;
  }

  String get_uid()
  {
    return this->uid;
  }

  String get_name()
  {
    return this->name;
  }

  float get_balance()
  {
    return this->balance;
  }

  /**
   * Attempt to purchase an item for <price>. Works if the card has sufficient funds
   * @returns True if the card had sufficient funds and the item was bought; false otherwise
   */
  bool buy_item(float price)
  {
    if (this->balance > price)
    {
      this->balance -= price;
      return true;
    }
    
    return false;
  }

  /**
   * Used when the user trades in a token using the ATM (tokens are the plastic gold coins).
   * The value of the token fluctuates like a stock.
   */
  void trade_in_token()
  {
    this->balance += TOKEN_MARKET_PRICE;
  }

  /**
   * Print the card's UID, name, and balance to the console
   */
  void print()
  {
    Serial.print("UID tag:         ");
    Serial.println(this->uid);
    Serial.print("Name on card:    ");
    Serial.println(this->name);
    Serial.print("Balance on card: $");
    Serial.println(this->balance);
    Serial.println();
  }
};

#endif