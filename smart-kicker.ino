/*
 * 
 * Tutorial links
 * http://henrysbench.capnfatz.com/henrys-bench/arduino-sensors-and-input/arduino-hc-sr501-motion-sensor-tutorial
 * https://randomnerdtutorials.com/security-access-using-mfrc522-rfid-reader-with-arduino
 
 * ----------------------------------------
 * | HC-SR501 Motion Detector | Arduino   |
 * ----------------------------------------
 * | RL (output)              | Digital 8 |
 * | GND                      | GND       |
 * | VCC                      | 5V        |
 * ----------------------------------------
 * 
 * -----------------------------------
 * | RC522 RFID Reader | Arduino     |
 * -----------------------------------
 * | SDA               | Digital 10  |
 * | SCK               | Digital 13  |
 * | MOSI              | Digital 11  |
 * | MISO              | Digital 12  |
 * | IRQ               | Unconnected |
 * | GND               | GND         |
 * | RST               | Digital 9   |
 * | 3.3V              | 3.3V        |
 * -----------------------------------
 *
 */

#include <SPI.h>
#include <MFRC522.h>
 
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int pirPin = 8; // Input for HC-S501
int pirValue; // Place to store read PIR Value
int oldPirValue = 0;
String content;
String oldContent = "";
unsigned long old_time;
unsigned long new_time;

unsigned long period = 1000;


void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();      // Initiate  SPI bus
  mfrc522.PCD_Init();   // Initiate MFRC522
  pinMode(pirPin, INPUT);
  new_time = millis();
}
void loop() 
{
  pirValue = digitalRead(pirPin);
  if (pirValue != oldPirValue) {
    // Change the presence sensor number when uploading to a specific device
    content = "{'presence': 1, 'value': ";
    content.concat(String(pirValue));
    content.concat("'}");
    oldPirValue = pirValue;
    Serial.print(content);
  }
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  // Change the rfid reader number when uploading to a specific device
  content = "{'rfid': 1, 'card': '";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     content.concat(String(mfrc522.uid.uidByte[i]));
  }
  content.concat("'}");
  if (content != oldContent) {
    Serial.print(content);
    oldContent = content;
  }
  new_time = millis();
  if (new_time - old_time > 5000) {
    oldContent = "";
  }
  old_time = new_time;
}