#include <SoftwareSerial.h>

#define RX_PIN 10
#define TX_PIN 11
#define SERVO_BAUD_RATE 38400
const byte SERVO_ID = 0x02;

SoftwareSerial mySerial(RX_PIN, TX_PIN);

void setup() {
  Serial.begin(9600);
  mySerial.begin(SERVO_BAUD_RATE);

  // Example usage: Write 2 bytes to address 6 (e.g., set baud rate to 38400 = 0x07)
  
  //writeData(0x01, 0x30, 0x00, 1);
  //delay(50);
  writeData(0x01, 0x05, 0x02, 1);
  //delay(50);
  //writeData(0x02, 0x30, 0x01, 1);
  
  
  /*
  //unlockEEPROM(SERVO_ID);
  byte addr = 0x2A;
  byte params[] = {0x00, 0x00};
  writeData(0x02, addr, params, sizeof(params));
  //lockEEPROM(SERVO_ID);
  */
}

void loop() {
  // Nothing here for now
}

void unlockEEPROM(uint8_t ID){
  writeData(ID, 0x30, 0x00, 1);//Unlocking the EEPROM
}

void lockEEPROM(uint8_t ID){
  writeData(ID, 0x30, 0x01, 1);//Locking the EEPROM
}

/*
 * Sends a WRITE command (0x03) to a serial servo.
 * ID      = Servo ID
 * addr    = Start address in EEPROM/SRAM
 * params  = Pointer to array of parameter bytes
 * len     = Number of parameters (max 20)
 */
void writeData(uint8_t ID, uint8_t addr, uint8_t* params, uint8_t len) {
  if (len > 20) len = 20; // Limit to 20 params

  byte packet[8 + len]; // Max size = headers + fixed bytes + 20 params
  packet[0] = 0xFF;           // Header
  packet[1] = 0xFF;           // Header
  packet[2] = ID;             // Servo ID
  packet[3] = len + 3;        // Length = instruction + addr + params + checksum
  packet[4] = 0x03;           // Instruction: WRITE
  packet[5] = addr;           // Starting address

  for (int i = 0; i < len; i++) {
    packet[6 + i] = params[i];  // Parameters
  }

  // Calculate checksum
  uint8_t sum = 0;
  for (int i = 2; i < 6 + len; i++) {
    sum += packet[i];
  }
  packet[6 + len] = ~sum & 0xFF; // Checksum

  mySerial.write(packet, 7 + len);

  // Optional: read status packet (7 bytes expected)
  byte response[7];
  int i = 0;
  unsigned long start = millis();
  while (i < 7 && (millis() - start) < 100) {
    if (mySerial.available()) {
      response[i++] = mySerial.read();
    }
  }

  Serial.print("Write response (");
  Serial.print(i);
  Serial.print(" bytes): ");
  for (int j = 0; j < i; j++) {
    Serial.print("0x");
    if (response[j] < 0x10) Serial.print("0");
    Serial.print(response[j], HEX);
    Serial.print(" ");
  }
  Serial.println();
}
