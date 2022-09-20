#include <Arduino.h>
#include <HX711.h>


HX711 balanca; 


const int LOADCELL_DOUT_PIN = 2;
const int LOADCELL_SCK_PIN = 3;
float factor = 48011.00;                                                        /* Fator de calibração para ajuste da célula */
float peso;  

                                                                     /* SCK= pino 2 e DT= pino 3 */

                                                                               /* variável peso */

void setup() {
  balanca.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);/* rotina de configurações */
  Serial.begin(9600);                                                                       /* Baud rate da comunicação */
  Serial.println("Remova todos os pesos da balança");                                       /* Printa "Remova todos os pesos da balança" na COM */
  delay(1000);                                                                              /* atraso de 1000ms = 1s */
  Serial.println("Após estabilização das leituras, coloque o peso conhecido na balança");   /* Printa "Após estabilização das leituras, coloque o peso conhecido na balança" na COM */
  delay(1000);                                                                              /* atraso de 1000ms = 1s */
  Serial.println("Pressione + para incrementar o fator de calibração");                     /* Printa "Pressione + para incrementar o fator de calibração" na COM */
  Serial.println("Pressione - para decrementar o fator de calibração");                     /* Printa "Pressione - para decrementar o fator de calibração" na COM */
  delay(1000);                                                                              /* atraso de 1000ms = 1s */
  balanca.set_scale();                                                                      /* seta escala */
  balanca.tare();                                                                           /* escala da tara */

  long zero_factor = balanca.read_average();                                                /* Realizando a leitura */
}

void loop() {                                                                               /* chama função de loop */

  balanca.set_scale(factor);                                                    /* a balanca está em função do fator de calibração */

  Serial.print("Peso: ");                                                                   /* Printa "Peso:" na COM */
  peso = balanca.get_units(), 10;                                                           /* imprime peso  */
  if (peso < 0)                                                                             /* se a unidade for menor que 0 será considerado 0 */
  {
    peso = 0.00;                                                                            /* Para o caso do peso ser negativo, o valor apresentado será 0  */
  }                                                  
  Serial.print(peso);                                                                       /* Printa o peso na serial  */
  Serial.print(" kg");                                                                      /* Printa "kg" na serial   */
  Serial.print(" Fator de calibração: ");                                                   /* Printa "Fator de calibração:" na serial */
  Serial.print(factor);                                                         /* Printa o fator de calibração na serial */
  Serial.println();                                                                         /* Pula linha no serial */
  delay(500);                                                                               /* atraso de 500ms = 0.5s*/

  if(Serial.available())                                                                    /* caso sejam inseridos caracteres no serial */
  {
    char temp = Serial.read();
    if(temp == '+')                                                                         /* Se o + for pressionado     */
      factor += 1;                                                              /* incrementa 1 no fator de calibração   */
    else if(temp == '-')                                                                    /* Caso o - seja pressionado   */
      factor -= 1;                                                              /* Decrementa 1 do fator de calibração  */
  }
}
