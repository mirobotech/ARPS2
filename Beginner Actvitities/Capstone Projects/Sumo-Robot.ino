/* ================================================================================
Capstone Project: Sumo Robot Capstone Project [Sumo-Robot]
April 28, 2026

Platform: mirobo.tech ARPS-2 circuit (Robot configuration)
Requires: ARPS2.h header file

Hardware used:
  SW2        - Start match (begins countdown then releases robot)
  SW5        - Emergency stop (halts from any state)
  LS1        - Piezo speaker (countdown beeps, state feedback tones)

  Q1 (A0) - Left floor sensor (ring-edge detection)
  Q3 (A1) - Right floor sensor (ring-edge detection)
  SONAR on H2/H3 - Ultrasonic distance sensor (opponent detection)

  Motor 1 (M1A/M1B) - Left drive motor
  Motor 2 (M2A/M2B) - Right drive motor

IMPORTANT - LED / motor pin sharing:
  LED2-LED5 share pins with the motor driver. The LEDs cannot be
  used while the motors are running. This sketch reserves LED2-LED5
  for the IDLE and STOPPED states only (motors off).

IMPORTANT - LED_BUILTIN / SONAR TRIG pin sharing:
  On the ARPS-2, LED_BUILTIN (pin 13) shares the same pin as TRIG
  (the SONAR trigger output). LED_BUILTIN cannot be used as a status
  indicator in this sketch because toggling it would interfere with
  the SONAR sensor. No on-board LED is available during motor states.

Sensor wiring:
  Mount the SONAR module on headers H2 (TRIG) and H3 (ECHO).
  Mount Q1 (left) and Q3 (right) floor sensors facing downward at
  the front-left and front-right corners of the robot. Q1 and Q3
  both read from analog pins (A0 and A1 respectively). The ring
  boundary (typically white tape on a dark surface, or dark tape on
  a light surface) will produce a sudden change in the Q1/Q3 readings
  as the sensor crosses the line.

--------------------------------------------------------------------------------
Sumo robot behaviour:
  IDLE      - Robot stationary. Press SW2 to begin the match.
              LED2-LED5 flash in a sequence to show the robot is ready.
              SW5 does nothing (already stopped).

  COUNTDOWN - Mandatory pre-match delay (START_DELAY ms). During this
              time the robot is stationary. The speaker counts down
              with beeps. Motors do not move.

  SEARCH    - No opponent within DETECT_RANGE cm. Robot executes a
              slow rotation to sweep the SONAR across the ring.
              If SONAR detects an opponent, transition to PUSH.
              If a floor sensor trips, transition to EDGE.

  PUSH      - Opponent detected within DETECT_RANGE cm. Robot drives
              forward at full speed to push the opponent out.
              SONAR is checked continuously: if opponent disappears
              (pushed out or dodged), return to SEARCH.
              If a floor sensor trips, transition to EDGE immediately.

  EDGE      - A floor sensor has detected the ring boundary. Robot
              backs up for BACKUP_TIME ms, then turns away from the
              detected edge for TURN_TIME ms, then returns to SEARCH.
              Edge detection is the highest-priority condition and
              interrupts any other state immediately.

  STOPPED   - Motors halted. Match ended, emergency stop pressed, or
              error state. Press SW2 to return to IDLE.
--------------------------------------------------------------------------------
Before you begin - complete your capstone plan:
  1. Write a plain-English description from the robot's perspective:
     what does it sense, what does it decide, what does it do?
  2. List all states and draw your state diagram with transitions.
  3. Complete the state details table (outputs, transitions, next states).
  4. List all constants and variables you will need.
  5. Write your testing plan - test each sensor and motor independently
     before combining them into state logic.
================================================================================*/
// IMPORTANT: Make sure that ARPS2.h is accessible as a tab in this project.
#include "ARPS2.h"

