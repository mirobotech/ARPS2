/*
Project:  ARPS2-Intro-4-Functions
Activity: mirobo.tech/arps-intro-4
Updated:  February 4, 2025

This introductory programming activity for the mirobo.tech ARPS2 circuit
demonstrates the use of functions and the ability to pass variables between
the program's main code and the function code.
 
Additional program analysis and programming activities examine the location of
function code within the program, reinforce the concepts of local and global
variables, and explore some benefits of functions.

See the https://mirobo.tech/arps webpage for additional ARPS2
resources, programming activities, and starter programs.
*/

// ARPS2 Educational Starter I/O devices
const int SW2 = 0;      // Pushbuttons SW2 and SW3 are fully supported on
const int SW3 = 1;      // Arduino UNO R4 Minima and Arduino UNO R4 WiFi
const int SW4 = 2;      // Pushbuttons SW4 and SW5 work on all Arduino UNO
const int SW5 = 4;      // circuit boards

const int LED2 = 3;     // ARPS2 LEDs
const int LED3 = 9;
const int LED4 = 10;
const int LED5 = 11;

const int BEEPER = 6;   // ARPS2 Piezo beeper LS1

// Pre-defined Arduino UNO LED
// LED_BUILTIN (D13)    // On-board LED (shared with H2 and SONAR TRIG)

// Define program constants
const int NOBUTTON = 0;
const int UP = 1;
const int DOWN = 2;
const int MAX = 3;

// Define program variables
int SW2State;
int SW3State;
int SW4State;
int SW5State;

int LED3Level = 127;
int button;

void setup() {
  pinMode(SW2,INPUT_PULLUP);
  pinMode(SW3,INPUT_PULLUP);
  pinMode(SW4,INPUT_PULLUP);
  pinMode(SW5,INPUT_PULLUP);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  pinMode(LED4,OUTPUT);
  pinMode(LED5,OUTPUT);
  pinMode(BEEPER,OUTPUT);
  pinMode(LED_BUILTIN,OUTPUT);
  tone(BEEPER,4000,100);  // Say hello!
}

int readButton(void) {
  SW2State = digitalRead(SW2);
  SW3State = digitalRead(SW3);
  SW4State = digitalRead(SW4);

  if(SW3State == LOW) {     // Change to SW5 for Arduino UNO R3
    return(DOWN);
  }
  else if (SW4State == LOW) {
    return(UP);
  }
  else if (SW2State == LOW) {
    return(MAX);
  }
  else {
    return(NOBUTTON);
  }
}

void setLED3Level(int btn) {
  if(btn == UP && LED3Level < 255) {
    LED3Level += 1;
  }

  if(btn == DOWN && LED3Level > 0) {
    LED3Level -= 1;
  }

  if(btn == MAX) {
    for(LED3Level; LED3Level != 255; LED3Level ++) {
      analogWrite(LED3,LED3Level);
      delay(1);
    }
  }
  analogWrite(LED3,LED3Level);
}

void loop() {
  // Read buttons
  button = readButton();

  // Set LED output level
  setLED3Level(button);

  // Add a short user delay
  delay(10);
}

// Move the function code to this location in Program Analysis Activities 6.


