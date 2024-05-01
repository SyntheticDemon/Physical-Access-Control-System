#include <EtherCard.h>

#define RED_DIODE_PORT 9
#define GREEN_DIODE_PORT 8
#define MOTOR_PORT1 7
#define MOTOR_PORT2 6

String TRUE_RFID = "1111";
String receivedRFID;

// // Static IP configuration
static byte myip[] = { 169, 254, 238, 216 }; // Arduino's static IP address
static byte gwip[] = { 169, 254, 238, 215 }; // Gateway IP address

// // MAC address of the Arduino
static byte mymac[] = { 0x74, 0x69, 0x69, 0x2D, 0x30, 0x31 };

// // Ethernet buffer size
byte Ethernet::buffer[700];

// Set a session
static byte session;

// Set a stash for making requests
Stash stash;

// Set the destination server as localhost
const byte serverIP[] = { 169, 254, 238, 215 };
const char DestWebsite[] PROGMEM = "localhost";


void sendRFID(String RFID_in){
  byte sd = stash.create();
  stash.print("{\"rfid\":\"");
  stash.print(RFID_in);
  stash.print("\"}");
  stash.save();
  int stash_size = stash.size();

  // Compose the http POST request, taking the headers below and appending
  // previously created stash in the sd holder.
  Stash::prepare(PSTR("POST http://$F/ HTTP/1.1" "\r\n"
    "Host: $F" "\r\n"
    "Content-Length: $D" "\r\n"
    "\r\n"
    "$H"),
  DestWebsite, DestWebsite, stash_size, sd);

  // send the packet - this also releases all stash buffers once done
  // Save the session ID so we can watch for it in the main loop.
  session = ether.tcpSend();
}

const char* sendAndRecievePacket(String receivedRFID){
  Serial.println("Sending RFID to the server.");
  bool flag = false;
  while(true){
    ether.packetLoop(ether.packetReceive());    
    if(flag == false){
      sendRFID(receivedRFID);
      flag = true;
    }
    const char* reply = ether.tcpReply(session);
      if (reply != nullptr) {
        Serial.println("Got a response!");
        Serial.println(reply);
        return reply;
    }
  }
}

void performOperation(const char* reply){

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
}

void setup() {
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
  // TO ASK
  ether.copyIp(ether.hisip, serverIP); 
}

void loop() {
  if (Serial.available() > 0) {
    // Read the data into the String object
    receivedRFID = Serial.readString();
    Serial.println("");
    Serial.println("Received RFID:" + receivedRFID);

    // Send RFID, and Read the received reply from the QT Access control server (Up on localhost:80)
    const char* reply = sendAndRecievePacket(receivedRFID);
    
    // Open/Close the door and handel LEDs based on the received response from the server 
    performOperation(reply);

    // Clear the String object for the next read
    receivedRFID = "";
  }
}
