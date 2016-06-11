/*
 * --------------------------------------------------------------------------------------------------------------------
 * Example sketch/program showing how to read new NUID from a PICC to serial.
 * --------------------------------------------------------------------------------------------------------------------
 * This is a MFRC522 library example; for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 * Example sketch/program showing how to the read data from a PICC (that is: a RFID Tag or Card) using a MFRC522 based RFID
 * Reader on the Arduino SPI interface.
 * 
 * When the Arduino and the MFRC522 module are connected (see the pin layout below), load this sketch into Arduino IDE
 * then verify/compile and upload it. To see the output: use Tools, Serial Monitor of the IDE (hit Ctrl+Shft+M). When
 * you present a PICC (that is: a RFID Tag or Card) at reading distance of the MFRC522 Reader/PCD, the serial output
 * will show the type, and the NUID if a new card has been detected. Note: you may see "Timeout in communication" messages
 * when removing the PICC from reading distance too early.
 * 
 * @license Released into the public domain.
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno           Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>

#define SS_PIN 10
#define RST_PIN 9 
 
MFRC522 rfid(SS_PIN, RST_PIN); // Instance of the class
MFRC522::MIFARE_Key key; 



// Init array that will store new NUID 
byte nuidPICC[3];


const int startUtPin = 4;
const int sluttUtPin = 7;

int oppgaveNr = 0;
int teller = 0;

void setup() { 
  Serial.begin(9600);
  SPI.begin(); // Init SPI bus
  rfid.PCD_Init(); // Init MFRC522 
  for (byte i = 0; i < 6; i++) {
    key.keyByte[i] = 0xFF;
  }
  
  //setter opp PINS som skal gi output.
  for(int pinNr = startUtPin; pinNr <= sluttUtPin ; pinNr++){
    pinMode(pinNr,OUTPUT);
    digitalWrite(pinNr,LOW);
  }

}

 
void loop() { 
  
  if ( ! rfid.PICC_IsNewCardPresent()){
    /*Det maa vere en delay for det blir sent LOW signaler tibake, fordi 
    signalet fra Brikkene ikke er konstante. Dette må skje i 10 millisekunder for 
    uoutputen bli satt til low*/
    
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
  
  if ( ! rfid.PICC_ReadCardSerial()){
  }

  
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  
    for (byte i = 0; i < 4; i++) {
      nuidPICC[i] = rfid.uid.uidByte[i];
    }
    
     if(nuidPICC[0] == 246 && nuidPICC[1] == 183 && nuidPICC[2] == 113 && nuidPICC [3] == 73) {
      Serial.println("TAG"); 
      for(int pinNr = startUtPin; pinNr <= sluttUtPin ; pinNr++){ 
        digitalWrite(pinNr,LOW );
      }
      oppgaveNr = 0;
 
    }
    
    
     
    if(nuidPICC[0] == 206 && nuidPICC[1] == 126 && nuidPICC[2] == 49 && nuidPICC [3] == 181 && oppgaveNr == 0) {
      digitalWrite(startUtPin,HIGH);
      Serial.println("Her1"); 
      oppgaveNr++;
    }   
    
 
    if(nuidPICC[0] == 62 && nuidPICC[1] == 148 && nuidPICC[2] == 49 && nuidPICC [3] == 181 && oppgaveNr == 1) {
      digitalWrite((startUtPin+oppgaveNr),HIGH);
      Serial.println("Her2"); 
      oppgaveNr++;
    }
    if(nuidPICC[0] == 46 && nuidPICC[1] == 109 && nuidPICC[2] == 49 && nuidPICC [3] == 181 && oppgaveNr == 2) {
      digitalWrite((startUtPin+oppgaveNr),HIGH);
     Serial.println("Her3"); 
      oppgaveNr++;
    }
    if(nuidPICC[0] == 142 && nuidPICC[1] == 109 && nuidPICC[2] == 49 && nuidPICC [3] == 181 && oppgaveNr == 3) {
      digitalWrite((startUtPin+oppgaveNr),HIGH);
      Serial.println("Her4"); 
      oppgaveNr=0;
    }  
  
  teller = 0;
}
