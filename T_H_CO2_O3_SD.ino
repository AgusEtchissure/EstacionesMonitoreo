///////// LIBRERÍAS PARA TRABAJAR CON LA SD ///////////
#include <SPI.h>
#include <SD.h>

//////// LIBRERÍAS PARA TRABAJAR CON SENSOR DE TEMPERATURA Y HUMEDAD ////////
#include <DHT_U.h>
#include <DHT.h>

/////// LIBRERÍA PARA SENSOR DE OZONO /////////
#include "DFRobot_OzoneSensor.h"

////// LIBRERÍA PARA SENSOR DE CO2 ///////
#include "Adafruit_CCS811.h"

////// DEFINICIÓN DE DATOS NECESARIOS ////////
#define SENSOR 2 // se define PIN de sensor DHT11
#define COLLECT_NUMBER  20 // se define desde donde extrae los datos el sensor de ozono
#define Ozone_IICAddress OZONE_ADDRESS_3 // se define la dirección del sensor de ozono


//////// VARIABLES ////////
int sensor = 0; // se define variable sensor inicializada en 0
float temperatura; // se define como variable flotante la temperatura
float humedad; // se define como variable flotante la humedad
int ozono; // se define como variable entera el ozono
int co2; //se define como variable entera el CO2

////// DECLARANDO OBJETOS ////////
DFRobot_OzoneSensor Ozone; //Declaración de un objeto del sensor de ozono
Adafruit_CCS811 ccs;  //Declaración de un objeto del sensor de CO2
DHT dht(SENSOR, DHT11); // PIN de donde extrae los datos y el tipo de sensor de temperatura y humedad

//////// FUNCIÓN PARA LEER CO2 ///////
int CO2_print(){
  int CO2val;
  CO2val=(ccs.geteCO2()); 
  if(ccs.available()){
    if(!ccs.readData()){
     // Serial.print("\n");
     // Serial.print(CO2val);
  return CO2val;
}
}
}

//////////////////////////////////////////////////////////////////////////////////////////

void setup()
{
  Serial.begin(9600);
  dht.begin(9600); 

  if(!ccs.begin()){
    Serial.println("No se pudo inicializar el sensor, revisa tus conexiones");
    while(1);     //se detiene el programa
  }
  //Esperando la transmisión de datos del sensor
 while(!ccs.available());  

  while (!Serial) {
    ; // Espera a que se inicie el puerto serial
  }
  Serial.print("Inicializando tarjeta SD ...");
 
  // Verifica que la tarjeta se inicialice
  if (!SD.begin()) {
    Serial.println("Fallo en tarjeta, o no hay tarjeta");
    return;  // No hace nada más:
  }
  Serial.println("Tarjeta lista");
  SD.remove("datalog.txt");  //Borra el archivo previo
 
  while(!Ozone.begin(Ozone_IICAddress)) { // se verifica si el sensor de ozono se encuentra o no disponible para medir
    Serial.println("I2c error de medición!");
    delay(1000);
  }  Serial.println("I2c conexión exitosa!");
 
  if(!ccs.begin()){ // se verifica si el sensor de CO2 se encuentra o no disponible para medir
    Serial.println("No se pudo inicializar el sensor, revisa tus conexiones");
    while(1);     //se detiene el programa
  }
  //Esperando la transmisión de datos del sensor
 while(!ccs.available());
 
}

//////////////////////////////////////////////////////////////////////////////////////////

void loop(){
  
  delay(1000);

 String dataString = "";  // Cadena vacía
 String dataString2 = ""; // Cadena vacía
 String dataString3 = ""; // Cadena vacía
 String dataString4 = ""; // Cadena vacía

 sensor = digitalRead(SENSOR);  // Datos de sensor DHT11 registrados desde el PIN "SENSOR"
 temperatura = dht.readTemperature();  //Temperatura en Celsius
 humedad = dht.readHumidity(); //Humedad en Porcentaje
 ozono = Ozone.readOzoneData(COLLECT_NUMBER); //Ozono en partes por millón
 co2 = CO2_print(); //CO2 en partes por millón
 dataString += String(temperatura);   //Almacena en la cadena
 dataString2 += String(humedad);   //Almacena en la cadena
 dataString3 += String(ozono);    //Almacena en la cadena
 dataString4 += String(co2);    //Almacena en la cadena
 
  // Abre el archivo en la memoria SD
  File dataFile = SD.open("datalog.txt", FILE_WRITE);

  // Revisa si el archivo está disponible para escribir:
  if (dataFile) {   // si esta disponible, imprime las cadenas de los datos de sensores en el archivo "dataFile"
    dataFile.print(dataString);
    dataFile.print("; ");
    dataFile.print(dataString2);
    dataFile.print("; ");
    dataFile.print(dataString3);
    dataFile.print("; "); 
    dataFile.print(dataString4);
    dataFile.println("; "); 
    dataFile.close(); // una vez registrados todos los datos al desactivar la fuente, se cierra la impresión de datos en el archivo
    
    // Envía las cadenas impresas por el puerto serial
    Serial.print(dataString);
    Serial.print("; ");
    Serial.print(dataString2);
    Serial.print("; ");
    Serial.print(dataString3);
    Serial.print("; ");
    Serial.print(dataString4);
    Serial.println("; ");

  }
  // Si el archivo no se encuentra abierto, manda un error:
  else {
    Serial.println("Error al abrir el archivo datalog.txt");
  }
 
  delay(1000);   // Toma un dato cada segundo

 
}  
