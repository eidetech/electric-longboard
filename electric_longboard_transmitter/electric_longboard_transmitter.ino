/*
Electric longboard 2021
Main transmitter sketch

Marcus Eide
*/

//Libraries
//SPI library
#include <SPI.h>
//nRF24L01 libraries
#include <nRF24L01.h>
#include <RF24.h>
//printf.h sketch for printing out nRF24l01 information
#include "printf.h"

//Radio pinout (7 and 8 for Arduino Pro Mini).
#define RADIO_1 7
#define RADIO_2 8

//Pipe address
const uint64_t pipeOut =  0xB00B1E5000LL;

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

void resetData() 
{
  data.dial1 = 0;
  data.switch1 = 0;
}

void setup()
{
  Serial.begin(9600);
  printf_begin();
  
  radio.begin();
  radio.setAutoAck(false);
  radio.setDataRate(RF24_250KBPS);
  radio.printDetails();

  radio.openWritingPipe(pipeOut);

  pinMode(2, INPUT_PULLUP);
  resetData();
}

void loop()
{
  data.dial1 = constrain( map( analogRead(A1), 0,   1021, 0, 255 ), 0, 255);
  if (digitalRead(2) == LOW) {
    data.switch1 = 1;
    }else{
    data.switch1 = 0;
    }
  
 if (radio.write(&data, sizeof(controllerdata))) {
  Serial.println(data.switch1);
  }else{
  
  Serial.println("Fail");
  }
}
