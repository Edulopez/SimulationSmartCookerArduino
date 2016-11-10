void setup() {
  // put your setup code here, to run once:

}

class Test
{
  public:
    int klk();
};

int Test::klk()
{
  return 0;
} 



// Read the switch and get the value of the intensity from 0 to 3
int PowerSwitchCheck()
{
  Test p;
  return p.klk();
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
}
