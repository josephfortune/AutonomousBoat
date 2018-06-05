// PWM motor speeds
#define FAST 255
#define SLOW 70

class BoatController
{
  public:
  BoatController(int leftEngine, int rightEngine);
  void adjustHeading(double relativeBearing, int speed);
  void stopEngines(void);
  
  private:
  int leftEnginePin;
  int rightEnginePin;
};
