/* ================================================================================
Capstone Project: Line Follower Robot Capstone Project [Line-Follower]
April 28, 2026

Platform: mirobo.tech ARPS-2 circuit (Robot configuration)
Requires: ARPS2.h header file

Hardware used:
  SW2        - Start following (begins immediately, no countdown)
  SW5        - Emergency stop (halts from any state)
  LS1        - Piezo speaker (start beep, line-lost alert)

  Q1 (A0) - Left line sensor  (L sensor module, left/outboard position)
  Q2 (A1) - Right line sensor (L sensor module, right/inboard position)
  Q3 (A1) - Line sensor (R sensor module, for alternate configurations)

  Motor 1 (M1A/M1B) - Left drive motor
  Motor 2 (M2A/M2B) - Right drive motor

IMPORTANT - LED / motor pin sharing:
  LED2-LED5 share pins with the motor driver. The LEDs cannot be
  used while the motors are running, but instead show motor states.

IMPORTANT - LED_BUILTIN / SONAR TRIG pin sharing:
  On the ARPS-2, LED_BUILTIN (pin 13) shares the same pin as the SONAR
  TRIG output. This sketch does not use the SONAR sensor, so LED_BUILTIN
  is available as a status indicator here. Initialise it in setup()
  before use, and be aware that adding SONAR later will require removing
  all LED_BUILTIN references.

Sensor module setup - two-sensor configuration (starting point):
  Install the L sensor module ('Floor/Line', labelled 'L') on the
  robot's centreline. The module contains two phototransistors: Q1 in
  the left position, and Q2 in the right position. When centred on the
  line, both Q1 and Q2 'see' the line. This works well for following
  a simple tape line (20mm or 3/4" electrical tape works well).

  Note: on the ARPS-2, Q2 and Q3 share analog pin A1. Q2_level() and
  Q3_level() both read A1. In this sketch Q2_level() is used for the
  right/inboard sensor. Q3 is not used.

  This sketch follows a dark line on a light surface (e.g. black tape
  on a white floor). Sensors return higher values on the light surface
  and lower values over the dark line (10-bit ADC, range 0-1023).
  Calibrate LINE_THRESHOLD to a value midway between your two surfaces.
  If your course uses a light line on a dark surface, invert the
  comparisons in read_sensors().

--------------------------------------------------------------------------------
Other sensor arrangements (see capstone development guide, below):

  Two-sensor floor detection (wider lines, T-stops, + intersections):
    Use two sensor modules, with the 'R' module on the robot's left,
    and the 'L' module on the robot's right. Q3 becomes the left sensor
    and Q1 becomes the right sensor, with both sensors detecting the
    floor on either side of the line. Update read_sensors() to read
    Q3_level() and Q1_level().

  Three-sensor row (narrower lines, sharper curves):
    Place the L module on the robot's LEFT and the R module on the robot's
    RIGHT, aligning the three phototransistors in a row:
      Q1 (L module, left position)  - left of line
      Q2 (L module, right position) - centre, on the line
      Q3 (R module, right position) - right of line
    Q1 reads A0, Q2 and Q3 both read A1 (they share the pin). This means
    the left and centre sensors are independent but the centre and right
    sensors read identically. In practice this gives 2.5 effective sensors:
    left, centre/right-combined. See the development guide for details.
--------------------------------------------------------------------------------
Line follower behaviour:
  IDLE      - Robot stationary. Press SW2 to begin following.
              Motors off. LED2-LED5 available for ready animation.

  FOLLOW    - Two-sensor differential steering:
                Q1 on line only -> turn left
                Q2 on line only -> turn right
                Both on line                     -> drive straight
                Neither on line                  -> line lost, enter RECOVER

  RECOVER   - Line lost. Robot rotates opposite the last-known turn
              direction for RECOVER_TIME ms to sweep back across the
              line. If the line is found again, return to FOLLOW
              immediately. If RECOVER_TIME elapses without finding
              the line, try changing to the opposite direction
              for RECOVER_TIME ms. If still not found, transition to
              STOPPED.

  STOPPED   - Motors halted. Press SW2 to return to IDLE.
--------------------------------------------------------------------------------
Before you begin - complete your capstone plan:
  1. Write a plain-English description of the robot's following behaviour.
  2. Draw the state diagram with all transitions.
  3. Complete the state details table (outputs, transitions, next states).
  4. List all constants and variables you will need.
  5. Write your testing plan - test each sensor individually before
     testing the complete following behaviour.
================================================================================*/
// IMPORTANT: Make sure that ARPS2.h is accessible as a tab in this project.
#include "ARPS2.h"

