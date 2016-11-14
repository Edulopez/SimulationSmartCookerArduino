
int LowLedRing = 9;           // the PWM pin the LED is attached to
int brightness = 0;    // how bright the LED is
int fadeAmount = 5;    // how many points to fade the LED by

void setup() {
  
 // declare pin 9 to be an output:
  pinMode(LowLedRing, OUTPUT);
}

// Read the switch and get the value of the intensity from 0 to 3
int PowerSwitchCheck()
{
 return 0;
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
  // put your main code here, to run repeatedly:
  Serial.print(PowerSwitchCheck());


  // set the brightness of pin 9:
  analogWrite(LowLedRing, brightness);

  // change the brightness for next time through the loop:
  brightness = brightness + fadeAmount;

  // reverse the direction of the fading at the ends of the fade:
  if (brightness <= 0 || brightness >= 255) {
    fadeAmount = -fadeAmount;
  }
  // wait for 30 milliseconds to see the dimming effect
  delay(10);
}
