/*
  Melody
 
 Plays a melody 
 
 circuit:
 * 8-ohm speaker on digital pin 8
 
 created 21 Jan 2010
 modified 30 Aug 2011
 by Tom Igoe 

This example code is in the public domain.
 
 http://arduino.cc/en/Tutorial/Tone
 
 */
 #include "pitches.h"
const int REST = 0;

// notes in the melody:
int melody[] = {
  NOTE_G3,NOTE_C4,NOTE_E4, NOTE_G4,NOTE_C5,NOTE_E5, NOTE_G5,NOTE_E5,REST,
  NOTE_GS3,NOTE_C4,NOTE_DS4, NOTE_GS4,NOTE_C5,NOTE_DS5, NOTE_GS5,NOTE_DS5,REST,
  NOTE_AS3,NOTE_D4,NOTE_F4, NOTE_AS4,NOTE_D5,NOTE_F5, NOTE_AS5,NOTE_AS5,NOTE_AS5,NOTE_AS5,NOTE_C6
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  8,8,8, 8,8,8, 3,3,REST,
  8,8,8, 8,8,8, 3,3,REST,
  8,8,8, 8,8,8, 3,8,8,8,1 
};

const int buttonPin = 2;     // the number of the pushbutton pin
const int ledPin =  13;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);      
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);    
  pinMode(buttonPin, INPUT_PULLUP); 
}

void playFanfare() {
   // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 29; thisNote++) {

    // to calculate the note duration, take one second 
    // divided by the note type.
    //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
    if (noteDurations[thisNote]) {
      int noteDuration = 1000/noteDurations[thisNote];
      tone(8, melody[thisNote],noteDuration);
  
      // to distinguish the notes, set a minimum time between them.
      // the note's duration + 30% seems to work well:
      int pauseBetweenNotes = noteDuration;
      delay(pauseBetweenNotes);
    } else {
      delay(1000/16);
    }
    // stop the tone playing:
    noTone(8);
  }
}

void loop(){
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {     
    // turn LED off:
    digitalWrite(ledPin, LOW); 
  } 
  else {
    // turn LED on:    
    digitalWrite(ledPin, HIGH);  
    // Dispense Candy
    playFanfare();
  }
}

