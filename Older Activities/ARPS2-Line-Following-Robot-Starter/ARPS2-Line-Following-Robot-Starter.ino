/*
Project:  Line Following Robot Starter
Activity: mirobo.tech/arps
Date:     February 5, 2024
 
Use this starter program template with the mirobo.tech ARPS2 circuit
and an Arduino UNO to create a simple line-following robot. Follow
the instructions in the program comments to test your motors and
floor sensors, and then complete your robot program!

See the https://mirobo.tech/arps webpage for additional ARPS
resources, programming activities, and starter programs.
*/

// Pre-defined Arduino UNO LED
// LED_BUILTIN (D13)    // On-board LED (shared with H2 and SONAR TRIG)

// ARPS2 Educational Starter I/O devices
const int SW2 = 0;      // Pushbuttons SW2 and SW3 are fully supported on
const int SW3 = 1;      // Arduino UNO R4 Minima and Arduino UNO R4 WiFi
const int SW4 = 2;      // Pushbuttons SW4 and SW5 work on all Arduino UNO
const int SW5 = 4;      // circuit boards

const int LED2 = 3;     // ARPS2 LEDs (shared with motor outputs)
const int LED3 = 9;
const int LED4 = 10;
const int LED5 = 11;

const int BEEPER = 6;   // ARPS2 Piezo beeper LS1

// ARPS2 optional analog input devices
const int Q1 = A0;      // Left floor/line sensor phototransistor Q1
const int Q2 = A1;      // Right line sensor phototransistor Q2
const int Q3 = A1;      // Right floor sensor phototransistor Q3
const int ATEMP = A2;   // Analog temperature sensor U1
const int ADIV = A3;    // Analog voltage divider (R20/R21) tap
const int AH1 = A4;     // Analog input from header H1 (shared with D18)
const int AH4 = A5;     // Analog input from header H4 (shared with D19)

// ARPS2 motor output pin definitions (redefines same I/O pins as LEDs)
const int M1A = 3;      // Motor 1 - output A
const int M1B = 9;      // Motor 1 - output B
const int M2A = 10;     // Motor 2 - output A
const int M2B = 11;     // Motor 2 - output B

// ARPS2 optional special purpose I/O pins
const int IR = 5;       // IR demodulator
const int H1 = 18;      // Header H1
const int H2 = 13;      // Header H2 (shared with SONAR TRIG and onboardLED)
const int TRIG = 13;    // Ultrasonic SONAR distance sensor TRIG(ger) output
const int H3 = 12;      // Header H3 (shared with SONAR ECHO)
const int ECHO = 12;    // Ultrasonic SONAR distance sensor ECHO input
const int H4 = 19;      // Header H4
const int H9 = 7;       // Digital I/O or Servo output pin
const int H10 = 8;      // Digital I/O or Servo output pin

// Define program constants
const int dark = 400;   // Set dark threshold for line
const int light = 200;  // Set light threshold for floor

// Define program variables
int SW2State;           // Button state variables
int SW3State;
int SW4State;
int SW5State;
int lineLeft;           // Left line sensor (Q1) raw light level
int lineRight;          // Right line sensor (Q2) raw light level

// Setup code runs once to configure I/O pins before running main loop
void setup() {
  Serial.begin(9600);   // Start the serial port at 9600 bps for UNO R3
  // Serial.begin(115200); // Start the serial port at 115200 bps for UNO R4

  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  pinMode(SW4, INPUT_PULLUP);
  pinMode(SW5, INPUT_PULLUP);
  pinMode(LED2, OUTPUT); // LEDs and motors share the same pins
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(BEEPER, OUTPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(IR, INPUT);
  pinMode(Q1, INPUT);
  pinMode(Q2, INPUT);   // Q2 and Q3 share the same pin
  pinMode(H1, INPUT);   // Define unused header pins as input
  pinMode(TRIG, OUTPUT);// H2/SONAR TRIG output (if installed)
  pinMode(ECHO, INPUT); // H3/SONAR ECHO input (if installed)
  pinMode(H4, INPUT);	  // Define unused header pins as input
  pinMode(ATEMP, INPUT);
  pinMode(ADIV, INPUT);

  // Set robot outputs and make a beep
  stop();               // Call stop function to keep motors off
  Serial.println(F("Starting ARPS2"));
  Serial.println(F("Press SW5 to start:"));
  tone(BEEPER, 4000, 100);  // Say hello!

  while(digitalRead(SW5) == HIGH);  // Wait for button SW5 to be pressed
}

// Main loop code repeats forever
void loop() {
  // Read both line sensors and store their values in variables
  lineLeft = analogRead(Q1);
  lineRight = analogRead(Q3);

  // View line sensor values in the Serial Monitor for testing.
  // Position your robot over the floor and line and use these
  // values to set the 'dark' and 'light' constants in the program.
  Serial.print("Left: ");
  Serial.println(lineLeft);
  Serial.print("Right: ");
  Serial.println(lineRight);
  Serial.println("");
  delay(250);           // Comment out the Serial.print() and delay()
  // statements after testing the light sensors to enable your robot
  // to quickly read the floor sensors and decide which way to go.

  // Compare the sensor values with the light and dark thresholds
  // to decide which way to go.
  if(lineLeft > dark && lineRight > dark) {
    goFwd();            // If both sensors see the line go forward
  }
  else if(lineLeft > dark && lineRight < light) {
    // Decide which way to go if the left sensor sees the line
  }
  else if(lineLeft < light && lineRight > dark) {
    // Decide which way to go if the right sensor sees the line
   }
  else {
    // Decide what to do if neither sensor sees the line
  }
}

/*
Sample motor functions. Create additional functions for every
direction that your robot needs to drive in. Can you come up with
eight different directions? Does your robot need to use all eight?
Creating all of the motor direction functions ahead of time will
let you easily adapt your robot for other tasks later.
*/

// Run motors in opposite directions to make the robot go forward.
// (Assumes both motors are wired to ARPS the same way.)
void goFwd(void) {
  digitalWrite(M1A, HIGH);      // Left motor rotates CCW
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);       // Right motor rotates CW
  digitalWrite(M2B, HIGH);
}

// Stop both motors
void stop(void) {
  digitalWrite(M1A, LOW);     // Left motor
  digitalWrite(M1B, LOW);
  digitalWrite(M2A, LOW);     // Right motor
  digitalWrite(M2B, LOW);
}
