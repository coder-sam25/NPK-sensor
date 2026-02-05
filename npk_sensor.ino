#include <SoftwareSerial.h>

#define RE 8  // RS485 Receive Enable pin
#define DE 7  // RS485 Driver Enable pin

SoftwareSerial mod(2,3); // RX, TX pins for Arduino connecting to MAX TTL-RS485  DI-3,RO-2

// Single command string to get all 7 parameters values (as per vendor)
const byte request[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x07, 0x04, 0x08}; // CRC is likely correct here

// Buffer for reading response bytes, correct length = 19 bytes for 7 registers
#define RESPONSE_LENGTH 19
byte response[RESPONSE_LENGTH];

void setup() {
  Serial.begin(4800);
  mod.begin(4800);
  pinMode(RE, OUTPUT);
  pinMode(DE, OUTPUT);

  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  Serial.println("Starting NPK sensor communication...");
}

void loop() {
  // Clear any previous data from the buffer
  while(mod.available()) mod.read(); 

  // Enable RS485 Transmit
  digitalWrite(DE, HIGH);
  digitalWrite(RE, HIGH);

  // Send request command
  mod.write(request, sizeof(request));
  mod.flush(); // Ensure all bytes are physically sent before switching direction
  // delay(10); // flush() should handle this

  // Enable RS485 Receive
  digitalWrite(DE, LOW);
  digitalWrite(RE, LOW);

  // Wait for sensor to respond and read bytes
  int bytesRead = 0;
  unsigned long startTime = millis();
  
  // Use a longer timeout for reliability
  while (bytesRead < RESPONSE_LENGTH && (millis() - startTime) < 1000) { 
    if (mod.available()) {
      response[bytesRead++] = mod.read();
      // Optional: Add a small inter-byte delay check if the sensor is slow between bytes
    }
  }

  if (bytesRead == RESPONSE_LENGTH) {
    Serial.println("Received response:");
    for (int i = 0; i < RESPONSE_LENGTH; i++) {
      if (response[i] < 0x10) Serial.print("0");
      Serial.print(response[i], HEX); Serial.print(" ");
    }
    Serial.println();

    // *** IMPORTANT: You should verify the CRC of the response here before processing data ***

    // Convert hex bytes to decimals for each parameter (Indices 3 through 16 are data)
    unsigned int M  = (response[3]  << 8) | response[4];   // Moisture 
    unsigned int T  = (response[5]  << 8) | response[6];   // Temperature
    unsigned int EC = (response[7]  << 8) | response[8];   // EC
    unsigned int pH = (response[9]  << 8) | response[10];  // pH
    unsigned int N  = (response[11] << 8) | response[12];  // Nitrogen
    unsigned int P  = (response[13] << 8) | response[14];  // Phosphorus
    unsigned int K  = (response[15] << 8) | response[16];  // Potassium
    // Bytes 17 and 18 are the CRC bytes

    // Print conversions
    Serial.print("Moisture (M): ");    Serial.println(M);
    Serial.print("Temperature (T): "); Serial.println(T );
    Serial.print("EC: ");              Serial.println(EC);
    Serial.print("pH: ");              Serial.println(pH);
    Serial.print("Nitrogen (N): ");    Serial.println(N);
    Serial.print("Phosphorus (P): ");  Serial.println(P);
    Serial.print("Potassium (K): ");   Serial.println(K);

  } else {
    Serial.print("Error: Expected ");
    Serial.print(RESPONSE_LENGTH);
    Serial.print(" bytes but received ");
    Serial.println(bytesRead);
  }

  Serial.println("-------------");
  delay(10000); // Delay before next reading
}help me add this to github
