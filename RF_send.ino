#include <SPI.h>
#include "RF24.h"

RF24 myRadio(7, 8); 
byte addresses[][6] = {"00001"}; 

const int sensorPin = 3;
struct __attribute__((packed)) package {
  int sensorState = 0;
};

typedef struct package Package;
Package data;

int sensorState = 0;

void setup() {
  Serial.begin(115200);
  delay(1000);

  pinMode(sensorPin, INPUT_PULLUP);
  
  myRadio.begin();
  myRadio.setChannel(90); 
  myRadio.setPALevel(RF24_PA_MAX);  
  myRadio.setDataRate(RF24_250KBPS); 
  myRadio.setAutoAck(false);
  myRadio.openWritingPipe(addresses[0]); 
  
  delay(1000);
}

void loop() {
  sensorState = digitalRead(sensorPin);
  
  if(sensorState == HIGH){
    Serial.println("STATE (1) SENSOR IS OPEN AND THE MAGNET IS AWAY FROM THE SENSOR");
    data.sensorState = 1;
  } else{
    Serial.println("STATE (0) SENSOR IS CLOSED AND THE MAGNET IS NEAR TO THE SENSOR");
    data.sensorState = 0;
  }

  bool success = myRadio.write(&data, sizeof(data));

  if (success) {
    Serial.println("Data sent successfully!");
  } else { 
    Serial.println("Failed to send data.");
  }

  Serial.println(" ");
  Serial.println(" ");

  delay(1000);
}
