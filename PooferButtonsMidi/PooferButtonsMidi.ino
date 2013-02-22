// PooferButtonsMidi 
// Runs MidiPoofer propane flares with MIDI and buttons
// coded by Matt Pinner, edits by Jerry Isdale
// Feb 22, 2013
// depends on USB and USBH_MIDI Libraries
// to use the MEGA the USB library avrpins.h file must be modified
// to define the MEGA macro.
// midiButtons.cpp is copied to end 'cause it gives errors when separate cpp.

#include "poofer.h"
#include "buttons.h"
#include "midiButtons.h"

#define DEBUG true

void setup(void) {
 if (DEBUG) Serial.begin(9600);
 // Serial.println("8 Poofer test");

  pooferSetup();

  buttonSetup();
  setupMidi();

}

void loop(void) {
  buttonLoop();
  midiLoop();

 // cycleAllPoofers();
  
}

/////////////////
// stupid cpp issue with Arduino - what is fix again?
#if defined(ARDUINO) && (ARDUINO >= 100)
#include "Arduino.h"
#else
#include "WProgram.h"
#endif

#include <Usb.h>
#include <usbh_midi.h>

USB  Usb;
MIDI Midi(&Usb);
byte midiOutBuf[ 3 ];
boolean isPoofer;
int currentPooferId;

#define MIDI_RESERVED 7
// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
const byte midiButtons[] = {
  48, 50, 52, 53, 
  55, 57, 59, 60
}; 

// the analog 0-5 pins are also known as 14-19
// This handy macro lets us determine how big the array up above is, by checking the size
#define NUM_MIDIBUTTONS sizeof(midiButtons)

void setupMidi() {
 
   //Workaround for non UHS2.0 Shield 
  pinMode(MIDI_RESERVED,OUTPUT);
  digitalWrite(MIDI_RESERVED,HIGH);

  if (Usb.Init() == -1) {
    //while(1); //halt
     if (DEBUG) Serial.println("MIDI USB Failed to Initialize");
    
  }//if (Usb.Init() == -1...
  delay( 200 );

  return; 
}

void midiLoop() 
{

  Usb.Task();
  
  if( Usb.getUsbTaskState() == USB_STATE_RUNNING ) {
    if( Midi.RcvData(midiOutBuf) == true ){
  
      //check if midi note is assigned to poofer  
      isPoofer = false; 
      for (int i = 0; i < NUM_MIDIBUTTONS; i++) {
        if (midiButtons[i] == midiOutBuf[1]) {
          isPoofer = true;
          currentPooferId = i;
          break;
        }   
      }
      
      
      if (DEBUG) Serial.print(midiOutBuf[1], DEC);
      if (DEBUG) Serial.print(" note ");
      
      if (128 == midiOutBuf[0]) { 
        
        Serial.println("off");
        if (isPoofer) {
          pooferOff(currentPooferId);
        }  
        
      }
      
      else if (144 == midiOutBuf[0]) { 

         if (DEBUG) Serial.println("on");
         if (isPoofer) {
           pooferOn(currentPooferId);
         }  

       }
       else { 
         if (DEBUG) Serial.println("unknown");
       }
       
       
   //   Serial.println(midiOutBuf[2], DEC); //velocity
      
    }  
  }
  delay(1);
  return;
}

