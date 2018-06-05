
// This is for a little piezeo speaker I have hooked up that beeps while the GPS is trying to get a fix, and the provides a countdown
class Beeper
{
  public:
  void begin(int pin);
  void beep(int ms);
  void beep3(void);
  void countdown(int mins);

  private:
  int pin;  
};
