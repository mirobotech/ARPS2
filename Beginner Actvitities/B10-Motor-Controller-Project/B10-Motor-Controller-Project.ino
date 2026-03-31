/* ================================================================================
Project: Motor Controller [B10-Motor-Controller-Project]
March 31, 2026

Platform: mirobo.tech ARPS-2 circuit (robot configuration)
Requires: ARPS2.h header file

Before starting this project, re-read GE1, GE2, GE3,
and GE4 from Activity 10: Analog Output.

IMPORTANT HARDWARE NOTES:
  The H-bridge motor driver outputs share pins with LED2-LED5.
  Using this project will prevent LED2-LED5 from being used as
  status indicators.

  Motor wiring (standard robot configuration):
    M1A (LED2) / M1B (LED3) - Left motor
    M2A (LED4) / M2B (LED5) - Right motor

  Motor power must be connected separately from logic power.
  Always test motor direction at low speed (25%) before increasing
  to full speed. If a motor runs the wrong direction, swap its
  two wires at the motor terminal.

  NOTE: Arduino's analogWrite() uses a fixed hardware timer
  frequency that may not be ideal for all motor driver chips.
  Check the driver chip datasheet for its recommended PWM
  frequency range - see GE3 in Activity 10.

  Speed control input: SW4 and SW5 step speed up and down.
  For analog speed control, connect a potentiometer to header
  H1 or H4 - see pot_to_speed() and Extension Activity 1.

Controls:
  SW2       - enable/disable drive (safety enable switch)
  SW4       - decrease speed of both motors
  SW5       - increase speed of both motors
================================================================================*/
// IMPORTANT: Make sure that ARPS2.h is accessible as a tab in this project.
#include "ARPS2.h"

// ---- Program Constants ---------------
const int STEP_DELAY = 20;            // Main loop delay (ms)
const int DEADBAND   = 5;             // Speed % below which motor is stopped
const int MAX_SPEED  = 100;           // Maximum speed percentage
const int SPEED_STEP = 5;             // Speed change per button press

// ---- Program Variables ---------------
bool drive_enabled = false;           // Safety enable flag
int  target_speed  = 0;               // Current speed for both motors (0-100%)


// ---- Program Functions ---------------

void left_motor(int speed)
{
    // Drive the left motor at the given speed (-100 to 100 percent).
    // Positive speed drives forward (M1A active, M1B LOW).
    // Negative speed drives reverse (M1B active, M1A LOW).
    // Zero or within deadband stops the motor (both outputs LOW).
    speed = constrain(speed, -MAX_SPEED, MAX_SPEED);
    if (abs(speed) < DEADBAND)
    {
        analogWrite(LED2, 0);
        analogWrite(LED3, 0);
        return;
    }
    int pwm = map(abs(speed), 0, 100, 0, 255);
    if (speed > 0)
    {
        analogWrite(LED2, pwm);
        analogWrite(LED3, 0);
    }
    else
    {
        analogWrite(LED2, 0);
        analogWrite(LED3, pwm);
    }
}

void right_motor(int speed)
{
    // Drive the right motor at the given speed (-100 to 100 percent).
    // Positive speed drives forward (M2B active, M2A LOW).
    // Negative speed drives reverse (M2A active, M2B LOW).
    // TODO: implement using LED4 and LED5, following left_motor() above
}

void motors(int left_speed, int right_speed)
{
    // Drive both motors at the given speeds.
    // TODO: call left_motor() and right_motor() with their respective speeds
}

int pot_to_speed(int pot_value)
{
    // Convert a potentiometer reading (0-1023) to a motor speed (-100 to 100).
    // The centre of the pot range maps to 0 (stopped).
    // Values within DEADBAND percent of centre return 0.
    // Connect an external pot to H1 or H4 and call this in loop():
    //   target_speed = pot_to_speed(analogRead(AH1));
    int speed = map(pot_value, 0, 1023, -MAX_SPEED, MAX_SPEED);
    speed = constrain(speed, -MAX_SPEED, MAX_SPEED);
    if (abs(speed) < DEADBAND)
        return 0;
    return speed;
}


