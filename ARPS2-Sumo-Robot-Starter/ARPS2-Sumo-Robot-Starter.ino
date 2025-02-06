/*
Project:  ARPS2 Sumo Robot Starter Program
Activity: mirobo.tech/arps
Date:     February 5, 2025
 
Use this starter program template to create a simple Sumo robot using
an Arduino UNO and ARPS2 circuit board. Follow the instructions in
the program comments to test your motors and optical floor sensors,
and then expand your program to complete your Sumo robot!

This program was designed for making simple classroom Sumo robots
that compete within the Robo-Sumo rules on the mirobo.tech website
(https://mirobo.tech/robo-sumo-rules) and can easily be adapted for
making other kinds of beginner robots.

See the https://mirobo.tech/arps webpage for additional ARPS2
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

// Define mode constants
const int TEST = 0;     // Test mode
const int STARTUP = 1;  // Startup delay
const int SEARCH = 2;   // Search mode
const int ATTACK = 3;   // Attack mode

// Define program variables
int SW2state;           // Button state variables
int SW3state;
int SW4state;
int SW5state;
int mode;               // Program operating mode
int floorLeft;          // Left floor sensor value
int floorRight;         // Right floor sensor value
int range;              // SONAR range

void setup() {
  Serial.begin(9600);   // Start the serial port at 9600 bps for UNO R3
  // Serial.begin(115200); // Start the serial port at 115200 bps for UNO R4

  // Initialize I/O pin directions/types
  pinMode(SW2, INPUT_PULLUP);
  pinMode(SW3, INPUT_PULLUP);
  pinMode(SW4, INPUT_PULLUP);
  pinMode(SW5, INPUT_PULLUP);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);
  pinMode(BEEPER, OUTPUT);
  pinMode(IR, INPUT);
  pinMode(H1, OUTPUT);
  pinMode(ECHO, INPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(H4, OUTPUT);
  tone(BEEPER, 4000, 100);  // Say hello and print startup messages
  Serial.println(F("Starting ARPS2 Sumo!"));

  // Start the robot...
  stop();               // Start with the motors off
  mode = TEST;          // Test mode for sensors and motors
  // mode = STARTUP;       // Sumo pre-fight startup mode
}

// Main loop code repeats forever
void loop() {
  // while() loops within loop() are like mini self-contained loop()s.
  // Use the mode variable to switch program operating modes.

  while(mode == TEST) {
    // Test the motors, optical sensors, and SONAR module here:

    // Motor test - verify the operation of each motor direction.
    goFwd();
    delay(1000);
    stop();
    
    // Floor sensor test - record light and dark values from each
    // floor sensor, and use thse to determine threshold values to
    // keep your robot in the Sumo ring.
    floorLeft = analogRead(Q1);	// Left phototransistor (Q1) light level
    Serial.print("Q1 Level: ");
    Serial.println(floorLeft);
    
    // Add your own SONAR distance sensor test code here:
    
    delay(1000);
  }
  
  while(mode == STARTUP) {
    // STARTUP mode - waits for a button to be pressed and then
    // starts a five second delay before switching to SEARCH mode.
    
    mode = SEARCH;
  }

  while(mode == SEARCH) {
    // SEARCH mode - use the SONAR module to search for an opponent
    // within the Sumo ring, and use the floor sensors to remain in
    // the ring. If an opponent is found, switch to ATTACK mode.

  }

  while(mode == ATTACK) {
    // ATTACK mode - push the opponent out of the Sumo ring while
    // using the floor sensors to remain in the ring. If an opponent
    // is no longer detected, switch back to SEARCH mode.

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

// Add other functions for your robot here. It will need a SONAR
// function to detect an opposing robot, and could implement floor
// sensing functions to shorten the program code in the SEARCH and
// ATTACK functions. If the IR demodulator is installed in your
// robot, an IR remote transmitter can be used to start the bout.
