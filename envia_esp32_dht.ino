#include <DHT_U.h>             // Libreria Adafruit Unified Sen
#include "SoftwareSerial.h"
#include <DHT.h>

#define DHTPIN 2       // Pin del sensor DHT11 conectado al Arduino
#define DHTTYPE DHT11  // Tipo de sensor DHT

DHT dht(DHTPIN, DHTTYPE);

SoftwareSerial mySerial(5, 6);

int sensor = 0;     // Variable que almacena valor cero
float temperatura;  // Variable de tipo flotante que almacena la lectura de la temperatura
float humedad;      // Variable de tipo flotante que almacena la lectura de la humedad

void setup() {
  Serial.begin(9600);    // Iniciar comunicación serial
  mySerial.begin(9600);  // Iniciar comunicación serial
  dht.begin();           // Iniciar sensor DHT11   

}

void loop() {

  sensor = digitalRead(DHTPIN);         // Lee el sensor
  float temperatura = dht.readTemperature();  // La lectura de temperatura del DHT la almacena en la variable flotante temperatura
  float humedad = dht.readHumidity();         // La lectura de humedad del DHT la almacena en la variable flotante humedad

  Serial.print(temperatura);            // Impresión de variable temperatura en el puerto serial
  Serial.print(" ");                  // Impresión de una coma y espacio para facilitar visualización y separación en el puerto serial
  Serial.print(humedad);           // Impresión de variable humedad en el puerto serial
  Serial.println(" ");                // Impresión de una coma y espacio para facilitar visualización y separación en el puerto serial
  mySerial.print(temperatura);            // Impresión de variable temperatura en el puerto serial
  mySerial.print(" ");                  // Impresión de una coma y espacio para facilitar visualización y separación en el puerto serial
  mySerial.print(humedad);           // Impresión de variable humedad en el puerto serial
  mySerial.println(" ");                // Impresión de una coma y espacio para facilitar visualización y separación en el puerto serial
  delay(5000);  // Esperar 2 segundos antes de volver a leer los datos
}