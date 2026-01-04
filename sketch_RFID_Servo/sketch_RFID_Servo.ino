#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

#define RST_PIN 9
#define SS_PIN 10
#define SERVO_PIN 6
#define LED_OK 8
#define LED_ERR 7
#define BUZZER 3 

Servo myservo;
MFRC522 mfrc522 (SS_PIN, RST_PIN);
byte uidCard[4] = {0x52, 0xF4, 0xA8, 0x5C};
int pos = 0;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  mfrc522.PCD_Init();
  
  mfrc522.PCD_DumpVersionToSerial();
  Serial.println();
  myservo.attach(SERVO_PIN);

  pinMode(LED_OK, OUTPUT);
  pinMode(LED_ERR, OUTPUT);
}

void loop() {
  if (! mfrc522.PICC_IsNewCardPresent()) return;

  if (! mfrc522.PICC_ReadCardSerial()) return;

  for(byte i = 0; i < 4; i++ ) {
    if(uidCard[i] != mfrc522.uid.uidByte[i]) {
      digitalWrite(LED_ERR, HIGH);

      for (int j = 0; j < 3; j++) {
        tone(BUZZER, 1000);   // 1000 Hz
        delay(200);
        noTone(BUZZER);
        delay(100);
      }

      delay(3000);
      digitalWrite(LED_ERR, LOW);
      return;
    } 
  }

  digitalWrite(LED_OK, HIGH);

  // один длинный писк (успех)
  tone(BUZZER, 2000);    // 2000 Hz - выше тон
  delay(500);
  noTone(BUZZER);
  // Если активный buzzer (пищит сам при подаче питания), замени tone()/noTone() на digitalWrite(BUZZER, HIGH/LOW).

  for (pos = 0; pos <= 180; pos += 1) {
    myservo.write(pos);
    delay(15);
  }
  for (pos = 180; pos >= 0; pos -= 1) {
    myservo.write(pos);
    delay(15);
  }

  delay(3000);
  digitalWrite(LED_OK, LOW);
  delay(3000);
}
