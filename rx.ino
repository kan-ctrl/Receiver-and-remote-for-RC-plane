#include <SPI.h>
#include <RF24.h>
#include <Servo.h>

Servo esc;
Servo servo1;
Servo servo2;
RF24 radio(9, 10);  // CE, CSN

const byte address[6] = "36154";

struct Data {
  int a;
  int b;
  int c;
};

void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.setDataRate(RF24_250KBPS);
  radio.openReadingPipe(1, address);
  radio.setPALevel(RF24_PA_LOW);
  radio.setChannel(108); 
  radio.startListening();
  esc.attach(3);
  servo1.attach(5);  
  servo2.attach(6);
  
}

void loop() {
  
  if (radio.available()) {
    Data receivedData;
    radio.read(&receivedData, sizeof(receivedData));

    Serial.print("Received: ");
    Serial.print(receivedData.a); Serial.print(", ");
    Serial.print(receivedData.b); Serial.print(", ");
    Serial.println(receivedData.c);
    esc.writeMicroseconds(receivedData.a);
    servo1.write(receivedData.b);
    servo2.write(receivedData.c);
    delay(5);
  } else{
    esc.writeMicroseconds(1000);
    servo1.write(90);
    servo2.write(90);
  }

}
