/*
Jared Gallina
jdg272
Biorobotics HW #1 Q4 Predator schema

The objective of this program is to create a schema that emulates a predator like 
behavior upon detecting another robot, but will otherwise roam the environment 
"searching" for prey
*/

//*****************************************************************************************************
#include "WinkHardware.h"  // Including basic hardware definitions for working with the 
                           // wink device

#define SENSOR_MOD 0.125   //a modification parameter for a read sensor value to ensure that it is
                          // scaled to a more appropriate level

#define BASE_SPEED 80     //base speed for the robot to travel with

#define DETECT_THRESH 200 //a parameter that represents ambient light levels coincident with another robot
                          //being "visible"

#define CAUGHT_THRESH 400 // a parameter that represents when ambient light levels are high enough to 
                          //determine a prey is "caught"

#define WALL_THRESH 30   //a parameter to determine when the wink has encountered a wall or not

#define OB_THRESH_L 200   //a parameter that corresponds to detecting an obstacle on the left side

#define OB_THRESH_R 200   //a parameter that corresponds to detecting an obstacle on the right side

//*****************************************************************************************************
                          
//below are the relevant variables to this program
//predator shema variables
float rightLight;         //read value of the right light sensor
float leftLight;          //read value of the right light sensor
boolean caught;           // whether or not the prey is caught
boolean detected;         //whether or not a prey is detected

//variables relevant to navigating the maze
float rightLine;          //read value of the right inner line sensor
float leftLine;           //read value of the left inner line sensor
int rightMot;
int leftMot;
float wallS;              //read value of the headlight sensor
boolean veer_l;           //whether we need to veer left
boolean veer_r;           //whether we need to veer right
boolean wall;             //presence of a wall in front of the robot


//*****************************************************************************************************

void setup(){
  hardwareBegin();        //initialize Wink's brain to work with his circuitry
  playStartChirp();       //Play startup chirp and blink eyes
  eyesPurple(255);        //set the initial eye color to purple
  rightLight =0;          //initializing to zero
  leftLight =0;           //initializing to zero
  rightLine =0;           //initializing to zero
  leftLine =0;            //initializing to zero
  caught = false;         //initializing to false
  detected = false;       //initializing to false
  veer_l = false;         //initializing to false
  veer_r = false;         //initializing to false
  wall = false;           //initializing to false
  
    
}

//*****************************************************************************************************
/* Inside the loop the objective is to manage the robot to first detect whether or not ther is a wall,
then if no wall it should check for obstacles, if no obstacles then there are two potential action archetypes 
that may be running. The first is called predator protocol, which is in effect from the detection of another robot
until the robot is "caught" or it escapes. Predator protocol is essentially BV 2b. The second is called explore where the robot will just randomly move 
about the environment in search of prey. 
*/

void loop(){

  digitalWrite(Headlight,HIGH); //start sensing headlight
  //The first step of the loop is to find the measure of all sensors to determine what state should be entered
  rightLight = analogRead(AmbientSenseRight);
  leftLight = analogRead(AmbientSenseLeft);
  rightLine = analogRead(LineRightInner);
  leftLine = analogRead(LineLeftInner);
  wallS = analogRead(AmbientSenseCenter);

  //next we set some arbitrary speeds for the robot to move in after execution 
  rightMot = BASE_SPEED + random(20);
  leftMot = BASE_SPEED + random(20);

  //determining if the read values are large enought to trigger the given responses
  //case one is for the wall sensing 
  if(wallS > WALL_THRESH) wall = true;
  else wall = false;

  Serial.print(wall);
  //case two is for right object detection
  if(rightLine > OB_THRESH_R) veer_l = true;
  else veer_l = false;
 
  //case three is for left obj detection
  if(leftLine > OB_THRESH_L) veer_r = true;
  else veer_r = false;

  //case of prey detection
  if((rightLight+leftLight) > DETECT_THRESH) detected = true;
  else detected = false;

  //case of prey being caught
  if((rightLight+leftLight) > CAUGHT_THRESH) caught = true;
  else caught = false;

  //performing functions based on previous readings
  //if the robot detects a wall or has come directly into contact with an object
  //the result is to simply spin in place before heading off in new direction
  if(veer_l){
    spinLeft(50);
    delay(2);
  }
  else if(veer_l and veer_r){}

/*
  //ensuring the motor speeds are not negative so the vehicle just stops rather than 
  //constantly oscillating between forward and back damaging the motors potentially
  rightLightSensor = max(0,rightLightSensor);
  leftLightSensor = max(0,leftLightSensor);

  //after finding the sensor values they can then be written to the motors as 
  //desired speeds of operation
  motors(((int) leftLightSensor), ((int) rightLightSensor));
  */
  //a simple wait at the end of the loop so that the robot isn't acting too spastically
  motors(leftMot, rightMot);
  delay(100);


} //closing curly of the “loop()” function






