// buttons.cpp
// Poofer buttons
// for 8 buttons + Big Red
// coded by Matt Pinner, edits by Jerry Isdale
// Feb 22, 2013

#if defined(ARDUINO) && (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif


#include "buttons.h"
#include "poofer.h"

// fwd declaration
void check_switches();

//  BigRedButton on pin 7 input
//  BigRedButton LED on pin 8 output
const int bigRedBtnPin = 10;     // the number of the pushbutton pin
const int bigRedLedPin = 9 ;

int redBtnState = 0;
int redLEDState = 0;

#define PRESSED LOW

#define DEBOUNCE 10  // button debouncer, how many ms to debounce, 5+ ms is usually plenty

// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
byte buttons[] = {6, 4, 12, 2, 8, 5, 11, 3, bigRedBtnPin}; // the analog 0-5 pins are also known as 14-19

// This handy macro lets us determine how big the array up above is, by checking the size
#define NUMBUTTONS sizeof(buttons)
// we will track if a button is just pressed, just released, or 'currently pressed' 
volatile byte pressed[NUMBUTTONS], justpressed[NUMBUTTONS], justreleased[NUMBUTTONS];


void buttonSetup() {
    // Big Red Button setup
  pinMode(bigRedBtnPin, INPUT_PULLUP);      
  pinMode(bigRedLedPin, OUTPUT);

  digitalWrite(bigRedLedPin, HIGH);  
  delay(500);
  digitalWrite(bigRedLedPin, HIGH);  
  delay(500);
  digitalWrite(bigRedLedPin, HIGH);  
  delay(500);
  digitalWrite(bigRedLedPin, LOW);  
  delay(500);


 // setup buttons and debounce

 // Make input & enable pull-up resistors on switch pins
  for (int i =0; i< NUMBUTTONS; i++) {
    pinMode(buttons[i], INPUT_PULLUP);
  }

  // Run timer2 interrupt every 15 ms 
  TCCR2A = 0;
  TCCR2B = 1<<CS22 | 1<<CS21 | 1<<CS20;

  //Timer2 Overflow Interrupt Enable
  TIMSK2 |= 1<<TOIE2;

}

// macro setup of timer service function
SIGNAL(TIMER2_OVF_vect) {
  check_switches();
}

void buttonLoop() {
  for (byte i = 0; i < NUMBUTTONS; i++) {
    if (justpressed[i]) {
      justpressed[i] = 0;
#ifdef DEBUG_BUTTONS
      Serial.print(i+1, DEC);
      Serial.println(" Just pressed"); 
#endif
      pooferOn(i);
      
      if (buttons[i] == bigRedBtnPin) { 
        digitalWrite(bigRedLedPin, HIGH);
        poofersAllOn();
      }

      // remember, check_switches() will CLEAR the 'just pressed' flag
    }
    if (justreleased[i]) {
      justreleased[i] = 0;
#ifdef DEBUG_BUTTONS
      Serial.print(i+1, DEC);
      Serial.println(" Just released");
#endif
      pooferOff(i);
            
      if (buttons[i] == bigRedBtnPin) { 
        digitalWrite(bigRedLedPin, LOW);
        poofersAllOff();
      }

      // remember, check_switches() will CLEAR the 'just pressed' flag
    }
    

    if (pressed[i]) {

    //  Serial.print(i, DEC);
     // Serial.println(" pressed");
      // is the button pressed down at this moment
    }
  }
}

void check_switches()
{
  static byte previousstate[NUMBUTTONS];
  static byte currentstate[NUMBUTTONS];
  static long lasttime;
  byte index;

  if (millis() < lasttime) {
     // we wrapped around, lets just try again
     lasttime = millis();
  }
  
  if ((lasttime + DEBOUNCE) > millis()) {
    // not enough time has passed to debounce
    return; 
  }
  // ok we have waited DEBOUNCE milliseconds, lets reset the timer
  lasttime = millis();
  
  for (index = 0; index < NUMBUTTONS; index++) {
     
    currentstate[index] = digitalRead(buttons[index]);   // read the button
    
    /*     
    Serial.print(index, DEC);
    Serial.print(": cstate=");
    Serial.print(currentstate[index], DEC);
    Serial.print(", pstate=");
    Serial.print(previousstate[index], DEC);
    Serial.print(", press=");
    */
    
    if (currentstate[index] == previousstate[index]) {
      if ((pressed[index] == LOW) && (currentstate[index] == LOW)) {
          // just pressed
          justpressed[index] = 1;
      }
      else if ((pressed[index] == HIGH) && (currentstate[index] == HIGH)) {
          // just released
          justreleased[index] = 1;
      }
      pressed[index] = !currentstate[index];  // remember, digital HIGH means NOT pressed
    }
    //Serial.println(pressed[index], DEC);
    previousstate[index] = currentstate[index];   // keep a running tally of the buttons
  }
}




