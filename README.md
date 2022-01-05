# DC Motor Control

<p align = "center">
    Avery Clowes, Grant Miner, Annika Pfister
</p>
<p align = "center">
    October 22, 2021
</p>

![Completed Prototype](/images/prototypeOverRight.jpg)

## 1 Introduction

The primary goal of this project was to create a line following robot that completes a closed loop
course as fast as possible. Our group was given a set of DC motors, infrared reflectivity sensors, and a
mounting board. To bring these components together, we used an Arduino Uno to read sensor data,
a motor shield board to control the motors, a breadboard to electrically connect to the sensors to the
Arduino with appropriate resistors, and a sensor mount to optimally position the sensors. The entire
assembly was run by code on the Arduino.

Our robot used three calibrated reflectivity sensors to check for the black electrical tape line on the
tile floor. The values they returned to the Arduino through analog input ports helped guide the robot
along the path without getting stuck. It only stopped when it reached a line of tape that crossed the
main course. It successfully drove around the entire course in both directions without error.

The code written for this robot also allows for anyone connected via a serial port to update the analog
threshold interpreted by the Arduino as the point at which the black electrical tape is detected. Moving
this threshold above the point where the black tape is first detected can drastically change the function
and path of the car because the analog values read from the reflectivity sensors can change significantly
due to other lighting conditions or dust on the surfaces.

![Example Robot Path](/images/path.jpg)

<p align = "center">
    Figure 1: Example course with start/edn cross indicator. 
    Dashed square indicates relative robot size.
</p>

## 2 Mechanical Design and Integration

The sensors we used for this project were TCRT5000s, each one composed of a infrared diode and phototransistor. For optimal performance, the datasheet suggested a 2mm working distance from the object to the sensor face. To accomplish this we designed and 3D printed a sensor mount. It has a series of mounting holes along the bottom edge to allow for testing different positions of the sensors. It also has a set of alignment holes at the top for integration with the existing holes in our mounting plate.

The extra mounting holes proved to be very helpful when implementing the robot, as we decided to add a third sensor in between with opposite logic from the other two (to keep the sensor on the line, instead of off of it). We were able to easily move the sensors outward along the mounting holes to change the robot’s behavior, though we realized that keeping them closer together actually improved the function of the robot and allowed the robot to move faster. 

Our mechanical integration also allowed for rapid changes in the chassis design, and the flexibility of our mounting proved more helpful than expected. We were able to very easily remove the sensor mount to add or rewire sensors without having to remove any screws or bolts. The same was true of our breadboard, especially when we decided to add a third reflectivity circuit to our robot. Below in figure 2, the mount CAD is shown, as well as the integration with the robot.

To assemble our chassis we followed the design instructions from the kit. Importantly though, we incorporated a series of mounting and strain relief methods do decrease the wear on our components. When mounting our motors, we used zip ties that held the power wires to the DC motors by their insulation. This decreased the strain on the solder joints, hopefully improving overall reliability. We also used a series of wires threaded through the chassis holes and into the mounting holes on the Arduino to hold them in place. This allowed us to easily change orientation if necessary.

## 3 Electrical Design

![Circuit Diagrams for sensors and motors](/images/)

We used a breadboard to assemble our sensor circuit, while the DC motors were connected directly
to the Adafruit v2.3 Motor Shield mounted in header pins on the Arduino. DC motors were powered
externally via a 12V power supply connected to the motor shield.

### 3.1 Calibration

To calibrate our infrared sensors, we took five measurements on three different surfaces: electrical tape,
the tile floor, and a wood table. We averaged these values and used them to inform our threshold,
such that it fell between expected readings for the tile floor (maximum 130) and the electrical tape
(minimum 600.2), thus allowing the robot to clearly differentiate between the two surfaces.

|      | Left (A1) | Right (A2) | Center (A3) |
|:-----| :-------: | :--------: | :---------: |
|Electrical Tape | 623.6 | 652.8 | 600.2 |
|Tile Floor | 54.2 | 130 | 56|
|Wood Table | 38.8 | 39.8 | 39|

## 4 Controller and Code Outline

Originally, we had planned to control the Arduino from a Python script, and only control the motor
speeds and read from the infrared sensors from the Arduino. We planned to pass all of this information
to the Python script, and use Python to determine what the robot should do next. We ran into issues
with this approach because both the Arduino and the Python code were trying to write and read from
the serial port at the same time.
After attempting to debug this problem for an extended amount of time, we discovered the input box
in the Arduino’s serial monitor that allows data to be sent to serial by a user. We quickly pivoted, and
we rewrote our entire logic and wheel command codebase in Arduino, abandoning Python completely.
This choice saved us great amounts of time by reducing debugging and overall complexity.
Our new controller uses the analog inputs read from the three sensors to determine the robot’s orientation
in relation to the line, all in Arduino code.

**Once the robot begins moving, we check each of the following cases continually in our control loop.**

<ol>
    <li>Left sensor detects line, so robot turns left wheel backward and right wheel forward to move the left sensor away from the line.</li>
    <li>Right sensor detects line, so robot turns right wheel backward and left wheel forward to move the right sensor away from the line.</li>
    <li>Only the middle sensor detects the line, so both wheels turn forward and continue moving along the path.</li>
    <li>None of the sensors detect the tape, so the robot runs the wheels in opposite directions until it intersects the line again.</li>
    <li>All sensors detect the tape, so the Arduino understands that it has hit the start/stop line, and stops the two motors.</li>
