#ifndef RFID_H
#define RFID_H

#include <SPI.h>
#include <MFRC522.h>

#include "Card.h"

#define NAME_BLOCK 4    // The first block of the RFID card is used to store the user's name (char*)
#define BALANCE_BLOCK 5 // The second block is used to store their balance (float)

#define SS_PIN 10 // 'SDA' CS -> D10
#define RST_PIN 9 // RST -> D9

MFRC522 rfid(SS_PIN, RST_PIN);

static MFRC522::MIFARE_Key RFID_KEY;
void init_auth_key()
{
  // Set default key = FF FF FF FF FF FF
  for (byte i = 0; i < 6; i++)
  {
    RFID_KEY.keyByte[i] = 0xFF;
  }
}

/**
 * Initializes the MFRC522 RFID module.
 * Meant to be called once in setup()
 */
void init_RFID_module()
{
  SPI.begin();
  rfid.PCD_Init();
  init_auth_key();
  delay(100);
}

/**
 * Helper function to read a single block of data from the RFID card
 */
void read_single_block(byte block, byte* buffer) {
  byte size = 18;
  MFRC522::StatusCode status;
  status = rfid.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &RFID_KEY, &(rfid.uid));
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Auth failed for block ")); Serial.println(block);
    return;
  }
  status = rfid.MIFARE_Read(block, buffer, &size);
  if (status != MFRC522::STATUS_OK) {
    Serial.print(F("Read failed for block ")); Serial.println(block);
    return;
  }
}

/**
 * Read the data block from the RFID card containing the user's name
 */
String read_name()
{
  byte data[16];
  read_single_block(NAME_BLOCK, data);
  data[15] = 0; // ensure null termination
  return String((char*)data);
}

/**
 * Read the data block from the RFID card containing the user's balance
 */
float read_balance()
{
  byte data[16];
  read_single_block(BALANCE_BLOCK, data);
  float value;
  memcpy(&value, data, sizeof(float));
  return value;
}

/**
 * Helper function for writing data to a block
 */
void write_card_block(byte block, byte *data) {
  MFRC522::StatusCode status;

  // Authenticate
  status = rfid.PCD_Authenticate(
    MFRC522::PICC_CMD_MF_AUTH_KEY_A,
    block,
    &RFID_KEY,
    &(rfid.uid)
  );
  
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("Auth failed for block "));
    Serial.println(block);
    return;
  }

  // Write data (must be exactly 16 bytes)
  status = rfid.MIFARE_Write(block, data, 16);
  if (status != MFRC522::STATUS_OK)
  {
    Serial.print(F("Write failed for block "));
    Serial.println(block);
    return;
  }

  Serial.print(F("Block ")); Serial.print(block); Serial.println(F(" written successfully!"));
}

/**
 * Rewrite the user's name in the card's name block
 */
void write_name(String name)
{
  byte data[16] = {0}; // clear block
  name.toCharArray((char*)data, 16); // copy string into bytes
  write_card_block(NAME_BLOCK, data);
}

/**
 * Rewrite the user's balance in the card's balance block
 */
void write_balance(float value)
{
  byte data[16] = {0};
  // to read a block you have to have a 16 byte buffer because all 16 bytes are read at once
  // copy float into first 4 bytes of buffer
  memcpy(data, &value, sizeof(float));
  write_card_block(BALANCE_BLOCK, data);
}

String read_UID()
{
  String uid = "";
  for (byte i = 0; i < rfid.uid.size; i++)
  {
    if (rfid.uid.uidByte[i] < 0x10) uid += "0"; // pad single hex digits
    uid += String(rfid.uid.uidByte[i], HEX);
  }

  uid.toUpperCase();
  return uid;
}

Card* read_card()
{  
  String uid = read_UID();
  String name = read_name();
  float bal = read_balance();
  return new Card(uid, name, bal);
}

#endif