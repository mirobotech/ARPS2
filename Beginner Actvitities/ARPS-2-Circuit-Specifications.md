# ARPS-2 Circuit Specifications

# ARPS-2 Circuit Introduction

ARPS stands for Arduino-compatible Robot Project Shield and -2 refers to it being the second major version of this board. 

ARPS-2 is a beginner Arduino UNO circuit shield designed for teaching Arduino programming and robotics using the Arduino IDE. ARPS-2 is designed to work best with Arduino UNO R4 Minima, and is also supported by Arduino UNO R4 WiFi, and Arduino UNO Rev 3. (Pushbuttons SW2 and SW3 will be inoperative during programming and debugging when using Arduino UNO Rev 3 as these pushbuttons share the Arduino UNO Rev 3 serial Tx/Rx lines used by the USB port).

# ARPS-2 Circuit Hardware

ARPS-2 includes the following on-board hardware devices:
- reset pushbutton (SW1)
- four user pushbuttons (SW2 - SW5)
- power LED (LED1)
- four user LEDs (LED2 - LED5)
- piezo buzzer (LS1)
- MCP9700A or TMP-36 analog temperature sensor (U1)
- TSOP38238 38kHz IR demodulator (U3)
- SH754410NE motor driver (U2) capable of driving two DC motors in forward and reverse (or one bi-polar stepper motor)
- two break-away optical sensor modules containing IR LEDs and phototransistor (the left module uses one LED (LED6) and either one phototransistor (Q1) to act as a robot floor sensor, or two phototransistors (Q1, Q2) to act as a single line sensor, and the right module uses one LED (LED7) and one phototransistor (Q3) to act as a matching floor sensor)
- an expansion header consisting of four parallel 3-pin headers (H1 - H4) allowing an optional HC-SR04 SONAR distance sensor module to be mounted on-board
- resistor voltage divider circuit (to sense battery input voltage)
- two digital I/O or servo expansion headers (H9, H10)

## ARPS-2 Arduino UNO GPIO Pin Mapping

RESET - Pushbutton SW1 input
D0 - pushbutton SW2 input using internal pull-up
D1 - pushbutton SW3 input using internal pull-up
D2 - pushbutton SW4 input using internal pull-up
D3 - LED D2 output, shared with motor driver 1A input (motor driver 1Y output becomes M1A - motor 1 (left motor), terminal A)
D4 - pushbutton SW5 input using internal pull-up
D5 - IR demodulator U3 input
D6 - piezo buzzer LS1 output
D7 - servo/digital I/O header H9
D8 - servo/digital I/O header H10
D9 - LED D3 output, shared with motor driver 2A input (motor driver 2Y output becomes M1B - motor 1 (left motor), terminal B)
D10 - LED D4 output, shared with motor driver 3A input (motor driver 3Y output become M2A - motor 2 (right motor), terminal A)
D11 - LED D5 output, shared with motor driver 4A input (motor driver 4Y output becomes M2B - motor 2 (right motor), terminal B)
D12 - header H3, shared as SONAR module ECHO pin input
D13 - header H2, shared as SONAR module TRIG pin output
A0 - left floor/line phototransistor Q1 input
A1 - right line phototransistor Q2 input, shared as right floor phototransistor Q3 input
A2 - analog temperature sensor U1 input
A3 - battery voltage divider circuit input
A4/D18 - I/O expansion header H1
A5/D19 - I/O expansion header H4

# ARPS-2 Component Placement

The ARPS-2 circuit is designed to be the same shape and form factor as an Arduino UNO, but with its break-away optical floor sensor modules protruding from the side opposite the Arduino UNO's power and USB connectors.

The layout and markings on ARPS-2 suggest it should be held as a vertical rectangle, with the break-away optical sensor modules at the top, and the battery and motor screw terminal strip at the bottom (above the Arduino UNO's power connector).

## LED arrangement

The four LEDs are arranged in a vertical line from top (D2) to bottom (D5), left of the board centreline and slightly below the pushbuttons.

## Pushbutton arrangement and labels

Pushbuttons SW2-SW5 are arranged in a diamond pattern to the right of the LEDs and the board centreline, at the following positions and with the associated silkscreen labels:

- SW2 (top), labelled with a circle
- SW3 (left), labelled with a left-facing triangle
- SW4 (right), labelled with a right-facing triangle
- SW5 (bottom), labelled with a square

