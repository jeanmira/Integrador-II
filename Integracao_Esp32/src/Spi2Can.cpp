/*=======================================================================//
                              SPI2CAN.CPP
//=======================================================================*/
#include "Spi2Can.hpp"

/*-----------------------------------------------------------------------//
                                VARIAVEIS
//-----------------------------------------------------------------------*/
// Selecionando o pino CS
MCP_CAN CAN0(23);

// auxiliares para mensagens
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

bool peso, cinto;

/*-----------------------------------------------------------------------//
                               data_att
//-----------------------------------------------------------------------//
    funcao auxiliar para atualizar valores booleanos vindos da decisao 
    do controlador de acordo com requisitos.
//-----------------------------------------------------------------------*/
void data_att(bool belt, bool weight)
{
    peso  = weight;
    cinto = belt;
}

/*-----------------------------------------------------------------------//
                               CAN_send
//-----------------------------------------------------------------------//
    funcao para o envio de mensagens para o modulo conversor MCP2515,
    de SPI para mensagens CAN.
//-----------------------------------------------------------------------*/
void CAN_send()
{

    wait = false;
    time0 = millis();
    if (!peso || !cinto) // se um dos dois está fora
    {
        while ((!peso || !cinto) && wait)
        {
            time1 = millis();
            if ((time1 - time0) > 500)
                wait = true;
        }
    }
    data_new[0] = cinto;
    data_new[1] = peso;
    data_new[2] = cont; // NAO SEI PRA QUE SERVE
    if ((data_old[0] != data_new[0]) || (data_old[1] != data_new[1]))
    { // VER AQUI
        // send data_new:  ID = 0x100, Standard CAN Frame, Data length = 8 bytes, 'data' = array of data bytes to send
        t0 = micros();
        Serial.println(micros());

        byte sndStat = CAN0.sendMsgBuf(0x100, 0, 3, data_new); // faz envio
        /*if(sndStat == CAN_OK){
          Serial.println("\t Message Sent Successfully!");
        } else {
          Serial.println("\t Error Sending Message...");
        }*/
        // sprintf(msgString, "Extended ID: 0x%.8lX  DLC: %1d  Data:", 0x100, 2); //teste para imprimir saída
        Serial.println(micros());
        Serial.print("msg ");
        Serial.print(cont++);
        for (byte i = 0; i < 3; i++)
        {
            sprintf(msgString, " 0x%.2X", data_new[i]);
            Serial.print(msgString);
        }
        Serial.println();
    }
    for (int i = 0; i < 2; i++)
        data_old[i] = data_new[i];
}
/*-----------------------------------------------------------------------//
                            CAN_setup
//-----------------------------------------------------------------------//
    funcao para realizacao da leitura das celulas de carga, conectadas
    ao modulo amplificador HX711.
//-----------------------------------------------------------------------*/
void CAN_setup(bool debug = false)
{
    if (CAN0.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK)
        CAN0.setMode(MCP_NORMAL); // Set operation mode to normal so the MCP2515 sends acks to received data.
    else if (debug)
        Serial.println("Erro de inicializacao do MCP2515...");

    attachInterrupt(digitalPinToInterrupt(BELTREAD_PIN), CAN_send, CHANGE); // CHANGE: aciona interrupção quando o estado do pino muda
    attachInterrupt(digitalPinToInterrupt(LOADCELL_DOUT_PIN), CAN_send, CHANGE);
}

/*=======================================================================//
                              SPI2CAN.CPP
//=======================================================================*/