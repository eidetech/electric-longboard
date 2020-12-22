/*
Electric longboard 2021
Main reveiver sketch

Marcus Eide
*/

//Libraries
//Servo library
#include <Servo.h>
//SPI library
#include <SPI.h>
//nRF24L01 libraries
#include <nRF24L01.h>
#include <RF24.h>
//printf.h sketch for printing out nRF24l01 information
#include "printf.h"

//Motor output
#define motorOutputPin 5

//Radio pinout (9 and 10 for Arduino Uno).
#define RADIO_1 9
#define RADIO_2 10

//Pipe address
const uint64_t pipeIn =  0xB00B1E5000LL;

//Define radio
RF24 radio(RADIO_1, RADIO_2);

/* Struct to hold variables from receiver
The size of this struct should not exceed 32 bytes */
struct controllerdata {
  byte dial1; //Magnetic dial (hall) sensor
  byte switch1; //Switch
};

//Data object
controllerdata data;

//Set up motor output to be a servo object.
Servo motorOutput;

void setup()
{
   Serial.begin(9600);
   printf_begin();
   pinMode(LED_BUILTIN, OUTPUT);

  // Set up radio module
  radio.begin();
  radio.setDataRate(RF24_250KBPS); // Both endpoints must have this set the same
  radio.setAutoAck(false);
  radio.openReadingPipe(1,pipeIn);
  radio.startListening();
  radio.printDetails();

  //Attach servos
  motorOutput.attach(motorOutputPin);
  // Reset values to zero
  data.dial1 = 0;
  data.switch1 = 0;
  //delay(5000); // Wait for ESC to boot
}

void loop() {

  if ( radio.available() ) {
  radio.read(&data, sizeof(controllerdata));
  
  Serial.println(data.switch1);
  if (data.switch1 == 1){
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(100);
  }else{
    digitalWrite(LED_BUILTIN, LOW);   // turn the LED on (HIGH is the voltage level)
    delay(100);
  }
  }
  }