// ---- State Constants -----------------
const int STATE_IDLE    = 0;  // Waiting for start - robot stationary
const int STATE_FOLLOW  = 1;  // Following the line using differential steering
const int STATE_RECOVER = 2;  // Line lost - searching for the line
const int STATE_STOPPED = 3;  // Halted - press SW2 to return to idle

const char* STATE_NAMES[] = { "IDLE", "FOLLOW", "RECOVER", "STOPPED" };

// ---- Sensor Threshold ----------------
// Calibrate by printing Q1/Q2 levels on both surfaces and choosing a value
// midway between them. The 10-bit ADC range is 0-1023.
const int LINE_THRESHOLD = 400;  // Adjust for your line and surface colours
// Readings below LINE_THRESHOLD indicate the sensor is over the dark line.
// Readings above LINE_THRESHOLD indicate the sensor is over the light surface.

// ---- Timing Constants ----------------
const int RECOVER_TIME = 600;    // Time to rotate in one direction when line is lost (ms)
const int LOOP_DELAY   = 5;      // Main loop delay (ms) - short for responsive steering

// ---- Program Variables ---------------
int           state        = STATE_IDLE;
unsigned long state_start  = 0;

// Sensor readings - updated every loop iteration in FOLLOW and RECOVER.
bool left_on_line  = false;   // True when Q1 (left) detects the line
bool right_on_line = false;   // True when Q2 (right) detects the line

// Recovery direction and attempt counter
bool recover_turn_left = true;
int  recover_attempt   = 0;    // 0 = first direction, 1 = opposite direction


// ---- Program Functions ---------------

void enter_state(int new_state, unsigned long current_time, const char* reason = "")
{
    motors_stop();
    state       = new_state;
    state_start = current_time;
    Serial.print("--> ");
    Serial.print(STATE_NAMES[new_state]);
    if (strlen(reason) > 0)
    {
        Serial.print(" (");
        Serial.print(reason);
        Serial.print(")");
    }
    Serial.println();
}

void read_sensors()
{
    // Read the two active line sensors and update the on-line flags.
    // Update LINE_THRESHOLD to suit your course.
    left_on_line  = Q1_level() < LINE_THRESHOLD;   // Q1: left  (A0)
    right_on_line = Q2_level() < LINE_THRESHOLD;   // Q2: right (A1)
}

bool any_on_line()
{
    return left_on_line || right_on_line;
}

void all_leds_off()
{
    // NOTE: only call this when motors are stopped - LEDs share motor pins.
    digitalWrite(LED2, LOW);
    digitalWrite(LED3, LOW);
    digitalWrite(LED4, LOW);
    digitalWrite(LED5, LOW);
}


void setup()
{
    // NOTE: LED_BUILTIN is available in this sketch because the SONAR
    // sensor is not used. If SONAR is added later, remove LED_BUILTIN
    // references (LED_BUILTIN shares pin 13 with SONAR TRIG).
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);

    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    pinMode(LED5, OUTPUT);
    all_leds_off();

    pinMode(SW2, INPUT_PULLUP);
    pinMode(SW5, INPUT_PULLUP);

    Serial.begin(9600);
    while (!Serial);

    Serial.println("Line Follower Robot");
    Serial.println("SW2: start following");
    Serial.println("SW5: emergency stop");
    Serial.println();
    Serial.println("Sensor check (raw ADC values):");
    Serial.print("  Q1: "); Serial.println(Q1_level());
    Serial.print("  Q2: "); Serial.println(Q2_level());
    Serial.print("  LINE_THRESHOLD: "); Serial.println(LINE_THRESHOLD);
    Serial.println();

    state_start = millis();
    enter_state(STATE_IDLE, state_start, "startup");
}


