#include <EtherCard.h>

#define RED_DIODE_PORT 13
#define GREEN_DIODE_PORT 12
#define MOTOR_PORT1 11
#define MOTOR_PORT2 10

String TRUE_RFID = "1111";
String receivedRFID;

// Static IP configuration
static byte myip[] = { 169, 254, 238, 216 }; // Arduino's static IP address
static byte gwip[] = { 169, 254, 12, 63 }; // Gateway IP address

// MAC address of the Arduino
static byte mymac[] = { 0x74, 0x69, 0x69, 0x2D, 0x30, 0x31 };

// Ethernet buffer size
byte Ethernet::buffer[700];

void setup() {
  // put your setup code here, to run once:

  // Initializing ports
  pinMode(RED_DIODE_PORT, OUTPUT);
  pinMode(GREEN_DIODE_PORT, OUTPUT);
  pinMode(MOTOR_PORT1, OUTPUT);
  pinMode(MOTOR_PORT2, OUTPUT);

  // Setting up a serial communication to communicate with a virtual terminal
  Serial.begin(9600);

  // Begin Ethernet communication with buffer size and MAC address
  ether.begin(sizeof Ethernet::buffer, mymac, SS);

  // Configure static IP and gateway IP
  ether.staticSetup(myip, gwip);
}

void loop() {
  // put your main code here, to run repeatedly:

  if (Serial.available() > 0) {
    // Read the data into the String object
    receivedRFID = Serial.readString();
    Serial.println("Received RFID:" + receivedRFID);
    Serial.println("True RFID:" + TRUE_RFID);

    // ***********
    // TODO: THE connection to the sever should be handled here:
    Serial.println("Waiting for the Server to respond...");
    while(true){
      // Handle incoming Ethernet packets and get the position of the data
      word pos = ether.packetLoop(ether.packetReceive());
      if(pos)
        break;
    }
    

    // ************

    if(receivedRFID.equals(TRUE_RFID)){
      Serial.println("Verification Successful! The door is opening!");
      
      // Turning on the Green Light
      digitalWrite(GREEN_DIODE_PORT, HIGH);

      // Opening the door
      digitalWrite(MOTOR_PORT1, HIGH);
      delay(3000);
      digitalWrite(MOTOR_PORT1, LOW);
      
      // Keep it opened for 5 seconds
      delay(5000);

      // Closing the door
      digitalWrite(MOTOR_PORT1, LOW);
      digitalWrite(MOTOR_PORT2, HIGH);
      delay(3000);
      digitalWrite(MOTOR_PORT2, LOW);

      // Turning off the Green Light
      digitalWrite(GREEN_DIODE_PORT, LOW);
    }
    else{
      Serial.println("Access Denied!");
      // Turning on the Red Light
      digitalWrite(RED_DIODE_PORT, HIGH);
      delay(3000);
      digitalWrite(RED_DIODE_PORT, LOW);
    }

    // Clear the String object for the next read
    receivedRFID = "";
  }
}
