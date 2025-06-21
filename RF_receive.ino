#include <SPI.h>
#include "RF24.h"

RF24 myRadio(7, 8); 
byte addresses[][6] = {"00001"}; 

// LED pin
const int ledPin = 3; 
const int yellowledPin = 4; 
const int speakerPin = 5;
int lastState = 1;

struct __attribute__((packed)) package {
  int sensorState = 0;
};

typedef struct package Package;
Package data;

void setup() {
  Serial.begin(115200);
  delay(1000);

  
  pinMode(ledPin, OUTPUT);
  pinMode(yellowledPin, OUTPUT);


  myRadio.begin();
  myRadio.setChannel(90); 
  myRadio.setPALevel(RF24_PA_MAX);  
  myRadio.setDataRate(RF24_250KBPS);  
  myRadio.openReadingPipe(1, addresses[0]); 
  myRadio.setAutoAck(false);
  myRadio.startListening(); 

  tone(speakerPin, 1000);  
  delay(25);
  noTone(speakerPin);
  Serial.println("System booted successfully!");
}

void loop() {
  if (myRadio.available()) {
    
    while (myRadio.available()) {
      myRadio.read(&data, sizeof(data)); 
    }
    digitalWrite(ledPin, HIGH);
    Serial.println("DOOR STATUS: ");
    Serial.print(data.sensorState);

    if(data.sensorState == 1){
      Serial.println("DOOR IS OPEN!");
      digitalWrite(yellowledPin, HIGH);
      if(lastState == 0){
        
        Serial.println("ALARM IS ACTIVE!");
        tone(speakerPin, 1000);  
        delay(25);
        noTone(speakerPin);
        delay(500);
        tone(speakerPin, 660);
        delay(700);
        tone(speakerPin, 550);
        delay(700);
        tone(speakerPin, 440);
        delay(700);
        noTone(speakerPin);

        lastState = 1;
        delay(10000);
      }
      
    }else{
      Serial.println("DOOR IS CLOSED");
      digitalWrite(yellowledPin, LOW);
      lastState = 0;
    }

    
  } else {
    digitalWrite(ledPin, LOW);
  }
}
