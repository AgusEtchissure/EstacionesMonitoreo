#include <WiFi.h>
#include <ESPAsyncWebServer.h>

const char* ssid = "WUTEC-Estudiantes";
const char* password = "ley19043";
const int ledPin = 2;  // Pin del LED interno del ESP32 receptor

AsyncWebServer server(80);

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

  server.on("/respuesta", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Hola");
    request->send(200, "text/plain", "Vapai");

    // Apagar el LED
    digitalWrite(ledPin, LOW);
  });

  server.on("/adios", HTTP_GET, [](AsyncWebServerRequest *request) {
    Serial.println("Solicitud de adiós recibida");
    request->send(200, "text/plain", "Adiós");

    // Encender el LED
    digitalWrite(ledPin, HIGH);
    delay(1000);  // Mantener el LED encendido durante 1 segundo
    digitalWrite(ledPin, LOW);  // Apagar el LED
  });

  server.begin();
}

void loop() {
  // No se requiere ninguna lógica adicional en el bucle del receptor
}