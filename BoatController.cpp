#include <Arduino.h>
#include "BoatController.h"

//#define DEBUG // enable serial output for debugging

/**********************************************
 * BoatController() - Constructor
 * Creating a boatcontroller object requires 
 * passing the constructor a left engine pin 
 * and a right engine pin number.
 **********************************************/
BoatController::BoatController(int leftEngine, int rightEngine)
{
  leftEnginePin = leftEngine;
  rightEnginePin = rightEngine;
  
  pinMode(leftEnginePin, OUTPUT);
  pinMode(rightEnginePin, OUTPUT);
}


/**********************************************
 * adjustHeading()
 * This function takes a relative bearing, and 
 * running speed as an argument. Relative
 * bearing is the angle in degrees between
 * where the boat is heading versus where it's 
 * supposed to be heading. An angle to the left
 * is negative. The angle is measured from the 
 * target to the front of the boat.
 **********************************************/
void BoatController::adjustHeading(double relativeBearing, int speed)
{
  double absRelativeBearing = abs(relativeBearing);
  int turnSpeed;
  
  /* *** CALCULATE TURNSPEED ***
   * In this first section we calculate what the turn speed should be. The further off course, the more one of
   * the engines will be slowed to allow a smooth turn */
  
  if (absRelativeBearing < 90)
    turnSpeed = speed * (1 - absRelativeBearing / 90);
  else
    turnSpeed = 0; // Really sharp turn
 
  #ifdef DEBUG
  Serial.print("\nRelBearing: ");
  Serial.print(relativeBearing);
  Serial.print(" ");
  #endif 
   
  if ( relativeBearing > 0 ) // positive bearing, turn left
  {
    
    // Ensures motors won't run while in debug mode
    #ifndef DEBUG
    analogWrite(leftEnginePin, turnSpeed); // slow down
    analogWrite(rightEnginePin, speed); // full speed
    #endif
    
    #ifdef DEBUG
    Serial.print("Turning left: (");
    Serial.print(turnSpeed);
    Serial.print(") (");
    Serial.print(speed);
    Serial.println(")");
    #endif
  }
  else // negative bearing, turn right
  {
    // Ensures motors won't run while in debug mode
    #ifndef DEBUG
    analogWrite(leftEnginePin, speed); // full speed
    analogWrite(rightEnginePin, turnSpeed); // slow down
    #endif
    
    #ifdef DEBUG
    Serial.println("Turning right");
    Serial.print(speed);
    Serial.print(") (");
    Serial.print(turnSpeed);
    Serial.println(")");
    #endif
  }
}


/**********************************************
 * stopEngines()
 * I feel like this one is pretty obvious.
 **********************************************/
 void BoatController::stopEngines(void)
 {
    analogWrite(leftEnginePin, 0);
    analogWrite(rightEnginePin, 0);
    
    #ifdef DEBUG
    Serial.println("Stopping");
    #endif
 }