// ---- State Constants -----------------
const int STATE_IDLE      = 0;  // Waiting for start - robot stationary
const int STATE_COUNTDOWN = 1;  // Pre-match delay - stationary, counting down
const int STATE_SEARCH    = 2;  // No opponent in range - rotating to sweep SONAR
const int STATE_PUSH      = 3;  // Opponent detected - driving forward to push
const int STATE_EDGE      = 4;  // Ring edge detected - backing up and turning away
const int STATE_STOPPED   = 5;  // Match ended or emergency stop

const char* STATE_NAMES[] = {
    "IDLE", "COUNTDOWN", "SEARCH", "PUSH", "EDGE", "STOPPED"
};

// ---- Timing Constants ----------------
const int          START_DELAY    = 5000; // Mandatory pre-match countdown (ms)
const int          BEEP_INTERVAL  = 1000; // Countdown beep interval (ms)
const int          BACKUP_TIME    = 400;  // Time to reverse after edge detection (ms)
const int          TURN_TIME      = 350;  // Time to turn away from edge (ms)
const unsigned int SONAR_INTERVAL = 50;   // Minimum time between SONAR readings (ms)

// ---- Sensor Thresholds ---------------
// Floor sensor threshold: readings above this value indicate the ring
// surface (10-bit ADC, range 0-1023). Calibrate by printing Q1_level()
// and Q3_level() on the ring surface and on the boundary tape before use.
const int EDGE_THRESHOLD = 200;  // Adjust for your surface and lighting

// SONAR opponent detection range. The robot charges when an opponent
// is detected within this distance.
const int DETECT_RANGE   = 40;   // cm - robot charges when opponent within this range

// ---- Loop Constant -------------------
const int LOOP_DELAY     = 10;   // Main loop delay (ms)

// ---- Program Variables ---------------
int           state           = STATE_IDLE;
unsigned long state_start     = 0;

unsigned long last_beep_time  = 0;   // Last countdown beep timestamp
unsigned long last_sonar_time = 0;   // Last SONAR reading timestamp
int           sonar_distance  = 999; // Most recent SONAR reading (cm)

// Edge direction detected (set in check_edge(), used in STATE_EDGE)
int  edge_direction = 0;
const int EDGE_BOTH  = 0;
const int EDGE_LEFT  = 1;
const int EDGE_RIGHT = 2;

// Sub-state within STATE_EDGE
bool edge_in_backup = true;  // true = backup phase, false = turn phase


// ---- Program Functions ---------------