/* Learn More -- Program Analysis Activities
 
1.  Let's learn about functions, and how to use them in your programs.
    Every function begins with a function declaration - it's the first
    line of the function and it includes the function's return type,
    the function's name, and any optional parameters that are required
    by the function. The 'void setup()' and 'void loop()' statements
    that have been a part of every introductory programming activity
    so far are actually functions declarations, and the program code
    inside their braces is a part of their respective functions.

    This program contains two new functions. The first one begins with
    the function declaration 'int readButton(void)'. The 'int' part of
    the declaration refers to the type of numeric data that the
    readButton() will return to the statement that called it, and is
    a simple way to pass data between the function and its calling
    code located in another part of the program. The 'void' parameter
    following the function's name means that this function does not
    accept any parameters and could have been left as empty brackets,
    exactly as was done by the setup() and loop() functions.

    The second function's declaration is 'void setLED3Level(int btn)'.
    Like both the setup() and loop() functions, this function has a
    'void' return type, which means that it will not return any data
    to its calling statement. Unlike the readButton() function, this
    setLED3Level() function does require a piece of data to be passed
    from its function call statement. What kind of data do you think
    the 'int btn' parameter defines, and what do you think this data
    will be used for inside the function?

2.  Based on your experience from the previous Intro-3-Loops activity,
    do you think the 'btn' variable that is part of the setLED3Level()
    function is global, or local to the function? Explain why?
 
3.  How does the 'btn' variable get assigned a value in the main loop
    of the program? Where does its value come from?
 
4.  Is the 'button' variable a global or local variable? How do you
    know?

5.  How does the 'button' variable get assigned a value, and in what
    part of the program does this take place?

6.  The new functions were added in between the existing setup() and
    loop() functions and didn't seem to affect how the program runs.
    This is because setup() and loop() are always the first and second
    functions to run in any Arduino program, so when this program runs
    the code in the newly-added functions won't run until it has been
    called.
    
    The new functions seem to be in an awkward place, between setup()
    and loop(). Let's see if the new functions be moved to a different
    location in the program. Move all of the code in the readButton()
    and setLED3Level() functions underneath the loop() function, in
    the location shown by the comment in the program code. Upload the
    program and try the buttons again. Does it still work the same way?

7.  Let's explore some of the advantages of using functions. Examine
    this program's loop() function and imagine what the code in the
    loop() function would look like if it contained all of the program
    code that's currently located in the two, new functions.

    It obviously took extra effort and more program code to craft the
    two new functions, so there must be a good reason to implement one
    or more new functions in a program. Explain any potential benefits
    of creating and using the two functions for this program. In what
    ways do the functions make this program easier for beginners to 
    understand? In what ways do the functions make it more difficult
    for beginners to understand the code?
 
8.  Multiple parameters can be passed to a function. Add the beepBoop()
    function, below, to your program:
 
void beepBoop(int hiFrq, int lowFrq, int dur) {
  tone(BEEPER, hiFrq);
  delay(dur);
  tone(BEEPER, lowFrq);
  delay(dur);
  noTone(BEEPER);
}

    Comment-out the program code that calls both the readButton() and
    setLED3Level() functions, and add the code, below, to call the
    beepBoop() function if SW5 is pressed:
 
  SW5State = digitalRead(SW5);
  if(SW5State == LOW) {
    beepBoop(1050,880,200);
  }

    Build and upload the program to your circuit to try it out.

9.  We can use this new beepBoop() function to help demonstrate another
    advantage of functions, namely the concept of code re-use. This new
    beepBoop() function can easily be used by more than one part of the
    program. To demonstrate this, add a second button input statement
    that calls the beepBoop() function using different paramters than
    those provided by SW5. Without functions, how much larger would
    the program have had to be to create different sets of tones for
    each of the four pushbuttons on your circuit?
 
Programming Activities
 
1.  Create a function that uses your circuit board's LEDs as a simple
    level gauge to display the value of a variable. Think about the
    range of values you would want to display, perhaps 0-100, or 0-255.
    Next, think about how the data will be displayed, and how that data
    will be reresented by the four LEDs. For example, when displaying
    data from 0-100, each LED could represent a value of 25, so that
    when all four LEDs are lit, the value could be inferred as 100.
    (Think about how some battery level indicators display 3 or 4 
    distinct bars inside a battery symbol.)
    
    As you create your function, you'll also have to decide if the LED
    representing the highest level will only be lit for that one value,
    of if it will light or extinguish at a sligly lower value instead.

    Test your function by using two pushbuttons to increase or decrease
    the value of the variable, the same way the setLED3Level() function
    does. If you think it's worthwhile, make your pushbutton code into
    a separate function.

2.  Create a function to display the binary numbers from 0-15 on four
    LEDs. Use the pushbuttons or a slowly changing loop counter variable
    to test your function.

3.  A function that converts an 8-bit value (0-255) into its equivalent
    3-digit decimal numbers might be useful in helping to debug your
    programs, or to output data to seven-segment LEDs and LCDs. Create
    a function that converts a number passed to it into three, separate
    variables represending the individual hundreds, tens, and ones digits
    of the number. For example, passing the function the value of 142
    will result in the hundreds digit variable containing the value 1,
    the tens digit variable containing 4, and the ones digit variable
    being 2. Test this function using the binary display function that
    you created in step 2, above.

4.  Create a sound effect function that sweeps between two frequencies
    passed to it by the calling code. Optionally, control speed of the
    frequency sweep using another parameter in the function.
*/
