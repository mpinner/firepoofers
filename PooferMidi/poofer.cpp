// Poofer v2 test rig - 8 relays
// Adafruit tftlcdtouch removed
#if ARDUINO >= 100
 #include "Arduino.h"
#else
 #include "WProgram.h"
#endif

#include "poofer.h"

#if defined(__AVR_ATmega1280__) || (__AVR_ATmega2560__)
const int poofer1 = 30;
const int poofer2 = 32;
const int poofer3 = 34;
const int poofer4 = 36;
const int poofer5 = 38;
const int poofer6 = 40;
const int poofer7 = 42;
const int poofer8 = 44;

/*
const int poofer1 = 0;
const int poofer2 = 1;
const int poofer3 = 2;
const int poofer4 = 3;
const int poofer5 = 4;
const int poofer6 = 5;
const int poofer7 = 6;
const int poofer8 = 7;
*/

#endif


const int poofers[] = {poofer1, poofer2, poofer3,poofer4, poofer5, poofer6, poofer7, poofer8};
const int numPoofers = 8;

boolean pooferState[numPoofers];

// from YourDuino Example: Relay Control 1.10
//  Handles "Relay is active-low" to assure
//  no relay activation from reset until
//  application is ready.
//   terry@yourduino.com

/*-----( Import needed libraries )-----*/
/*-----( Declare Constants )-----*/
#define RELAY_ON 0
#define RELAY_OFF 1

void pooferSetup()
{
  //-------( Initialize Pins so relays are inactive at reset)----
  poofersAllOff();
  
  //---( THEN set pins as outputs )----  
  for (int i=0;i<numPoofers;i++) {
    pinMode(poofers[i], OUTPUT);  
  }
  
}

void pooferOn(int id)
{
  if (id >= 0 && id < numPoofers)
   {
    digitalWrite(poofers[id], RELAY_ON);
    pooferState[id] = 1;
   }
}

void pooferOff(int id)
{
  if (id >= 0 && id < numPoofers)
   {
    digitalWrite(poofers[id], RELAY_OFF);
    pooferState[id] = 0;
   }
}

void poofersAllOn()
{
  for (int i=0;i<numPoofers;i++)
    pooferOn(i);
}

void poofersAllOff()
{
  for (int i=0;i<numPoofers;i++)
    pooferOff(i);
}

void cycleAllPoofers()
{
  Serial.println("Begin Cycle All Poofers");
  
  for (int i=0;i<numPoofers;i++) {
    pooferOn(i);
    delay(500);
    pooferOff(i);
  }
  
  Serial.println("All Poofers on together, then off");

  poofersAllOn();
  delay(500);
  poofersAllOff();
  
  Serial.println("END Cycle All Poofers");

}



