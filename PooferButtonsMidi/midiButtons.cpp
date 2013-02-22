//// MidiButtons.cpp
//// intended to handle midi controls
//// but weirdness of arduino build does not include Usb properly
//
//#if defined(ARDUINO) && (ARDUINO >= 100)
//#include "Arduino.h"
//#else
//#include "WProgram.h"
//#endif
//
//#include <Usb.h>
//#include <usbh_midi.h>
//
//#include "midiButtons.h"
//
//
//USB  Usb;
//MIDI Midi(&Usb);
//byte midiOutBuf[ 3 ];
//boolean isPoofer;
//int currentPooferId;
//
//#define MIDI_RESERVED 7
//// here is where we define the buttons that we'll use. button "1" is the first, button "6" is the 6th, etc
//const byte midiButtons[] = {
//  48, 50, 52, 53, 
//  55, 57, 59, 60
//}; 
//
//// the analog 0-5 pins are also known as 14-19
//// This handy macro lets us determine how big the array up above is, by checking the size
//#define NUM_MIDIBUTTONS sizeof(midiButtons)
//
//void setupMidi() {
// 
//   //Workaround for non UHS2.0 Shield 
//  pinMode(MIDI_RESERVED,OUTPUT);
//  digitalWrite(MIDI_RESERVED,HIGH);
//
//  if (Usb.Init() == -1) {
//    //while(1); //halt
//    Serial.println("MIDI USB Failed to Initialize");
//    
//  }//if (Usb.Init() == -1...
//  delay( 200 );
//
//  return; 
//}
//
//void midiLoop() 
//{
//
//  Usb.Task();
//  
//  if( Usb.getUsbTaskState() == USB_STATE_RUNNING ) {
//    if( Midi.RcvData(midiOutBuf) == true ){
//  
//      //check if midi note is assigned to poofer  
//      isPoofer = false; 
//      for (int i = 0; i < NUM_MIDIBUTTONS; i++) {
//        if (midiButtons[i] == midiOutBuf[1]) {
//          isPoofer = true;
//          currentPooferId = i;
//          break;
//        }   
//      }
//      
//      
//      Serial.print(midiOutBuf[1], DEC);
//      Serial.print(" note ");
//      
//      if (128 == midiOutBuf[0]) { 
//        
//        Serial.println("off");
//        if (isPoofer) {
//          pooferOff(currentPooferId);
//        }  
//        
//      }
//      
//      else if (144 == midiOutBuf[0]) { 
//
//         Serial.println("on");
//         if (isPoofer) {
//           pooferOn(currentPooferId);
//         }  
//
//       }
//       else { 
//         Serial.println("unknown");
//       }
//       
//       
//   //   Serial.println(midiOutBuf[2], DEC); //velocity
//      
//    }  
//  }
//  delay(1);
//  return;
//}

