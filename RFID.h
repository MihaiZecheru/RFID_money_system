#ifndef RFID_H
#define RFID_H

#include <SPI.h>
#include <MFRC522.h>

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
 * @note names must be at most 6 characters
 */
String read_name()
{ // TODO: fix name being cccc
  byte data[16];
  read_single_block(NAME_BLOCK, data);
  
  // Find the end of the name or the end of the buffer
  byte i = 0;
  while (i < 16 && data[i] != '\0' && data[i] != ' ' && data[i] != '\n' && data[i] != '\r') i++;
  data[i] = '\0';

  String s = String((char*)data);
  if (s.length() > 6) s = s.substring(0, 6);
  return s;
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
}

/**
 * Rewrite the user's name in the card's name block
 * @note names must be at most 6 characters
 */
void write_name(String name)
{
  if (name.length() > 6) name = name.substring(0, 6);
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

/**
 * Prevent the same card from being read back-to-back. Blocking function.
 * @returns True if a new RFID card was detected
 */
bool wait_for_new_rfid_card()
{
  while (true)
  {
    if (!rfid.PICC_IsNewCardPresent()) continue;
    if (!rfid.PICC_ReadCardSerial()) continue;
    return true;
  }
}

/**
 * Prevents read collision and allow card to be read again at a later time
 */
void rfid_cleanup()
{
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

#endif