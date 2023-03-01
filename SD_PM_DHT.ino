#include "Adafruit_PM25AQI.h"  // Librería para sensor PM
#include "GravityRtc.h"        // Librería para sensor RTC
#include "Wire.h"              // Librería para sensor RTC
#include <DHT.h>               // Libreria DHT de Adafruit
#include <DHT_U.h>             // Libreria Adafruit Unified Sen
#include <SPI.h>               // Librería para SD
#include <SD.h>                // Librería para SD
#include <SoftwareSerial.h>    // Librería para sensor PM
#define SENSOR 2               // Definición de pin digital 2 con nombre "SENSOR"
#define MAESTRO 10             // Definición de pin digital 10 con nombre "MAESTRO"

SoftwareSerial pmSerial(9, 8);  // Definición de pines digitales de lectura de sensor PM

Adafruit_PM25AQI aqi = Adafruit_PM25AQI();  // Se define el nombre del sensor como "aqi"

DHT dht(SENSOR, DHT11);  // Se define sensor DHT con lectura en el pin "SENSOR" y del tipo DHT11

GravityRtc rtc;  //Inicialización del RTC

int sensor = 0;     // Variable que almacena valor cero
float temperatura;  // Variable de tipo flotante que almacena la lectura de la temperatura
float humedad;      // Variable de tipo flotante que almacena la lectura de la humedad


File myFile;  // Variable del tipo File denominada "myFile"


void setup() {
  Serial.begin(9600);    // Inicialización del puerto serial
  pmSerial.begin(9600);  // Inicialización del sensor PM
  dht.begin(9600);       // Inicialización del sensor DHT
  delay(100);            // Espera de 100 milisegundo

  pinMode(MAESTRO, OUTPUT);  // Se define como pin de salida de la SD el pin "MAESTRO"

  rtc.setup();  // Inicialización del RTC

  rtc.adjustRtc(2023, 3, 1, 2, 12, 21, 0);  // Se define fecha y hora manualmente. Formato Año/Mes/Día, Día de la semana, Hora:Minuto:Segundo

  while (!Serial) {
    ;  //Esperar a que se conecte el puerto serial.
  }
  Serial.print("Inicializando tarjeta SD...");  // Impresión de mensaje en puerto serial
  delay(100);                                   // Espera de 100 milisegundo
  if (!SD.begin(MAESTRO)) {                     // Si no se puede acceder a la tarjeta SD por alguna razón
    Serial.println("Inicialización fallida!");  // Impresión de mensaje en puerto serial
    while (1)
      ;  // Mientras que se permita el acceso
  }
  Serial.println("initialization done.");  // Impresión de mensaje en puerto serial

  Serial.println("Tarjeta lista");  // Impresión de mensaje en puerto serial
  SD.remove("datos.txt");           //Borra el archivo previo si lo hubiese

  if (!aqi.begin_UART(&pmSerial)) {                   // Si el sensor PM no se encuentra conectado correctamente
    Serial.println("No se encuentra el sensor PM!");  // Impresión de mensaje en puerto serial
    while (1) delay(10);                              // Mientras este habilitado se espera de 10 milisegundos
  }
  Serial.println("PM25 encontrado!");  // Impresión de mensaje en puerto serial
}



