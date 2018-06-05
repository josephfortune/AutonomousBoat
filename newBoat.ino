
/* Dear hackers, makers, and engineers, 
*
*  Welcome!
*
*  This is a complete rewrite of the original code that drove my autopilot RC boat. When I wrote the first
*  version, I never expected people would attempt to use it or understand it for use in their personal
*  projects, so the code was very difficult and sloppy, and for that, I apologize. I hope you will find
*  this version helpful and much easier to read. If you have any questions, feel free to shoot me an email
*  at joefortune11@gmail.com. Best of luck to you in your projects!
*
*  -Joseph Fortune 
*/

// Everything that was included in the other files had to also be included here to work for some reason
#include <Adafruit_GPS.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_LSM303.h>
#include <avr/sleep.h>

// *** Includes ***
#include "BoatController.h"
#include "Navigator.h"
#include "Path.h"
#include "Beeper.h"

// *** Macros ***
#define LEFT_ENGINE_PIN 11
#define RIGHT_ENGINE_PIN 10
#define BEEPER_PIN 5

/****************************** DEBUG MODE ***********************************
* It is EXTREMELY helpful to have serial feedback on what the boat is doing.
* For example you might want to set up some test waypoints in your yard, and
* walk around, checking that the boat is turning the correct way and that
* the waypoints are registering at a reasonable distance. This is what debug
* mode does when the macro is enabled. All the #ifdefs made the code look
* blotchy, but its well worth it when something isn't working right. Note that
* some of the other classes may have their own debug mode as well. 
******************************************************************************/
//#define DEBUG // Enables serial output feedback for basic functions

// *** Globals ***
Navigator nav;
BoatController controller(LEFT_ENGINE_PIN, RIGHT_ENGINE_PIN);
Path path;
Beeper beeper;

void setup()
{
  nav.begin();
  beeper.begin(BEEPER_PIN);
  path.addWaypoint(30.123323, -81.728044, SLOW);
  path.addWaypoint(30.123661, -81.728074, FAST);
  path.addWaypoint(30.123336, -81.727713, FAST);
  path.addWaypoint(30.123328, -81.727487, SLOW);
  
  #ifdef DEBUG
  Serial.begin(9600);
  #endif
  
  // Waiting for GPS to get a signal fix, and beeping in the meantime
  while (!nav.hasFix())
  {
    #ifdef DEBUG
    Serial.println("Waiting for fix...");
    #endif
    
    beeper.beep(100);
    delay(2900);
  }
  beeper.beep3(); // A happy little 3 chirps to know we have fix
  
  // Countdown 3 minutes to allow the GPS time to really get an accurate read
  #ifdef DEBUG
  Serial.println("Waiting 3 minutes for GPS to get accurate position...");
  #endif
  beeper.countdown(3);
  
}

void loop()
{
  // While we still have waypoints to reach
  while(path.hasWaypoints())
  {
    // Lock in the current waypoint
    nav.setTarget(path.getLat(), path.getLon());

    // While we haven't reached waypoint
    while(nav.getDistance() > WAYPOINT_PROXIMITY)
    {
      // Get the relative bearing to adjust the motors accordingly
      controller.adjustHeading(nav.getRelativeBearing(), path.getSpeed());
      
      #ifdef DEBUG
      Serial.print("Distance: "); 
      Serial.println(nav.getDistance());
      delay(1000);
      #endif     
    }    
    
    // Waypoint reached
    #ifdef DEBUG
    Serial.println("\nWAYPOINT REACHED!!!");
    beeper.beep3();
    #endif
    path.nextWaypoint(); 
  }
  
  // All waypoints reached
  controller.stopEngines();
  
  // Shutdown
  cli(); // Disable interrupts
  sleep_enable();
  sleep_cpu();
  
}
