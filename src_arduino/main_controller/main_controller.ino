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

  // Se inicializan los 3 cultivos ( todo: buscar la manera de que sea dinamico)
  Cultivo cultivo_1;
  Cultivo cultivo_2;
  Cultivo cultivo_3;
  
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
  // asca va el codigo que va a ejecutar indefinidamente


  // Para enviar el correo seria
  //enviarMensaje("texto del mensaje", &smtp);  nota: quizas sobrecargar funcion para distinto des-
  //tinatario o encabezado  
  // 
  ArduinoOTA.handle();

  digitalWrite(RELAY_VENTILADOR, LOW); 
  delay(2000); // tiempo espera dos segundos

  
  /*
  float humSensor1 = sensorDht_1.readHumidity();

  verificaRangos( cultivo)

  float tempSensor1 = sensorDht_1.readTemperature();*/
  Serial.print("Sensor 1 data: ");
  Serial.printf("Humidity: %f", sensorDht_1.readHumidity()  );
  Serial.printf("Temperature: %f", sensorDht_1.readTemperature());
  Serial.println("\n-------------------");
  Serial.print("Sensor 2 data: ");
  Serial.printf("Humidity: %f", sensorDht_2.readHumidity()  );
  Serial.printf("Temperature: %f", sensorDht_2.readTemperature() );
  Serial.println("\n\n-------------------");
  Serial.printf("humedad min tomates : %f",TOMATES_HUMEDAD_MIN);
  
  digitalWrite(RELAY_VENTILADOR, HIGH);
  delay(5000);


}
