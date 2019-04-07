#include <Adafruit_Fingerprint.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Servo.h>

/* Initialisation capteur RFID */
#define SS_PIN 10
#define RST_PIN 9
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 

/* Initialisation empreinte digitale */
SoftwareSerial mySerial(2, 3);
Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

/* Initialisation Servo */
Servo myServo;

/* Variables de verification */
bool verifFinger = false;
bool verifRFID = false;
int counter = 0;

void setup()  
{
  Serial.begin(9600);
  while (!Serial);
  delay(100);

  // Pin pour les LEDs externes
  pinMode(6, OUTPUT);
  pinMode(7, OUTPUT);
  digitalWrite(6, LOW);
  digitalWrite(7, LOW);

  // Pin du servo
  myServo.attach(5);
  myServo.write(0);
  delay(500);
  myServo.detach();
  
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 

  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }

  // set the data rate for the sensor serial port
  finger.begin(57600);
  
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  finger.getTemplateCount();
  Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  Serial.println("Waiting for valid finger...");
  digitalWrite(13, LOW);
}

void loop()
{
  if(verifFinger == false)  getFingerprintIDez();
  if(verifRFID == false)    getNUID();
  
  delay(50);
  
  if ((verifFinger == true) || (verifRFID == true)) {
    if ((verifFinger == true) && (verifRFID == true)) {
      myServo.attach(5);
      myServo.write(174);
      verifFinger = false;
      verifRFID = false;
      counter = 0;
      danseLEDs();
      myServo.write(0);
      delay(500);
      myServo.detach();
    }

    else{
      counter++;
      if (verifFinger == true)  digitalWrite(6, HIGH);
      else if (verifRFID == true)  digitalWrite(7, HIGH);
      
      Serial.println(counter);
      if (counter >= 100) {
        digitalWrite(6, LOW);
        digitalWrite(7, LOW);
        counter = 0;
        verifFinger = false;
        verifRFID = false;
      }
    }
  }
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;
  
  // Empreinte correspondante
  Serial.println("VALIDE");
  verifFinger = true;
  return finger.fingerID; 
}

// Verifie les si il y a une carte RFID posée
void getNUID() {
  // Look for new cards
  if ( ! rfid.PICC_IsNewCardPresent())
    return;

  // Verify if the NUID has been readed
  if ( ! rfid.PICC_ReadCardSerial())
    return;
    
  Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&  
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    return;
  }
  
  Serial.println(F("The NUID tag is:"));
  Serial.print(F("In hex: "));
  printHex(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();
  Serial.print(F("In dec: "));
  printDec(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();

  if(verifUID(rfid.uid.uidByte, rfid.uid.size))
  {
    verifRFID = true;
  }

  // Halt PICC
  rfid.PICC_HaltA();

  // Stop encryption on PCD
  rfid.PCD_StopCrypto1();
}

// Affiche le NUID en hexadécimal
void printHex(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

// Affiche le NUID en décimal
void printDec(byte *buffer, byte bufferSize) {
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], DEC);
  }
}

/* Retourne 1 si la valeur de la carte est la bonne */
uint8_t verifUID(byte *buffer, byte bufferSize) {
  byte goodUID[4] = { 63, 234, 56, 41 };
  uint8_t compteur = 0;
  
  if(bufferSize != 4) { return -1; }
  
  else{
    for (byte i = 0; i < 4; i++) {
      if(buffer[i] == goodUID[i]) {
        compteur++;
      }
    }
  }
  return 1;
}

void danseLEDs() {
  digitalWrite(6, HIGH);
  delay(100);
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  delay(100);
  digitalWrite(7, LOW);
  digitalWrite(6, HIGH);
  delay(100);
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  delay(100);
  digitalWrite(7, LOW);
  digitalWrite(6, HIGH);
  delay(100);
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  delay(100);
  digitalWrite(7, LOW);
  digitalWrite(6, HIGH);
  delay(100);
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  delay(100);
  digitalWrite(7, LOW);
  digitalWrite(6, HIGH);
  delay(100);
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  delay(100);
  digitalWrite(7, LOW);
  digitalWrite(6, HIGH);
  delay(100);
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  delay(100);
  digitalWrite(7, LOW);
  digitalWrite(6, HIGH);
  delay(100);
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  delay(100);
  digitalWrite(7, LOW);
  digitalWrite(6, HIGH);
  delay(100);
  digitalWrite(6, LOW);
  digitalWrite(7, HIGH);
  delay(100);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  delay(250);
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  delay(100);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  delay(100);
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  delay(100);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  delay(100);
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  delay(100);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  delay(100);
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  delay(100);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  delay(100);
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  delay(100);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  delay(100);
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  delay(100);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  delay(100);
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  delay(100);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
  delay(100);
  digitalWrite(7, HIGH);
  digitalWrite(6, HIGH);
  delay(4000);
  digitalWrite(7, LOW);
  digitalWrite(6, LOW);
}
