#define MAX_WAYPOINTS 20

struct Waypoint
{
  double lat, lon;
  int speed;
};

class Path
{
  public:
  void addWaypoint(double lat, double lon, int speed);
  double getLat(void);
  double getLon(void);
  int getSpeed(void);
  void nextWaypoint(void);
  bool hasWaypoints(void);
  
  private:
  Waypoint waypoints[MAX_WAYPOINTS];
  int store_index = 0; // Used while adding waypoints
  int running_index = 0; // Used while traversing waypoints
  
};
