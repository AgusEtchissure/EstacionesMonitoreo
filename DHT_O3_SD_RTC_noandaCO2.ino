#include "GravityRtc.h"
#include "Wire.h"  
#include <SPI.h>
#include <SD.h>
#include <DHT.h>    // incluye libreria DHT de Adafruit
#include <DHT_U.h>    // incluye libreria Adafruit Unified Sen
#include "DFRobot_OzoneSensor.h"
#define COLLECT_NUMBER  20              
#define Ozone_IICAddress OZONE_ADDRESS_3
#define SENSOR 2
DFRobot_OzoneSensor Ozone;
 
#include "Adafruit_CCS811.h" //Biblioteca para el sensor CCS811
 Adafruit_CCS811 ccs;
 
#define SENSOR 2
 
DHT dht(SENSOR, DHT11);  
 
GravityRtc rtc;     //RTC Initialization
 
int sensor = 0;
float temperatura;
float humedad;
int ozono;
int co2;
 
//////// FUNCIÓN PARA LEER CO2 ///////
 int CO2_print() {
   int CO2val;
   CO2val = (ccs.geteCO2());
   if (ccs.available()) {
     if (!ccs.readData()) {
       return CO2val;
     }
   }
 }
 
void setup() {
  Serial.begin(9600);
  dht.begin();      // inicializacion de sensor
 
  rtc.setup();
 
  //Set the RTC time automatically: Calibrate RTC time by your computer time
  rtc.adjustRtc(F(__DATE__), F(__TIME__));
 
  //Set the RTC time manually
  //rtc.adjustRtc(2017,6,19,1,12,7,0);  //Set time: 2017/6/19, Monday, 12:07:00
 
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
 
    while(!Ozone.begin(Ozone_IICAddress)) {
    Serial.println("I2c error de medición!");
    delay(1000);
  }  Serial.println("I2c conexión exitosa!");
 
    if(!ccs.begin()){
     Serial.println("No se pudo inicializar el sensor, revisa tus conexiones");
     while(1);     //se detiene el programa
   }
   //Esperando la transmisión de datos del sensor
  while(!ccs.available());
 
}
 
void loop() {
  rtc.read();
  //*************************Time********************************
 String dataString = "";  // Cadena vacía
 String dataString2 = "";
 String dataString3 = "";
 String dataString4 = "";
 sensor = digitalRead(SENSOR);  // Lee el sensor
 temperatura = dht.readTemperature();  //Temperatura en Celsius
 humedad = dht.readHumidity();
 ozono = Ozone.readOzoneData(COLLECT_NUMBER);
 co2 = CO2_print();
 dataString += String(temperatura);   //Almacena en la cadena
 dataString2 += String(humedad);   //Almacena en la cadena
 dataString3 += String(ozono);
 dataString4 += String(co2);
   // Abre el archivo en la memoria SD
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  // Revisa si el archivo está disponible para escribir:
  if (dataFile) {
  dataFile.print(rtc.day);
  dataFile.print("/");//month
  dataFile.print(rtc.month);
  dataFile.print("/");//day
  dataFile.print(rtc.year);
  dataFile.print("    ");  
  dataFile.print(rtc.hour);
  dataFile.print(":");//minute
  dataFile.print(rtc.minute);
  dataFile.print(":");//second
  dataFile.print(rtc.second);
  dataFile.print("    ");//second
  dataFile.print(dataString);
  dataFile.print("; ");
  dataFile.print(dataString2);
  dataFile.print("; ");
  dataFile.print(dataString3);
  dataFile.print("; ");
  dataFile.print(dataString4);
  dataFile.println("; ");
  dataFile.close();
 
  Serial.print(rtc.day);
  Serial.print("/");//month
  Serial.print(rtc.month);
  Serial.print("/");//day
  Serial.print(rtc.year);
  Serial.print("    ");  
  Serial.print(rtc.hour);
  Serial.print(":");//minute
  Serial.print(rtc.minute);
  Serial.print(":");//second
  Serial.print(rtc.second);
  Serial.print("    ");//second
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
