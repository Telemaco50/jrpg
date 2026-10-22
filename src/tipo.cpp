#include "tipo.h"
#include "logger.h"



constexpr float efectividad[4] [4] = {

{1.0f, 1.0f, 2.0f, 0.5f}, //Criptobro
{1.0f, 1.0f, 0.5f, 2.0f}, //Otaku
{0.5f, 2.0f, 2.0f, 1.0f}, //Correa
{0.5f, 2.0f, 1.0f, 1.0f}, //Narcolepsia

};

float efectividadAtaque(Tipo atacante, Tipo defensor){
 int fila= static_cast<int>(atacante);
 int columna= static_cast<int>(defensor);

 return efectividad[fila][columna];

};