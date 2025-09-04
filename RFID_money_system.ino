#include <SPI.h>
#include <MFRC522.h>

#include "RFID.h"
#include "Card.h"

void setup()
{
  Serial.begin(9600);
  init_RFID_module();
  Serial.println("\n\nPlace your RFID tag near the reader...\n");
}

void loop()
{
  // Look for new cards
  if (!rfid.PICC_IsNewCardPresent()) return;
  if (!rfid.PICC_ReadCardSerial()) return;

  Card* card = read_card();
  card->print();
  delete card;

  // Clean up - prevents read collision
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}
