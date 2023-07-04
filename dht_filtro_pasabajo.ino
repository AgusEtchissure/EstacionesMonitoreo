#include <DHT.h>
#include <Ubidots.h>

const char *WIFI_SSID = "WUTEC-Estudiantes";  // Put here your Wi-Fi SSID
const char *WIFI_PASS = "ley19043";           // Put here your Wi-Fi password


#define UBIDOTS_TOKEN "BBFF-hkNmnRiiubNGnAGSuh8DpGa5ZSNwtL"  // Put here your Ubidots TOKEN
#define DEVICE_LABEL "prueba"                                // Put here your Device label to which data  will be published
#define temp "temp"
#define temp_fil "temp_fil" 
#define hum "hum"
#define hum_fil "hum_fil" 

Ubidots embebido(UBIDOTS_TOKEN, UBI_HTTP);

#define DHTPIN 4       // Pin del sensor DHT11
#define DHTTYPE DHT11  // Tipo de sensor DHT (DHT11, DHT21, DHT22)

DHT dht(DHTPIN, DHTTYPE);

// Parámetros del filtro pasa bajo
float alpha = 0.2;  // Factor de suavizado (ajustable según tus necesidades)
float filteredValue_T = 0.0;
int filteredValue_H = 0;



void setup() {
  Serial.begin(9600);
  dht.begin();

  embebido.wifiConnect(WIFI_SSID, WIFI_PASS);

  Serial.println("Conectado a la red Wifi");
  Serial.println(WiFi.SSID());
  Serial.print("IP: ");
  Serial.println(WiFi.localIP());
}

void loop() {
  // Lectura de la humedad y temperatura del sensor DHT11
  int humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // Aplicación del filtro pasa bajo
  filteredValue_T = alpha * temperature + (1 - alpha) * filteredValue_T;
  filteredValue_H = alpha * humidity + (1 - alpha) * filteredValue_H;

  // Mostrar los valores filtrados en el monitor serial
  Serial.print(temperature);
  Serial.print("; ");
  Serial.print(filteredValue_T);
  Serial.print(" --- ");
  Serial.print(humidity);
  Serial.print("; ");
  Serial.println(filteredValue_H);


  embebido.add(temp, temperature);
  embebido.add(hum, humidity);
  embebido.add(temp_fil, filteredValue_T);
  embebido.add(hum_fil, filteredValue_H);

  bool buffersent = false;

  buffersent = embebido.send(DEVICE_LABEL);

  if (buffersent) {
    Serial.println("Datos enviados al dispositivo");
  }
  delay(15000);  // Esperar 2 segundos entre lecturas
}
