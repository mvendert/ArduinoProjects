#include <Arduino.h>

#define LedPinRed 11
#define LedPinYelow 13
#define butPin 4
#define LedBlinkMs 500

#define NoDoDebug

int selectedLed = LedPinRed;

unsigned long millisecs;

//Button state
bool buttonstate = false;
bool buttonstateprev = false;

//Led state
int ledStateTime;
int ledOffTime;
bool LedIsOn = false;

//Debug buffer
char buffer[255];

void setup() {  
  #ifdef DoDebug
    Serial.begin(9600);  //Debugging messages
  #endif

  pinMode(LedPinRed, OUTPUT);
  pinMode(LedPinYelow, OUTPUT);
  pinMode(butPin, INPUT_PULLUP);

  digitalWrite(LedPinRed,LOW);
  digitalWrite(LedPinYelow,LOW); 

  #ifdef DoDebug
    Serial.println("Initialized");
  #endif
  selectedLed = LedPinYelow;
}

void SwitchLeds()
{
  if (selectedLed == LedPinRed) {
    selectedLed = LedPinYelow;    
  } else {
    selectedLed = LedPinRed;    
  } 
  #ifdef DoDebug
    sprintf(buffer,"Switched to Led: %d",selectedLed);
    Serial.println(buffer);
  #endif
  
}

bool CheckButtonPressed()
{  
  bool buttonPressed = false;
  int butvalue = digitalRead(butPin);
  buttonstate = butvalue == 1 ? false : true;
    if (buttonstate != buttonstateprev) {
      if (buttonstate) {  // alleen bij van niet in gedrukt naar in wel ingedrukt, niet bij loslaten
                          // als je alleen checked op ingedrukt, doe je dat zeer veel keer per seconde...
        buttonPressed = true;        
      }
    buttonstateprev = buttonstate;
  } 
  #ifdef DoDebug
  if (buttonPressed) {    
    sprintf(buffer,"Buttonstate: %d",buttonPressed);
    Serial.println(buffer);
  }
  #endif
  return buttonPressed;  
}

bool SetLedStateOnOffDidSwitch(unsigned long ms, unsigned long lStateTime )
{
  bool SwitchedLed = false;
  if (abs(ms-lStateTime) > LedBlinkMs)  {
      LedIsOn = !LedIsOn;      
      #ifdef DoDebug
        sprintf(buffer,"Switched LedIsOn to %d",LedIsOn);
        Serial.println(buffer);
      #endif
      SwitchedLed = true;
      ledStateTime = millisecs;
  }  
  return SwitchedLed;
}


void loop() {
  
  millisecs = millis();

  if (CheckButtonPressed())
  {
    SwitchLeds();
  }

  if (SetLedStateOnOffDidSwitch(millisecs,ledStateTime)) {
    if (LedIsOn) {
      digitalWrite(selectedLed,HIGH);    
    } else {
      digitalWrite(LedPinRed,LOW);  
      digitalWrite(LedPinYelow,LOW);    
    }
  }
}







/*
  digitalWrite(LedPinYelow, HIGH);
  delay(10);
  digitalWrite(LedPinYelow,LOW);
  delay (100);
  digitalWrite(LedPinRed, HIGH);
  delay(10);
  digitalWrite(LedPinRed,LOW);
  delay(1000);
*/
/*
 /*
  #define timeMargin 500
  unsigned long  timestart;
  unsigned long  timestop;

  //Als we iets willen doen met lange en korte indrukken, moeten we de tjid bewaren
  //sprintf(buffer,"Buttonstate: %d",buttonstate);
  //Serial.println(buffer);
  //sprintf(buffer,"buttonstateprev: %d",buttonstateprev);
  //Serial.println(buffer);
  if (buttonstate != buttonstateprev) {

    buttonstateprev = buttonstate; // we are going to handle the change now..
    if (buttonstate) {
      timestart = millisecs;      
      //sprintf(buffer,"Starttime: %d",timestart);
      //Serial.println(buffer);
    } else {      
      timestop = millisecs;
      //sprintf(buffer,"timestop: %d",timestop);
      //Serial.println(buffer);
      //sprintf(buffer,"Press delta time: %d",abs(timestart-timestop));
      //Serial.println(buffer);
      if ( abs(timestart - timestop) > timeMargin) {
        if (selectedLed == LedPinRed) {
          selectedLed = LedPinYelow;
        } else
        {
          selectedLed = LedPinRed;
        }
      }
    }
  }
  */
