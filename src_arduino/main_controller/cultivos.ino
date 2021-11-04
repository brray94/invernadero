#include "cultivos.h"
#include "valores_cultivos.h"



// metodo recibe la estructura cultivo y el codigo de la planta, construye los atributos segun la planta
void cambiaCultivo(Cultivo* cult, int codigo_planta){

  switch(codigo_planta){
    case 1:
        cult->nombre_cultivo = "Tomates";
        cult->humedad_min = TOMATES_HUMEDAD_MIN;
        cult->humedad_max = TOMATES_HUMEDAD_MAX;
        cult->riego_semana = TOMATES_RIEGO_SEMANA;
        cult->temp_min = TOMATES_TEMP_MIN;
        cult->temp_max = TOMATES_TEMP_MAX;
        break;
        
    case 2:
        cult->nombre_cultivo = "Pimientos";
        cult->humedad_min = PIMIENTO_HUMEDAD_MIN;
        cult->humedad_max = PIMIENTO_HUMEDAD_MAX;
        cult->riego_semana = PIMIENTO_RIEGO_SEMANA;
        cult->temp_min = PIMIENTO_TEMP_MIN;
        cult->temp_max = PIMIENTO_TEMP_MAX;
        break;
        
    case 3:
        cult->nombre_cultivo = "Calabacines";
        cult->humedad_min = CALABACIN_HUMEDAD_MIN;
        cult->humedad_max = CALABACIN_HUMEDAD_MAX;
        cult->riego_semana = CALABACIN_RIEGO_SEMANA;
        cult->temp_min = CALABACIN_TEMP_MIN;
        cult->temp_max = CALABACIN_TEMP_MAX;
        break;
        
    case 4:
        cult->nombre_cultivo = "Melones";
        cult->humedad_min = MELON_HUMEDAD_MIN;
        cult->humedad_max = MELON_HUMEDAD_MAX;
        cult->riego_semana = MELON_RIEGO_SEMANA;
        cult->temp_min = MELON_TEMP_MIN;
        cult->temp_max = MELON_TEMP_MAX;
        break;
        
    case 5:
        cult->nombre_cultivo = "Pepinos";
        cult->humedad_min = PEPINO_HUMEDAD_MIN;
        cult->humedad_max = PEPINO_HUMEDAD_MAX;
        cult->riego_semana = PEPINO_RIEGO_SEMANA;
        cult->temp_min = PEPINO_TEMP_MIN;
        cult->temp_max = PEPINO_TEMP_MAX;
        break;
        
    case 6:
        cult->nombre_cultivo = "Judias";
        cult->humedad_min = JUDIA_HUMEDAD_MIN;
        cult->humedad_max = JUDIA_HUMEDAD_MAX;
        cult->riego_semana = JUDIA_RIEGO_SEMANA;
        cult->temp_min = JUDIA_TEMP_MIN;
        cult->temp_max = JUDIA_TEMP_MAX;
        break;
        
    case 7:
        cult->nombre_cultivo = "Espinacas";
        cult->humedad_min = ESPINACA_HUMEDAD_MIN;
        cult->humedad_max = ESPINACA_HUMEDAD_MAX;
        cult->riego_semana = ESPINACA_RIEGO_SEMANA;
        cult->temp_min = ESPINACA_TEMP_MIN;
        cult->temp_max = ESPINACA_TEMP_MAX;
        break;
        
    case 8:
        cult->nombre_cultivo = "Zanahoria";
        cult->humedad_min = ZANAHORIA_HUMEDAD_MIN;
        cult->humedad_max = ZANAHORIA_HUMEDAD_MAX;
        cult->riego_semana = ZANAHORIA_RIEGO_SEMANA;
        cult->temp_min = ZANAHORIA_TEMP_MIN;
        cult->temp_max = ZANAHORIA_TEMP_MAX;
        break;
        
    case 9:
        cult->nombre_cultivo = "Lechugas";
        cult->humedad_min = LECHUGA_HUMEDAD_MIN;
        cult->humedad_max = LECHUGA_HUMEDAD_MAX;
        cult->riego_semana = LECHUGA_RIEGO_SEMANA;
        cult->temp_min = LECHUGA_TEMP_MIN;
        cult->temp_max = LECHUGA_TEMP_MAX;
        break;
        
    case 10:
        cult->nombre_cultivo = "Brocolis";
        cult->humedad_min = BROCOLI_HUMEDAD_MIN;
        cult->humedad_max = BROCOLI_HUMEDAD_MAX;
        cult->riego_semana = BROCOLI_RIEGO_SEMANA;
        cult->temp_min = BROCOLI_TEMP_MIN;
        cult->temp_max = BROCOLI_TEMP_MAX;
        break;
        
    case 11:
        cult->nombre_cultivo = "Coliflores";
        cult->humedad_min = COLIFLOR_HUMEDAD_MIN;
        cult->humedad_max = COLIFLOR_HUMEDAD_MAX;
        cult->riego_semana = COLIFLOR_RIEGO_SEMANA;
        cult->temp_min = COLIFLOR_TEMP_MIN;
        cult->temp_max = COLIFLOR_TEMP_MAX;
        break;
    
    }
  }

  // activa la bomba de agua por el tiempo definido (esto haltea al sistema por ese mismo tiempo)
  void ActivaBombaAgua(int pinNo, int msTime){
      // se verifica que el tiempo de regado sea prudente ( no mayor a 25seg)
      if(msTime < 25000){
        digitalWrite(pinNo, LOW);
        delay(msTime);
        digitalWrite(pinNo, HIGH);
      }
      
  }

  // recibe el pin del ventilador asi como si se desea el ventilador encendido o no
  
  void CambiaEstadoVentilador(int pinVentilador, bool encender){

      if(encender == true && ventiladoresEncendidos == false){
         ventiladoresEncendidos = true;
         digitalWrite(pinVentilador, LOW);
      }else if(encender == false && ventiladoresEncendidos == true){
         ventiladoresEncendidos = false;
         digitalWrite(pinVentilador, HIGH);
      }
    
  }

  // crear metodo verificar estado, este debe llamar a los metodos "CambiaEstadoVentilador" y "ActivaBombaAgua" segun necesidad

  int verificarHumedad(float valorHumedad, Cultivo* cult){
    // devuelve -1 si esta por debajo del valor min, 0 si se encuentra en rango y 1 si hay humedad de mas 

    
    //condicional para verificar que el valor leído por el sensor esté entre el rango de minimo y maximo
    if ((valorHumedad >= cult->humedad_min) && (valorHumedad <= cult->humedad_max)){
      return 0;      
    }else if (valorHumedad < cult->humedad_min){
      return -1;
    }else if (valorHumedad > cult->humedad_max){
      return 1;
    }
  }


  // leer sensor analogo tierra
  int leerHumedadSuelo(int pinSensor){
    int tmpVal = analogRead(pinSensor);

    if(tmpVal > 1000){
      // posible error ocurrido
      return -1;
    }else{
      // el valor esta al reves y entre mas se acerca a 0 realmente es mas humedo, por eso la operacion
      // cuando deberiamos estar obteniendo el porcentaje de humedad del suelo
      return (100-(tmpVal/10)); // normaliza para rango de 0 - 100
    }
  }
  
  // crear metodos que lean los sensores 
  
