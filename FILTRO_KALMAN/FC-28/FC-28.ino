#include <SimpleKalmanFilter.h>

SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.01);

#include <DHT.h>

#define DHTPIN 34       // Pin del sensor DHT11 conectado al Arduino
#define DHTTYPE DHT11  // Tipo de sensor DHT

DHT dht(DHTPIN, DHTTYPE);


// Serial output refresh time
const long SERIAL_REFRESH_TIME = 100;
long refresh_time;

//const int pin34 = 34;

void setup() {
  Serial.begin(9600);
  dht.begin();
}

void loop() {

  // read a reference value from A0 and map it from 0 to 100
  float real_value = map(analogRead(DHTPIN), 4096, 0, 20, 50); // 4096 corresponde a los bits del esp32
  //float real_value = digitalRead(DHTPIN); // 4096 corresponde a los bits del esp32
  
  // add a noise to the reference value and use as the measured value
  float measured_value = real_value + random(-100,100)/100.0;

  // calculate the estimated value with Kalman Filter
  float estimated_value = simpleKalmanFilter.updateEstimate(measured_value);

  // send to Serial output every 100ms
  // use the Serial Ploter for a good visualization
  if (millis() > refresh_time) {
    Serial.print(real_value,2);
    Serial.print(",");
    Serial.print(measured_value,2);
    Serial.print(",");
    Serial.print(estimated_value,2);
    Serial.println();
    
    refresh_time = millis() + SERIAL_REFRESH_TIME;
  }

}