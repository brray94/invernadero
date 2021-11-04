#ifndef CULTIVOS
#define CULTIVOS  //header guard
#include "cultivos.h"
#include "valores_cultivos.h"

//este es el esqueleto del metodo contenido en "cultivos.ino" igualmente los metodos que requiramos para los cultivos

// deben colocarse aca el esqueleto y al otro lado la parte logica

// se crea la estructura para cultivo, ¿ quizas colocar pin de medicion de temp, humedad y control bomba agua ?

bool ventiladoresEncendidos;

struct Cultivo{
  String nombre_cultivo = "";
  float humedad_min = 0.0;
  float humedad_max = 0.0;
  float riego_semana = 0.0;
  float temp_min = 0.0;
  float temp_max = 0.0;
  
};

void cambiaCultivo(Cultivo*,int);

//recibe #pin y tiempo de activacion
void ActivaBombaAgua( int, int);

// recibe pin del ventilador y bool correspondiente a encendido/apagado
void CambiaEstadoVentilador(int, bool );

//recibe la lectura del sensor y verifica si esta en el rango de la planta
// devuelve -1 si esta por debajo del valor min, 0 si se encuentra en rango y 1 si hay humedad de mas 
int verificarHumedad (float, Cultivo*);
// retorne verdadero o falso, usar ese metodo para activar las bombas de agua
// igual otro que verifique la humedad del aire y active los ventiladores

// si es >1000 es mas seguro una falla, entonces reportar error

// devuelve valor entre -1 y 100, siendo -1 error
int leerHumedadSuelo(int);


#endif
