/*
If for some ungodly reason, you are using these motors, may the gods be with you.
They are absolutely maddeningly random, and are some of the weirdest things I've ever worked with.
My frist experience with them was at the LAM research challenge, when a sadistic engineer decided to screw with a bunch of college kids
10/10 would never reccomend using again
However, it is a really good exercise in debugging skills to work with this. My progress is documented on GitHub so please read it, it'll hopefully make sense to any beginners out there
All the best!
*/

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
const byte SERVO_ID = 0x01;

//Initializing the SoftwareSerial connection
SoftwareSerial mySerial(RX_PIN, TX_PIN);

void setup() {
  //Initializing Serial  for our own debugging
  Serial.begin(9600);
  while(!Serial);

  //Initializing the Software Serial at the required baud ate
  mySerial.begin(SERVO_BAUD_RATE);    

  Serial.println("Pinging Servo...");
  delay(250);

  //This function is defined at the bottom
  pingServo(SERVO_ID);
}

void loop() {

}

//Function to ping the servo
void pingServo(byte id){
  //define the message to be sent
  byte packet[6];

  // Build the ping packet
  packet[0] = 0xFF;
  packet[1] = 0xFF;
  packet[2] = id;
  packet[3] = 0x02;      // length = instruction + checksum
  packet[4] = 0x01;      // PING instruction
  packet[5] = ~(id + packet[3] + packet[4]);  // checksum

  //Sending the signal using the software serial port
  mySerial.write(packet, 6);

  //Reading Response
  byte response[6];
  int i = 0;
  unsigned long start = millis();
  while (i < 6 && (millis() - start) < 100) {
    if (mySerial.available()) {
      response[i++] = mySerial.read();
    }
  }

  // Print response short form
  if (i == 6 && response[0] == 0xFF && response[1] == 0xFF && response[2] == id && (response[4] == 0x00 || response[4] == 0x01)) {
    Serial.println("Servo responded OK!");
  } else {
    Serial.println("No response or bad response from servo.");
  }

  // Now print the detailed response for debugging
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
