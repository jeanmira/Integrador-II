#include <Arduino.h>
#include "HX711.h"

#define BeltRead_Pin 22
#define LOADCELL_DOUT_PIN  33
#define LOADCELL_SCK_PIN  32

HX711 scale;

float calibration_factor = -5450; //mais ou menos o valor para ter saida em kg

void setup() {
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
  
  // Verifica RF1 - Balanca
  if ((scale.get_units()*0.453592, 3) > 0) {
    Serial.print("Atende RF1");
  }
  else{
    Serial.print("Não atende RF1");  
  }

  // Verifica RNF3 - Balanca 
  if ((scale.get_units()*0.453592, 3) >= 40)
  {
    Serial.print("Atende RNF3");
  }
  else{
    Serial.print("Não atende RNF3");  
  }

  Serial.print(scale.get_units()*0.453592, 3);
  Serial.print(" kg, Belt read: ");
  Serial.print(digitalRead(BeltRead_Pin));

  // Verifica RF2 - Cinto
  if (digitalRead(BeltRead_Pin) == 0 || digitalRead(BeltRead_Pin) == 1) {
    Serial.print("Atende RF2");
  }
  else{
    Serial.print("Não atende RF2");  
  }

  // Alerta RF8 - Balanca e Cinto 
  if (digitalRead(BeltRead_Pin) == 1 && (scale.get_units()*0.453592, 3) > 0) {
    Serial.print("Atende RF8");
  }
  else{
    Serial.print("Não atende RF8");  
  }

  Serial.println();
  delay(100);
}