void loop()
{
    unsigned long current_time = millis();
    unsigned long elapsed      = current_time - state_start;

    // ---- Emergency stop (SW5) - checked before all state logic ----
    if (digitalRead(SW5) == LOW &&
        state != STATE_IDLE && state != STATE_STOPPED)
    {
        all_leds_off();
        digitalWrite(LED_BUILTIN, LOW);
        enter_state(STATE_STOPPED, current_time, "emergency stop");
    }

    // ---- State: Idle ----
    else if (state == STATE_IDLE)
    {
        // LED2-LED5 are available here (motors stopped).
        // TODO: implement a ready animation on LED2-LED5.

        if (digitalRead(SW2) == LOW)
        {
            all_leds_off();
            tone(LS1, 880, 100);
            digitalWrite(LED_BUILTIN, HIGH);
            enter_state(STATE_FOLLOW, current_time, "SW2 pressed");
        }
    }

    // ---- State: Follow ----
    else if (state == STATE_FOLLOW)
    {
        read_sensors();

        if (!any_on_line())
        {
            // Both sensors off the line - transition to recovery.
            // Set recovery direction based on which sensor last saw the line.
            // If the right sensor (Q2) was last on, the line is to
            // the right - turn right. Otherwise turn left.
            recover_turn_left = !right_on_line;
            recover_attempt   = 0;
            enter_state(STATE_RECOVER, current_time, "line lost");
        }
        else
        {
            // Two-sensor differential steering:
            //
            //   Q1 on line only (left/outboard):
            //     Robot has drifted right - line is now under Q1.
            //     Turn left: stop left motor, right motor forward.
            //
            //   Q2 on line only (right/inboard):
            //     Robot has drifted left - line is now under Q2.
            //     Turn right: stop right motor, left motor forward.
            //
            //   Both on line:
            //     Robot is centred, or line is wider than sensor spacing.
            //     Drive straight: both motors forward.

            if (left_on_line && !right_on_line)
            {
                // Q1 on line only:
                //   Robot has drifted right - line is now under Q1.
                // TODO: turn robot left (implement left correction)
            }
            else if (right_on_line && !left_on_line)
            {
                // Q2 on line only:
                //   Robot has drifted left - line is now under Q2.
                // TODO: turn robot right (implement right correction)
            }
            else
            {
                // Both sensors on line - drive straight.
                // TODO: drive both motors forward.
            }
        }
    }

    // ---- State: Recover ----
    else if (state == STATE_RECOVER)
    {
        read_sensors();

        if (any_on_line())
        {
            enter_state(STATE_FOLLOW, current_time, "line found");
        }
        else if (elapsed < (unsigned long)RECOVER_TIME)
        {
            // TODO: rotate toward recover_turn_left:
            //       if recover_turn_left: left reverse, right forward
            //       else:                 left forward, right reverse
            if (recover_turn_left)
            {
                // TODO: turn robot left
            }
            else
            {
                // TODO: turn robot right
            }
        }
        else
        {
            if (recover_attempt == 0)
            {
                recover_turn_left = !recover_turn_left;
                recover_attempt   = 1;
                state_start       = current_time;
                Serial.println("    recovery attempt 2: opposite direction");
            }
            else
            {
                tone(LS1, 220, 500);
                all_leds_off();
                digitalWrite(LED_BUILTIN, LOW);
                enter_state(STATE_STOPPED, current_time, "line not found");
            }
        }
    }

    // ---- State: Stopped ----
    else if (state == STATE_STOPPED)
    {
        if (digitalRead(SW2) == LOW)
        {
            all_leds_off();
            digitalWrite(LED_BUILTIN, LOW);
            enter_state(STATE_IDLE, current_time, "SW2 pressed");
        }
    }

    delay(LOOP_DELAY);
}


