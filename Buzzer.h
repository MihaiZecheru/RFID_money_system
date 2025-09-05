#ifndef BUZZER_H
#define BUZZER_H

#define BUZZER_PIN A0

/**
 * Play when the card is read and the transaction is completed
 */
void success_beep()
{
  tone(BUZZER_PIN, 500);
  delay(150);
  tone(BUZZER_PIN, 1000);
  delay(300);
  noTone(BUZZER_PIN);
}

/**
 * Play if something failed and the transaction could not be completed
 */
void error_beep()
{
  tone(BUZZER_PIN, 250);
  delay(1000);
  noTone(BUZZER_PIN);
}

#endif