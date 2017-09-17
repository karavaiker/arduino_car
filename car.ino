#include <IRremote.h>
// ports
int FORWAD = 3;
int BACKYARD = 2;
int LEFT = 4;
int RIGHT = 5;

int RECV_PIN = 11;
int LED = 13;

//Vairable
int SPEED = 255;

int MUV = 0;
int RUD = 0;

int TIME_DELAY1=0;
int TIME_DELAY2=0;
int DELTA = 50;

//Connect IR
IRrecv irrecv(RECV_PIN);
decode_results results; 


void setup() {
  Serial.begin(9600);  
  irrecv.enableIRIn();
  pinMode(FORWAD, OUTPUT);
  pinMode(BACKYARD, OUTPUT);
  pinMode(LEFT, OUTPUT);
  pinMode(RIGHT, OUTPUT);
  pinMode(LED,OUTPUT);
}



void blink(int diode,int count){
  for(int i=0; i<count; i++){
    digitalWrite(diode, HIGH);
    delay(200);
    digitalWrite(diode, LOW);
    delay(200);
  }
}

void set_speed(){
  switch (SPEED) {
      case 255:
        SPEED = 170;
        blink(LED,2);
        break;
      case 170:
        SPEED = 80;
        blink(LED,1);
        break;
      case 80:
        SPEED=255;
        blink(LED,3);
        break;
  }
}


void set_move(int fb){
  switch(fb){
    case 0:
      analogWrite(FORWAD, 0);
      analogWrite(BACKYARD, 0);
      break;
    case 1:
      analogWrite(FORWAD, SPEED);
      analogWrite(BACKYARD, 0);
      break;
    case 2:
      analogWrite(FORWAD, 0);
      analogWrite(BACKYARD, SPEED);
      break;
  }  
}


void ruder(int v){
  switch(v){
    case 0:
      digitalWrite(LEFT, LOW);
      digitalWrite(RIGHT, LOW);
      break;
    case 1:
      digitalWrite(LEFT, HIGH);
      digitalWrite(RIGHT, LOW);
      break;
    case 2:
      digitalWrite(LEFT, LOW);
      digitalWrite(RIGHT, HIGH);
      break;
  } 
}



void loop() {
  if (irrecv.decode(&results)) {
        switch (results.value) {
            //allStop
            case 0xF336020A:
              MUV=0;
              RUD=0;
              set_move(0);
              ruder(0);
              blink(LED,1);
              break;
            //Forwadrd
            case 0x33404188:
              MUV=1;
              TIME_DELAY1=DELTA;
              break;           
            //backyard
            case 0xE196A20F:
              MUV=2;
              TIME_DELAY1=DELTA;
              break;            
            //LEFT
            case 0x30137BBB:
              RUD=1;
              TIME_DELAY2=DELTA;
              break;            
            //RIGHT
            case 0xCC4F8749:
              RUD=2;
              TIME_DELAY2=DELTA;
              break;
            //FR
            case 0xF173E77B:
              RUD=1;
              MUV=1;
              TIME_DELAY1=DELTA;
              TIME_DELAY2=DELTA;
              break;
            //FL
            case 0x5826B927:
              RUD=2;
              MUV=1;
              TIME_DELAY1=DELTA;
              TIME_DELAY2=DELTA;
              break;
            //BL
            case 0x45998722:
              RUD=2;
              MUV=2;
              TIME_DELAY1=DELTA;
              TIME_DELAY2=DELTA;
              break;
            //BR
            case 0x830719A6:
              RUD=1;
              MUV=2;
              TIME_DELAY1=DELTA;
              TIME_DELAY2=DELTA;
              break;
            //Change speed
            case 0x66DA73B6:
              set_speed();
              break;            
        }  
              
    irrecv.resume();
  }

Serial.println(TIME_DELAY1);
        if (TIME_DELAY1> 0){
          set_move(MUV);          
          TIME_DELAY1 = TIME_DELAY1 - 1;
        }else{
          MUV=0;
          set_move(0);          
        }

        if (TIME_DELAY2>0){
          ruder(RUD);
          TIME_DELAY2 = TIME_DELAY2 - 1;        
        }else{
          RUD=0;
          ruder(0);
        }
   
}
