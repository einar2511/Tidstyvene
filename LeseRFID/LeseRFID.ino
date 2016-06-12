/*
 * 
 * Standar PIN Layout for RFID leser MFRC522:
 * ---------------------------------------
 *             MFRC522      Arduino       
 *             Reader/PCD   Uno           
 * Signal      Pin          Pin           
 * ---------------------------------------
 * RST/Reset   RST          9             
 * SPI SS      SDA(SS)      10            
 * SPI MOSI    MOSI         11 / ICSP-4   
 * SPI MISO    MISO         12 / ICSP-1   
 * SPI SCK     SCK          13 / ICSP-3   
 */

//Importerer bibloteker til RFID leser
#include <SPI.h>
#include <MFRC522.h>

//Definerer PINs som RFID leseren skal bruke
#define SS_PIN 10
#define RST_PIN 9

/*Definerer forste og siste PIN som skal sende signal til Slave arduinoen.
PINs i intervallet mellom disse vil også bli brukt til dette.*/
const int startUtPin = 4;
const int sluttUtPin = 7;

//Oppretter en klasser for initsialisering av RFID leser, og nokkel.
MFRC522 rfid(SS_PIN, RST_PIN);
MFRC522::MIFARE_Key key; 

//Oppretter en array som holder på ID til NFC tag paa RFID leser.
byte nuidPICC[3];

//Variabel for aktiv oppgave.
int oppgaveNr = 0;

int teller = 0;

void setup() { 
  
  //Gjor klar de ulike kompoentene i RFID leser...
  SPI.begin();
  rfid.PCD_Init();
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  
  //Setup av PINs som skal sende signaler.
  for(int pinNr = startUtPin; pinNr <= sluttUtPin ; pinNr++){
    pinMode(pinNr,OUTPUT);
    digitalWrite(pinNr,LOW);
  }
  
  //Serial.begin(9600);
  
}
 
void loop() {
    
  // PICC_IsNewCardPresent() søker om det er en NFC tag paa leseren.
  if ( ! rfid.PICC_IsNewCardPresent()){
    /*Signalet fra NFC tagen er ikke konstant. Paa denne maaten må PICC_IsNewCardPresent()
    ikka faa signaler i 20 millisekunder foer signal PINene blir satt LOW.*/   
    if(teller == 20){
      teller++;
      for(int pinNr = startUtPin; pinNr <= sluttUtPin ; pinNr++){ 
        digitalWrite(pinNr,LOW );
      }
    }else if(teller < 20){
      teller++;    
    }
    delay(1); 
    return;
  }
  
  //Leser ID til NFC tag paa RFID leser.
  rfid.PICC_ReadCardSerial()
  /*
  if ( ! rfid.PICC_ReadCardSerial()){
  }*/

  
  //MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
    
  //Gir bytes fra NFC tag, som brukes til identifikasjon. 
  for (byte i = 0; i < 4; i++) {
    nuidPICC[i] = rfid.uid.uidByte[i];
  }
    
  //Hvis ID og oppgaveNr stemmer blir det sendt HIGH signal gjennom en ut pin.  
  if(nuidPICC[0] == 206 && nuidPICC[1] == 126 && nuidPICC[2] == 49 && nuidPICC [3] == 181 && oppgaveNr == 0) {
    digitalWrite(startUtPin,HIGH);
    oppgaveNr++;
  }   
    
  if(nuidPICC[0] == 62 && nuidPICC[1] == 148 && nuidPICC[2] == 49 && nuidPICC [3] == 181 && oppgaveNr == 1) {
    digitalWrite((startUtPin+oppgaveNr),HIGH);
    oppgaveNr++;
  }
  
  if(nuidPICC[0] == 46 && nuidPICC[1] == 109 && nuidPICC[2] == 49 && nuidPICC [3] == 181 && oppgaveNr == 2) {
    digitalWrite((startUtPin+oppgaveNr),HIGH); 
    oppgaveNr++;
  }
  
  if(nuidPICC[0] == 142 && nuidPICC[1] == 109 && nuidPICC[2] == 49 && nuidPICC [3] == 181 && oppgaveNr == 3) {
    digitalWrite((startUtPin+oppgaveNr),HIGH); 
    oppgaveNr=0;
  }  
    
  teller = 0;
  
}
