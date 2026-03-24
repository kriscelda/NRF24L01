#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// S3-WROOM Safe Pin Definitions
#define CE_PIN    9
#define CSN_PIN   10
#define SCK_PIN   12
#define MISO_PIN  13
#define MOSI_PIN  11
#define JOY_SW    1  // Your Joystick button

RF24 radio(CE_PIN, CSN_PIN);
const byte address[6] = "S3_01"; 

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("\n--- S3-WROOM TRANSMITTER STARTING ---");

  pinMode(JOY_SW, INPUT_PULLUP);
  
  // 1. FORCE PIN MODES
  pinMode(CE_PIN, OUTPUT);
  pinMode(CSN_PIN, OUTPUT);
  digitalWrite(CSN_PIN, HIGH);

  // 2. INITIALIZE SPI FOR S3
  SPI.begin(SCK_PIN, MISO_PIN, MOSI_PIN, CSN_PIN);
  delay(100);

  if (!radio.begin()) {
    Serial.println("Transmitter Radio Hardware NOT found!");
    while (1); // Halt if radio is missing
  }

  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_LOW); // Keep it LOW for desk testing
  radio.stopListening();
  
  Serial.println("Transmitter Ready. Press Joystick to Send!");
}

void loop() {
  // Read the joystick button (Low when pressed)
  if (digitalRead(JOY_SW) == LOW) {
    int val = 1;
    bool report = radio.write(&val, sizeof(val));

    if (report) {
      Serial.println("Button: PRESSED -> Sent Successfully!");
    } else {
      Serial.println("Button: PRESSED -> Delivery Failed (No Receiver found)");
    }
    
    delay(200); // Debounce
  }
}