//We need to use software serial on any arduino boards as there's only one UART port on the UNO. 
//We use the main UART port for debugging through serial monitor, we use the software one to send data to the board
#include <SoftwareSerial.h>

//RX of the uno goes to RX of the board, same with TX 
#define RX_PIN 10
#define TX_PIN 11

//The default servo baud rate is 1000000. However, SoftwareSerial isn't capable of handling that. You have to drop the baud rate using the python code
//Python code is also in the github, don't set baud rate to 76800 because the python code doesn't like that
//Software serial caps out at around 50000 baud rate, so don't necessarily go higher than that.
#define SERVO_BAUD_RATE 38400

//Servo ID is by default 0x01, you can edit it as you please
const byte SERVO_ID = 0x02;

//Initializing the SoftwareSerial connection
SoftwareSerial mySerial(RX_PIN, TX_PIN);

void setup() {
  //Serial Monitor for debugging
  Serial.begin(9600);

  //Software Serial Initialization
  mySerial.begin(SERVO_BAUD_RATE);

  readData(0x02, 0x30, 1);

}

void loop() {
  // put your main code here, to run repeatedly:

}

void readData(uint8_t ID, uint8_t ADDR, int NUM_BYTES){
  byte packet[7];
  packet[0] = 0xFF;   //Standard starting Byte
  packet[1] = 0xFF;   //Standard starting Byte
  packet[2] = ID;     //ID
  packet[3] = 0x04;   //Length of Command, always going to be 4 bytes
  packet[4] = 0x02;   //The specific write instruction, always going to be 0x02
  packet[5] = ADDR;   //The address of the memory table we're looking to read
  packet[6] = NUM_BYTES;  //Number of bytes we want to read
  packet[7] = ~(packet[3] + packet[4] + packet[5] + packet[6]);   //Standard checksum

  //Writing the packet to the motor
  mySerial.write(packet, 8);

  //Reading the response
  byte response[7];
  int i = 0;
  unsigned long start = millis();
  while (i < 7 && (millis() - start) < 100) {
    if (mySerial.available()) {
      response[i++] = mySerial.read();
    }
  }

  Serial.print("Received (");
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
