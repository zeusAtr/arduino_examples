#include <SPI.h>
#include <MFRC522.h>

#define RST_PIN 9
#define SS_PIN 10

MFRC522 mfrc522 (SS_PIN, RST_PIN);
byte uidCard[4] = {0x52, 0xF4, 0xA8, 0x5C};


void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  
  mfrc522.PCD_DumpVersionToSerial();
  Serial.println();
}

void loop() {
  if (! mfrc522.PICC_IsNewCardPresent()) {
    return;
  }

  if (! mfrc522.PICC_ReadCardSerial()) {
    return;
  }

  for(byte i = 0; i < 4; i++ ) {
    if(uidCard[i] != mfrc522.uid.uidByte[i]) {
      digitalWrite(2, HIGH);
      digitalWrite(7, HIGH);
      delay(3000);
      digitalWrite(7, LOW);

      return;
    } 
  }

  digitalWrite(8, HIGH);
  delay(3000);
  digitalWrite(8, LOW);
  delay(3000);
}
