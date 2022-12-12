#include <SPI.h>    // incluye libreria interfaz SPI
#include <SD.h>     // incluye libreria para tarjetas SD
#include <DHT.h>    // incluye libreria DHT de Adafruit
#include <DHT_U.h>    // incluye libreria Adafruit Unified Sensor
#include "Adafruit_CCS811.h" //Biblioteca para el sensor CCS811
#include "Adafruit_PM25AQI.h"
#include <SoftwareSerial.h>
#include "DFRobot_OzoneSensor.h"
 
#define SENSOR 2    // constante SENSOR en pin digital 4 (senal de DHT11)
#define SSpin 2   // Slave Select en pin digital 10
DHT dht(SENSOR, DHT11);   // objeto dht del tipo DHT en pin 4 y modelo DHT11
#define COLLECT_NUMBER  20              
#define Ozone_IICAddress OZONE_ADDRESS_3
Adafruit_CCS811 ccs;
SoftwareSerial pmSerial(6, 7);
 
DFRobot_OzoneSensor Ozone;
 
int TEMPERATURA;    // variable para almacenar valor de temperatura
int HUMEDAD;      // variable para almacenar valor de humedad
int CO2; // variable que almacena CO2
int OZONO; // variable que almacena OZONO
int PM10; // variable que almacena material particulado 1.0
int PM25; // variable que almacena material particulado 2.5
int PM100; // variable que almacena material particulado 10
 
File archivo;     // objeto archivo del tipo File
 
Adafruit_PM25AQI aqi = Adafruit_PM25AQI();

 
void setup() {
  Serial.begin(9600);
 // pmSerial.begin(9600);
  dht.begin(9600);
    if(!ccs.begin()){
    Serial.println("No se pudo inicializar el sensor, revisa tus conexiones");
    while(1);     //se detiene el programa
  }
  //Esperando la transmisión de datos del sensor
 while(!ccs.available());
 
 if (! aqi.begin_UART(&pmSerial)) { // connect to the sensor over hardware serial
 // if (! aqi.begin_UART(&pmSerial)) { // connect to the sensor over software serial
    Serial.println("Could not find PM 2.5 sensor!");
    while (1) delay(10);
  }
 
  Serial.println("PM25 found!");
 
  Serial.println("Inicializando tarjeta ...");  // texto en ventana de monitor
  if (!SD.begin(SSpin)) {     // inicializacion de tarjeta SD
    Serial.println("fallo en inicializacion !");// si falla se muestra texto correspondiente y
    return;         // se sale del setup() para finalizar el programa
  }
  Serial.println("inicializacion correcta");  // texto de inicializacion correcta
  archivo = SD.open("datos.txt", FILE_WRITE); // apertura para lectura/escritura de archivo datos.txt
 
  PM25_AQI_Data data;
 
 // if (! aqi.read(&data)) {
 //   Serial.println("Could not read from AQI");
 //   delay(500);  // try again in a bit!
//    return;
//  }

 
  if (archivo) { 
    for (int i=1; i < 289; i++){    
     
      TEMPERATURA = dht.readTemperature();
      HUMEDAD = dht.readHumidity();
      CO2 = ccs.readData();
      OZONO = Ozone.readOzoneData(COLLECT_NUMBER);
      PM10 = data.pm10_env;
      PM25 = data.pm25_env;
      PM100 = data.pm100_env;
 
      archivo.print(TEMPERATURA);  
      archivo.print(" *C; ");    
      archivo.print(HUMEDAD);  
      archivo.print(" %; ");    
      archivo.print(CO2);  
      archivo.print(" ppm; ");    
      archivo.print(OZONO);  
      archivo.print(" ppm; ");    
      archivo.print(PM10);    //PM 1.0
      archivo.print(" ug/m3; ");
      archivo.print(PM25);    //PM 2.5
      archivo.print(" ug/m3; ");  
      archivo.print(PM100);    //PM 10
      archivo.print(" ug/m3; \n");          
 
 
      Serial.print(TEMPERATURA);  
      Serial.print(" *C; ");    
      Serial.print(HUMEDAD);  
      Serial.print(" %; ");    
      Serial.print(CO2);  
      Serial.print(" ppm; ");    
      Serial.print(OZONO);  
      Serial.print(" ppm; ");    
      Serial.print(PM10);    //PM 1.0
      Serial.print(" ug/m3; ");
      Serial.print(PM25);    //PM 2.5
      Serial.print(" ug/m3; ");  
      Serial.print(PM100);    //PM 10
      Serial.print(" ug/m3; \n");
 
 
      delay(5000);        // demora de 5 segundos
      }
    archivo.close();        // cierre de archivo
    Serial.println("escritura correcta"); // texto de escritura correcta en monitor serie
  } else {
    Serial.println("error en apertura de datos.txt"); // texto de falla en apertura de archivo
  }
}
 
void loop() {     // funcion loop() obligatoria de declarar pero no utilizada
  // nada por aqui
}
