#include <SPI.h>
#include <MFRC522.h>

#include "RFID.h"
#include "Card.h"
#include "Keypad.h"
#include "Buzzer.h"
#include "OLED.h"

void setup()
{
  Serial.begin(9600);
  init_RFID_module();
  init_oled_display();
}

void loop()
{
  write_starting_instructions();
  uint8_t quantity = get_quantity_input();
  float price = handle_price_input(quantity);
  Serial.print("\n$");
  Serial.println(price);


  Serial.println("\n\nPlace your RFID tag near the reader...\n");

  // Look for new cards
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  Card* card = read_card();
  card->print();
  write_balance(card->get_balance() - price);
  card->buy_item(price);
  card->print();
  delete card;
  success_beep();

  // Clean up - prevents read collision
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
