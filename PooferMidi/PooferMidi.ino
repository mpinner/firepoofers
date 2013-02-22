#include "poofer.h"
#include <Usb.h>
#include <usbh_midi.h>


#define DEBUG true

USB  Usb;
MIDI Midi(&Usb);
byte midiOutBuf[ 3 ];
boolean isPoofer;
int currentPooferId;


#define MIDI_RESERVED 7


//  BigRedButton on pin 7 input
//  BigRedButton LED on pin 8 output
const int bigRedBtnPin = 10;     // the number of the pushbutton pin
const int bigRedLedPin = 9 ;

int redBtnState = 0;
int redLEDState = 0;

#define PRESSED LOW

#define DEBOUNCE 10  // button debouncer, how many ms to debounce, 5+ ms is usually plenty

// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
byte midiButtons[] = {
  48, 50, 52, 53, 
  55, 57, 59, 60
}; 

// the analog 0-5 pins are also known as 14-19
// This handy macro lets us determine how big the array up above is, by checking the size
#define NUMBUTTONS sizeof(midiButtons)
// we will track if a button is just pressed, just released, or 'currently pressed' 
volatile byte pressed[NUMBUTTONS], justpressed[NUMBUTTONS], justreleased[NUMBUTTONS];




void setup(void) {
 Serial.begin(9600);
 // Serial.println("8 Poofer test");

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


  setupMidi();

  pooferSetup();
  
}



void loop(void) {

  // buttonLoop();
  
  midiLoop();
  
 /* cycleAllPoofers();
  
  redBtnState = digitalRead(bigRedBtnPin);
 // Serial.print("RedBtn "); Serial.println(redBtnState);
  digitalWrite(bigRedLedPin,!redBtnState);

  if (redBtnState == PRESSED)
  {
    // CLEAR ... both on
  //  Serial.println("FIRE ALL POOFERS!!");
    poofersAllOn();
    delay(10000);
    poofersAllOff();
  }*/

}

void midiLoop() {

  Usb.Task();
  
  if( Usb.getUsbTaskState() == USB_STATE_RUNNING ) {
    if( Midi.RcvData(midiOutBuf) == true ){
  
      //check if midi note is assigned to poofer  
      isPoofer = false; 
      for (int i = 0; i < NUMBUTTONS; i++) {
        if (midiButtons[i] == midiOutBuf[1]) {
          isPoofer = true;
          currentPooferId = i;
          break;
        }   
      }
      
      
      Serial.print(midiOutBuf[1], DEC);
      Serial.print(" note ");
      
      if (128 == midiOutBuf[0]) { 
        
        Serial.println("off");
        if (isPoofer) {
          pooferOff(currentPooferId);
        }  
        
      }
      
      else if (144 == midiOutBuf[0]) { 

         Serial.println("on");
         if (isPoofer) {
           pooferOn(currentPooferId);
         }  

       }
       else { 
         Serial.println("unknown");
       }
       
       
   //   Serial.println(midiOutBuf[2], DEC); //velocity
      
    }  
  }
  delay(1);
  return;
}
/*
void buttonLoop() {
  for (byte i = 0; i < NUMBUTTONS; i++) {
    if (justpressed[i]) {
      justpressed[i] = 0;
      Serial.print(i, DEC);
      Serial.println(" Just pressed"); 
      pooferOn(i);
      
      if (buttons[i] == bigRedBtnPin) { 
        digitalWrite(bigRedLedPin, HIGH);
        poofersAllOn();
      }

      // remember, check_switches() will CLEAR the 'just pressed' flag
    }
    if (justreleased[i]) {
      justreleased[i] = 0;
      Serial.print(i, DEC);
      Serial.println(" Just released");
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


void setupButtons() {
  
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


SIGNAL(TIMER2_OVF_vect) {
  check_switches();
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
*/

void setupMidi(){
 
   //Workaround for non UHS2.0 Shield 
  pinMode(MIDI_RESERVED,OUTPUT);
  digitalWrite(MIDI_RESERVED,HIGH);

  if (Usb.Init() == -1) {
    //while(1); //halt
    
  }//if (Usb.Init() == -1...
  delay( 200 );


  return; 
}