</ol>

All of these recovery modes rely on the Arduino’s computational speed. Because the Arduino can
catch the cases the moment they occur, such as when the entire vehicle leaves the line, it is able to
correct itself rapidly.

An interesting edge case is when the robot has left the line, and goes into its rotation mode to find
the line again. No matter which side sensor hits the line first, the program will continue to rotate in
that direction until the sensor no longer touches the line. This design prevents the robot from getting
stuck if it is to leave the line, though it will get stuck if started far off of the line. The code only spins
the robot when all sensors do not see the tape, so if the tape is not within the sensor’s path, it will
get stuck in a loop. However, if the robot encounters this problem in certain circumstances, it may
reverse directions on the loop. There does not seem to be any way to predict its reversal, as this error
only happened once among a large number of test runs.


Our code takes the form of a main control loop and a few helper functions to supplement it, which also
make the code more readable and understandable. The main module is the Arduino loop() function,
which calls the other functions to determine whether it needs to change the motor speeds. The complete
code is available in the appendix.

## 4.1 Code Breakdown

The Arduino implements four main functions when it is driving untethered (not connected to a laptop):
setup(), loop(), movementLogic(), and seesBlack(). The first function, setup() is included in every
Arduino program. It sets initial values and connects variables to the pins they represent. It also sets
the motors to initial speeds. Given its ubiquity, we will not explore it in this section.

To start with the helper functions, seesBlack() reads the sensor values and determines whether each
sensor sees the electrical tape. The code below reads the analog value from a sensor pin and saves it
to an integer, after which the value is checked against the threshold value. It then resets the boolean
to zero, so that if it does not see black, the value is already set to zero. Lastly, the analog value is
compared to the threshold, and it is only set to true if the sensor sees the color black.

~~~
int left_analog = analogRead(IR1PIN) ;
lb = 0 ; // set to "False"
if (left_analog > threshold) { lb = 1 ; }
~~~

Next comes determining what the robot should do. Our seesBlack() code does not return any values.
Instead, it updates global variables for the various booleans each time it runs. The next function,
movementLogic() takes these booleans as inputs, as well as current speeds for the motors, and the
various speeds the motor may take. Refer to the appendix for the complete case logic or see the
diagrams and explanations in the previous section.

The code below demonstrates the base case where the center sensor sees the black tape, but the left
and right sensors both do not detect it. In this case, both motors are set to the ”cruise” speed. Other
speed options include ”slow,” for the motor to slow and let the other turn it until it is aligned with
the track again. ”Catchup” used to be a speed faster than ”cruise,” but we determined that in the
turning state, having both motors turn at the same speed worked perfectly well. ”stop speed” is the
speed the motor takes on when it wants to stop. This is defaulted to zero, but if the user does not
want the robot to stop at the end of the track, the number can be made to be positive.

~~~
if (!lb && !rb) {
    ls = cruise;
    rs = cruise;
}
~~~
This case checking is repeated for all of the other possible cases, updating the global variables for ls
and rs, or left speed and right speed, respectively.
Finally comes the loop() function, which runs all of these helper functions together. It also deals
with the actual setting of the motors. Below is a basic method for setting the speed of the left motor
only.

~~~
seesBlack() ;
movementLogic(lb, rb, cb, threshold, cruise, slow, catchup);
motorLeft−>run(RELEASE); // stop motor

if (ls > 0) {
    motorLeft−>run(FORWARD);
}
else {
    motorLeft−>run(BACKWARD);
}

motorLeft−>setSpeed(abs(ls));
~~~

The function runs the two helper functions first, to determine what the motors should do. After
this, it releases the left motor in case it needs to be run backwards. It then determines whether the
speed sent to it by movementLogic is greater than or less than zero, changing the motor’s direction
appropriately.

The last step is setting the motor’s speed to the absolute value of ls, ensuring it rotates at speed ls
but with the direction already determined. This code was implemented after bugs were encountered
when trying to pass negative values to the motors’ setSpeed() function.

The last function in our code is the parameter passing function, recvWithStartEndMarkers(). It was
taken from an Arduino forum post, with minor modifications.(1) This function receives data from
serial by looking for start and end markers, at which point it is parsed and used to change parameters
in the code, *without ever recompiling or stopping the Arduino.*

The function first reads a character from the serial input. It waits until that character is the start
character it is programmed with. It then makes a list including all of the characters received from
serial until it receives the end character. The code below demonstrates the final assignment to the
threshold parameter.

~~~
if (rc == endMarker) { // end of data
    \\ split list at commas
    strtokIndx = strtok(receivedChars, ”,”) ;
    threshold = atoi(strtokIndx); // converts to integer
}
~~~

After it terminates reading characters, it makes sure that the final character is the end character, then
turns the list into a string by splitting it at the commas. Finally, it turns the string into an integer
and sets the threshold equal to that integer. All of this is performed only when there is serial data
to read, so the code runs without any serial input going about its course, until the threshold value is
changed.

For example, if ”<700 >” is written to serial through the input field in the serial monitor, the Arduino
will immediately update the threshold value from its default of 500 to the new 700 values, and continue
trying to solve the course with this new value as a guide.

As discussed earlier, the threshold should be less than 600 to ensure that the sensors see the tape.
Changing the value to 700 may result in the robot ignoring all of the lines completely, or continuing
on unaffected, depending on lighting conditions and other factors.

In future iterations we would like to add the opportunity to update other parameters while the robot
moves through the course, such as the cruise speed. 