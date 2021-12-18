/*
  Gegenphasige PWM an PIN PB0 und PB1
  Schaltbar Ã¼ber PB4
  Status-LED an PB3
*/

#include <Arduino.h>

// constants won't change. Used here to set a pin number:
const int ledPin =  PB3;// the number of the LED pin

// Variables will change:
int ledState = LOW;             // ledState used to set the LED

// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis = 0;        // will store last time LED was updated

// constants won't change:
const long interval = 200;           // interval at which to blink (milliseconds)

bool state_PB4_old = 0;

int adcVal = 0;

//declaration of functions
void pwmInit(void);
void pwmStop(void);

void setup() {
  // set the digital pin as output:
  //pinMode(ledPin, OUTPUT);
  pinMode(PB0, OUTPUT);
  pinMode(PB1, OUTPUT);
  pinMode(PB4, INPUT);
  pinMode(ledPin, OUTPUT); //PB3

  pwmInit();
  pwmStop();
}

void loop() {
  // here is where you'd put code that needs to be running all the time.

  if (digitalRead(PB4) == 1) { 
    //PWM einschalten
    pwmInit();
    digitalWrite(ledPin, HIGH);
    if (digitalRead(PB4) != state_PB4_old) {
      state_PB4_old = digitalRead(PB4);
      adcVal = analogRead(A1)/8;
    }
    else {
      
    }
  }
  else {
    //PWM ausschalten
    pwmStop();
    digitalWrite(ledPin, LOW);
    state_PB4_old = digitalRead(PB4);
  }
  
}

void pwmInit(void) {
  //Timer0 setup
  //TCCR0A = 0; //Timer/Counter Control Register A
  TCCR0A |= (1<<COM0A1) | (1<<COM0A0) | (1<<COM0B1) | (0<<COM0B0);
  TCCR0A |= (1<<WGM01) | (1<<WGM00);
  //TCCR0B = 0; //Timer/Counter Control Register B
  TCCR0B |= (0<<CS02) | (0<<CS01) | (1<<CS00); //0 1 0 entspricht 1 zu 8
  TCCR0B |= (0<<WGM02);
  //TCNT0 = 0; // Timer/Counter Register: The Timer/Counter Register gives direct access, both for read and write operations, to theTimer/Counter unit 8-bit counte
  //OCR0A = 0; //Output Compare Register A
  OCR0A = adcVal;
  //OCR0B = 0; //Output Compare Register B
  OCR0B = 255 - adcVal;
  //TIMSK0 = 0; // Timer/Counter Interrupt Mask Registe
  //TIFR0 = 0; //  Timer/Counter 0 Interrupt Flag Registe
  //Ende Timer0 setup
}

void pwmStop(void) {
  TCCR0A &= (0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0);
  TCCR0B &= ~((1<<CS02)|(1<<CS01)|(1<<CS00));
  //digitalWrite(PB0, LOW);
  //digitalWrite(PB1, LOW);
}
