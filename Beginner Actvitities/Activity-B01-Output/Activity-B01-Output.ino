/* ================================================================================
Beginner Activity 1: Output [Activity-B01-Output]
February 2, 2026

Platform: mirobo.tech ARPS-2 circuit (any configuration)
Requires: ARPS2.h header file
=================================================================================*/
// IMPORTANT: Make sure that ARPS2.h is accessible as a tab in this project.
#include "ARPS2.h"      // Define ARPS-2 I/O devices

// ---------------------------------------------------------------
// Setup runs once at power-up
// ---------------------------------------------------------------
void setup()
{
    // Configure ARPS-2 LEDs as outputs
    pinMode(LED2, OUTPUT);
    pinMode(LED3, OUTPUT);
    pinMode(LED4, OUTPUT);
    pinMode(LED5, OUTPUT);

    // Turn on one LED
    digitalWrite(LED2, HIGH);
}

// ---------------------------------------------------------------
// Loop runs forever
// ---------------------------------------------------------------
void loop()
{
    // Nothing here for now
}

/*
Program Analysis Activities

1.  The 'included' ARPS2.h header file helps you start programming
    more quickly. It does this by defining each ARPS-2 I/O device,
    and the pin of the Arduino microcontroller that it connects to. 
    
    For example, the program statement:
    
    pinMode(LED2, OUTPUT);
    
    first defines LED2 as an OUTPUT device. Then, the statement:
    
    digitalWrite(LED2, HIGH);
    
    makes the output voltage of the LED2 pin either HIGH (5V), or
    LOW (0V). All of the other ARPS-2 LEDs (LED3 - LED5) can be
    controlled in exactly the same way. Let's try it!

    Add a second LED output statement to light LED3 into your
    program below the existing statement that lights LED2. Run
    the program to verify that it works as expected.

2.  Arduino microcontrollers run every program by reading and
    executing each program statement, from the top to the bottom
    of the program code. 
    
    While the statement that turns LED2 on in this program only runs
    once, most microcontroller programs typically run a group of
    statements over and over again in a program loop. In Arduino C
    code, program statements inside the setup() function run once,
    while program statements in the loop() function are repeated.
    
    In Arduino programs, all program statements must be part of a
    function, and this can be ensured by placing them inside the
    curly braces '{ }' that enclose the function.
    
    In this program, would it make sense to put the digitalWrite
    statement that turns LED2 on inside the loop? Why or why not?
    What would the program be doing if the statement used to turn
    LED2 on was moved into the loop?

3.  Since the loop() function contains no program code, you might
    think it would be possible to simply get rid of it. Instead of
    deleting it (you can try, and we'll add a new loop() in the next
    step), let's just comment-out the loop code using double slashes,
    like this:

// void loop()
// {
    // Nothing here for now
// }

    Double slashes signify the start of a code comment, used by
    programmers to explain the meaning or actions of complex code.
    The C compiler ignores anything in the same line following the
    double slashes, so this code will be 'commented-out', or 
    ignored. Try to compile the program and describe the result.

4.  Let's try to blink LED3 on and off. Replace the contents of the
    main loop() function with two LED statements, one to turn the 
    LED on followed by a second one to turn the LED off, so that it
    looks like this:

void loop()
{
    digitalWrite(LED3, HIGH);
    digitalWrite(LED3, LOW);
}

    The loop will run each statement once, first turning LED3 on,
    and then turning LED3 off. After that, the loop will repeat
    from the top, turning LED3 on and off again... forever.
   
    Run the program and observe LED3. Is LED3 on, off, or flashing?
    What should be happening? How could you test if it actually is?


Programming Activities

1.  The setup() function includes a statement to turn LED2 on. What do
    you think will happen if this statement is immediately followed by
    a second statement to turn LED2 off?

    Try it! Do you see LED2 turning on and then off? Explain what is
    happening and why you think this happens.
    
2.  Predict what will happen if the order of the two LED2 statements
    in activity 1, above, is reversed so that LED2 is turned off first,
    and then turned on.
    
    Try it! Does the state of LED2 match your prediction?
    
3.  Create a program that lights a pattern using at least two of the
    on-board LEDs. Run your program to verify that it works as expected.

4.  The ARPS2.h header file refers to the pin definition for the Arduino
    UNO's on-board LED (it doesn't need to define it since the Arduino
    IDE already knows that it is part of the Arduino UNO circuit.)
    
    To use the Arduino UNO's on-board LED, it must first be configured
    as an output using the pinMode() function:

    pinMode(LED_BUILTIN, OUTPUT);

    Then, it can be turned on using the digitalWrite() function,
    exactly as with the ARPS-2 LEDs:
    
    digitalWrite(LED_BUILTIN, HIGH);

    Add these statements to your program so that the Arduino UNO's LED
    acts as a status indicator to show you that your program is running.
    (It's not a great status indicator, since it's hidden under the 
    ARPS-2 circuit, but you should be able to see it light on the Arduino
    UNO board.) Run the program to verify that it works.

*/