// smtp email
#include <ESP_Mail_Client.h>
#include <ESP_Mail_FS.h>
#include <SDK_Version_Common.h>
#include "SMTP.h"

// invernadero
#include "DHT.h"
#include "valores_cultivos.h"
#include "cultivos.h"
#define PINSENSOR_1 4  // pin al que esta conectado sensor 1 humedad
#define PINSENSOR_2 17 // pin al que se conecta sensor 2 humedad
#define DHTTYPE DHT11 // define el tipo de sensor de humedad
#define RELAY_VENTILADOR 2 // define primer relay  
#define RELAY_BOMBA_1 16 // pin 16 para bomba agua 1
#define RELAY_BOMBA_2 17 // pin 17 para bomba agua 2
#define RELAY_BOMBA_3 5 // pin 5 para bomba agua 3
#define HUMEDAD_SUELO_1 32 // pin 32 para sensor 1
#define HUMEDAD_SUELO_2 33 // pin 33 para sensor 2

// ota config
#include "ota.h"
#include "wifiCredentials.h"
#define ESP32

// Sensores humedad / tmp
DHT sensorDht_1 (PINSENSOR_1, DHTTYPE); // inicializa var sensorDht_1, recibe numero del pin y tipo de sensor 
DHT sensorDht_2 (PINSENSOR_2, DHTTYPE);

 // objeto libreria smtp (correo)
  SMTPSession smtp;

void setup() {
  // aca se inicializa el codigo y las variables de entorno
  //se asigna la velocidad para la conexion serial 
  Serial.begin(115200);
  Serial.println("Iniciando sistema .\n");

  // Ota. hostname y config del wifi
  ArduinoOTA.setHostname("Invernadero IOT");
  setupOTA("Invernadero IOT", mySSID, myPASSWORD);

  // Inicializa sensores de temperatura / humedad
  sensorDht_1.begin();
  sensorDht_2.begin();

  // Se inicializan los pins encargados del relay
  pinMode(RELAY_VENTILADOR, OUTPUT);

  
  
  //imprime por serial ip del invernadero
  Serial.println("Invernadero dir ip : ");
  Serial.print(WiFi.localIP());

  // smtp config

  smtp.debug(0);
  //smtp.callback(smtpCallback);
  
  
  ESP_Mail_Session session;
  session = cargarConfigSession(session);

  if(!smtp.connect(&session)){
    Serial.println("! ! !  Error al crear session sistema de reportes (SMTP) ! ! !");
  }


}

void loop() {
   // todo: mover esto al setup, para evitar la re-inicializacion por cada vuelta al loop ( mayor consumo energia)
  // Se crea el array para los 3 cultivos 
  Cultivo arrCult[3];

  // se inicializan los cultivos con valores para pruebas
  cambiaCultivo(&arrCult[0], 7); // se asigna espinaca a cultivo 1 
  cambiaCultivo(&arrCult[1], 1); // se asigna tomate a cultivo 2
  cambiaCultivo(&arrCult[2], 4); // se asigna melon a cultivo 3
  //////////////////

  
  // Para enviar el correo seria
  //enviarMensaje("texto del mensaje", &smtp);  nota: quizas sobrecargar funcion para distinto des-
  //tinatario o encabezado  
  // 
  ArduinoOTA.handle();

  
  // arrCult  array
  for(int i = 0; i < 3; i++){
    // pd este codigo va a ser feo por el sensor faltante 
    float tmpHumedadAire;
    float tmpHumedadSuelo;
    float tmpTemperatura;
    int tmpPinBombaAgua;
    
    if(i == 0){ // primera planta
      tmpHumedadAire = sensorDht_1.readHumidity();
      tmpHumedadSuelo = leerHumedadSuelo(HUMEDAD_SUELO_1);
      tmpTemperatura = sensorDht_1.readTemperature();
      tmpPinBombaAgua = RELAY_BOMBA_1;
      
    }else if( i == 1){ // segundo nivel invernadero
      // por la falta del sensor, se promedian los resultados de los sensores que hay
      tmpHumedadAire = (sensorDht_1.readHumidity()+sensorDht_2.readHumidity())/2;
      tmpHumedadSuelo = (leerHumedadSuelo(HUMEDAD_SUELO_1)+leerHumedadSuelo(HUMEDAD_SUELO_2))/2;
      tmpTemperatura = (sensorDht_2.readTemperature()+sensorDht_1.readTemperature())/2;
      tmpPinBombaAgua = RELAY_BOMBA_2;
    }else{ // tercer nivel
      tmpHumedadAire = sensorDht_2.readHumidity();
      tmpHumedadSuelo = leerHumedadSuelo(HUMEDAD_SUELO_2);
      tmpTemperatura = sensorDht_2.readTemperature();
      tmpPinBombaAgua = RELAY_BOMBA_3;
    }
    
    // verificar que este en valores optimos

    int intTmpRes = verificarHumedad(tmpHumedadSuelo, &arrCult[i]);

    if( intTmpRes == -1){
      // Se encuentra por debajo del optimo
      ActivaBombaAgua(tmpPinBombaAgua, 2000); // se le pasan 2000 ms == 2 seg
    }else if( intTmpRes == 1){
      // se encuentra por encima del optimo, reportar al usuario y activar ventiladores
      CambiaEstadoVentilador(RELAY_VENTILADOR, true);
    }else{
      CambiaEstadoVentilador(RELAY_VENTILADOR, false);
    }
    
  }

 
}
