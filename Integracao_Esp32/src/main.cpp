/*=======================================================================//
                                MAIN.CPP
//=======================================================================*/
/*-----------------------------------------------------------------------//
                           INCLUDES / DEFINES
//-----------------------------------------------------------------------*/
#include "PanelAux.hpp"
#include "Spi2Can.hpp"
#include "LoadCell.hpp"
#include "SeatBelt.hpp"

/*-----------------------------------------------------------------------//
                                 SETUP
//-----------------------------------------------------------------------*/
void setup() 
{
  Serial.begin(115200);
  Panel_setup();
  Belt_setup();
  LoadCell_setup(false);
  CAN_setup(false);
}

/*-----------------------------------------------------------------------//
                                 LOOP
//-----------------------------------------------------------------------*/
void loop() 
{
  float leitura_peso = LoadCell_read(false);
  bool peso_ok, cinto_ok;
  
  if (leitura_peso >= WEIGHT_THRESHOLD)
    peso_ok = true;
  else
    peso_ok = false;
  
  cinto_ok = Belt_read();

  if (cinto_ok && peso_ok) 
  {
    Seat_LED_off();
    Weight_LED_off();
  }
  else
  {
    Buzzer_sound();
    if(!cinto_ok) Seat_LED_on();
    if(!peso_ok)  Weight_LED_on();
  }

  data_att(cinto_ok, peso_ok);
  CAN_send();
  delay(100);
}
/*=======================================================================//
                                MAIN.CPP
//=======================================================================*/