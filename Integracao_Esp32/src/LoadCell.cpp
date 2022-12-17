/*=======================================================================//
                              LOADCELL.CPP
//=======================================================================*/
#include "LoadCell.hpp"
/*-----------------------------------------------------------------------//
                                VARIAVEIS
//-----------------------------------------------------------------------*/
HX711 scale;
// valor encontrado experimentalmente
const float calibration_factor = -11450;

/*-----------------------------------------------------------------------//
                            LoadCell_setup
//-----------------------------------------------------------------------//
    funcao para realizacao da das configuracoes das celulas de carga 
    conectadas ao modulo amplificador HX711.
//-----------------------------------------------------------------------*/
void LoadCell_setup(bool debug=false)
{
    if(debug)
    {
        Serial.println("Calibracao do modulo HX711:");
        Serial.println("Remova objetos externos do banco.");
    }

    scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);
    scale.set_scale();
    scale.tare(); //Reset the scale to 0

    long zero_factor = scale.read_average(); //leitura base

    if(debug)
    {
        Serial.println("Para uma estrutura fixa, reajustar o valor: ");
        Serial.print("Zero factor: "); 
        Serial.println(zero_factor);
    }
}

/*-----------------------------------------------------------------------//
                            LoadCell_read
//-----------------------------------------------------------------------//
    funcao para realizacao da leitura das celulas de carga, conectadas
    ao modulo amplificador HX711.
//-----------------------------------------------------------------------*/
float LoadCell_read(bool debug=false)
{
    float weight_ret;
    scale.set_scale(calibration_factor);
    // valor encontrado experimentalmente
    weight_ret = scale.get_units()*0.453592;

    if(debug)
    {
        Serial.print("Leitura de peso: ");
        Serial.print(weight_ret, 3);
        Serial.println("kg");
    }
    return weight_ret;
}

/*=======================================================================//
                              LOADCELL.CPP
//=======================================================================*/