/* ================================================================================
Beginner Activity 4 Project: LED Level Indicator [B04_Level_Indicator_Project]
Version: 1.2
Updated: September 7, 2026

Platform: mirobo.tech ARPS-2 circuit (any configuration)
Requires: ARPS2.h header file
=================================================================================*/
// IMPORTANT: Make sure that ARPS2.h is accessible as a tab in this project.
#include "ARPS2.h"      // Define ARPS-2 I/O devices

/* ================================================================================
LED Level Indicator
================================================================================

Many real devices - volume controls, brightness dials, battery
gauges - show a level using a row of lights instead of a number.
This project lets you build one, using ARPS-2's four LEDs as a
simple bar graph.

This project uses SW4 to increase and SW5 to decrease the level,
rather than SW2 and SW3, since SW4 and SW5 remain fully compatible
with Arduino UNO Rev 3.

Your level indicator should:

 - Use SW4 to increase and SW5 to decrease a 'level' variable
   ranging from 0 to 4.
 - Use edge-detection - the same _pressed/_last pattern from this
   activity's EA 2 - so holding a button changes the level once per
   press, not repeatedly while it's held.
 - Keep the level from going below 0 or above 4.
 - Light LEDs cumulatively to show the current level as a bar graph:
   level 1 lights LED2 only, level 2 lights LED2 and LED3, and so on
   up to level 4, which lights all four LEDs. Level 0 lights none.

Stretch goals:

 - Add a SCALE value to represent an alternate range - a volume
   level from 0 to 20 in steps of 5, or a battery percentage from
   0 to 100 in steps of 25, for example - without changing anything
   else in your program. (See the hints in the TODOs.)
 - Play a short tone each time the level changes, using a different
   frequency for each level.
 - Briefly blink the top or bottom LED when a button press would
   push the level past its minimum or maximum, to signal the limit
   was reached.
*/

// ---- Program Constants ----------------
const int LEVEL_MIN = 0;
const int LEVEL_MAX = 4;
// TODO: Define a named SCALE constant for converting level into a
// displayed value.

// ---- Program Variables ----------------
int level = 0;
bool up_pressed = false;
bool up_last = false;
bool down_pressed = false;
bool down_last = false;

void setup()
{
    // Configure Arduino UNO status LED as output
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);    // Status LED on

    // Configure ARPS-2 LEDs as outputs
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    pinMode(LED5, OUTPUT);

    // Configure ARPS-2 pushbuttons as inputs
    // NOTE: Arduino UNO Rev 3 users should comment out the SW2 and SW3
    // lines below, since these pins are shared with the USB serial port.
    pinMode(SW2, INPUT_PULLUP);
    pinMode(SW3, INPUT_PULLUP);
    pinMode(SW4, INPUT_PULLUP);
    pinMode(SW5, INPUT_PULLUP);

    Serial.begin(9600);                 // Initialize serial output for debugging
}

void loop()
{
    up_pressed = (digitalRead(SW4) == LOW);
    down_pressed = (digitalRead(SW5) == LOW);

    // TODO: Increase level by 1 when up_pressed is true and up_last
    // was false - but only if level is below LEVEL_MAX. Update
    // up_last, and Serial.print() the new displayed value
    // (level * SCALE).

    // TODO: Decrease level by 1 when down_pressed is true and
    // down_last was false - but only if level is above LEVEL_MIN.
    // Update down_last, and Serial.print() the new displayed value
    // (level * SCALE).

    // TODO: Light LED2 through LED5 based on the current level, using
    // four independent if conditions (not else if) so each LED lights
    // when level is at least as high as its position in the bar graph.

    delay(20);  // Short delay for button debouncing
}
