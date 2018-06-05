#include "Navigator.h"

SoftwareSerial softSerial(8, 7); // These pins may change depending on what board you're using
Adafruit_GPS GPS(&softSerial);


/**********************************************
 * Interrupt Vector
 * This function is called every millisecond
 * to check for new GPS data.
 **********************************************/
SIGNAL(TIMER0_COMPA_vect) 
{
  GPS.read();
}


/**********************************************
 * useInterrupt()
 * Activates or deactivates the interrupt.
 **********************************************/
void Navigator::useInterrupt(boolean v) 
{
  if (v) 
  {
    // See datasheet to understand this lower-level sorcery
    OCR0A = 0xAF;
    TIMSK0 |= _BV(OCIE0A);
    usingInterrupt = true;
  } 
  else 
  {
    // do not call the interrupt function COMPA anymore
    TIMSK0 &= ~_BV(OCIE0A);
    usingInterrupt = false;
  }
}


/**********************************************
 * begin()
 * Initialize all the navigational subsystems.
 **********************************************/
void Navigator::begin(void)
{
  // *** GPS ***
  GPS.begin(9600);
  GPS.sendCommand(PMTK_SET_NMEA_OUTPUT_RMCGGA);
  GPS.sendCommand(PMTK_SET_NMEA_UPDATE_1HZ);   // Send update every second (its a boat, not a rocket)
  useInterrupt(true);
  
  // *** Comapss ***
  lsm.begin();

}


/**********************************************
 * getDistance()
 * Returns distance from the current target.
 * The distance is not measured in a known unit.
 * Under the assumption that the boat is not 
 * going to traverse any long distance, the
 * cartesian distance between to lat/lon points
 * is used for distance, and then scaled up to
 * an integer value. Basically were assuming
 * the earth is flat. 
 **********************************************/
double Navigator::getDistance(void)
{
  if (GPS.newNMEAreceived())
    GPS.parse(GPS.lastNMEA()); // Retrieve GPS data if new data available
  
  double deltaLat = GPS.latitudeDegrees - targetLat;
  double deltaLon = GPS.longitudeDegrees - targetLon;
  return sqrt(deltaLat * deltaLat + deltaLon * deltaLon) * 100000;
}


/**********************************************
 * setTarget()
 * Used to set the lat/lon of the current
 * target from which the navigational operations
 * will be calculated.
 **********************************************/
void Navigator::setTarget(double lat, double lon)
{
  targetLat = lat;
  targetLon = lon; 
}


/**********************************************
 * readCompass()
 * Read the raw magnometer data and return it
 * as degrees.
 **********************************************/
double Navigator::readCompass(void)
{
  lsm.read(); // Read from magnometer
  float heading = atan2(lsm.magData.y, lsm.magData.x) * 180 / 3.14159265359; // Convert to degrees
  
  // Normalize to 0-360
  if (heading < 0)
    heading = 360 + heading;
    
  return heading; 
}


/**********************************************
 * getRelativeBearing()
 * Calculate the angle from the front of the
 * boat to the target. If the target is to the
 * right of the boat, the angle will negative.
 **********************************************/
double Navigator::getRelativeBearing(void)
{
  if (GPS.newNMEAreceived())
    GPS.parse(GPS.lastNMEA()); // Retrieve GPS data if new data available
  
  // Calculate angle from current position to target with respect to North
  double bearing = atan2(targetLon - GPS.longitudeDegrees, targetLat - GPS.latitudeDegrees ) * 180 / 3.14159265359;;
  
  // Relative bearing
  double relativeBearing = readCompass() - bearing + MAGNETIC_DECLINATION + CALIBRATION;
  
  // Normalize such that the front of the boat reresents 0 degrees, and left is negative
  if (relativeBearing > 180)
    relativeBearing -= 360;
   
  return relativeBearing; 
}


/**********************************************
 * hasFix()
 * Returns true if the GPS has a signal fix
 **********************************************/
bool Navigator::hasFix()
{
  if (GPS.newNMEAreceived())
    GPS.parse(GPS.lastNMEA()); // Retrieve GPS data if new data available
  return GPS.fix;  
}
