#include "pitches.h"
#include <Wire.h>
#include <Adafruit_MotorShield.h>
#include "utility/Adafruit_PWMServoDriver.h"

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

const int buttonPin = 2;
const int buttonLightPin = 3;
const int middleLedPin = 4;
const int deathLedPin =  5;      
const int candyLedPin =  6;
const int speakerPin =  7;

boolean testMode = false;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);

void setup() {
  AFMS.begin();  // create with the default frequency 1.6KHz
  //AFMS.begin(1000);  // OR with a different frequency, say 1KHz
  
  myMotor->setSpeed(10);  // 10 rpm   
  
  // initialize the LED pin as an output:
  pinMode(deathLedPin, OUTPUT);
  pinMode(candyLedPin, OUTPUT);  
  pinMode(middleLedPin, OUTPUT);  
  pinMode(buttonLightPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);    
  delay(1000); // Wait for button levels to normalize? I have no idea what I'm doing.
  testMode = (digitalRead(buttonPin) == LOW);
}

void playFanfare() {
   // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 29; thisNote++) {
    if (noteDurations[thisNote] == REST) {
      delay(1000/16);
    } else {
      int noteDuration = 1000/noteDurations[thisNote];
      tone(speakerPin, melody[thisNote],noteDuration);
  
      int pauseBetweenNotes = noteDuration;
      delay(pauseBetweenNotes);
    }
    // stop the tone playing:
    noTone(speakerPin);
  }
}

void playGame() {
  int time = 30;
  boolean candy = false;
  while (time < 1500 || !candy) {
    time = time + (time / 10);
    if (candy) {
      digitalWrite(deathLedPin, HIGH);
      tone(speakerPin, NOTE_E5, 250);
    } else {
      digitalWrite(candyLedPin, HIGH); 
      tone(speakerPin, NOTE_G5, 250);
    }
    candy = !candy;    
    delay(time);
    digitalWrite(deathLedPin, LOW); 
    digitalWrite(candyLedPin, LOW);
    noTone(speakerPin);
  }
  digitalWrite(middleLedPin, LOW);
  digitalWrite(candyLedPin, HIGH);
}


void dispenseCandy() {
  myMotor->step(300, BACKWARD, DOUBLE); // DOUBLE has higher torque
  myMotor->release(); 
}

void buttonPress() {
  digitalWrite(buttonLightPin, LOW); 
  playGame();        
  playFanfare();
  dispenseCandy();
}

void lightSequence() {
  digitalWrite(candyLedPin, HIGH); 
  delay(1000);
  digitalWrite(candyLedPin, LOW); 
  digitalWrite(middleLedPin, HIGH);
  delay(1000);
  digitalWrite(middleLedPin, LOW);
  digitalWrite(deathLedPin, HIGH); 
  delay(1000);
  digitalWrite(deathLedPin, LOW); 
  delay(1000);
}

void runTest() {
  lightSequence();
  for (int i = 0; i < 120; i++) {
    buttonPress();
  }
}

void loop(){
  if (testMode) {
    runTest();
    testMode = false;
  }
  // read the state of the pushbutton value:
  int buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {     
    digitalWrite(buttonLightPin, HIGH); 
    digitalWrite(deathLedPin, LOW);
    digitalWrite(middleLedPin, LOW);
    digitalWrite(candyLedPin, LOW); 
  } 
  else {
    buttonPress();
  }
}

