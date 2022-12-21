/*=======================================================================//
                              PANELAUX.HPP
//=======================================================================*/
/*-----------------------------------------------------------------------//
                           INCLUDES / DEFINES
//-----------------------------------------------------------------------*/
#ifndef PANELAUX_H
#define PANELAUX_H

#include <Arduino.h>

#define LED_BELT_PIN 26
#define LED_WEIGHT_PIN 27
#define BUZZER_PIN 25
#define NOTE_C5  523

/*-----------------------------------------------------------------------//
                                FUNCOES
//-----------------------------------------------------------------------*/
void Panel_setup();
void Buzzer_sound();
void Buzzer_off();
void Seat_LED_on();
void Seat_LED_off();
void Weight_LED_on();
void Weight_LED_off();

#endif
/*=======================================================================//
                              PANELAUX.HPP
//=======================================================================*/