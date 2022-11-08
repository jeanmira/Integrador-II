#include <Arduino.h>
#include "HX711.h"

#define BeltRead_Pin 22
#define LOADCELL_DOUT_PIN  33
#define LOADCELL_SCK_PIN  32
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

float calibration_factor = -5450; //mais ou menos o valor para ter saida em kg

void setup() {
  pinMode(LED_BELT_PIN, OUTPUT);
  pinMode(LED_WEIGHT_PIN, OUTPUT);
  pinMode(BUZZER_PIN, OUTPUT);

  Serial.begin(9600);

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
}

void loop() {

  scale.set_scale(calibration_factor); //Adjust to this calibration factor
  Serial.print("Reading: ");
  weightRead = scale.get_units()*0.453592;
  time_now = millis();
  
  // Verifica RF1 - Balanca
  if (weightRead > 0) {
    //Serial.print("Atende RF1");
  }
  else{
    //Serial.print("Não atende RF1");  
  }

  // Verifica RNF3 - Balanca 
  if (weightRead >= WEIGHT_THRESHOLD)
  {
    //Serial.print("Atende RNF3");
    digitalWrite(LED_BELT_PIN, LOW);
  }
  else{
    //Serial.print("Não atende RNF3");  
    digitalWrite(LED_BELT_PIN, HIGH);
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
