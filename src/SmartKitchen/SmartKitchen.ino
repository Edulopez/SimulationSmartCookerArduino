/** Arduino UNO pins*/
//14 digital input/output pins (of which 6 can be used as PWM outputs)
//6 analog inputs

/* Used pins */
//0  2 3 4 5 6 7 8  9 10 11 11  A0 A1 A2 A3 A4 13

/*Unused pins */
// 1


#define MAXHEAT 50
#define MAXSWITCHREADS 2
#define FADEAMOUNT 1 // how many points to fade the LED by

/* Pins */
#define PANSENSORPIN A0  //
#define PANSENSORPIN2 A1 //

#define VoiceSpeakerPin 13 //

#define WaterSensorPin A5 //

#define StatusLedPin 0 //

#define trigPin 12 //-
#define echoPin 8 //-

// Heat rings pin
#define LowLedRing1Pin 9 //
#define MidLedRing1Pin 10 //
#define HighLedRing1Pin 11 //
#define LowLedRing2Pin 3 //
#define MidLedRing2Pin 5   //      
#define HighLedRing2Pin 6 //

// Switch pins
#define SwitchPinsSize 3 
#define Switch1Pin1 2 //-
#define Switch1Pin2 4 //-
#define Switch1Pin3 7 //-

#define Switch2Pin1 A2 //-
#define Switch2Pin2 A3 //-
#define Switch2Pin3 A4 //-


int Switch1Pins[SwitchPinsSize] = {Switch1Pin1,Switch1Pin2,Switch1Pin3};
int Switch2Pins[SwitchPinsSize] = {Switch2Pin1,Switch2Pin2,Switch2Pin3};
 
int RingHeatDelay = 30;
int RingCoolDelay = 50;

bool CookerStatus = false;
bool SomeoneClose = true;
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
      Serial.println("Starting voice speaker sound");
     // return;
      digitalWrite(Pin, HIGH);
      delay(2000);
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
      //Serial.print("HeatRing: ");
      //Serial.print(Pin);
      //Serial.print("--");
      //Serial.print(Brightness);
      //Serial.print("\n");
      //Serial.print("Heat value: ");
      //Serial.print(heat);
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
      
      Serial.print("Switch Itensity: ");
      Serial.print(ConvertSwitchValueToIntensityValue(ReadValue(MaxReads)));
      Serial.print("\n");
      return ConvertSwitchValueToIntensityValue(ReadValue(MaxReads));
    }  
};

HeatRing LowHeatRing (LowLedRing1Pin,FADEAMOUNT,RingHeatDelay,RingCoolDelay,0);
HeatRing MidHeatRing (MidLedRing1Pin,FADEAMOUNT,RingHeatDelay,RingCoolDelay,0);
HeatRing HighHeatRing (HighLedRing1Pin,FADEAMOUNT,RingHeatDelay,RingCoolDelay,0);

HeatRing LowHeatRing2 (LowLedRing2Pin,FADEAMOUNT,RingHeatDelay,RingCoolDelay,0);
HeatRing MidHeatRing2 (MidLedRing2Pin,FADEAMOUNT,RingHeatDelay,RingCoolDelay,0);
HeatRing HighHeatRing2 (HighLedRing2Pin,FADEAMOUNT,RingHeatDelay,RingCoolDelay,0);

Switch Switch1 (Switch1Pins,SwitchPinsSize,MAXSWITCHREADS);
Switch Switch2 (Switch2Pins,SwitchPinsSize,MAXSWITCHREADS);

VoiceSpeaker VoiceSpeakerModule(VoiceSpeakerPin);

Zwave ZWaveModule = Zwave();
CookerBurner CookerBurner1(LowHeatRing,MidHeatRing,HighHeatRing);
CookerBurner CookerBurner2(LowHeatRing2,MidHeatRing2,HighHeatRing2);

void setup() {
  Serial.begin(9600);
  pinMode(WaterSensorPin, INPUT );
  pinMode(StatusLedPin, OUTPUT );
  pinMode(trigPin, OUTPUT); //set trigpin as output
  pinMode(echoPin, INPUT);  //set echopin as input
}

