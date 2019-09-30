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
//constants relevant to movement speed
#define CHASE_SPEED 50   //a max additional speed for the robot when chasing prey

#define BASE_SPEED 50     //base speed for the robot to travel with


//constants relevant to light sensors
#define DETECT_THRESH 30 //a parameter that represents ambient light levels coincident with another robot
                          //being "visible"

#define CAUGHT_THRESH 205 // a parameter that represents when ambient light levels are high enough to 
                          //determine a prey is "caught"
                          
#define LIGHT_WEIGHT_L .03   //a parameter that corresponds to a weight for left motor values

#define LIGHT_WEIGHT_R .02   //a parameter that corresponds to a weight for right motor values


//constants relevant to object/barrier detection
#define OB_THRESH_L 800   //a parameter that corresponds to detecting an obstacle on the left side

#define OB_THRESH_R 700   //a parameter that corresponds to detecting an obstacle on the right side


//defining the states of operation

#define WANDER 0
#define TURN_AROUND 1
#define VEER_L 2
#define VEER_R 3
#define DETECTED 4
#define CAUGHT 5



//*****************************************************************************************************
                          
//below are the relevant variables to this program
//predator shema variables
float rightLight;         //read value of the right light sensor
float leftLight;          //read value of the right light sensor

//variables relevant to navigating the maze
float rightLine;          //read value of the right inner line sensor
float leftLine;           //read value of the left inner line sensor
int rightMot;
int leftMot;

//variable for what state the robot is in
int STATE;


//*****************************************************************************************************
//helper funtions for operation
//this function takes in the light sensor values and then operates the wink
//like a Braitenberg Vehicle 2b to chase the prey 
void BV_2b(int left,int right){
  //BV 2b has a crossed positive relation between sensor and motor
  //we want to keep the motor scaled to values between 0 and CHASE_SPEED + BASE_SPEED 
  left = left*LIGHT_WEIGHT_L*CHASE_SPEED + BASE_SPEED;
  right = right*LIGHT_WEIGHT_R*CHASE_SPEED + BASE_SPEED;
  motors(right,left);
  return;
}


void setup(){
  hardwareBegin();        //initialize Wink's brain to work with his circuitry
  playStartChirp();       //Play startup chirp and blink eyes
  rightLight =0;          //initializing to zero
  leftLight =0;           //initializing to zero
  rightLine =0;           //initializing to zero
  leftLine =0;            //initializing to zero
  digitalWrite(LineLeftOuter,LOW);       //turn off outer IR light sources
  digitalWrite(LineRightOuter,LOW);
  STATE = WANDER;
 
}

//*****************************************************************************************************
/* Inside the loop the objective is to manage the robot to first detect whether or not ther is a wall,
then if no wall it should check for obstacles, if no obstacles then there are two potential action archetypes 
that may be running. The first is called predator protocol, which is in effect from the detection of another robot
until the robot is "caught" or it escapes. Predator protocol is essentially BV 2b. The second is called explore where the robot will just randomly move 
about the environment in search of prey. 
*/

void loop(){

  //The first step of the loop is to find the measure of all sensors to determine what state should be entered
  //process for reading the line sensors, this process is from the provided example code for the wink
  //module, specifically Wink CH14 BottomSenseBasics_EX01 
  digitalWrite(LineLeftInner,LOW);
  digitalWrite(LineRightInner,LOW);
  delayMicroseconds(300);
  leftLine = analogRead(LineSenseLeft);
  rightLine = analogRead(LineSenseRight);
  digitalWrite(LineLeftInner,HIGH);
  digitalWrite(LineRightInner,HIGH);
  delayMicroseconds(300);
  leftLine = analogRead(LineSenseLeft) - leftLine;
  rightLine= analogRead(LineSenseRight) - rightLine;
  digitalWrite(LineLeftInner,LOW);
  digitalWrite(LineRightInner,LOW);
  //end of reading line sensors

  //next is to read the Ambient Sensors 
  rightLight = analogRead(AmbientSenseRight);
  leftLight = analogRead(AmbientSenseLeft);

  //next we set some arbitrary speeds for the robot to move in after execution 
  rightMot = BASE_SPEED + random(-20,20);
  leftMot = BASE_SPEED - random(-20,20);

  //determining if the read values are large enought to trigger state transitions
  //if both sensors on black then we need to turn around
  if((rightLine < OB_THRESH_R) and (leftLine < OB_THRESH_L)) STATE = TURN_AROUND;
  
  //case one is for right object detection
  else if(rightLine < OB_THRESH_R) STATE = VEER_L;

  //case two is for left obj detection
  else if(leftLine < OB_THRESH_L) STATE = VEER_R;

  //case of prey being caught
  else if((rightLight+leftLight) > CAUGHT_THRESH) STATE = CAUGHT;
  
  //case of prey detection
  else if((rightLight+leftLight) > DETECT_THRESH) STATE=DETECTED;

  else STATE = WANDER;

  //performing functions based on previous readings
  //if the robot detects a wall or has come directly into contact with an object
  //the result is to simply spin in place before heading off in new direction

  switch(STATE){

    //case zero, the robot wanders
    case WANDER:
      motors(leftMot, rightMot);
      break;
    //case one, robot turns around
    case TURN_AROUND:
      spinLeft(BASE_SPEED);
      delay(300);
      motors(BASE_SPEED, BASE_SPEED);
      break;
    //case two, robot turns left
    case VEER_L:
      spinLeft(BASE_SPEED);
      delay(100);
      motors(BASE_SPEED, BASE_SPEED);
      break;
    //case three, robot turns right
    case VEER_R:
      spinRight(BASE_SPEED);
      delay(100);
      motors(BASE_SPEED, BASE_SPEED);
      break;
    //case four, robot detects prey
    case DETECTED:
      BV_2b(leftLight, rightLight);
      break;
    //case five, robot has caught prey
    case CAUGHT:
      beStill();
      eyesPurple(255);
      delay(5000);
      eyesOff();
      break;
    }

  delay(50);


} //closing curly of the “loop()” function




