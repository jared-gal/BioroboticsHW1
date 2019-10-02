/*
Jared Gallina
jdg272
Biorobotics HW #1 Q4 Calibration

The purpose of this file is to simply measure the ambient light values to clibrate the cat program
*/

//*****************************************************************************************************
#include "WinkHardware.h"  // Including basic hardware definitions for working with the 
                           // wink device

//*****************************************************************************************************
                          
//below are the relevant variables to this program
//light sensor values
float rightLight;         //read value of the right light sensor
float leftLight;          //read value of the right light sensor


void setup(){
  hardwareBegin();        //initialize Wink's brain to work with his circuitry
  playStartChirp();       //Play startup chirp and blink eyes
  rightLight =0;          //initializing to zero
  leftLight =0;           //initializing to zero
 
}

//*****************************************************************************************************
/* Inside the loop the objective is to manage the robot to first detect whether or not ther is a wall,
then if no wall it should check for obstacles, if no obstacles then there are two potential action archetypes 
that may be running. The first is called predator protocol, which is in effect from the detection of another robot
until the robot is "caught" or it escapes. Predator protocol is essentially BV 2b. The second is called explore where the robot will just randomly move 
about the environment in search of prey. 
*/

void loop(){

  //measure the light values 
  rightLight = analogRead(AmbientSenseRight);
  leftLight = analogRead(AmbientSenseLeft);

  //display the measured values to the screen
  Serial.print("Left: "); Serial.print(leftLight); Serial.print("Right: "); Serial.println(rightLight); 

  delay(50);


} //closing curly of the “loop()” function



