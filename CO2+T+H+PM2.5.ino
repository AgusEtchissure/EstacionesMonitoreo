#include "Adafruit_CCS811.h" //Biblioteca para el sensor CCS811
Adafruit_CCS811 ccs;  //Declaración de un objeto del sensor

// Incluimos librería
#include <DHT.h>
 
// Definimos el pin digital donde se conecta el sensor
#define DHTPIN 2
// Dependiendo del tipo de sensor
#define DHTTYPE DHT11
 
// Inicializamos el sensor DHT11
DHT dht(DHTPIN, DHTTYPE);
 
#include "Adafruit_PM25AQI.h"

#include <SoftwareSerial.h>
SoftwareSerial pmSerial(6, 7);

Adafruit_PM25AQI aqi = Adafruit_PM25AQI();


void setup()
{
  Serial.begin(9600);
  pmSerial.begin(9600);
  dht.begin(9600); 

  if(!ccs.begin()){
    Serial.println("No se pudo inicializar el sensor, revisa tus conexiones");
    while(1);     //se detiene el programa
  }
  //Esperando la transmisión de datos del sensor
 while(!ccs.available());  
  
}

void loop() {

  delay(5000);

 if(ccs.available())  //si el sensor esta disponible
  if(!ccs.readData()){   //lee los datos
     Serial.print("\nCO2: ");
     Serial.print(ccs.geteCO2());
     Serial.print(" ppm: ;\n"); 
   }
  else{
       Serial.println("ERROR al leer el sensor");
       ccs.readData();
   }


  // Leemos la humedad relativa
  float h = dht.readHumidity();
  // Leemos la temperatura en grados centígrados (por defecto)
  float t = dht.readTemperature();
    // Leemos la temperatura en grados Fahreheit
  float f = dht.readTemperature(true);
 
  // Comprobamos si ha habido algún error en la lectura
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Error obteniendo los datos del sensor DHT11");
    return;
  }
 

  Serial.print("\nHumedad: ");
  Serial.print(h);
  Serial.print(" %;\n");
  Serial.print("Temperatura: ");
  Serial.print(t);
  Serial.print(" *C;\n");

    PM25_AQI_Data data;
  
  Serial.print("\nPM 2.5: "); 
  Serial.print(data.particles_25um); 
  Serial.print(" ug/m3 ");
 
}  
