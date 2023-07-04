#include <HardwareSerial.h>
#include "WiFi.h"

const uint8_t rx2Pin = 16;
const uint8_t tx2Pin = 17;

HardwareSerial SerialPort2(2);

const char* ssid = "Enzo-2.4Ghz";  //SSID de vuestro router.
const char* password = "monua2014";       //Contraseña de vuestro router.

WiFiClient cliente;

String data = "";

String strs[10];

int StringCount = 0;

void setup() {
  Serial.begin(9600);  // Iniciar comunicación serial

  SerialPort2.begin(9600, SERIAL_8N1, rx2Pin, tx2Pin);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wifi conectado!");
}

void loop() {
  if (SerialPort2.available() > 0) {           // Si hay datos disponibles en el puerto serial
    data = SerialPort2.readStringUntil('\n');  // Leer los datos hasta que se reciba un salto de línea
                                               //Serial.println(data); // Imprimir los datos recibidos
                                              
    Serial.println(data);
    while (data.length() > 0) {
      int index = data.indexOf(' ');
      if (index == -1)  // No space found
      {
        strs[StringCount++] = data;
        break;
      } else {
        strs[StringCount++] = data.substring(0, index);
        data = data.substring(index + 1);
      }
    }

    // Show the resulting substrings
    for (int i = 0; i < StringCount; i++) {
      Serial.print(strs[0]);
      Serial.print(" ");
      Serial.print(strs[1]);
      Serial.println(" ");
    }
    
  }
}
