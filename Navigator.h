#include <Adafruit_GPS.h>
#include <Wire.h>
#include <Adafruit_LSM303.h>

#define MAGNETIC_DECLINATION -6.25f // Adjust for your area
#define CALIBRATION -15 // You might need to adjust this if your compass readouts are as poopy as mine.

// This is the distance from a waypoint before we consider it "reached"
// The value was derived from doing the cartesian distance between 2 lat/lon points
// This distance is approximately a radius of 6 meters.
#define WAYPOINT_PROXIMITY 5

class Navigator
{
  public:
  void begin();
  
  void useInterrupt(boolean v);
  double getDistance(void);
  void setTarget(double lat, double lon);
  double readCompass(void);
  double getRelativeBearing(void);
  bool hasFix(void);
  
  private:
  Adafruit_LSM303 lsm;
  boolean usingInterrupt = false;
  double targetLat, targetLon;
};

