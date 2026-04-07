/*
Project:  ARPS-Intro-5-Analog-Input
Activity: mirobo.tech/arps-intro-5
Date:     February 4, 2025
 
This introductory programming activity for the mirobo.tech ARPS2 circuit
demonstrates analog input and serial output for data output or to assist
in debugging. This activity requires either the temperature sensor or the
voltage divider resistors R20 and R21 to be installed in ARPS2. 

Additional program analysis and programming activities examine temperature
calculations, float numeric types, and conversion between numeric types.

See the https://mirobo.tech/arps website for additional ARPS2
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

// ARPS2 optional analog input devices
const int Q1 = A0;      // Left floor/line sensor phototransistor
const int Q2 = A1;      // Right line sensor phototransistor
const int Q3 = A1;      // Right floor sensor phototransistor
const int ATEMP = A2;   // Analog temperature sensor U1
const int ADIV = A3;    // Analog voltage divider (R20/R21) tap
const int AH1 = A4;     // Analog input from header H1 (shared with D18)
const int AH4 = A5;     // Analog input from header H4 (shared with D19)

// Define program variables
int SW2State;
int SW3State;
int SW4State;
int SW5State;

int dec0;           // Decimal number digit 0 - ones digit (LSB)
int dec1;           // Decimal number digit 1 - tens digit
int dec2;           // Decimal number digit 2 - hundreds digit
int rawTemp;        // Raw/analog temperature conversion result 
int rawVolts;       // Raw/analog voltage divider conversion result
int tempDegC;       // Temperature in degrees C
float volts;        // Voltage divider output voltage

void setup() {
  Serial.begin(9600);   // Start the serial port at 9600 bps for UNO R3
  // Serial.begin(115200); // Start the serial port at 115200 bps for UNO R4

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

  pinMode(ATEMP,INPUT);
  pinMode(ADIV,INPUT);

  tone(BEEPER,4000,100);  // Say hello!
}

void loop() {
  // Read temperature input
  rawTemp = analogRead(ATEMP);
  
  // Send results to the serial monitor
  Serial.println(rawTemp);

  delay(1000);
}

// Convert an 8-bit binary number to 3 decimal digits
void binToDec(unsigned char bin)
{
  dec0 = bin;             // Store number in ones digit
  dec1 = 0;               // Clear tens digit
  dec2 = 0;               // Clear hundreds digit
  
  // Count hundreds digits in dec2 variable
  while(dec0 >= 100) {
      dec2 ++;
      dec0 = dec0 - 100;
  }
  // Count tens digits in dec1 variable, dec0 will contain remaining ones
  while(dec0 >= 10) {
      dec1 ++;
      dec0 = dec0 - 10;
  }
}

/* Learn More -- Program Analysis Activities

1.  Let's use this program to learn how to process analog data from
    a temperature sensor or a voltage divider (to do this, your ARPS2
    needs ot have U1 installed to measure the temperature, or have
    resistors R20 and R21 installed to measure an external potential
    applied to the battery input of the screw terminal strip). Arduino
    boards provide an incredibly useful debugging tool in the form of
    a serial port (passed through the USB connection), which allows
    their microcontroller to send data to the Arduino IDE.

    To start and configure the serial port, a new function call has
    been added to the setup() function of this program:
 
  Serial.begin(9600);   // Start the serial port at 9600 bps for UNO R3

    The number inside the brackets (9600) refers to the speed, in bits
    per second, of the data that will be sent through the serial port.
    Both the microcontroller and the Arduino IDE have to be set to the
    same data rate, commonly called 'baud' rate, for the two to be able
    to communicate successfully. The Arduino UNO R3 should use this
    relatively slow rate. The newer microcontrollers used in the
    Arduino UNO R4 family can be set to communicate at 115200 bits/s
    (baud), over 10x faster than its older sibling.
    
    How can we use the serial data? The main loop() of the program
    consists of the following statements:

  // Read temperature input
  rawTemp = analogRead(VTMP);

  // Send results to the serial monitor
  Serial.println(rawTemp);

  delay(1000);

    The 'analogRead()' function converts the voltage on the selected
    ADC (analog-to-digital converter) pin to a numeric equivalent,
    and the 'Serial.println(rawTemp);' statement prints this value to
    the serial port for display in the Arduino IDE.

    Build and upload the program into your circuit, and then click on
    the Serial Monitor icon at the top right of the Arduino IDE screen.
    A Serial Monitor tab should open beside the output tab at the bottom
    of the Arduino IDE screen, and a new number representing the ADC
    conversion result should appear in the Serial Monitor every second.

    What are your results? Grab and hold the temperature sensor with
    your fingers to warm it up. How do your results change? Try it
    with the Serial Plotter as well. How does it show the results?

2.  The ADC in the Arduino UNO quantizes input voltages between 0V and
    5V into 10-bit binary values representing the input voltage. How
    many different states or numbers can be represented using ten
    binary digits? What range of numbers do you expect the temperature
    (or any other ADC conversion result) to be within?

3.  The sensitivity of an ADC circuit can be expressed as the amount
    of input potential corresponding to a single bit change in the
    output result. For the Arduino UNO family, the input voltage range
    is 0V to 5V, and you know the number of output states (from above).
    
    Calculate the ADC sensitivity, in units of mV/bit, by dividing the
    input voltage range by the number of output states. It tells you
    how much the input must change by to get a single digit output
    change.

4.  The analog temperature sensor used in the ARPS2 circuit has a
    temperature coefficient of 10mV/°C. This means that its output
    voltage will increase by 10mV for every 1°C rise in temperature.
    The sensor also an output voltage offset to allows its minimum
    measurable temperature to be less than 0°C.
    
    In order to obtain a useful temmperature from it, we will need to
    develop an algrithm that factors in both its temperature
    coefficient as well as its voltage offset. The first part, its
    temperature coefficient, will need to be scaled to the ADC's
    sensitivity (the mV/bit value calculated above). The scale factor
    can be obtained by dividing the sensor's temperature coefficient
    by the ADC sensitivity:

    10mV/°C / mV/bit 

    Try this with your calculated ADC sensitivity. The result you 
    get should be close to 2. This means that for every 1°C change
    in temperature, there will be an approximately 2-digit change in
    the ADC output value. Or, expressed another way, every single-
    digit change in the ADC value corresponds to roughly 0.5°C of
    temprature change.

    Next, we can factor in the voltage offset. It should be as simple
    as subtracting an offset value from the raw data before scaling
    the data. Using a single offset value will provide a result that
    is close to the actual temperature over a reasonable range of
    inputs. For increased accuracy it is usually necessary to
    individually calibrate each circuit over a range of expected 
    temperatures and accurately scale both outputs and offsets, but
    we don't need to go to that effort here.

    For the MCP9700A temperature sensor shown in the ARPS2 schematic, 
    and when used with Arduino UNO R3, try a value of 100 for the 
    offset. For Arduino UNO R4, try 127 to start.
    
    If you're simulating the APRS circuit using Tinkercad, the offset
    required for the TMP36 temperature sensor is easy to find. Simply
    adjust the temperature to 0°C (after clicking on the sensor), and
    then use the raw conversion result obtained as the offset!
    
    Add the temperature conversion calculation, below, to the program
    right below the the analogRead() function:
 
  tempDegC = (rawTemp - 100) / 2;   // Based on MCP9700A and Arduino UNO R3

    Next, replace the single Serial.println() function with the
    following lines:

  Serial.print("rawTemp: ");
  Serial.print(rawTemp);
  Serial.print("\t");
  Serial.print("Temp: ");
  Serial.print(tempDegC);
  Serial.println("C");

    Rather than just outputting a number, these functions build a
    line of text, starting with the string "rawTemp: ", and adding
    the value of temperature variable after it. The '\t' character
    adds a tab to space elements apart, and the very last statement,
    Serial.println("C"); adds a new line after the current line of
    text (this is represented by the 'ln' at the end of 'println').

    Upload the program to your APRS, and adjust the voltage offset in
    your program to calibrate your sensor to the current temperature.
    Then, heat and cool your sensor to see its response.

5.  The values of the voltage divider resistors in the ARPS2 circuit
    were chosen so that input voltages of greater than 5V will be
    reduced to less than 5V in order to prevent damage to the ADC.
    Calculating the battery voltage can be done in a manner similar
    to the previously developed algorithm for temperature but, in this
    case, it needs to factor in the voltage divider ratio. The output
    of the voltage divider resistors can be calculated using the
    voltage divider formula:

    Vout = Vin * ( Rout / ( Rout + Rdiv ))

    Assuming the use of 1.24k and 5.1k voltage divider resistors, as
    shown in the ARPS2 schematic diagram, and a voltage input of 6V,
    the output across the 1.24k output resistor (Rout) will be 1.173V
    (safely within the 5V maximum ADC input value), as shown below:

    1.173V = 6V * (1.24 / (1.24 + 5.1 ))

    Since the resistor values are fixed, an input voltage will be
    scaled in the same way, and using just the resistor values from
    the equation above will provide the voltage divider ratio:

    Vdiv-ratio = (1.24 / ( 1.24 + 5.1 )) = 0.196

    Every input voltage will be scaled to roughly 1/5 of its value,
    allowing voltages of up to approximately 25V to be measured by
    ARPS2. Knowing this, the ADC output value corresponding to any
    input voltage should be able to be determined by scaling the input
    by the voltage divider ratio, and then dividing by the ADC
    sensitivity, like this:

    ADCvalue = Vin * Vdiv-ratio / ADC sensitivity

    With a 6V input to ARPS2, and the calculated voltage divider ratio
    and ADC sensitivity calculated earlier, you should be able to
    determine that the output 240 will be used to represent to 6V.

    Knowing the real input voltage and having determined its output
    value, it will be easy to find a scale factor that converts the
    ADC output to an actual voltage:

    Vdiv scale = ADC output / ADC input (V)
               = 240 / 6V
               = 40

    Let's try it in the program. Add the following lines to the loop()
    function of your program (before the Serial output statements).

  // Read voltage divider and convert to volts
  rawVolts = analogRead(ADIV);
  volts = float(rawVolts) / 40;

    Add this new code to your program, and also add instructions to
    print the rawVolts and volts variables to the serial monitor. To
    take actual voltage measurements, your ARPS2 and Arduino will 
    need to be powered using an external power supply connected to
    the DC inputs of the ARPS2 screw terminal strip. Do that now, and
    then upload the program to view the results. Does the voltage
    reading in the Serial Monitor match the input voltage?

6.  Whether or not your ARPS2 has its voltage divider resistors
    installed, the previous example includes an important numeric
    type conversion statement that it would be good to understand.
    To start, comment out the line that assigns rawVolts from the
    ADC, and add a new line to assign a fixed value of 238 below it: 

  // rawVolts = analogRead(ADIV); 
  rawVolts = 238;
  volts = float(rawVolts) / 40;

    Run the program. The output in the Serial Monitor should read
    5.95 volts.
     
    Next, remove the float conversion function from the volts
    calculation so that the line reads like this:

  volts = rawVolts / 40;

    Run the program again. What is the voltage output now?
    
    The 'volts' variable was defined as a 'float' type at the top of
    the program. What variable type is the 'rawVolts' variable? Do 
    you think the number 40 is the same type? What type will the
    result of a division of these two numeric types be?

    To ensure the accuracy of the result in this case, the dividend
    (rawVolts) must become a float type before division, as was done
    with the float() conversion function. Return the float() function
    to the program and ensure it works properly again.

7.  This program contains a binToDec() function that demonstrates a
    simple method of converting an 8-bit binary value into three,
    separate decimal digit variables. A function like this might be
    useful if a value needs to be displayed as individual digits on
    a 7-segment display, for example.

    Can you figure out how the function works? Explain the function,
    or create a flow chart to describe its operation.

Programming Activities

1.  Create a program that uses the binToDec() function to convert
    the temperature into three separate digits and then displays the
    digits in succession on as binary numbers on ARPS2's LEDs.

2.  Extend the binToDec() function so that it is able to convert a
    larger integer, such as the 10-bit ADC result, into four decimal
    digits.

3.  Analog input values above or below chosen thresholds can be used
    to trigger other actions, or to provide very simple indications
    of state. One of the features of the ARPS2 circuit is its break-
    away optical floor sensor modules which are designed to make it
    easy for beginners to build it into a simple floor sensing or
    line-following robot.

    Create two new program functions, one each for the left and right
    floor sensors, that will return a Boolean state representing the
    status of that floor sensor to the calling code. For example,
    a function to determine if the left floor sensor detects the
    presence of the floor could be implemented like this:

    if leftFloor() == true ...

4.  Create a function that could be used by a robot control program
    to determine when its battery voltage drops below a critical
    threshold. As an exammple, a Sumo robot powered by 4-AA batteries
    (nominally producing 6V) could alert its human handler when the
    battery voltage drops below 5.6V, limiting its ability to push
    an opponent out of the Sumo ring. 

5.  Analog inputs can be used to control analog output devices, either
    directly, or after processing the input value in some way. For
    example, a potentiometer connected to one of ARPS2's header pins
    could be used to generate variable-width PWM pulses to control
    the brightness of LEDs, or the speed of a motor.

    An Arduino function that can be very helpful in implementing
    algorithms that need to convert values between different analog
    ranges is the map() function. Look up the map() function in the
    Arduino documentation to see how it can be used.
    
    Use the map() function to create an algorithm to control the speed
    of ventillation fans in a greenhouse. The greenhouse owner has
    installed a temperature sensor in the greenhouse and would like
    to have the ventillation fans running at 10% speed when the
    inside temperature reaches 20°C, and ramp up to 100% speed at a
    temperature of 28°C. This means the fans should be running at
    approximately 2/3 speed (66%) at a temperature of 25°C.

6.  While a simple line-following robot can be made by sensing floor
    phototransistors and activating motors digitally (i.e. if the line
    is present, turn a motor on), a much better way to control a line-
    following robot is by using analog control. The amount of light
    reflected by the floor (and line) can be read as analog values
    from the two line/floor sensor phototransistors, and these values
    can then be converted to become analog motor speed values for the
    robot's left and right motors.
    
    Plan, implement, and test a line-following robot program. You will
    need to consider what happens when neither line sensor detects the
    line, and you might want to implement some sort of sensor
    calibration if the left and right line sensor phototransistors
    give different results.
*/
