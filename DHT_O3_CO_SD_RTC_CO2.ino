#include "GravityRtc.h"
#include "Wire.h" 
#include <SPI.h> 
#include <SD.h>
#include "DFRobot_OzoneSensor.h"
#include <DHT.h>    // incluye libreria DHT de Adafruit
#include <DHT_U.h>    // incluye libreria Adafruit Unified Sen

#define COLLECT_NUMBER  20              
#define Ozone_IICAddress OZONE_ADDRESS_3
#define SENSOR 2

DFRobot_OzoneSensor Ozone;

DHT dht(SENSOR, DHT11); 
 
GravityRtc rtc;     //RTC Initialization

int sensor = 0;
float temperatura;
float humedad;
int ozono;
int co;

int sensorIn = A0;


File dataFile;

void setup() {
  Serial.begin(9600);
  dht.begin(9600); 
 
  rtc.setup();

  analogReference(DEFAULT);
 
 
  //Set the RTC time manually
  rtc.adjustRtc(2023,6,22,3,17,50,0);  //Set time: 2017/6/19, Monday, 12:07:00
 
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
 
}
 
void loop() {

rtc.read();
  //*************************Time********************************
 String dataString = "";  // Cadena vacía
 String dataString2 = "";
 String dataString3 = "";
 String dataString4 = "";
 String dataString5 = "";

 ozono = Ozone.readOzoneData(COLLECT_NUMBER);
 int val = digitalRead(4);//Read Gas value from analog 0
 co = (val,DEC);
 int CO2 = analogRead(sensorIn);
 sensor = digitalRead(SENSOR);  // Lee el sensor
 temperatura = dht.readTemperature();  //Temperatura en Celsius
 humedad = dht.readHumidity();

 dataString += String(temperatura);
 dataString2 += String(humedad);
 dataString3 += String(ozono);
 dataString4 += String(co);
 dataString5 += String(CO2);
 delay(500);
   // Abre el archivo en la memoria SD
  File dataFile = SD.open("datalog.txt", FILE_WRITE);
  // Revisa si el archivo está disponible para escribir:
  if (dataFile) {
  // dataFile.print(rtc.day);
  // dataFile.print("/");//month
  // dataFile.print(rtc.month);
  // dataFile.print("/");//day
  // dataFile.print(rtc.year);
  // dataFile.print("    ");  
  // dataFile.print(rtc.hour);
  // dataFile.print(":");//minute
  // dataFile.print(rtc.minute);
  // dataFile.print(":");//second
  // dataFile.print(rtc.second);
  // dataFile.print(";   ");//second
  dataFile.print(dataString);
  dataFile.print("; ");
  dataFile.print(dataString2);
  dataFile.print("; ");
  dataFile.print(dataString3);
  dataFile.print("; ");
  dataFile.print(dataString4);
  dataFile.print("; ");
  dataFile.print(dataString5);
  dataFile.println("; ");
  dataFile.close();
 
  // Serial.print(rtc.day);
  // Serial.print("/");//month
  // Serial.print(rtc.month);
  // Serial.print("/");//day
  // Serial.print(rtc.year);
  // Serial.print("    ");  
  // Serial.print(rtc.hour);
  // Serial.print(":");//minute
  // Serial.print(rtc.minute);
  // Serial.print(":");//second
  // Serial.print(rtc.second);
  // Serial.print(";    ");//second
  Serial.print(dataString);
  Serial.print("; ");
  Serial.print(dataString2);
  Serial.print("; ");
  Serial.print(dataString3);
  Serial.print("; ");
  Serial.print(dataString4);
  Serial.print("; ");
  Serial.print(dataString5);
  Serial.println("; ");

  delay(10000);   // Toma un dato cada 30 segundos
 
   }
  // Si el archivo no se encuentra abierto, manda un error:
  else {
    Serial.println("Error al abrir el archivo datalog.txt");
  }
 
  
 
}
