#define RED_DIODE_PORT 13
#define GREEN_DIODE_PORT 12
#define MOTOR_PORT1 7
#define MOTOR_PORT2 6

String TRUE_RFID = "1111";
String receivedRFID;

void setup() {
  // put your setup code here, to run once:

  // Initializing ports
  pinMode(RED_DIODE_PORT, OUTPUT);
  pinMode(GREEN_DIODE_PORT, OUTPUT);
  pinMode(MOTOR_PORT1, OUTPUT);
  pinMode(MOTOR_PORT2, OUTPUT);

  // Setting up a serial communication to communicate with a virtual terminal
  Serial.begin(9600);
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