void setup()
{
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, HIGH);  // Status LED on

    // Configure motor output pins
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    pinMode(LED5, OUTPUT);

    // Configure pushbuttons as inputs
    pinMode(SW2, INPUT_PULLUP);
    pinMode(SW3, INPUT_PULLUP);
    pinMode(SW4, INPUT_PULLUP);
    pinMode(SW5, INPUT_PULLUP);

    Serial.begin(9600);
    while (!Serial);

    motors(0, 0);
    Serial.println("Motor Controller");
    Serial.println("SW2: enable/disable drive");
    Serial.println("SW4: speed down   SW5: speed up");
    Serial.println("Drive DISABLED - press SW2 to enable");
}

void loop()
{
    // SW2 toggles drive enable (press and release)
    if (digitalRead(SW2) == LOW)
    {
        drive_enabled = !drive_enabled;
        if (drive_enabled)
        {
            digitalWrite(LED_BUILTIN, HIGH);  // Status LED on
            Serial.println("Drive ENABLED");
        }
        else
        {
            motors(0, 0);
            target_speed = 0;
            digitalWrite(LED_BUILTIN, LOW);   // Status LED off
            Serial.println("Drive DISABLED");
        }
        while (digitalRead(SW2) == LOW) {}    // Wait for release
    }

    if (drive_enabled)
    {
        // SW4 decreases speed, SW5 increases speed (both motors together)
        if (digitalRead(SW4) == LOW)
        {
            target_speed -= SPEED_STEP;
            target_speed = constrain(target_speed, -MAX_SPEED, MAX_SPEED);
            Serial.print("Speed: "); Serial.println(target_speed);
            delay(100);               // Debounce
        }
        else if (digitalRead(SW5) == LOW)
        {
            target_speed += SPEED_STEP;
            target_speed = constrain(target_speed, -MAX_SPEED, MAX_SPEED);
            Serial.print("Speed: "); Serial.println(target_speed);
            delay(100);               // Debounce
        }
        // TODO: call right_motor() with target_speed once right_motor() is implemented
        left_motor(target_speed);
    }
    else
    {
        motors(0, 0);
    }

    delay(STEP_DELAY);
}


/*
Extension Activities

1.  Connect an external potentiometer to H1 following the ARPS-2
    schematic. Replace the SW4/SW5 stepped speed control in loop()
    with pot-based control using pot_to_speed():

  target_speed = pot_to_speed(analogRead(AH1));
  motors(target_speed, target_speed);

    For independent left and right motor control, connect a second
    pot to H4 and use pot_to_speed() for each:

  motors(pot_to_speed(analogRead(AH1)),
         pot_to_speed(analogRead(AH4)));

    This gives the same control feel as a joystick-style
    differential drive.

2.  Add acceleration limiting so that motor speed changes gradually
    rather than jumping to the target immediately. Declare static
    variables inside loop() to track the current speed for each
    motor, and move each toward its target by a fixed step per
    iteration. What step size gives a natural-feeling acceleration?

  static int current_left  = 0;
  static int current_right = 0;

3.  Implement 'drive_timed(int left_spd, int right_spd, int ms)'
    that drives both motors for a set duration then stops. Use it
    to create a simple repeatable movement sequence - forward,
    turn, forward, stop.

    Note that 'delay()' inside this function blocks loop() for
    its full duration - Activity 11 introduces non-blocking timing
    that allows other actions to continue during a timed movement.

4.  Four-pump or four-fan controller: the H-bridge driver can
    control four independent single-direction loads by wiring each
    load between one motor output pin and ground, with the other
    pin held LOW. Use SW2-SW5 to enable each pump independently
    and an external pot on H1 to set a shared duty cycle:

  analogWrite(LED2, duty);
  analogWrite(LED3, 0);         // Hold opposite pin LOW

    Add a maximum run time per pump using 'millis()' to prevent
    overheating or tank overflow. This previews the non-blocking
    timing technique covered in Activity 11.

*/