void loop() {



  PM25_AQI_Data data;  // Se define la lectura de las variables que registra el sensor PM como "data"


  if (!aqi.read(&data)) {
    Si no se encuentra habilitada la lectura
      Serial.println("No se puede leer desde AQI");  // Impresión de mensaje en puerto serial
    delay(500);                                      // Espera de 500 milisegundos
    return;                                          // Vuelve a revisar el estado del sensor PM y reitera el ciclo
  }
  Serial.println("Lectura exitosa desde AQI");

  rtc.read();  // Permite lectura del RTC

  String dataString = "";   // Cadena vacía para almacenar valores de temperatura
  String dataString2 = "";  // Cadena vacía para almacenar valores de humedad

  sensor = digitalRead(SENSOR);         // Lee el sensor
  temperatura = dht.readTemperature();  // La lectura de temperatura del DHT la almacena en la variable flotante temperatura
  humedad = dht.readHumidity();         // La lectura de humedad del DHT la almacena en la variable flotante humedad

  dataString += String(temperatura);  // Cadena vacía para almacenar valores de temperatura
  dataString2 += String(humedad);     // Cadena vacía para almacenar valores de humedad

  myFile = SD.open("datos.txt", FILE_WRITE);  // En el archivo myFile se crea el archivo datos.txt y se define en la SD como escritura

  if (myFile) {                          // Si la SD se encuentra disponible para escritura
    myFile.print(rtc.day);               // Impresión de variable día en tarjeta SD
    myFile.print("/");                   // Impresión de / en tarjeta SD para facilitar lectura "tipo fecha"
    myFile.print(rtc.month);             // Impresión de variable mes en tarjeta SD
    myFile.print("/");                   // Impresión de / en tarjeta SD para facilitar lectura "tipo fecha"
    myFile.print(rtc.year);              // Impresión de variable año en tarjeta SD
    myFile.print("    ");                // Impresión de un espacio para facilitar visualización en tarjeta SD
    myFile.print(rtc.hour);              // Impresión de variable hora en tarjeta SD
    myFile.print(":");                   // Impresión de : en tarjeta SD para facilitar lectura "tipo reloj"
    myFile.print(rtc.minute);            // Impresión de variable minuto en tarjeta SD
    myFile.print(":");                   // Impresión de : en tarjeta SD para facilitar lectura "tipo reloj"
    myFile.print(rtc.second);            // Impresión de variable segundo en tarjeta SD
    myFile.print(";   ");                // Impresión de una coma y espacio para facilitar visualización en tarjeta SD y posterior procesamiento (separación de columnas CSV)
    myFile.print(dataString);            // Impresión de variable temperatura en tarjeta SD
    myFile.print("; ");                  // Impresión de una coma y espacio para facilitar visualización en tarjeta SD y posterior procesamiento (separación de columnas CSV)
    myFile.print(dataString2);           // Impresión de variable humedad en tarjeta SD
    myFile.print("; ");                  // Impresión de una coma y espacio para facilitar visualización en tarjeta SD y posterior procesamiento (separación de columnas CSV)
    myFile.print(data.particles_03um);   // Impresión de variable PM 0.3um en tarjeta SD
    myFile.print("; ");                  // Impresión de una coma y espacio para facilitar visualización en tarjeta SD y posterior procesamiento (separación de columnas CSV)
    myFile.print(data.particles_05um);   // Impresión de variable PM 0.5um en tarjeta SD
    myFile.print("; ");                  // Impresión de una coma y espacio para facilitar visualización en tarjeta SD y posterior procesamiento (separación de columnas CSV)
    myFile.print(data.particles_10um);   // Impresión de variable PM 10um en tarjeta SD
    myFile.print("; ");                  // Impresión de una coma y espacio para facilitar visualización en tarjeta SD y posterior procesamiento (separación de columnas CSV)
    myFile.print(data.particles_25um);   // Impresión de variable PM 25um en tarjeta SD
    myFile.print("; ");                  // Impresión de una coma y espacio para facilitar visualización en tarjeta SD y posterior procesamiento (separación de columnas CSV)
    myFile.print(data.particles_50um);   // Impresión de variable PM 50um en tarjeta SD
    myFile.print("; ");                  // Impresión de una coma y espacio para facilitar visualización en tarjeta SD y posterior procesamiento (separación de columnas CSV)
    myFile.print(data.particles_100um);  // Impresión de variable PM 100um en tarjeta SD
    myFile.println("; ");                // Impresión de una coma y espacio para facilitar visualización en tarjeta SD y posterior procesamiento (separación de columnas CSV)
    myFile.close();                      // Se cierra la impresión en el archivo cuando no se encuentre conectado el circuito

    Serial.print(rtc.day);                         // Impresión de variable día en el puerto serial
    Serial.print("/");                             // Impresión de / en el puerto serial para facilitar lectura "tipo fecha"
    Serial.print(rtc.month);                       // Impresión de variable mes en el puerto serial
    Serial.print("/");                             // Impresión de / en el puerto serial para facilitar lectura "tipo fecha"
    Serial.print(rtc.year);                        // Impresión de variable año en el puerto serial
    Serial.print("    ");                          // Impresión de un espacio para facilitar visualización en el puerto serial
    Serial.print(rtc.hour);                        // Impresión de variable hora en el puerto serial
    Serial.print(":");                             // Impresión de : en el puerto serial para facilitar lectura "tipo reloj"
    Serial.print(rtc.minute);                      // Impresión de variable minuto en el puerto serial
    Serial.print(":");                             // Impresión de : en el puerto serial para facilitar lectura "tipo reloj"
    Serial.print(rtc.second);                      // Impresión de variable segundo en el puerto serial
    Serial.print(";   ");                          // Impresión de una coma y espacio para facilitar visualización y separación en el puerto serial
    Serial.print(dataString);                      // Impresión de variable temperatura en el puerto serial
    Serial.print("; ");                            // Impresión de una coma y espacio para facilitar visualización y separación en el puerto serial
    Serial.print(dataString2);                     // Impresión de variable humedad en el puerto serial
    Serial.print("; ");                            // Impresión de una coma y espacio para facilitar visualización y separación en el puerto serial
    Serial.print(data.particles_03um);             // Impresión de variable PM 0.3um en el puerto serial
    Serial.print("; ");                            // Impresión de una coma y espacio para facilitar visualización y separación en el puerto serial
    Serial.print(data.particles_05um);             // Impresión de variable PM 0.5um en el puerto serial
    Serial.print("; ");                            // Impresión de una coma y espacio para facilitar visualización y separación en el puerto serial
    Serial.print(data.particles_10um);             // Impresión de variable PM 10um en el puerto serial
    Serial.print("; ");                            // Impresión de una coma y espacio para facilitar visualización y separación en el puerto serial
    Serial.print(data.particles_25um);             // Impresión de variable PM 25um en el puerto serial
    Serial.print("; ");                            // Impresión de una coma y espacio para facilitar visualización y separación en el puerto serial
    Serial.print(data.particles_50um);             // Impresión de variable PM 50um en el puerto serial
    Serial.print("; ");                            // Impresión de una coma y espacio para facilitar visualización y separación en el puerto serial
    Serial.print(data.particles_100um);            // Impresión de variable PM 100um en el puerto serial
    Serial.println("; ");                          // Impresión de una coma y espacio para facilitar visualización y separación en el puerto serial
  } else {                                         // Si la SD no se encuentra disponible para escritura
    Serial.println("Error abriendo archivo.txt");  // Impresión de mensaje en puerto serial
  }
  delay(5000);  // Espera de 5 segundos entre cada lectura
}