/*
Capstone Development Guide

Work through these steps in order. Test on the floor and hold the robot
clear of any surface when testing motor directions for the first time.

Step 1 - Sensor calibration
  Before any state logic, calibrate your line sensors. Add a temporary
  calibration loop at the end of setup():

    while (true) {
        Serial.print("Q1: "); Serial.print(Q1_level());
        Serial.print("  Q2: "); Serial.println(Q2_level());
        delay(100);
    }

  Hold each sensor over the light surface and note the readings, then
  over the dark tape. Set LINE_THRESHOLD to a value comfortably midway
  between the two. A good threshold leaves a clear gap on both sides
  so that minor lighting variation does not trigger false readings.

  Note: Q2 and Q3 share analog pin A1 on the ARPS-2. Install the right-
  side sensor phototransistor in the Q2 (right/inboard) position of the
  L module. Q3_level() is not used in this sketch.

  If your course uses a light line on a dark surface, change the
  comparisons in read_sensors() from < to >.

Step 2 - Motor direction verification
  Verify motor directions before driving on a course:

    left_motor_forward();
    delay(500);
    motors_stop();

  If a motor runs backward, swap its wiring on CON1. Both motors should
  drive the robot forward when both are set to forward.

Step 3 - Straight-line following
  Place the robot centred on the line and verify that STATE_FOLLOW
  drives it straight with both sensors triggering. Both left_on_line
  and right_on_line should be true when the robot is centred.

Step 4 - Steering corrections
  Test each steering correction on a gentle curve. Confirm that:
  - The robot turns left when only Q1 (left) triggers
  - The robot turns right when only Q2 (right) triggers
  If corrections are in the wrong direction, check that Q1 is physically
  on the left side of the robot (from the front).

Step 5 - Recovery
  Test STATE_RECOVER by lifting the robot off the line mid-follow and
  placing it back. Confirm it finds the line and returns to FOLLOW.
  Test the two-attempt recovery by placing the robot away from the line
  and verifying it transitions to STOPPED after both attempts fail.

Step 6 - Complete course
  Run the robot on a closed loop. Watch the serial monitor for state
  transitions. A well-tuned robot should stay in STATE_FOLLOW for the
  entire lap except when recovering from sharp curves.

Step 7 - Sensor configuration extensions

  a) Two-sensor floor detection (wider lines, T-stops, + intersections):
     Place the R module ('Floor', labelled 'R') on the robot's LEFT side
     and the L module ('Floor/Line', labelled 'L') on the robot's RIGHT
     side. Q3 (R module outboard, reads A1) becomes the left floor sensor
     and Q1 (L module outboard, reads A0) becomes the right floor sensor.
     Both inboard positions now detect the floor on either side of a wider
     line rather than the line itself.

     TODO: update read_sensors() for floor detection:
       left_on_line  = Q3_level() < LINE_THRESHOLD;  // Q3: left floor (A1)
       right_on_line = Q1_level() < LINE_THRESHOLD;  // Q1: right floor (A0)
     Invert the steering logic: if both sensors are HIGH (on the floor),
     drive straight. Correct when one sensor goes LOW (over the line).

  b) Three-sensor row (note: ARPS-2 hardware constraint):
     Place the L module on the robot's LEFT and the R module on the RIGHT,
     aligning the three phototransistors in a row:
       Q1 (L module, left/outboard)  - left of line     (A0)
       Q2 (L module, right/inboard)  - centre, on line  (A1)
       Q3 (R module, right/outboard) - right of line    (A1, same as Q2!)
     Because Q2 and Q3 share pin A1, the centre and right sensors read
     identically. This effectively gives a left sensor (Q1, independent)
     and a centre/right combined sensor (Q2=Q3, A1). The robot can detect
     left drift and centred/right conditions but cannot distinguish between
     centred and right-drift.

     TODO: update read_sensors() to add a centre flag:
       left_on_line   = Q1_level() < LINE_THRESHOLD;  // Q1: left  (A0)
       centre_on_line = Q2_level() < LINE_THRESHOLD;  // Q2/Q3: centre+right (A1)
     Steering logic: turn left if left only; drive straight if centre.
     True right-only detection is not possible without a third analog pin.
     Consider wiring an external phototransistor to header H1 (AH1, A4)
     as a dedicated right sensor for full three-sensor capability.

  c) Speed gradient using PWM: rather than stopping a motor for
     corrections, reduce its duty cycle proportionally. This gives
     smoother following and helps compensate for the inherent oscillation
     of two-sensor bang-bang control. Use analogWrite() on the motor pins
     to set speed, and compare the result to the binary approach.

  d) Lap counter: increment a counter each time both sensors trigger
     simultaneously for more than MARKER_TIME ms after a period where
     only one sensor was active. Print the lap count and lap time to the
     serial monitor each lap.

  e) Course map: record each steering correction as a direction and
     elapsed time since the previous correction. After one lap, print
     the map to the serial monitor. Can you predict the course layout
     from the map?

*/
