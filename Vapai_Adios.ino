#include <WiFi.h>
#include <HTTPClient.h>

const char* ssid = "WUTEC-Estudiantes";
const char* password = "ley19043";
const int ledPin = 2;  // Pin del LED interno del ESP32 emisor

void setup() {
  Serial.begin(9600);

  pinMode(ledPin, OUTPUT);  // Configurar el pin del LED como salida
  //digitalWrite(ledPin, LOW);  // Apagar el LED inicialmente

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Conectando a WiFi...");
  }

  Serial.println("Conectado a la red WiFi");
}

void loop() {
  HTTPClient http;

  http.begin("http://10.211.3.6/respuesta");  // Reemplaza <IP_receptor_ESP32> con la dirección IP del ESP32 receptor

  int httpCode = http.GET();
  if (httpCode > 0) {
    String response = http.getString();
    Serial.println(response);
    
    // Encender el LED
    digitalWrite(ledPin, HIGH);
    delay(500);  // Mantener el LED encendido durante 1 segundo
    digitalWrite(ledPin, LOW);  // Apagar el LED
    delay(500);
    digitalWrite(ledPin, HIGH);
    delay(500);  // Mantener el LED encendido durante 1 segundo
    digitalWrite(ledPin, LOW);  // Apagar el LED
    
    // Enviar "Adiós"
    HTTPClient httpAdios;
    httpAdios.begin("http://10.211.3.6/adios");  // Reemplaza <IP_receptor_ESP32> con la dirección IP del ESP32 receptor
    int httpCodeAdios = httpAdios.GET();
    if (httpCodeAdios > 0) {
      String responseAdios = httpAdios.getString();
      Serial.println(responseAdios);
    }
    httpAdios.end();
  }

  http.end();

  delay(5000);  // Espera 5 segundos antes de enviar otra solicitud
}