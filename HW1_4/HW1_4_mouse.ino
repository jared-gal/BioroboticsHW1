/*
Jared Gallina
jdg272
Biorobotics HW#1 Q4 Prey Schema

In this file the purpose is to emulate a random wandering prey that is only capable of object
avoidance and not much else

*/

#include "WinkHardware.h"  // Leave this line first. Do not edit this line. This causes Arduino
                           // to include background functions when turning your code into
                           // machine language Wink can understand.


#define BASE_SPEED 30     //base speed of the prey

#define OB_THRESH_L 300   //a parameter that corresponds to detecting an obstacle on the left side

#define OB_THRESH_R 300   //a parameter that corresponds to detecting an obstacle on the right side

//the possible states for the prey, the same as the first few for the predator
#define WANDER 0
#define TURN_AROUND 1
#define VEER_L 2
#define VEER_R 3

//variables relevant to navigating the maze
float rightLine;          //read value of the right inner line sensor
float leftLine;           //read value of the left inner line sensor
int rightMot;             //default value for right motor for wandering
int leftMot;              //default value for left motor for wandering

//variable for what state the robot is in
int STATE;

// Below is the "setup" function. It runs one time as soon as Wink turns on. You can add stuff
// to this function if you want, but hardwareBegin() should always be the first code in the list.

void setup(){
  hardwareBegin();        //initialize Wink's brain to work with his circuitry
  playStartChirp();       //Play startup chirp and blink eyes
  eyesWhite(255);         //turn the eyes to white and all the way on
  rightMot=0;             //initializing to 0
  leftMot=0;              //initializing to 0
  rightLine=0;            //initializing to 0
  leftLine=0;             //initializing to 0
  STATE=WANDER;           //initializing to initial state
  digitalWrite(LineLeftOuter,LOW);       //turn off outer IR light sources
  digitalWrite(LineRightOuter,LOW);
}


// Below is the "loop" function. This is where you'll write most of your code. Whatever is included
// inside the "loop" function will run over and over until Wink runs out of power or you turn him off.

void loop(){
  
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
  
 //next we set some arbitrary speeds for the robot to move in after execution
  rightMot = BASE_SPEED + random(-10,10);
  leftMot = BASE_SPEED - random(-10,10);
  
   //determining if the read values are large enought to trigger state transitions
  //if both sensors on black then we need to turn around
  if((rightLine < OB_THRESH_R) and (leftLine < OB_THRESH_L)) STATE = TURN_AROUND;

  //case two is for right object detection
  else if(rightLine < OB_THRESH_R) STATE = VEER_L;

  //case three is for left obj detection
  else if(leftLine < OB_THRESH_L) STATE = VEER_R;
  
  //default case is to wander
  else STATE = WANDER;
  
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
    //case two robot veers left
    case VEER_L:
      spinLeft(BASE_SPEED);
      delay(100);
      motors(BASE_SPEED, BASE_SPEED);
      break;
    //case three robot veers right
    case VEER_R:
      spinRight(BASE_SPEED);
      delay(100);
      motors(BASE_SPEED, BASE_SPEED);
      break;
  }
  
 
  delay(30);        //wait .03 second
  
} //closing curly of the “loop()” function




