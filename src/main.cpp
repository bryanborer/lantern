#include <Arduino.h>
#include "main.h"

const uint8_t buttonOnePin = 2;
const uint8_t buttonTwoPin = 3;

const uint8_t whiteLEDs  = 4;
const uint8_t greenLEDs  = 5;
const uint8_t redLEDs    = 6;
const uint8_t blueLEDs   = 7;
const uint8_t yellowLEDs = 8;

volatile uint8_t buttonState = 0;

void setup() {

  Serial.begin(9600);

  pinMode(whiteLEDs,  OUTPUT);
  pinMode(greenLEDs,  OUTPUT);
  pinMode(redLEDs,    OUTPUT);
  pinMode(blueLEDs,   OUTPUT);
  pinMode(yellowLEDs, OUTPUT);

  //Enable pullup resistors for buttons
  pinMode(buttonOnePin, INPUT_PULLUP);
  pinMode(buttonTwoPin, INPUT_PULLUP);

  // Attach an interrupt to the ISR vectors 0 and 1, corresponding to pins 2 and 3
  attachInterrupt(0, pin_ISR1, LOW);
  attachInterrupt(1, pin_ISR2, LOW);
}

void loop() {
  //Finite state machine for different color schemes
  switch (buttonState) {

  //Solid White
  case 0:
    digitalWrite(whiteLEDs, 1);
    break;
  
  //Solid Green
  case 1:
    digitalWrite(greenLEDs, 1);
    break;

  //Solid Red
  case 2:
    digitalWrite(redLEDs, 1);
    break;

  //Solid Blue
  case 3:
    digitalWrite(blueLEDs, 1);
    break;
  
  //Solid Yellow
  case 4:
    digitalWrite(yellowLEDs, 1
    );
    break;
  
  default:
    resetLEDs();
    break;
  }

  Serial.print(buttonState);
}

void pin_ISR1() {
  noInterrupts(); // Disable interrupts while in ISR
  resetLEDs(); // Turn off all LEDs
  buttonState++; // Change Color scheme
  while(digitalRead(buttonOnePin) == LOW); // Wait until pin goes HIGH again
  interrupts(); // Re-enable interrupts
}

void pin_ISR2() {
  noInterrupts();
  resetLEDs();
  buttonState--;
  while(digitalRead(buttonTwoPin) == LOW);
  interrupts();

}

void resetLEDs() {
  //Reset the LED states in between changing states
  digitalWrite(whiteLEDs,  0);
  digitalWrite(greenLEDs,  0);
  digitalWrite(redLEDs,    0);
  digitalWrite(blueLEDs,   0);
  digitalWrite(yellowLEDs, 0);
}
