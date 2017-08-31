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
boolean turnForward = false;

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 

// Connect a stepper motor with 200 steps per revolution (1.8 degree)
// to motor port #2 (M3 and M4)
Adafruit_StepperMotor *myMotor = AFMS.getStepper(200, 2);

void setup() {
  AFMS.begin();  // create with the default frequency 1.6KHz
  Serial.begin(9600);

  myMotor->setSpeed(10);  // 10 rpm   

  // initialize the LED pin as an output:
  pinMode(deathLedPin, OUTPUT);
  pinMode(candyLedPin, OUTPUT);  
  pinMode(middleLedPin, OUTPUT);  
  pinMode(buttonLightPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);    
  delay(1000); // Wait for button levels to normalize (should have used a pull down resistor instead).
  testMode = (digitalRead(buttonPin) == LOW);
}

void playFanfare() {
  // iterate over the notes of the melody:
  for (int thisNote = 0; thisNote < 29; thisNote++) {
    if (noteDurations[thisNote] == REST) {
      delay(1000/16);
    } 
    else {
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
  digitalWrite(middleLedPin, HIGH);
  int time = 30;
  boolean death = false;
  while (time < 1500 || !death) {
    time = time + (time / 10);
    if (death) {
      digitalWrite(deathLedPin, HIGH);
      tone(speakerPin, NOTE_E5, 250);
    } 
    else {
      digitalWrite(candyLedPin, HIGH); 
      tone(speakerPin, NOTE_G5, 250);
    }
    death = !death;    
    delay(time);
    digitalWrite(deathLedPin, LOW); 
    digitalWrite(candyLedPin, LOW);
    noTone(speakerPin);
  }
  digitalWrite(middleLedPin, LOW);
  for (int i = 0; i < 5; i++) {
    digitalWrite(candyLedPin, HIGH);
    tone(speakerPin, NOTE_G5, 250);
    delay(250);
    digitalWrite(candyLedPin, LOW);
    delay(250);
  }
}

void dispenseCandy() {
  int direction = BACKWARD;
  if (turnForward) {
    direction = FORWARD;  
  }
  myMotor->step(533, direction, DOUBLE); // DOUBLE has higher torque
  myMotor->release(); 
  turnForward = !turnForward;
}

void buttonPress() {
  //digitalWrite(buttonLightPin, LOW); 
  //playGame();        
  //digitalWrite(candyLedPin, HIGH);
  Serial.println("button");
  //playFanfare();
  delay(45000);
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

void waitMode() {
  digitalWrite(buttonLightPin, HIGH); 
  digitalWrite(deathLedPin, HIGH);
  digitalWrite(middleLedPin, HIGH);
  digitalWrite(candyLedPin, HIGH); 
}

void loop(){
  if (testMode) {
    runTest();
    testMode = false;
  }
  // read the state of the pushbutton value:
  int buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH) {     
    waitMode();
  } 
  else {
    buttonPress();
  }
}


