/*
Project:  ARPS2-SONAR-Starter
Date:     March 8, 2025

Robot starter program to develop an HC-SR04 SONAR range function.

This program is configured for Arduino UNO R4 Minima and UNO R4 Wifi
by default, and also works with Arduino UNO Rev 3. When using Arduino
UNO Rev 3, edit the Serial.begin() function to select a 9600 bps rate
in the program's setup() function, and note that SW2 and SW3 will be
inoperable since these pushbuttons share share the UART RX and TX I/O
pins used for USB communication.

See the https://mirobo.tech/arps webpage for additional ARPS-2
resources, programming activities, and starter programs.
*/

// Pre-defined Arduino UNO LED
// LED_BUILTIN (D13)    // On-board LED (shared with H2 and SONAR TRIG)

// ARPS-2 Educational Starter I/O devices
const int SW2 = 0;      // Pushbuttons SW2 and SW3 are fully supported on
const int SW3 = 1;      // Arduino UNO R4 Minima and Arduino UNO R4 WiFi
const int SW4 = 2;      // Pushbuttons SW4 and SW5 work on all Arduino UNO
const int SW5 = 4;      // circuit boards

const int LED2 = 3;     // ARPS-2 LEDs (shared with motor outputs)
const int LED3 = 9;
const int LED4 = 10;
const int LED5 = 11;

const int BEEPER = 6;   // ARPS-2 Piezo beeper LS1

// ARPS2 optional analog input devices
const int Q1 = A0;      // Left floor/line sensor phototransistor Q1
const int Q2 = A1;      // Right line sensor phototransistor Q2
const int Q3 = A1;      // Right floor sensor phototransistor Q3
const int ATEMP = A2;   // Analog temperature sensor U1
const int ADIV = A3;    // Analog voltage divider (R20/R21) tap

// ARPS-2 motor output pin definitions (redefines same I/O pins as LEDs)
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

// Define program variables
int SW2State;           // Pushbutton states
int SW3State;
int SW4State;
int SW5State;
int distance;           // SONAR distance to objects in cm

int rawQ1;              // Analog input levels
int rawQ3;
int rawTemp;
int rawVolts;
float volts;

void setup() {
  // Serial.begin(9600);   // Start the serial port at 9600 bps for UNO Rev 3
  Serial.begin(115200); // Start the serial port at 115200 bps for UNO R4

  // Initialize I/O pin directions/types
  pinMode(SW2,INPUT_PULLUP);
  pinMode(SW3,INPUT_PULLUP);
  pinMode(SW4,INPUT_PULLUP);
  pinMode(SW5,INPUT_PULLUP);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);
  pinMode(LED5,OUTPUT);
  pinMode(BEEPER,OUTPUT);
  pinMode(H1,INPUT);
  pinMode(ECHO,INPUT);
  pinMode(TRIG,OUTPUT);
  pinMode(H4,INPUT);
  tone(BEEPER,4000,100);  // Say hello
}

void loop() {
  SW2State = digitalRead(SW2);  // SW2 only works with UNO R4 family
  SW3State = digitalRead(SW3);  // SW3 only works with UNO R4 family
  SW4State = digitalRead(SW4);
  SW5State = digitalRead(SW5);

  if(SW2State == LOW) {         // Run motors forward (SW2 only works with UNO R4 family)
    goFwd();
  }

  if(SW3State == LOW) {         // Turn left (SW3 only works with UNO R4 family)
    goLeft();
  }

  if(SW4State == LOW) {         // Turn right
    goRight();
  }

  if(SW5State == LOW) {         // Stop
    stop();
  }

  rawQ1 = analogRead(Q1);
  rawQ3 = analogRead(Q3);
  Serial.print("Q1 Level: ");
  Serial.print(rawQ1);
  Serial.print("\t");
  Serial.print("Q3 Level: ");
  Serial.println(rawQ3);

  rawVolts = analogRead(ADIV);
  volts = float(rawVolts) / 40;
  Serial.print("Volts: ");
  Serial.print(volts);
  Serial.println("V");

  distance = sonarRange(20);
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println("cm");
  Serial.println("");

  delay(1000);
}

int sonarRange(unsigned long max) {
  // Make a TRIG pulse to start distance measurement

  // Wait for ECHO to go high and time the ECHO pulse 

  // Convert ECHO duration to distance, and return distance value

}

// Stop both motors
void stop(void) {
  digitalWrite(M1A,LOW);
  digitalWrite(M1B,LOW);
  digitalWrite(M2A,LOW);
  digitalWrite(M2B,LOW);
}

// Go forward (run motors in opposite directions)
void goFwd(void) {
  digitalWrite(M1A,LOW);
  digitalWrite(M1B,HIGH);
  digitalWrite(M2A,HIGH);
  digitalWrite(M2B,LOW);
}

void goLeft(void) {
  // Add digitalWrite statements to run the motors here
}

void goRight(void) {
  // Add digitalWrite statements to run the motors here
}

void goRev(void) {
  digitalWrite(M1A,HIGH);
  digitalWrite(M1B,LOW);
  digitalWrite(M2A,LOW);
  digitalWrite(M2B,HIGH);
}


/*
The completed sonarRange function is included below:

// Returns SONAR range to closest target within the maximum distance.
// Example usage:  distance = sonarRange(50);
int sonarRange(unsigned long max) {
  // Check for previous ping in progress
  if(digitalRead(ECHO) == HIGH) {
    return -1;      // ECHO in progress. Return error code
  }

  // Make a TRIG pulse to start a distance measurement
  digitalWrite(TRIG,HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG,LOW);

  // Wait for ECHO to go high and time the ECHO pulse 
  unsigned long duration = pulseIn(ECHO,HIGH,(max+1)*58+200);

  // Return 0 if ECHO times out (no target within range)
  if(duration == 0) {
    return 0;
  }

  // Convert ECHO duration to distance, and return distance value
  return(int(duration/58));
}

*/
