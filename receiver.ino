#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// S3-WROOM Safe Pin Definitions
#define CE_PIN    9
#define CSN_PIN   10
#define SCK_PIN   12
#define MISO_PIN  13
#define MOSI_PIN  11
#define LED_PIN   14 

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "S3_01"; 

void setup() {
  Serial.begin(115200);
  delay(2000); 
  Serial.println("\n--- S3-WROOM RECEIVER STARTING ---");

  pinMode(LED_PIN, OUTPUT);
  
  // Force Pin Modes
  pinMode(CE_PIN, OUTPUT);
  pinMode(CSN_PIN, OUTPUT);
  digitalWrite(CSN_PIN, HIGH);

// Add this BEFORE SPI.begin
  pinMode(MISO_PIN, INPUT_PULLUP); 


  // Initialize SPI
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CSN_PIN);
  delay(100);

  if (!radio.begin()) {
    Serial.println("Radio hardware NOT found! Check pins 9-13.");
    while (1); 
  }

  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.startListening();
  
  Serial.println("Receiver is Ready and Listening...");
}

// THIS IS THE PART THAT WAS MISSING OR BROKEN:
void loop() {
  if (radio.available()) {
    int receivedMsg = 0;
    radio.read(&receivedMsg, sizeof(receivedMsg));

    Serial.print("Signal Received: ");
    Serial.println(receivedMsg);

    if (receivedMsg == 1) {
      digitalWrite(LED_PIN, HIGH);
      Serial.println("Action: LED ON");
    } else {
      digitalWrite(LED_PIN, LOW);
      Serial.println("Action: LED OFF");
    }
  }
}