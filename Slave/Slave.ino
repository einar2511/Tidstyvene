
//Importerer bibloteker til NeoPixel
#include <Adafruit_NeoPixel.h> 

//Er 0 hvis porten som lyttes paa er LOW, er 1 hvis porten er HIGH
int switchState = 0;

//PIN som gir OUTPUT til NeoPixelLed
int neoPixelLedPin = 9;
//PIN som gir OUTPUT til NeoPixelRing.
int neoPixelRingPin = 8; 

//Definerer forste og siste PIN som skal brukes til 
const int forsteLedPin = 2; 
const int sisteLedPin = 7;

/*Definerer forste og siste PIN som skal lytte etter signal fra Master arduinoen.
PINs i intervallet mellom disse vil også bli brukt til dette.*/
const int forsteLyttePin = 10;
const int sisteLyttePin = 13;

//Definerer NeoPixel Lysstripe(LEDer) og Ring.
Adafruit_NeoPixel neoPixelStrip = Adafruit_NeoPixel(60, neoPixelLedPin, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel neoPixelRing = Adafruit_NeoPixel(60, neoPixelRingPin, NEO_GRB + NEO_KHZ800);

//Tider til de forskjellige oppgavene i millisekunder
const unsigned long tider[] = {100000,10000,10000,100000};

//Variabel for lagring av tid
unsigned long previousTime = 0;

//Antall LED i nettelling.
const int antLed = (sisteLedPin-forsteLedPin)+1;

//Antall blink
int antBlink = 8;

//Tid pr blink i millisekunder
int tidBlink = 300;

//Variabel for aktiv oppgave.
int oppgaveNr = 0;

void setup() {
  
  //Setter opp nedPixel objekter.
  neoPixelStrip.begin();
  neoPixelRing.begin();
  neoPixelStrip.show();  
  neoPixelRing.show();
  
  //Setter opp lytte PINs
  for(int pinNr = forsteLyttePin; pinNr <= sisteLyttePin ; pinNr++){
    pinMode(pinNr,INPUT);
  }
 
 //Setter opp PINs for bruk i nedtelling.
 for(int pinNr = forsteLedPin; pinNr <= sisteLedPin; pinNr++){
     delay(2);
    pinMode(pinNr,OUTPUT);
    digitalWrite(pinNr,LOW);
  }
  
  //setter opp utputt til NeoPixel
  pinMode(neoPixelLedPin,OUTPUT);
  pinMode(neoPixelRingPin,OUTPUT);
  
  //Serial.begin(9600);
}

void loop() { 
  /*Kjorer gjennom alle lytterene og ser om noen av de gir ut et HIGH signal.
  Det lytteren som git HIGH signalet, nå også stemmer overens med oppgaven som systemet
  naa er paa. Hvis den gjør dette, starter den en klokke*/
  for(int i = forsteLyttePin; i <=sisteLyttePin; i++){
      switchState = digitalRead(i);
      if(switchState == HIGH && (oppgaveNr+forsteLyttePin) == i){
        settRingHvit();
        startKlokke(tider[i-forsteLyttePin],i);
        oppgaveNr++;
      }
      settRingAv();
  }
  
  //Resetter boksen etter 10 sekunder
  if(oppgaveNr == 4){
    delay(10000);
    for(int i = 0; i < 4; i++){
      neoPixelStrip.setPixelColor(i, 0, 0, 0);
    }
    neoPixelStrip.show();
    oppgaveNr = 0;
  }  
}

void startKlokke(unsigned long tidHandling, int lytter){
  
  //Henter tid fra systemets start.
  unsigned long currentTime = millis();
  
  //Tid mellom hver LED skal starte.
  unsigned long intervall = (tidHandling/antLed);  
  
  //Heller hvilken LED som er den siste.
  int tellerLed = 0;
  
  /*ens man ikke er på den siste LEDen(er ferdig nedtelt), og det blir sendt HIGH signal
  fra masteren til lytteporten*/
  while(tellerLed < antLed && switchState == HIGH){
    
    //Skrur paa neste LED.
    digitalWrite((tellerLed+forsteLedPin),HIGH);
    tellerLed++; 
    
    //Venter til neste LED skal lyse.
    unsigned long currentTime = 0;
    while(currentTime - previousTime <= intervall) {
      switchState = digitalRead(lytter);
      
      //Hvis signalet som blir sendt fra master blir LOW.(Er oppgaven ferdig inne tiden).
      if(switchState == LOW ){
        settLedAv();
        skruPaStrip(oppgaveNr);
        blinkRing(); 
        return;
      }
      delay(5);
      currentTime += 5;
    }
    
   //Hvis alle LED er paa, starter det og blinke.
   if(tellerLed == antLed-1 && switchState == HIGH){
      startBlinkLed();
      switchState = digitalRead(lytter);
      
      //Hvis du rekker og taa av innen det er ferdig og blinke, faar du en stjerne.
      if(switchState == LOW ){
        settLedAv();
        skruPaStrip(oppgaveNr); 
        blinkRing();       
        return;
      }else{
        
        //Saa lenge boksen er paa lyser ring rodt.
        while(switchState == HIGH){
        settRingRod();
        switchState = digitalRead(lytter);
        }
        settRingAv();
      }  
    }
  }
  
  if(switchState == LOW ){
    settLedAv();
    settRingAv();
    return;
  }  
}

//LEDer binker et gitt antall ganger.
void startBlinkLed(){
  
  boolean statusLed = false;  
  for(int i = 0; i < antBlink; i++){
    if(statusLed){
      settLedPaa();
      delay(tidBlink); 
    }else{
      settLedAv();
      delay(tidBlink);
    }   
    statusLed = !statusLed;
 }  
}

//Skrur paa en bestemt NeoPixel led i rekken, basert på nummer.
void skruPaStrip(int nr){
  neoPixelStrip.setPixelColor(nr, 255, 255, 0);
  neoPixelStrip.setBrightness(255);
  neoPixelStrip.show();  
}

//skrur av alle LED i klokken
void settLedAv(){
  for(int pinNr = forsteLedPin; pinNr <=sisteLedPin; pinNr++){
          digitalWrite(pinNr,LOW);
    }
}

//Skrur paa elle LED i klokken
void settLedPaa(){
  for(int pinNr = forsteLedPin; pinNr <=sisteLedPin; pinNr++){
          digitalWrite(pinNr,HIGH);
    }
}

//Skrur av ring
void settRingAv(){
  for(int i = 0; i < 24; i++){
      neoPixelRing.setPixelColor(i, 0, 0, 0);
    }  
   neoPixelRing.show();
}

//Setter ring til Hvit
void settRingHvit(){
  for(int i = 0; i < 24; i++){
      neoPixelRing.setPixelColor(i, 255, 255, 255);
    }  
   neoPixelRing.setBrightness(255);
   neoPixelRing.show();
}

//Setter ring til Rod
void settRingRod(){
  for(int i = 0; i < 24; i++){
      neoPixelRing.setPixelColor(i, 255, 0, 0);
    }  
   neoPixelRing.setBrightness(255);
   neoPixelRing.show(); 
}

void settRingGron(){
  for(int i = 0; i < 24; i++){
      neoPixelRing.setPixelColor(i, 0, 255, 0);
    }  
   neoPixelRing.setBrightness(255);
   neoPixelRing.show(); 
}

//Ring binker et gitt antall ganger.
void blinkRing(){
  boolean statusLed = false; 
  for(int i = 0; i < antBlink; i++){
  if(statusLed){
    settRingGron();
    delay(tidBlink); 
  }else{
    settRingAv();
    delay(tidBlink);
  }   
  statusLed = !statusLed;
 } 
}
