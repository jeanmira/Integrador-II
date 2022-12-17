#include <Arduino.h>
#include "HX711.h"


#include <mcp_can.h>
#include <SPI.h>
#include <time.h>


#define BeltRead_Pin 35
#define LOADCELL_DOUT_PIN  32
#define LOADCELL_SCK_PIN  33
#define LED_BELT_PIN 25
#define LED_WEIGHT_PIN 26
#define BUZZER_PIN 27

#define NOTE_C5  523
int melody[] = {
  NOTE_C5, 0
};

int noteDurations[] = {
  4, 4
};
int period = 100;
unsigned long time_now = 0;

#define WEIGHT_THRESHOLD 5

HX711 scale;

float weightRead = 0;
bool beltRead = 0;

float calibration_factor = -11450; //mais ou menos o valor para ter saida em kg

MCP_CAN CAN0(23);                               // Set CS to pin 10 arduino and 5 esp

/*da mensagem CAN*/
long unsigned int rxId;
unsigned char len = 0;
unsigned char rxBuf[24];
char msgString[24];                        // Array to store serial string
int d, x, t0;
unsigned int cont = 0; //para testes
bool wait;
bool weight_ok = 0;


byte data_new[3];
byte data_old[2];

long unsigned int time0, time1;

void CAN_send();

void setup() {
  pinMode(LED_BELT_PIN, OUTPUT);
  pinMode(LED_WEIGHT_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(115200);

  pinMode(BeltRead_Pin, INPUT);

  Serial.println("HX711 calibration sketch");
  Serial.println("Remove all weight from scale");
  Serial.println("After readings begin, place known weight on scale");
  
  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
  scale.set_scale();
  scale.tare(); //Reset the scale to 0
  
  long zero_factor = scale.read_average(); //Get a baseline reading
  Serial.print("Zero factor: "); //This can be used to remove the need to tare the scale. Useful in permanent scale projects.
  Serial.println(zero_factor);

  if(CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK)
    CAN0.setMode(MCP_NORMAL);                     // Set operation mode to normal so the MCP2515 sends acks to received data.
  else Serial.println("Error Initializing MCP2515...");
 

  attachInterrupt(digitalPinToInterrupt(BeltRead_Pin), CAN_send, CHANGE); // CHANGE: aciona interrupção quando o estado do pino muda
  attachInterrupt(digitalPinToInterrupt(LOADCELL_DOUT_PIN), CAN_send, CHANGE);
}

void loop() {

  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  Serial.print("Reading: ");
  weightRead = scale.get_units()*0.453592;
  time_now = millis();
  
  // Verifica RNF3 - Balanca 
  if (weightRead >= WEIGHT_THRESHOLD)
  {
    //Serial.print("Atende RNF3");
    digitalWrite(LED_BELT_PIN, LOW);
    weight_ok = 1;
  }
  else{
    //Serial.print("Não atende RNF3");  
    digitalWrite(LED_BELT_PIN, HIGH);
    weight_ok = 0;
  }

  Serial.print(weightRead, 3);
  Serial.print(" kg, Belt read: ");

  beltRead = digitalRead(BeltRead_Pin);
  Serial.print(beltRead);

  // Verifica RF2 - Cinto
  if (beltRead == 0 || beltRead == 1) {
    //Serial.print("Atende RF2");
  }
  else{
    //Serial.print("Não atende RF2");  
  }

  // Alerta RF8 - Balanca e Cinto 
  if (beltRead == 0 && weightRead > 0) {
    //Serial.print("Atende RF8");
  }
  else{
    //Serial.print("Não atende RF8");  

   for (int thisNote = 0; thisNote < 2; thisNote++) 
    { // MUDAR ISSO AQUI POR CAUSA DO DELAY
      int noteDuration = 1000 / noteDurations[thisNote];
      tone(BUZZER_PIN, melody[thisNote], noteDuration);

      int pauseBetweenNotes = noteDuration * 1.30;
      
      while(millis() < time_now + pauseBetweenNotes+10){}
      noTone(BUZZER_PIN);
    }
  }

  if (beltRead == 0) {
    digitalWrite(LED_WEIGHT_PIN, LOW);
  }
  else {
    digitalWrite(LED_WEIGHT_PIN, HIGH);
  }

  Serial.println();
  delay(100);
}


void CAN_send(){
  wait = false;
  time0 = millis();
  if (weight_ok == 0 || digitalRead(BeltRead_Pin) == 0) // se um dos dois está fora
  {
    while ((weight_ok == 0 || digitalRead(BeltRead_Pin) == 0) && wait){
      time1 = millis();
      if ((time1-time0) > 500) wait = true;     
    }   
  }
  data_new[0] = digitalRead(BeltRead_Pin);
  data_new[1] = digitalRead(LOADCELL_DOUT_PIN);
  data_new[2] = cont;
  if ((data_old[0] != data_new[0]) || (data_old[1] != data_new[1])){   // VER AQUI
    // send data_new:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
    t0 = micros();
        Serial.println(micros());

    byte sndStat = CAN0.sendMsgBuf(0x100, 0, 3, data_new); // faz envio
    /*if(sndStat == CAN_OK){
      Serial.println("\t Message Sent Successfully!");
    } else {
      Serial.println("\t Error Sending Message...");
    }*/
    //sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", 0x100, 2); //teste para imprimir saída
    Serial.println(micros());
    Serial.print("msg ");
    Serial.print(cont++);
    for(byte i = 0; i<3; i++){
        sprintf(msgString, " 0x%.2X", data_new[i]);
        Serial.print(msgString);
     }
    Serial.println();
  }
  for (int i = 0; i<2; i++) data_old[i] = data_new[i];
}