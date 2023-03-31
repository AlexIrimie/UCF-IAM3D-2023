#include <IBusBM.h>
#include <Servo.h>
 
//--------------------PWM PIN DESIGNATIONS-------------------//
//PWM Pins on the mega: (2-13, 44-46)
//Rudder
int rudderPin = 2;
//Claw
int clawPin = 3;
// Motor A pins (Inflation, 2 motors)
int inflatorPin = 4;
//Motor D pins (Thrust)
int thrustPin = 5;
// Motor B pins (Left)
int leftPin = 6;
// Motor C pins (Right)
int rightPin = 7;
//------------------------------------------------------------//

// Create Servo, Motor, Ibus Objects
IBusBM ibus;
Servo rudder;
Servo claw;
Servo inflator;
Servo thrust;
Servo left;
Servo right;
 
// Read the number of a given channel and convert to the range provided.
// If the channel is off, return the default value
int readChannel(byte channelInput, int minLimit, int maxLimit, int defaultValue) {
  uint16_t ch = ibus.readChannel(channelInput);
  if (ch < 100) return defaultValue;
  return map(ch, 1000, 2000, minLimit, maxLimit);
}
 
// Read the channel and return a boolean value
bool readSwitch(byte channelInput, bool defaultValue) {
  int intDefaultValue = (defaultValue) ? 100 : 0;
  int ch = readChannel(channelInput, 0, 100, intDefaultValue);
  return (ch > 50);
}
 
void setup() {
  // Start serial monitor
  Serial.begin(115200);
  // Attach iBus object to serial port
  ibus.begin(Serial);

  //Attach servos and Electronic Speed Controllers (ESCs)
  rudder.attach(rudderPin);                //Attach the rudder servo to pin 2 digital
  claw.attach(clawPin);                    //Attach the claw servo to pin 3 digital
  inflator.attach(inflatorPin,1000,2000);  //Attach the inflator motor to pin 4 digital
  thrust.attach(thrustPin,1000,2000);      //Attach the thrust motor to pin 5 digital
  left.attach(leftPin,1000,2000);          //Attach the left motor to pin 6 digital
  right.attach(rightPin,1000,2000);        //Attach the right motor to pin 7 digital

  //Set initial throttle input for the ESCs to 0
  inflator.write(0);
  thrust.write(0);
  left.write(0);
  right.write(0);

  //This delay is very important. the ESCs must have 0 throttle input for at least 1 second
  delay(1000); 
}
 
void loop() {

  // Cycle through first 5 channels and determine values
  // Print values to serial monitor
  // Note IBusBM library labels channels starting with "0"
 
  for (byte i = 0; i < 5; i++) {
    int value = readChannel(i, -100, 100, 0);
    Serial.print("Ch");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.print(value);
    Serial.print(" | ");
  }
 
  // Print channel 6 (switch) boolean value
  Serial.print("Ch6: ");
  Serial.print(readSwitch(5, false));
  Serial.println();
  

  //-------THIS IS THE CODE TO EDIT TO ADD/MODIFY THE SERVO AND MOTOR VALUES--------------------------
  rudder.write(readChannel(0,0,180,0));  //This is the left stick lef/right
  claw.write(readChannel(5,0,180,0));   //This is the top right dial (channel 6)
  inflator.write(readChannel(4,0,180,0));
  thrust.write(readChannel(3,0,180,0));
  left.write(readChannel(2,0,180,0));
  right.write(readChannel(1,0,180,0));

  delay(10);
}
/*
void inflation() {
	// Turn on motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, HIGH);

	analogWrite(enA, readChannel(4,0,100,0)); //channel 5
	
	// Now turn off motors
	digitalWrite(in1, LOW);
	digitalWrite(in2, LOW);
}
void direction() {
	// Turn on motors
	digitalWrite(in3, LOW);
	digitalWrite(in4, HIGH);
  digitalWrite(in5, LOW);
	digitalWrite(in6, HIGH);

	analogWrite(enB, readChannel(3,-100,100,0)); //channel 4
	analogWrite(enC, readChannel(3,-100,100,0)); //channel 4
	
	// Now turn off motors
	digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
  digitalWrite(in3, LOW);
	digitalWrite(in4, LOW);
}
void speed() {
	// Turn on motors
	digitalWrite(in7, LOW);
	digitalWrite(in8, HIGH);

	analogWrite(enD, readChannel(1,-100,100,0)); //channel 2
	
	// Now turn off motors
	digitalWrite(in7, LOW);
	digitalWrite(in8, LOW);
}
*/