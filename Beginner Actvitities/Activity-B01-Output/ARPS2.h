/* =============================================================================
ARPS2.h
February 1, 2026

Board header file for the mirobo.tech ARPS-2 circuit.

This header file defines all of the ARPS-2 I/O devices allowing
beginners to focus on learning programming concepts more quickly.

ARPS-2 hardware notes:
- Pushutton switches use internal pull-up resistors (so pressed == LOW)
- LEDs and motor driver share I/O pins
- Headers H1-H4 are shared between digital I/O, analog I/O, I2C, and SONAR
==============================================================================*/

#ifndef ARPS2_H
#define ARPS2_H

// Pre-defined Arduino UNO LED
// LED_BUILTIN (D13)      // On-board LED (shared with H2 and SONAR TRIG)

/* =====================================
 * Pushbutton Pins (Active LOW)
 * ====================================*/
// NOTE: SW2 and SW3 are fully supported on Arduion UNO R4 Minima and Arduino 
// UNO R4 WiFi. Arduino UNO Rev 3 uses D0 and D1 as serial Rx and Tx lines
// which prevents SW2 and SW3 from operating during programming and debugging.

const uint8_t SW2 = 0;    // UNO R4 only!
const uint8_t SW3 = 1;    // UNO R4 only!
const uint8_t SW4 = 2;
const uint8_t SW5 = 4;


/* =====================================
 * LED Pins
 * ====================================*/
// IMPORTANT: LED pins are shared with the motor controller. Using the
// LEDs while the the motors are active will affect motor behavior!

const uint8_t LED2 = 3;   // M1A
const uint8_t LED3 = 9;   // M1B
const uint8_t LED4 = 10;  // M2A
const uint8_t LED5 = 11;  // M2B


/* =====================================
 * Motor Pins
 * ====================================*/
// ARPS2 motor output pin definitions (redefines same I/O pins as LEDs)

const uint8_t M1A = 3;    // Left motor terminal A
const uint8_t M1B = 9;    // Left motor terminal B
const uint8_t M2A = 10;   // Right motor terminal A
const uint8_t M2B = 11;   // Right motor terminal B


/* =====================================
 * Piezo Beeper Pin
 * ====================================*/

const uint8_t LS1 = 6;    // ARPS2 Piezo beeper LS1


/* =====================================
 * Expansion Header I/O Pins
 * ====================================*/
// NOTE: I/O headers H1 and H4 are shared with analog inputs A4 (AH1) and A5 (AH4).

const uint8_t H1 = 18;    // Header H1 (digital, shared with A4)
const uint8_t H2 = 13;    // Header H2 (shared with SONAR TRIG and onboardLED)
const uint8_t TRIG = 13;  // Ultrasonic SONAR distance sensor TRIG(ger) output
const uint8_t H3 = 12;    // Header H3 (shared with SONAR ECHO)
const uint8_t ECHO = 12;  // Ultrasonic SONAR distance sensor ECHO input
const uint8_t H4 = 19;    // Header H4 (digital, shared with A5)
const uint8_t H9 = 7;     // Digital I/O or Servo output pin
const uint8_t H10 = 8;    // Digital I/O or Servo output pin


/* =====================================
 * Optional IR Demodulator Input Pin
 * ====================================*/

const uint8_t IR = 5;     // Demodulator U3

/* =====================================
 * Analog I/O Pins
 * ====================================*/
// NOTE: Analog inputs AH1 and AH4 are shared with D18, D19, and I2C SDA, SCL

const uint8_t Q1 = A0;    // Left floor/line sensor phototransistor Q1
const uint8_t Q2 = A1;    // Right line sensor phototransistor Q2
const uint8_t Q3 = A1;    // Right floor sensor phototransistor Q3
const uint8_t ATEMP = A2; // Analog temperature sensor U1
const uint8_t ADIV = A3;  // Analog voltage divider (R20/R21) tap
const uint8_t AH1 = A4;   // Analog input from header H1 (shared with D18)
const uint8_t AH4 = A5;   // Analog input from header H4 (shared with D19)

#endif