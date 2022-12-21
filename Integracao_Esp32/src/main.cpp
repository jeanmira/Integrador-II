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
	bool peso_ok, cinto_ok;
	float leitura_peso = LoadCell_read(false);

	if (leitura_peso >= WEIGHT_THRESHOLD)
		peso_ok = true;
	else
		peso_ok = false;

	cinto_ok = Belt_read();

	if (!cinto_ok)
		Seat_LED_on(); // sem cinto
	else
		Seat_LED_off();

	if (!peso_ok)
		Weight_LED_on();
	else
		Weight_LED_off();

	if(!cinto_ok || !peso_ok)
		Buzzer_sound();
	else
		Buzzer_off();
	
	data_att(cinto_ok, peso_ok);
	CAN_send();
	delay(100);

}
/*=======================================================================//
								MAIN.CPP
//=======================================================================*/