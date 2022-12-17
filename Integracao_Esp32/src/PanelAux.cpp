/*=======================================================================//
                              PANELAUX.CPP
//=======================================================================*/
#include "PanelAux.hpp"
/*-----------------------------------------------------------------------//
                                VARIAVEIS
//-----------------------------------------------------------------------*/
const int melody[] = {
  NOTE_C5, 0
};

const int noteDurations[] = {
  4, 4
};
const int period = 100;
unsigned long time_now = 0;

/*-----------------------------------------------------------------------//
                            Panel_setup
//-----------------------------------------------------------------------//
    funcao para realizacao da das configuracoes do painel com LEDs e 
    Buzzer, para avisos visuais.
//-----------------------------------------------------------------------*/
void Panel_setup()
{
    pinMode(LED_BELT_PIN, OUTPUT);
    pinMode(LED_WEIGHT_PIN, OUTPUT);
    pinMode(BUZZER_PIN, OUTPUT);
}

/*-----------------------------------------------------------------------//
                            Buzzer_sound
//-----------------------------------------------------------------------//
    funcao para controle do som emitido pelo Buzzer do painel de avisos.
//-----------------------------------------------------------------------*/
void Buzzer_sound()
{
    for (int thisNote = 0; thisNote < 2; thisNote++)
    { // MUDAR ISSO AQUI POR CAUSA DO DELAY
        int noteDuration = 1000 / noteDurations[thisNote];
        tone(BUZZER_PIN, melody[thisNote], noteDuration);

        int pauseBetweenNotes = noteDuration * 1.30;

        while (millis() < time_now + pauseBetweenNotes + 10){}
        noTone(BUZZER_PIN);
    }
}

/*-----------------------------------------------------------------------//
                              Extras
//-----------------------------------------------------------------------//
    funcoes simples para facilitar futuras alteracoes.
//-----------------------------------------------------------------------*/
void Seat_LED_on()
{
    digitalWrite(LED_BELT_PIN, HIGH);
}

void Seat_LED_off()
{
    digitalWrite(LED_BELT_PIN, LOW);
}

void Weight_LED_on()
{
    digitalWrite(LED_WEIGHT_PIN, HIGH);
}

void Weight_LED_off()
{
    digitalWrite(LED_WEIGHT_PIN, LOW);
}

/*=======================================================================//
                              PANELAUX.CPP
//=======================================================================*/