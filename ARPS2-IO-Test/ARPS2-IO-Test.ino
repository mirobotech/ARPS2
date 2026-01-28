/*
Project:  ARPS2-IO-Test
Date:     February 13, 2025

Functional test of all on-board ARPS2 I/O devices.

This program is configured for Arduino UNO R4 Minima and UNO R4 Wifi
by default, and also works on Arduino UNO R3. When used with Arduino
UNO R4 circuits, Servo library v1.2.2 (or later) by Michael Margolis/
Arduino is required (it may need to be installed manually from the
Library Manager if uploading the program to the Arduino generates a
servo library error message).

When running this program on Arduino UNO R3, edit the Serial.begin()
function to select a 9600 bps rate in the program's setup() function,
and note that SW2 and SW3 will be inoperable as their I/O pins share
the UART RX and TX pins used for USB communication on Arduino UNO R3.

To verify the operation of the optional voltage divider circuit,
connect an external 6-12V power supply to the battery terminals of
screw terminal header CON1.

This program also includes a SONAR range function designed to verify
the operation of an optional 5V HC-SR04 or 3.3V-capable HC-SR04P 
ultrasonic distance sensor module installed in the optional H1-H4
header socket. Install the distance sensor module before powering
the ARPS2 circuit or connecting it to your computer.

See the https://mirobo.tech/arps webpage for additional ARPS2
resources, programming activities, and starter programs.
*/

#include <Servo.h>
Servo H9SERVO;          // Create servo objects for on-board headers
Servo H10SERVO;

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

// Define program variables
int SW2State;           // Pushbutton states
int SW3State;
int SW4State;
int SW5State;

int rawTemp;            // Analog input levels
int rawVolts;
int rawQ1;
int rawQ2;
float volts;

bool servoMax = false;  // Servo output position flag
int loopCount = 100;    // Analog/SONAR update loop counter

int range;              // SONAR range result

void setup() {
  // Serial.begin(9600);   // Start the serial port at 9600 bps for UNO R3
  Serial.begin(115200); // Start the serial port at 115200 bps for UNO R4

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
  H9SERVO.attach(H9, 1000, 2000);   // Set timing for 90° servos
  H10SERVO.attach(H10, 1000, 2000);
  H9SERVO.write(90);                // Set servos to mid-point
  H10SERVO.write(90);
  tone(BEEPER, 4000, 100);  // Say hello and print startup messages

  Serial.println(F("Starting ARPS2"));
  Serial.println(F("SW2 - light LED2"));
  Serial.println(F("SW3 - light LED3"));
  Serial.println(F("SW4 - LED sequence and tones"));
  Serial.println(F("SW5 - Monitor IR Input"));
  delay(1000);
}

void loop() {
  SW2State = digitalRead(SW2);  // Only usable on UNO R4 family
  SW3State = digitalRead(SW3);  // Only usable on UNO R4 family
  SW4State = digitalRead(SW4);
  SW5State = digitalRead(SW5);

  if(SW2State == LOW) {         // Only usable on UNO R4 family
    digitalWrite(LED2, HIGH);
  }
  else {
    digitalWrite(LED2, LOW);
  }

  if(SW3State == LOW) {         // Only usable on UNO R4 family
    digitalWrite(LED3, HIGH);   // LED3 will blink on UNO R3
  }
  else {
    digitalWrite(LED3, LOW);
  }

  // Sequence LEDs while playing tones
  if(SW4State == LOW) {
    digitalWrite(LED2, HIGH);
    tone(BEEPER, 523);
    delay(100);
    digitalWrite(LED3, HIGH);
    tone(BEEPER, 659);
    delay(100);
    digitalWrite(LED4, HIGH);
    tone(BEEPER, 784);
    delay(100);
    digitalWrite(LED5, HIGH);
    tone(BEEPER, 1047);
    delay(100);

    digitalWrite(LED2, LOW);
    tone(BEEPER, 2093);
    delay(100);
    digitalWrite(LED3, LOW);
    tone(BEEPER, 4186);
    delay(100);
    digitalWrite(LED4, LOW);
    tone(BEEPER, 8372);
    delay(100);
    digitalWrite(LED5, LOW);
    noTone(BEEPER);
    delay(100);
  }

  // Monitor IR pulses from U3 using LED2
  if(SW5State == LOW) {
    if(digitalRead(IR) == LOW) {
      digitalWrite(LED5, HIGH);
    }
    else {
      digitalWrite(LED5, LOW);
    }
  }

  // Update distance and analog measurements, and alternate servo
  // output approximately every second.
  if(loopCount == 0) {
    loopCount = 100;

    // Get SONAR range
    range = sonarRange(50);    // Look for a target within 50cm
    Serial.print("Range: ");
    Serial.print(range);
    Serial.println("cm");

    // Read analog input levels
    rawQ1 = analogRead(Q1);
    rawQ2 = analogRead(Q2);
    rawTemp = analogRead(ATEMP);
    rawVolts = analogRead(ADIV);
    volts = float(rawVolts) / 40;

    Serial.print("Q1 value: ");
    Serial.println(rawQ1);
    Serial.print("Q2 value: ");
    Serial.println(rawQ2);
    Serial.print("Raw temp: ");
    Serial.println(rawTemp);
    Serial.print("Battery: ");
    Serial.print(volts);
    Serial.println("V");
    Serial.println();

    // Alternate servo positions
    if(servoMax == true) {
      H9SERVO.write(180);
      H10SERVO.write(180);
      servoMax = false;
    }
    else {
      H9SERVO.write(0);
      H10SERVO.write(0);
      servoMax = true;
    }
  }

  loopCount -= 1;

  delay(10);
}

/*
SONAR ranging function with maximum range limit and error checking.
Returns the range to the closest target in cm. The 'max' parameter
(cm) limits the maximum search range. Returns either: -1 if a previous
ranging operation is still in progress; 0 if no target is found within
the max range; or the range to closest target in cm. Example use:
range = sonarRange(100);  // Find closest target within 100 cm
*/
int sonarRange(unsigned long max) {
  if(digitalRead(ECHO) == HIGH) {
    return -1;                // ECHO in progress. Return error code.
  }
  digitalWrite(TRIG, HIGH);   // All clear? Trigger a new SONAR ping.
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);
  // Time the ECHO pulse duration (includes TRIG setup and transmit
  // time suitable for most 5V HC-SR04 SONAR modules).
  // unsigned long duration = pulseIn(ECHO, HIGH, max * 58 + 260);

  // Note: some 3.3V-capable HC-SR04P modules may have much longer
  // TRIG setup times than 5V HC-SR04 modules. Comment out the line
  // above and use the line below when using a HC-SR04P module.

  // Time the ECHO pulse duration for HC-SR04P SONAR modules
  unsigned long duration = pulseIn(ECHO, HIGH, max * 58 + 2320);

  if(duration == 0) {
    return 0;                 // Return 0 if no target is within max range
  }
  return(int(duration / 58));	// Return distance to target in cm
}