void enter_state(int new_state, unsigned long current_time, const char* reason = "")
{
    motors_stop();
    state           = new_state;
    state_start     = current_time;
    edge_in_backup  = true;
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

bool check_edge()
{
    // Read both floor sensors and return true if either detects the ring
    // boundary. Sets edge_direction for the EDGE state.
    bool left_on_edge  = Q1_level() < EDGE_THRESHOLD;
    bool right_on_edge = Q3_level() < EDGE_THRESHOLD;
    if (left_on_edge && right_on_edge)
        { edge_direction = EDGE_BOTH;  return true; }
    else if (left_on_edge)
        { edge_direction = EDGE_LEFT;  return true; }
    else if (right_on_edge)
        { edge_direction = EDGE_RIGHT; return true; }
    return false;
}

void read_sonar(unsigned long current_time)
{
    // Read the SONAR sensor at most once every SONAR_INTERVAL ms.
    if ((current_time - last_sonar_time) >= SONAR_INTERVAL)
    {
        int result = int(sonar_range(DETECT_RANGE + 20));
        if (result >= 0)
            sonar_distance = result;
        last_sonar_time = current_time;
    }
}

bool opponent_detected()
{
    return (sonar_distance > 0 && sonar_distance <= DETECT_RANGE);
}


void setup()
{
    // NOTE: LED_BUILTIN is NOT initialised here - it shares the TRIG pin
    // and will be configured as OUTPUT by the SONAR setup below.

    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    pinMode(LED5, OUTPUT);
    leds_off();

    pinMode(SW2, INPUT_PULLUP);
    pinMode(SW5, INPUT_PULLUP);

    pinMode(TRIG, OUTPUT);      // Also configures LED_BUILTIN (pin 13)
    pinMode(ECHO, INPUT);
    digitalWrite(TRIG, LOW);

    Serial.begin(9600);
    while (!Serial);

    Serial.println("Sumo Robot");
    Serial.println("SW2: start match (5-second countdown)");
    Serial.println("SW5: emergency stop");
    Serial.println();
    Serial.print("Sensor check:  Q1="); Serial.print(Q1_level());
    Serial.print("  Q3="); Serial.print(Q3_level());
    Serial.print("  SONAR="); Serial.print(sonar_range_cm(100));
    Serial.println("cm");
    Serial.println();

    state_start    = millis();
    last_beep_time = millis();

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
    	motors_stop();
        tone(LS1, 200, 300);
        enter_state(STATE_STOPPED, current_time, "emergency stop");
    }

    // ---- State: Idle ----
    else if (state == STATE_IDLE)
    {
        // Robot is stationary, waiting for match start.
        // LED2-LED5 are available here (motors stopped).
        // TODO: implement a ready animation on LED2-LED5 (e.g. a slow
        //       rotating pattern) using the Activity 11 repeating-timer
        //       pattern with a separate last_idle_time variable.

        if (digitalRead(SW2) == LOW)
        {
            all_leds_off();
            last_beep_time = current_time;
            enter_state(STATE_COUNTDOWN, current_time, "SW2 pressed");
        }
    }

    // ---- State: Countdown ----
    else if (state == STATE_COUNTDOWN)
    {
        // Pre-match mandatory delay. Robot is stationary.
        // TODO: beep once per BEEP_INTERVAL ms using last_beep_time.
        //       Calculate the remaining seconds from elapsed and START_DELAY
        //       and print each countdown second to the serial monitor.

        if (elapsed >= (unsigned long)START_DELAY)
        {
            enter_state(STATE_SEARCH, current_time, "countdown complete");
        }
    }

    // ---- State: Search ----
    else if (state == STATE_SEARCH)
    {
        if (check_edge())
        {
            const char* dir = (edge_direction == EDGE_LEFT)  ? "left edge"  :
                              (edge_direction == EDGE_RIGHT) ? "right edge" : "both edges";
            enter_state(STATE_EDGE, current_time, dir);
        }
        else
        {
            read_sonar(current_time);
            if (opponent_detected())
            {
                char reason[24];
                snprintf(reason, sizeof(reason), "opponent at %dcm", sonar_distance);
                enter_state(STATE_PUSH, current_time, reason);
            }
            else
            {
                // TODO: implement a search rotation. A simple approach is to
                //       rotate one motor forward and one reverse (spin in place).
                //       More advanced: alternate sweep directions after a timeout,
                //       or drive in a slow arc to cover the ring surface.
                //       Remember: both forward directions must be correct for your
                //       motor wiring. Test left_motor_forward() and
                //       right_motor_forward() individually first.
            }
        }
    }

    // ---- State: Push ----
    else if (state == STATE_PUSH)
    {
        if (check_edge())
        {
            const char* dir = (edge_direction == EDGE_LEFT)  ? "left edge"  :
                              (edge_direction == EDGE_RIGHT) ? "right edge" : "both edges";
            enter_state(STATE_EDGE, current_time, dir);
        }
        else
        {
            read_sonar(current_time);
            if (!opponent_detected())
                enter_state(STATE_SEARCH, current_time, "opponent lost");
            else
            {
                // TODO: drive both motors forward to push.
            }
        }
    }

    // ---- State: Edge ----
    else if (state == STATE_EDGE)
    {
        if (edge_in_backup)
        {
            // TODO: drive both motors in reverse.
            if (elapsed >= (unsigned long)BACKUP_TIME)
            {
                edge_in_backup = false;
                state_start    = current_time;
            }
        }
        else
        {
            if (edge_direction == EDGE_LEFT)
            {
                // TODO: Turn right (away from left edge)
            }
            else
            {
                // TODO: Turn left (away from right or both)
            }

            if (elapsed >= (unsigned long)TURN_TIME)
                enter_state(STATE_SEARCH, current_time, "edge cleared");
        }
    }

    // ---- State: Stopped ----
    else if (state == STATE_STOPPED)
    {
        if (digitalRead(SW2) == LOW)
        {
            all_leds_off();
            enter_state(STATE_IDLE, current_time, "SW2 pressed");
        }
    }

    delay(LOOP_DELAY);
}


