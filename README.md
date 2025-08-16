# Receiver-and-remote-for-RC-plane

I build remotes and receivers for RC planes.

Equipment I used: 

  arduino nano (2)
  
  nRF24L01 (2)
  
  capacitor 100uf (2)
  
  capacitor 104 = 0.1 µF (2)
  
  ams1117 (2)
  
  ky-023 (1)
  
  mt3608 (1)
  
  volume slide (1)
  
  Latch switch (2)
  
  Resistor 100-1000 (2)
  
  led light bulb (2)
  
  18650 lithium-ion battery (2)
  
  bms 1s (1)
  
  perfboard 6*4 (1)
  
  perfboard 12*8 (1)
  
  some wire
  
remotes:
<img width="1126" height="801" alt="ams1117 (1)" src="https://github.com/user-attachments/assets/cd76a51a-3d4b-4971-a1fa-3250c5f318d8" />
![1000003447](https://github.com/user-attachments/assets/bcf928d4-08fb-4f63-af60-ca6dcda54a75)
![1000003448](https://github.com/user-attachments/assets/e62bae9f-1ee6-4ea2-be22-eb01d728bba6)
code:
[Uploading tx.ino#include <SPI.h>
#include <RF24.h>
 
const int VRx = A2;
const int VRy = A1;

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
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MAX);
  radio.setChannel(108);  
  radio.stopListening();
  pinMode(A0,INPUT);
  pinMode(7,OUTPUT);
  pinMode(4, INPUT_PULLUP);
}

void loop() {
  int v = analogRead(A0);
  int x = analogRead(VRx);
  int y = analogRead(VRy);
  int btn = digitalRead(4);
  int vr = 1000;
  if(btn==0){
    vr = map(v, 0, 990, 1000, 1700);
    digitalWrite(7,HIGH);
  } else{
    digitalWrite(7,LOW);
  }
  int xr = map(x, 0, 1023, 0, 180);
  int yr = map(y, 0, 1023, 0, 180);
  int r = xr;//145
  int l = 180-xr;//45
  yr = 180-yr ;
  if(yr > 92 ){
    r = r+(yr/2) ;
    l = l+(yr/2) ;
  }else if (yr < 88){
    r = r-((180-yr)/2) ;
    l = l-((180-yr)/2) ;
  }
  if(r>145){
    r = 145;
  }
  if(l<45){
    l = 45;
  }
  if(r<0){
    r=0;
  }
  if(l>180){
    l=180;
  }
  
  Data dataToSend;
  dataToSend.a = vr;
  dataToSend.b = r;
  dataToSend.c = l;

  radio.write(&dataToSend, sizeof(dataToSend));

  //Serial.print("Sent: ");
  //Serial.print(dataToSend.a); Serial.print(", ");  
  //Serial.print(r); Serial.print(", ");
  //Serial.println(l);
  
}
…]()


receivers:
<img width="1277" height="700" alt="ams1117" src="https://github.com/user-attachments/assets/22c5e6cd-22ee-4626-912f-c86d87892f40" />
![1000003450](https://github.com/user-attachments/assets/6a6b070f-01a4-46e1-81be-280c87ff7569)
![1000003449](https://github.com/user-attachments/assets/c0462060-7db6-4a81-a107-7fd143ad336d)
code:
[Uploading rx.i#include <SPI.h>
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
no…]()


It doesn't look that great, but I had a little fun doing it and it's a bit cheaper than a pre-built remote and receiver.

Overall, it functional but if your goal is to make a RC plane then you should just buy pre-built one. And of course, you can adapt it to other wireless devices, because of course, it's made with Arduino.

**The disadvantage is that it looks ugly and the receiver is quite larger than the pre-made one.
