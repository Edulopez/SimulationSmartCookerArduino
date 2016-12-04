

#define MAXHEAT 255
#define MAXSWITCHREADS 2
#define FADEAMOUNT 1 // how many points to fade the LED by
#define PANSENSORPIN A0
#define SpeakerPin 9
#define MoistureSensorPin 0

#define PINSSIZE1 3
int SwitchPins1[3] = {2,4,7};
int LowLedRing = 9;           // the PWM pin the LED is attached to
  
int RingHeatDelay = 30;
int RingCoolDelay = 50;

class HeatRing
{
  int Pin;
  int Brightness = 0;
  int FadeAmount;
  int HeatDelay;
  int CoolingDelay;
  
  public:
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
    // Heat the LEDs
    void Heat(bool heat = true)
    {
      //Serial.print(Pin);
      //Serial.print("--");
      //Serial.print(Brightness);
      //Serial.print("\n");
      analogWrite(Pin, Brightness);
      
      // wait to see the dimming effect
      if(heat)
        delay(HeatDelay);
      else
        delay(CoolingDelay);
      
      if(!heat && !IsHot()) return;
      if(heat && Brightness == MAXHEAT) return;
      
      if( Brightness >= 0 && Brightness <= MAXHEAT)
      {
        if(heat == false)
          Brightness -= FadeAmount ; 
        else
          Brightness += FadeAmount;
      } 
      if ( Brightness < 0) Brightness = 0;
      else if (Brightness > MAXHEAT) Brightness = MAXHEAT;
    }
};

class Switch
{ 
  private:
    const static int MaxPinsSize = 4;
    
    // If all the bytes are on it means the switch is on OFF ( 111 = OFF )
    int OffValue;

    // Pins numbers on the arduino
    int Pins[MaxPinsSize]={0,0,0};

    // Numbers of pings used in the pins numers array
    int PinsSize;

    // Maximun amount of reading to get a precise reading of the switch
    int MaxReads;


    // Get The value of the switch
    int ReadValue()
    {
      int switchValues = 0;
      for(int i =0 ; i < PinsSize ; ++i)
      {
        switchValues = switchValues << 1;
        switchValues |= digitalRead(Pins[i]);
      }
      return switchValues; 
    }

    // Get the value of the switch until it get stabilized without noise
    int ReadValue(int maxReads)
    {
       int mainValue = 0 ;
      for(int i = 0 ; i < MaxReads ; i++)
      {
        mainValue = ReadValue();
        delay(10);
        int value = ReadValue();
        
        if(mainValue == value)
          return mainValue;
      }
      return mainValue;
    }

    int ConvertSwitchValueToIntensityValue(int switchValues)
    {
      // If all the bits are on it means off
      if(switchValues == OffValue)
        return 0; // Off
      
      int intensityValue = 1;
      for(int i = 0 ; i < PinsSize ; ++i)
      {
        // Check rigthmost bit to check if is marked
        if((switchValues & 1) == 0)
          return intensityValue;

        // rigth shift all the bits
        switchValues = switchValues >> 1;
        intensityValue++;
      }

      return 0; // off as a base case
    }
    
  public:
    
    Switch(int pin[], int pinsSize, int maxReads)
    {
      PinsSize=pinsSize;
      OffValue = (2 << (PinsSize-1))-1;
      
      for(int i =0 ; i < PinsSize ; i++){
        Pins[i]=pin[i];
        pinMode(Pins[i],INPUT_PULLUP);
      }
      MaxReads = maxReads;
    }

    // Get the intensity marked in the switch and ensure to read it
    int GetIntensity()
    {
      //Serial.print(ConvertSwitchValueToIntensityValue(ReadValue(MaxReads)));
      //Serial.print("\n");
      return ConvertSwitchValueToIntensityValue(ReadValue(MaxReads));
    }

  
};
HeatRing LowHeatRing (9,FADEAMOUNT,RingHeatDelay,RingCoolDelay,0);
HeatRing MidHeatRing (10,FADEAMOUNT,RingHeatDelay,RingCoolDelay,0);
HeatRing HighHeatRing (11,FADEAMOUNT,RingHeatDelay,RingCoolDelay,0);


Switch Switch1 (SwitchPins1,PINSSIZE1,MAXSWITCHREADS);
void setup() {

  
  pinMode(4,INPUT_PULLUP);
  pinMode(7,INPUT_PULLUP);
 Serial.begin(9600);
}

// Read the switch and get the value of the intensity from 0 to 3
int PowerSwitchCheck()
{
 return Switch1.GetIntensity();
}

// Run all the actions of the burner
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
  int waterSensorValue;
  waterSensorValue = analogRead(MoistureSensorPin);
  Serial.print("moisture sensor reads: ");
  Serial.println(waterSensorValue);

  int sensor0Reading = analogRead (SpeakerPin); //read input for frequency
  int frequency = map(sensor0Reading, 0, 1023, 100, 5000);
  int duration = 3000; //time sound last (ms)
  /*if (moisture_val > 0){
    analogWrite(3,255);
  }
  else{
    analogWrite(3,0);
  }*/

  if (waterSensorValue > 0)
  {
    MakeSound(frequency,duration);
  }
  delay(500);
}

// Check if a pan is on the burner
void PanCheck()
{
   int sensorValue = 0;

   // results are not %100 accurate, but it will do for now
   sensorValue = analogRead(PANSENSORPIN);

   if (sensorValue < 300) {
   Serial.println("It's on!");
   }else{
   }
   Serial.println("It's off!");
   delay(100);

}

void MakeSound(int frequency,int  duration)
{
  tone(SpeakerPin, frequency, duration); //speakerPin, frequency, duration
}

void loop() {
 BurnerActions();
}
