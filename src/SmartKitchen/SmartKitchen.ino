/** Arduino UNO pins*/
//14 digital input/output pins (of which 6 can be used as PWM outputs)
//6 analog inputs

/* Used pins */
// 2 3 4 5 6 7 8 9 10 11 11 13

/*Unused pins */
// 1 14 analogs


#define MAXHEAT 255
#define MAXSWITCHREADS 2
#define FADEAMOUNT 1 // how many points to fade the LED by

/* Pins */
#define PANSENSORPIN A0
#define SpeakerPin 8
#define VoiceSpeakerPin 13
#define MoistureSensorPin 0

#define LowLedRingPin1 9
#define MidLedRingPin1 10
#define HighLedRingPin1 11

#define SwitchPinsSize 3
#define Switch1Pin1 2
#define Switch1Pin2 4
#define Switch1Pin3 7

const int trigPin = 12;
const int echoPin = 5;
const int ledPin = 6;

int Switch1Pins[SwitchPinsSize] = {Switch1Pin1,Switch1Pin2,Switch1Pin3};

int RingHeatDelay = 30;
int RingCoolDelay = 50;

class Zwave
{
  
  public:
    Zwave()
    {
      
    }
    void SendRingSettings(int ringSettings, int ringNumber)
    {
    }
  
    void SendRingTempeture(int tempeture , int ringNumber)
    {
      
    }
  
    void SendBoilOverData(int data)
    {
      
    }
  
    int GetRingSettings()
    {
      return 0;
    }
};
class VoiceSpeaker
{
  
  private:
    int Pin;
    
  public:

    VoiceSpeaker(int pin)
    {
      Pin = pin;
      pinMode(Pin, OUTPUT);
    }

    void Play()
    {
      Serial.print("Starting voice speaker sound");
      digitalWrite(Pin, HIGH);
      delay(3000);
      digitalWrite(Pin, LOW);
      Serial.print("Ending voice speaker sound");
      delay(200);
    }
};

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
    HeatRing()
    {
      
    }
    bool IsHot()
    {
       return Brightness > 0;
    }

    bool GetTempeture()
    {
      return Brightness;
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

class CookerBurner
{
  HeatRing LowHeatRing;
  HeatRing MidHeatRing;
  HeatRing HighHeatRing;
  public:
    CookerBurner(HeatRing low, HeatRing mid, HeatRing high)
    {
      LowHeatRing = low;
      MidHeatRing = mid;
      HighHeatRing = high;
    }
    bool IsHot()
    {
       return LowHeatRing.IsHot() || MidHeatRing.IsHot () || HighHeatRing.IsHot();
    }

    bool GetTempeture()
    {
      return 
      (LowHeatRing.GetTempeture ()+
      MidHeatRing.GetTempeture ()+
      HighHeatRing.GetTempeture());
    }
    // Heat the LEDs
    void Heat(int powerValue)
    {
      LowHeatRing.Heat (powerValue > 0);
      MidHeatRing.Heat (powerValue > 1);
      HighHeatRing.Heat(powerValue > 2);
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
      Serial.print(ConvertSwitchValueToIntensityValue(ReadValue(MaxReads)));
      Serial.print("\n");
      return ConvertSwitchValueToIntensityValue(ReadValue(MaxReads));
    }

  
};

HeatRing LowHeatRing (LowLedRingPin1,FADEAMOUNT,RingHeatDelay,RingCoolDelay,0);
HeatRing MidHeatRing (MidLedRingPin1,FADEAMOUNT,RingHeatDelay,RingCoolDelay,0);
HeatRing HighHeatRing (HighLedRingPin1,FADEAMOUNT,RingHeatDelay,RingCoolDelay,0);

HeatRing LowHeatRing2 (LowLedRingPin1,FADEAMOUNT,RingHeatDelay,RingCoolDelay,0);
HeatRing MidHeatRing2 (MidLedRingPin1,FADEAMOUNT,RingHeatDelay,RingCoolDelay,0);
HeatRing HighHeatRing2 (HighLedRingPin1,FADEAMOUNT,RingHeatDelay,RingCoolDelay,0);

Switch Switch1 (Switch1Pins,SwitchPinsSize,MAXSWITCHREADS);

VoiceSpeaker VoiceSpeakerModule(VoiceSpeakerPin);

Zwave ZWaveModule = Zwave();
CookerBurner CookerBurner1(LowHeatRing,MidHeatRing,HighHeatRing);
CookerBurner CookerBurner2(LowHeatRing2,MidHeatRing2,HighHeatRing2);

void setup() {
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
void SentZwateData()
{
  ZWaveModule.SendRingSettings(Switch1.GetIntensity(), 1);
  ZWaveModule.SendRingTempeture(CookerBurner1.GetTempeture(), 1);
  ZWaveModule.SendBoilOverData(BoilingCheck);
}
//Sound feedback depending on the power value
void PowerVoiceFeedback(int powerValue)
{
  if(powerValue > 0)
    VoiceSpeakerModule.Play();
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
   CookerBurner1.Heat(powerValue);
}

// Check if someone is close to the burner and give feedbacks
void ProximityCheck()
{
  long duration, inches, cm;
  
  pinMode(trigPin, OUTPUT);
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);
  pinMode(echoPin, INPUT);
  duration = pulseIn(echoPin, HIGH);
  
  if(LowHeatRing.IsHot())
  {
    analogWrite(ledPin, 0);
    VoiceSpeakerModule.Play();
  }
  else
  {
    //ledValue = 255 - (cm * 3);
  }

  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
  delay(100);
}
long microsecondsToInches(long microseconds)
{
  return microseconds / 74 / 2;
}
 
long microsecondsToCentimeters(long microseconds)
{
  return microseconds / 29 / 2;
}
  

// Check is something is boiling and gives feedbacks
int BoilingCheck()
{
  int waterSensorValue;
  waterSensorValue = analogRead(MoistureSensorPin);
  Serial.print("moisture sensor reads: ");
  Serial.println(waterSensorValue);

  int sensor0Reading = analogRead (SpeakerPin); //read input for frequency
  int frequency = map(sensor0Reading, 0, 1023, 100, 5000);
  int duration = 3000; //time sound last (ms)

  if (waterSensorValue > 0)
  {
    MakeSound(frequency,duration);
    return waterSensorValue;
  }
  delay(500);
}

// Check if a pan is on the burner
bool PanCheck()
{
   int sensorValue = 0;
   // results are not %100 accurate, but it will do for now
   sensorValue = analogRead(PANSENSORPIN);
   if (sensorValue < 300) 
   {
    Serial.println("It's on!");
   }
   else
   {
    Serial.println("It's off!");
   }
   return sensorValue < 300;
   delay(100);
}

void MakeSound(int frequency,int  duration)
{
  tone(SpeakerPin, frequency, duration); //speakerPin, frequency, duration
}

void loop() {
 BurnerActions();
}
