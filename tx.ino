#include <SPI.h>
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
