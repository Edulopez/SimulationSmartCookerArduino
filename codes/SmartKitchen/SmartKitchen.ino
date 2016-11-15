
int LowLedRing = 9;           // the PWM pin the LED is attached to
int fadeAmount = 5;    // how many points to fade the LED by
int RingHeatDelay = 30;

class HeatRing
{
  int Pin;
  int Brightness = 0;
  int FadeAmount;
  int HeatDelay;
  public:

    HeatRing(int pin)
    {
      HeatRing(pin,5,30); 
    }

    HeatRing(int pin, int fadeAmount, int heatDelay)
    {
      Pin = pin; 
      FadeAmount = fadeAmount;
      HeatDelay = heatDelay;
    }

    HeatRing(int pin, int fadeAmount, int heatDelay, int brightness)
    {
      Pin = pin; 
      FadeAmount = fadeAmount;
      HeatDelay = heatDelay;
      Brightness = brightness;
    }
    bool IsHot()
    {
       // set the brightness of pin 9:
      analogWrite(Pin, Brightness);
    
      // change the brightness for next time through the loop:
      Brightness = Brightness + FadeAmount;
    
      // reverse the direction of the fading at the ends of the fade:
      if (Brightness <= 0 || Brightness >= 255) {
        FadeAmount = -FadeAmount;
      }
       return Brightness > 0;
    }
    
    void Heat(bool heat = true)
    {
      analogWrite(Pin, Brightness);
      
      if( Brightness >= 0 && Brightness < 255)
      {
        if(heat == false)
          Brightness -= FadeAmount; 
        else
          Brightness += FadeAmount;

        Serial.print((FadeAmount * -1));
        Serial.print("--");
        Serial.print("\n");
      } 
      if ( Brightness < 0) Brightness = 0;
      
      //Serial.print(Pin);
      //Serial.print("--");
      //Serial.print(Brightness);
      //Serial.print("\n");
      // wait to see the dimming effect
      delay(HeatDelay);
    }
    
};

HeatRing hh (9,fadeAmount,RingHeatDelay,0);
HeatRing h1 (10,fadeAmount,RingHeatDelay,0);
HeatRing h2 (11,fadeAmount,RingHeatDelay,255);

void setup() {
  
 // declare pin 9 to be an output:
 Serial.begin(9600);
}

// Read the switch and get the value of the intensity from 0 to 3
int PowerSwitchCheck()
{
 return 2;
}

void BurnerActions()
{
 int powerValue = PowerSwitchCheck(); 
 
 PowerVoiceFeedback(powerValue);
 PowerVisualFeedback(powerValue);
 LedPowerModule(powerValue);
}

//Sound feedback depending on the power value
void PowerVoiceFeedback(int powerValue)
{
  
}

//Visual feedback depending on the power value
void PowerVisualFeedback(int powerValue)
{
  
}

// Control the intensity and the number of heat rings on
void LedPowerModule(int powerValue)
{
   hh.Heat( powerValue > 0);
   h1.Heat( powerValue > 1);
   h2.Heat( powerValue > 2);
   
  HeatSimulator(powerValue);
  IntensitySimulator(powerValue);
}

//Simulate the heating and cooling process of the leds
void HeatSimulator(int powerValue)
{
}

// Control the intensity  or amount of heat rings on
void IntensitySimulator(int powerValue)
{
  
}

// Check if someone is close to the burner and give feedbacks
void ProximityCheck()
{
}

// Check is something is boiling and gives feedbacks
void BoilingCheck()
{
  
}

// Check if a pan is on the burner
void  PanCheck()
{
  
}

void loop() {
 BurnerActions();
}
