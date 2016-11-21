
int LowLedRing = 9;           // the PWM pin the LED is attached to
int fadeAmount = 1;    // how many points to fade the LED by
int RingHeatDelay = 30;
int RingCoolDelay = 200;
int MaxHeat =255;

class HeatRing
{
  int Pin;
  int Brightness = 0;
  int FadeAmount;
  int HeatDelay;
  int CoolingDelay;
  
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
    HeatRing(int pin, int fadeAmount, int heatDelay, int coolingDelay, int brightness)
    {
      Pin = pin; 
      FadeAmount = fadeAmount;
      HeatDelay = heatDelay;
      Brightness = brightness;
      CoolingDelay = coolingDelay;
    }
    bool IsHot()
    {
       return Brightness > 0;
    }
    void Heat(bool heat = true)
    {
      Serial.print(Pin);
      Serial.print("--");
      Serial.print(Brightness);
      Serial.print("\n");
      analogWrite(Pin, Brightness);
      
      // wait to see the dimming effect
      if(heat)
        delay(HeatDelay);
      else
        delay(CoolingDelay);
      
      if(!heat && !IsHot()) return;
      if(heat && Brightness == MaxHeat) return;
      
      if( Brightness >= 0 && Brightness <= MaxHeat)
      {
        if(heat == false)
          Brightness -= FadeAmount ; 
        else
          Brightness += FadeAmount;
      } 
      if ( Brightness < 0) Brightness = 0;
      else if (Brightness > MaxHeat) Brightness = MaxHeat;
    }
};

HeatRing LowHeatRing (9,fadeAmount,RingHeatDelay,RingCoolDelay,0);
HeatRing MidHeatRing (10,fadeAmount,RingHeatDelay,RingCoolDelay,255);
HeatRing HighHeatRing (11,fadeAmount,RingHeatDelay,RingCoolDelay,255);

void setup() {
  
 // declare pin 9 to be an output:
 Serial.begin(9600);
}

// Read the switch and get the value of the intensity from 0 to 3
int PowerSwitchCheck()
{
 return 1;
}

void BurnerActions()
{
 int powerValue = PowerSwitchCheck(); 
 
 PowerVoiceFeedback(powerValue);
 PowerVisualFeedback(powerValue);
 BoilingCheck();
 PanCheck();
 ProximityCheck();
 
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
  //Simulate the heating and cooling process of the leds
  // Control the intensity  or amount of heat rings on
   LowHeatRing.Heat( powerValue > 0);
   MidHeatRing.Heat( powerValue > 1);
   HighHeatRing.Heat( powerValue > 2);
   
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
  if(LowHeatRing.IsHot())
  {
    // do something
  }
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
