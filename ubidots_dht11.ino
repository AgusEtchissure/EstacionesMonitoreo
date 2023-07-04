#include <Ubidots.h>

#include <DHT.h>

#define UBIDOTS_TOKEN "BBFF-hkNmnRiiubNGnAGSuh8DpGa5ZSNwtL"  // Put here your Ubidots TOKEN
#define DEVICE_LABEL "ESP32"   // Put here your Device label to which data  will be published
#define VARIABLE_LABEL "VARIABLE_LABEL_TEMPERATURE"
#define VARIABLE_LABEL2 "VARIABLE_LABEL_HUMIDITY"// Put here your Variable label to which data  will be published

const char *WIFI_SSID = "WUTEC-Estudiantes";      // Put here your Wi-Fi SSID
const char *WIFI_PASS = "ley19043";      // Put here your Wi-Fi password

Ubidots embebido(UBIDOTS_TOKEN, UBI_HTTP);

#define PIN_DHT 5
#define DHTTYPE DHT11
 
DHT dht(PIN_DHT, DHTTYPE);

float temp;
int hum;


void setup() {
  Serial.begin(9600);
  dht.begin();

  embebido.wifiConnect(WIFI_SSID, WIFI_PASS);
  Serial.println("Conectado a la red Wifi");
  Serial.println(WiFi.SSID());
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());

  temp = 0;
  hum = 0;
}

void loop() {
  hum = dht.readHumidity();
  temp = dht.readTemperature();

  Serial.println("Temperatura:"+ String(temp) + "; Humedad:" + String(hum));
  embebido.add(VARIABLE_LABEL, temp);
  embebido.add(VARIABLE_LABEL2, hum);

  bool buffersent = false;
    buffersent = embebido.send(DEVICE_LABEL);
  
  if(buffersent) {
    Serial.println("Datos enviados al dispositivo");
  }

  delay(15000);

}
