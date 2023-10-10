#include <SPI.h>
#include <MFRC522.h>

//....................................
#include <WiFi.h>
#include "ThingSpeak.h"

const char* ssid = "m52";                            // your wifi name
const char* password = "12346578";                  // your wifi password

WiFiClient  client;

unsigned long myChannelNumber = 1;                           // channel number
const char * myWriteAPIKey = "4KN47V39OY767QBP";      // your api key
//.............................................



#define RST_PIN     2    // Configurable, see typical pin layout above
#define SS_PIN      5    // Configurable, see typical pin layout above

MFRC522 mfrc522(SS_PIN, RST_PIN);  // Create MFRC522 instance

void setup() {
  Serial.begin(9600);  

      if(WiFi.status() != WL_CONNECTED){
      
       Serial.print("Attempting to connect");
      
    while(WiFi.status() != WL_CONNECTED){
        WiFi.begin(ssid, password); 
        delay(5000);     
      } 
        Serial.println("\nConnected.");
    }

    
  WiFi.mode(WIFI_STA);                         
  ThingSpeak.begin(client);   
    while (!Serial);
    SPI.begin();
    mfrc522.PCD_Init();
    delay(4);
    mfrc522.PCD_DumpVersionToSerial();
    Serial.println(F("Scan PICC to see UID, SAK, type, and data blocks..."));
}

void loop() {
    // Reset the loop if no new card is present
    if (!mfrc522.PICC_IsNewCardPresent()) {
        return;
    }

    // Select one of the cards
    if (!mfrc522.PICC_ReadCardSerial()) {
        return;
    }

    // Get the UID
    String uid = "";
    for (byte i = 0; i < mfrc522.uid.size; i++) {
        uid += String(mfrc522.uid.uidByte[i], HEX);
    }

    // Print UID to the serial monitor
    Serial.print("Card UID: ");
    Serial.println(uid);
    ThingSpeak.writeField(myChannelNumber, 1, uid, myWriteAPIKey);


    // Halt PICC
    mfrc522.PICC_HaltA();
    // Stop encryption on PCD
    mfrc522.PCD_StopCrypto1();
}