/*
Capstone Development Guide

Work through these steps in order. Never attach the robot to a surface
during motor testing unless you are certain of the motor directions -
motors can drive the robot off a table unexpectedly. Test on the floor
or hold the robot clear of any surface.

Step 1 - Sensor calibration
  Before writing any state logic, calibrate your sensors. Add temporary
  Serial.print statements in setup() to print readings in a loop:

    while (true) {
        Serial.print("Q1:"); Serial.print(Q1_level());
        Serial.print(" Q3:"); Serial.print(Q3_level());
        Serial.print(" SONAR:"); Serial.print(sonar_range_cm(100));
        Serial.println("cm");
        delay(100);
    }

  Place the robot on the ring surface and note the Q1 and Q3 readings.
  Move the robot to the boundary tape and note the change. Set
  EDGE_THRESHOLD to a value midway between the two readings. Test with
  the same lighting conditions you will use during the match.

  Note: on the ARPS-2, Q3 shares the same analog pin (A1) as Q2. This
  is correct by design - Q3 is used alone for floor sensing on the right
  side (right module) while Q2 is used for line sensing. Confirm that
  Q3_level() returns meaningful readings from your right-side sensor.

Step 2 - Motor direction verification
  Verify motor directions before connecting to drive wheels. Add
  temporary code in setup() and confirm "forward" moves the robot in
  the intended direction:

    left_motor_forward();
    delay(500);
    motors_stop();

  If a motor runs backward, swap its wiring on CON1 (the motor terminal
  block) rather than changing the code. Both motors should drive the
  robot forward when left_motor_forward() and right_motor_forward() are
  called together.

Step 3 - Edge detection response
  Test STATE_EDGE in isolation. Temporarily enter STATE_EDGE from IDLE
  and confirm the robot backs up, then turns in the correct direction
  for each edge_direction value. Walk the robot to each edge of the ring
  and verify the correct direction prints to the serial monitor.

Step 4 - Search behaviour
  With edge detection working, test STATE_SEARCH. The robot should spin
  in place and transition to PUSH when an object is held within
  DETECT_RANGE. Verify it returns to SEARCH when the object is removed.

Step 5 - Full match cycle
  Run the complete state machine from IDLE through COUNTDOWN to SEARCH.
  Watch the serial monitor to confirm state transitions print correctly.
  Test the emergency stop (SW5) from SEARCH and PUSH.

Step 6 - Extensions and strategy improvements

  a) Proportional SONAR steering: in STATE_PUSH, correct course if
     the opponent moves to one side by briefly slowing one motor.
     To detect sideways movement, take two SONAR readings in quick
     succession while rotating slightly left and right, and compare
     which direction gives the shorter reading.

  b) Floor sensor ring check in PUSH: if one floor sensor approaches
     the edge threshold during a push, steer away from that side to
     avoid chasing the opponent all the way to your own edge.

  c) Search strategy improvement: add a STATE_ADVANCE that drives
     forward for a short burst between search sweeps, breaking the
     mirror pattern that trips up simple spin-in-place strategies.

  d) Battery monitoring: ADIV_level() reads the battery voltage through
     a resistor divider (A3). Low battery causes erratic motor behaviour
     that can look like software bugs. Print the ADIV reading at startup
     and periodically during the match. Add a low-battery warning
     (beep and slow flashing LED) if the reading drops below a
     threshold you determine experimentally.

  e) Match timer: add a MATCH_DURATION constant and transition to
     STATE_STOPPED when the time since COUNTDOWN ended exceeds
     MATCH_DURATION. Print the match duration when the match ends.

*/