// Run all the actions of the burner
void BurnerActions()
{
  int powerValueSwitch1 = Switch1.GetIntensity();
  int powerValueSwitch2 = Switch2.GetIntensity(); 

  PowerVoiceFeedback(powerValueSwitch1+powerValueSwitch2);
  PowerVisualFeedback(powerValueSwitch1+powerValueSwitch2);
  
  bool isBoiling = BoilingCheck();
  bool isPan = PanCheck();
  ProximityCheck();
  LedPowerModule(powerValueSwitch1,powerValueSwitch2); 
}
void SentZwateData()
{
  ZWaveModule.SendRingSettings(Switch1.GetIntensity(), 1);
  ZWaveModule.SendRingTempeture(CookerBurner1.GetTempeture(), 1);
  ZWaveModule.SendRingSettings(Switch2.GetIntensity(), 2);
  ZWaveModule.SendRingTempeture(CookerBurner2.GetTempeture(), 2);
  ZWaveModule.SendBoilOverData(BoilingCheck);
}
//Sound feedback depending on the power value
void PowerVoiceFeedback(int powerValue)
{
  if(powerValue > 0 && CookerStatus == false)
    VoiceSpeakerModule.Play();

    CookerStatus = powerValue > 0;
}

//Visual feedback depending on the power value
void PowerVisualFeedback(int powerValue)
{
  if(powerValue > 0)
  {
     digitalWrite(StatusLedPin, HIGH);   // turn the LED on (HIGH is the voltage level)
  }
  else
     digitalWrite(StatusLedPin, LOW);   // turn the LED on (HIGH is the voltage level)
}

// Control the intensity and the number of heat rings on
void LedPowerModule(int powerValue1,int powerValue2)
{
  //Simulate the heating and cooling process of the leds
  // Control the intensity  or amount of heat rings on 
   CookerBurner1.Heat(powerValue1);
   CookerBurner2.Heat(powerValue2);
}

// Check if someone is close to the burner and give feedbacks
void ProximityCheck()
{

   // The trigpin sends out a signal, whoch bounces off an obstacle and comes back
  // Echopin recieves this signal and gives out +5v setting the aurduino pin on which is connected to high

  long duration, distance;
  digitalWrite(trigPin, LOW); 
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  duration = pulseIn(echoPin, HIGH);
  distance = (duration/2) / 29.1;

  Serial.print("Proximity check: ");
  //Serial.print(duration);
  Serial.print(distance);
  Serial.print("\n");
  
  if(!CookerBurner1.IsHot() && ! CookerBurner2.IsHot())
    return;
    
  if (distance > 1   && distance <= 40)  {
    if(SomeoneClose == false)
      VoiceSpeakerModule.Play();
    
    SomeoneClose = true;
    
   // Serial.println("Someone in Range");
  }
   else {
    SomeoneClose = false;
   // Serial.println("Out of range");
  }


}

// Check is something is boiling and gives feedback
int BoilingCheck()
{
  int waterSensorValue;
  waterSensorValue = analogRead(WaterSensorPin);
  Serial.print("moisture sensor reads: ");
  Serial.print(waterSensorValue);
  Serial.print("\n");

  //int sensor0Reading = analogRead (SpeakerPin); //read input for frequency
  //int frequency = map(sensor0Reading, 0, 1023, 100, 5000);
  //int duration = 3000; //time sound last (ms)

  if (waterSensorValue > 450)
  {
    digitalWrite(StatusLedPin, HIGH);   
    delay(200);                       
    digitalWrite(StatusLedPin, LOW); 
    delay(100);  
    digitalWrite(StatusLedPin, HIGH);
    //VoiceSpeakerModule.Play();
  }
  delay(200);
  return waterSensorValue;
}

// Check if a pan is on the burner
bool PanCheck()
{
   int sensorValue = 0;
   int sensorValue2 =0;
   // results are not %100 accurate, but it will do for now
   sensorValue = analogRead(PANSENSORPIN);
   sensorValue2 = analogRead(PANSENSORPIN2);

   //Serial.println("Ligth sensor:");
   //Serial.println(sensorValue);
   //Serial.println(sensorValue2);
   bool res = sensorValue > 0 || sensorValue2 > 0;
   if (res) 
   {
      //Serial.println("There's  a pan on");
   }
   else
   {
      //Serial.println("There isn't a pan");
   }
   delay(100);
   
   return res;
}

void loop() 
{ 
 BurnerActions();
}
