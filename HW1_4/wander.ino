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


#define BASE_SPEED 30
#define OB_THRESH_L 300   //a parameter that corresponds to detecting an obstacle on the left side

#define OB_THRESH_R 300   //a parameter that corresponds to detecting an obstacle on the right side

#define WANDER 0
#define TURN_AROUND 1
#define VEER_L 2
#define VEER_R 3

//variables relevant to navigating the maze
float rightLine;          //read value of the right inner line s\
ensor
float leftLine;           //read value of the left inner line se\
nsor
int rightMot;
int leftMot;

//variable for what state the robot is in
int STATE;

// Below is the "setup" function. It runs one time as soon as Wink turns on. You can add stuff
// to this function if you want, but hardwareBegin() should always be the first code in the list.

void setup(){
  hardwareBegin();        //initialize Wink's brain to work with his circuitry
  playStartChirp();       //Play startup chirp and blink eyes
  eyesWhite(255);
  rightMot=0;
  leftMot=0;
  rightLine=0;
  leftLine=0;
  STATE=WANDER;
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

  //case one is for right object detection
  else if(rightLine < OB_THRESH_R) STATE = VEER_L;

  //case two is for left obj detection
  else if(leftLine < OB_THRESH_L) STATE = VEER_R;
  
  else STATE = WANDER;
  
  switch(STATE){

    //case zero, the robot wanders
    case WANDER:
      motors(leftMot, rightMot);
      break;
    case TURN_AROUND:
      spinLeft(BASE_SPEED);
      delay(300);
      motors(BASE_SPEED, BASE_SPEED);
      break;
    case VEER_L:
      spinLeft(BASE_SPEED-20);
      delay(100);
      motors(BASE_SPEED, BASE_SPEED);
      break;
    case VEER_R:
      spinRight(BASE_SPEED);
      delay(100);
      motors(BASE_SPEED, BASE_SPEED);
      break;
  }
  
  //Serial.print("Left: ");Serial.print( leftLine); Serial.print( "\t Right: "); Serial.print( rightLine); Serial.print("State: "); Serial.println(STATE);
  
  delay(30);        //wait .1 second
  
} //closing curly of the “loop()” function




