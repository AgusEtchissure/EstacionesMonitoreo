#include <SimpleKalmanFilter.h>

SimpleKalmanFilter simpleKalmanFilter(2, 2, 0.01); // 0: valor inicial estimado; 2: incertidumbre inicial; 0.01: varianza del ruido de medición

// Serial output refresh time
const long SERIAL_REFRESH_TIME = 100;
long refresh_time;

const int pin34 = 4;

void setup() {
  Serial.begin(9600);
}

void loop() {

  // read a reference value from A0 and map it from 0 to 100
  //float real_value = analogRead(pin34)/4096.0 * 100.0; // 4096 corresponde a los bits del esp32
  float real_value = map(analogRead(pin34), 0, 4096, 0, 1000);
  // add a noise to the reference value and use as the measured value
  float measured_value = real_value + random(-100, 100) / 100.0;

  // calculate the estimated value with Kalman Filter
  float estimated_value = simpleKalmanFilter.updateEstimate(measured_value);

  // send to Serial output every 100ms
  // use the Serial Ploter for a good visualization
  if (millis() > refresh_time) {
    Serial.print(real_value, 2);
    Serial.print(",");
    Serial.print(measured_value, 2);
    Serial.print(",");
    Serial.print(estimated_value, 2);
    Serial.println();

    refresh_time = millis() + SERIAL_REFRESH_TIME;
  }
}