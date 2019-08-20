#include <Arduino.h>
#include <string.h>
#include "main.h"

const uint8_t buttonOnePin = 2;
const uint8_t buttonTwoPin = 3;

const uint8_t whiteLEDs  = 5;
const uint8_t greenLEDs  = 6;
const uint8_t redLEDs    = 9;
const uint8_t blueLEDs   = 10;
const uint8_t yellowLEDs = 11;

uint8_t LED_pins[] = {5, 6, 9, 10, 11};

volatile uint8_t buttonState = 0;

#define NUM_OF_CASES 6

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
    digitalWrite(yellowLEDs, 1);
    break;

  //Fast alternation
  case 5:
    fade_all(5000, 5);
    break;

  //Slow alternation
  case 6:
    fade_all(1000, 6);
    break;
  
  default:
    break;
  }

  Serial.print(buttonState);
}

void fade_all(unsigned int delay_us, unsigned int case_state){
  /*
  * delay_us   * Delay time in microseconds
  * case_state * State of the FSM, or case in the main func 
  */
  while(buttonState == case_state){
    for(int pin = 0; pin < 5; pin++){

      //Increase in brightness
      for(int fade = 0; fade < 256; fade++){
        analogWrite(LED_pins[pin], fade);
        delayMicroseconds(delay_us);
        if (buttonState != case_state){
          break;
        }
      }

      if (buttonState != case_state){
          break;
        }

      //Decrease in brightness
      for(int fade = 255; fade >= 0; fade--){
        analogWrite(LED_pins[pin], fade);
        delayMicroseconds(delay_us);
        if (buttonState != case_state){
          break;
        }
      }

    }
  }
}

void pin_ISR1() {
  noInterrupts(); // Disable interrupts while in ISR
  resetLEDs(); // Turn off all LEDs

  // Change Color scheme
  // Allow for rotating through the colors i.e. going from white to yellow and vice versa
  if (buttonState < NUM_OF_CASES){
    buttonState++;
  } else {
    buttonState = 0;
  }

  while(digitalRead(buttonOnePin) == LOW); // Wait until pin goes HIGH again (button unpressed)
  interrupts(); // Re-enable interrupts
}

void pin_ISR2() {
  noInterrupts();
  resetLEDs();
  
  if (buttonState > 0){
    buttonState--;
  } else {
    buttonState = NUM_OF_CASES;
  }

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
