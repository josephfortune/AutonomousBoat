#include "Path.h"

/*******************************************************************************************/

void Path::addWaypoint(double lat, double lon, int speed)
{
  waypoints[store_index].lat = lat;
  waypoints[store_index].lon = lon;
  waypoints[store_index].speed = speed;
  store_index++;
}

/*******************************************************************************************/

double Path::getLat()
{
  return waypoints[running_index].lat;  
}

/*******************************************************************************************/

double Path::getLon()
{
  return waypoints[running_index].lon;
}

/*******************************************************************************************/

int Path::getSpeed()
{
  return waypoints[running_index].speed;
}

/*******************************************************************************************/

void Path::nextWaypoint()
{
  running_index++;
}

/*******************************************************************************************/

bool Path::hasWaypoints()
{
  return running_index < store_index;
}
