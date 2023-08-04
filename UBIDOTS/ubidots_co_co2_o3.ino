#include <HardwareSerial.h>
#include <Ubidots.h>

const uint8_t rx2Pin = 16;
const uint8_t tx2Pin = 17;


HardwareSerial SerialPort2(2);

const char *WIFI_SSID = "WUTEC-Estudiantes";  // Put here your Wi-Fi SSID
const char *WIFI_PASS = "ley19043";           // Put here your Wi-Fi password


#define UBIDOTS_TOKEN "BBFF-hkNmnRiiubNGnAGSuh8DpGa5ZSNwtL"  // Put here your Ubidots TOKEN
#define DEVICE_LABEL "Sala_IAGRO"                            // Put here your Device label to which data  will be published
#define CO2 "co2"
#define O3 "ozono"  // Put here your Variable label to which data  will be published
#define CO "co"

Ubidots embebido(UBIDOTS_TOKEN, UBI_HTTP);


String data = "";

String strs[10];

int StringCount = 0;

int dioxido;
int monoxido;
int o3;

void setup() {
  Serial.begin(9600);  // Iniciar comunicación serial

  SerialPort2.begin(9600, SERIAL_8N1, rx2Pin, tx2Pin);

  embebido.wifiConnect(WIFI_SSID, WIFI_PASS);

  Serial.println("Conectado a la red Wifi");
  Serial.println(WiFi.SSID());
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  if (SerialPort2.available() > 0) {           // Si hay datos disponibles en el puerto serial
    data = SerialPort2.readStringUntil('\n');  // Leer los datos hasta que se reciba un salto de línea
                                               //Serial.println(data); // Imprimir los datos recibidos
                                               //ThingSpeak.writeFields(channelID,WriteAPIKey);
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
      Serial.print(" ");
      Serial.print(strs[2]);
      Serial.println(" ");
    }

    o3 = strs[0].toInt();
    monoxido = strs[1].toInt();
    dioxido = strs[2].toInt();

    embebido.add(O3, o3);
    embebido.add(CO, monoxido);
    embebido.add(CO2, dioxido);

    bool buffersent = false;
    buffersent = embebido.send(DEVICE_LABEL);

    if (buffersent) {
      Serial.println("Datos enviados al dispositivo");
    }
  }
}
