#include <Adafruit_NeoPixel.h> 


int switchState = 0; // denne variablene er 0 eller 1, avhengig om den porten som bli lyttet pp                    //gir ut HIGH signal
int NeoPixelPIN = 9; //Pin som gir OUTPUT til NeoPixel
int NeoSirkelPIN = 8; //Pin som styrer neoPixel sirkeheln. 

//Pin hvor LEDer for tidsvising starter og slutter
const int startNrLed = 2; 
const int sluttNrLed = 7;

//Pin hvor det blir lyttet
const int startNrLytter = 10;
const int sluttNrLytter = 13;

//definerer NeoPixel perene.
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, NeoPixelPIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel sirkel = Adafruit_NeoPixel(60, NeoSirkelPIN, NEO_GRB + NEO_KHZ800);

//En variael som brukes for og lagre tid i klokken.
unsigned long previousTime = 0;

//Tider til de forskjellige oppgavene i millisekunder
const unsigned long tider[] = {100000,10000,10000,100000};

//antalLed i klokken.
int antLed = 6;
//antall blink som 
int antBlink = 8;

//tid pr milliskeunder i blink.
int tidBlink = 300;

//Hvilken oppgave som er paa gang.
int oppgaveNr = 0;

void setup() {
  Serial.begin(9600);
  //Starter NeoPixel
  strip.begin();
  sirkel.begin();
  strip.show();  
  sirkel.show();
  
  //Setter opp stedet som skal lytte.
  for(int pinNr = startNrLytter; pinNr <= sluttNrLytter ; pinNr++){
    pinMode(pinNr,INPUT);
  }
 
 //Setter opp utputteen til klokken
 for(int pinNr = startNrLed; pinNr <= sluttNrLed; pinNr++){
     delay(2);
    pinMode(pinNr,OUTPUT);
    digitalWrite(pinNr,LOW);
  }
  
  //setter opp utputt til NeoPixel
  pinMode(NeoPixelPIN,OUTPUT);
  //setter opp utputt til NeoSirkel
  pinMode(NeoSirkelPIN,OUTPUT);
}

void loop() { 
  /*Kjorer gjennom alle lytterene og ser om noen av de gir ut et HIGH signal.
  Det lytteren som git HIGH signalet, nå også stemmer overens med oppgaven som systemet
  naa er paa. Hvis den gjør dette, starter den en klokke*/
  for(int i = startNrLytter; i <=sluttNrLytter; i++){
      switchState = digitalRead(i);
      if(switchState == HIGH && (oppgaveNr+startNrLytter) == i){
        settBunnplateHvit();
        startKlokke(tider[i-startNrLytter],i);
        oppgaveNr++;
      }
      settBunnplateAv();
  }
  if(oppgaveNr == 4){
    Serial.println("Her");
    delay(10000);
    for(int i = 0; i < 4; i++){
      strip.setPixelColor(i, 0, 0, 0);
    }
    strip.show();
    oppgaveNr = 0;
  }  
}

void startKlokke(unsigned long tidHandling, int lytter){
  // henter tid fra hvor lenge systemet har vært i gang.
  unsigned long currentTime = millis();
  //tid pr Led.
  unsigned long intervall = (tidHandling/antLed);  
  
  //teller hvilken Led klokken er paa.
  int teller = 0;
  
  
  while(teller < antLed && switchState == HIGH){
    //Skrur paa neste LED.
    digitalWrite((teller+startNrLed),HIGH);
    teller++; 
    //Hopper ut naar neste LEd skal lyse.
    unsigned long currentTime = 0;
    while(currentTime - previousTime <= intervall) {
      switchState = digitalRead(lytter);
      //Hvis boksen blir tatt av.
      if(switchState == LOW ){
        skruAvAlle();
        //Skrur paa en NeoPixel LED.
        strip.setPixelColor(oppgaveNr, 255, 255, 0);
        strip.setBrightness(255);
        strip.show();
        blinkBunnplate(); 
        return;
      }
      delay(5);
      currentTime += 5;
    }
   //Hvis alle LEd er slott paa. Saa starter det og blinke.
   if(teller == antLed-1 && switchState == HIGH){
      startBlink();
      switchState = digitalRead(lytter);
      //Hvis du rekker og taa av innen det er ferdig og blinke, faar du en stjerne.
      if(switchState == LOW ){
        skruAvAlle();
        strip.setPixelColor(oppgaveNr, 255, 255, 0);
        strip.setBrightness(255);
        strip.show();      
        blinkBunnplate();       
        return;
      }else{
        while(switchState == HIGH){
        settBunnplateRod();
        switchState = digitalRead(lytter);
        }
        settBunnplateAv();
      }  
    }
  }
  
  if(switchState == LOW ){
    skruAvAlle();
    settBunnplateAv();
    return;
  }  
}

//Blinker i et gitt antall ganger.
void startBlink(){
  boolean statusLed = false; 
  for(int i = 0; i < antBlink; i++){
  if(statusLed){
    skruPaaAlle();
    delay(tidBlink); 
  }else{
    skruAvAlle();
    delay(tidBlink);
  }   
  statusLed = !statusLed;
 }
  
}

//skrur av alle LED i klokken
void skruAvAlle(){
  for(int pinNr = startNrLed; pinNr <=sluttNrLed; pinNr++){
          digitalWrite(pinNr,LOW);
    }
}

//skrur paa elle LED i klokken
void skruPaaAlle(){
  for(int pinNr = startNrLed; pinNr <=sluttNrLed; pinNr++){
          digitalWrite(pinNr,HIGH);
    }
}

void settBunnplateHvit(){
  for(int i = 0; i < 24; i++){
      sirkel.setPixelColor(i, 255, 255, 255);
    }  
   sirkel.setBrightness(255);
   sirkel.show();
}

void settBunnplateAv(){
  for(int i = 0; i < 24; i++){
      sirkel.setPixelColor(i, 0, 0, 0);
    }  
   sirkel.show();
}

void settBunnplateRod(){
  for(int i = 0; i < 24; i++){
      sirkel.setPixelColor(i, 255, 0, 0);
    }  
   sirkel.setBrightness(255);
   sirkel.show(); 
}

void settBunnplateGron(){
  for(int i = 0; i < 24; i++){
      sirkel.setPixelColor(i, 0, 255, 0);
    }  
   sirkel.setBrightness(255);
   sirkel.show(); 
}

void blinkBunnplate(){
  Serial.println("Bunnplate");
  boolean statusLed = false; 
  for(int i = 0; i < antBlink; i++){
  if(statusLed){
    settBunnplateGron();
    delay(tidBlink); 
  }else{
    settBunnplateAv();
    delay(tidBlink);
  }   
  statusLed = !statusLed;
 } 
  
}
