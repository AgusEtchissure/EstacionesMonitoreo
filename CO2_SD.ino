#include <SoftwareSerial.h>
#include <SPI.h>
#include <SD.h>

SoftwareSerial mySerial(10, 11);                                                      // RX, TX
unsigned char hexdata[9] = { 0xFF, 0x01, 0x86, 0x00, 0x00, 0x00, 0x00, 0x00, 0x79 };  //Read the gas density command /Don't change the order
void setup() {

  Serial.begin(9600);
  while (!Serial) {
  }
  mySerial.begin(9600);
  Serial.print("Inicializando tarjeta SD ...");

  // Verifica que la tarjeta se inicialice
  if (!SD.begin()) {
    Serial.println("Fallo en tarjeta, o no hay tarjeta");
    return;  // No hace nada más:
  }
  Serial.println("Tarjeta lista");
  SD.remove("datalog.txt");  //Borra el archivo previo
}

void loop() {
  mySerial.write(hexdata, 9);
  // delay(1000);


  for (int i = 0, j = 0; i < 9; i++) {
    if (mySerial.available() > 0) {
      long hi, lo, CO2;
      int ch = mySerial.read();

      if (i == 2) { hi = ch; }  //High concentration
      if (i == 3) { lo = ch; }  //Low concentration
      if (i == 8) {
        CO2 = hi * 256 + lo;  //CO2 concentration
        return CO2;
      }
      // mySerial.write(hexdata,9);
      File dataFile = SD.open("datalog.txt", FILE_WRITE);
      if (dataFile) {
        dataFile.print("CO2 concentration: ");
        dataFile.print(CO2);
        dataFile.println("ppm");
        dataFile.close();
        Serial.print("CO2 concentration: ");
        Serial.print(CO2);
        Serial.println("ppm");
      }
      // Si el archivo no se encuentra abierto, manda un error:
      else {
        Serial.println("Error al abrir el archivo datalog.txt");
      }

      // delay(1000);
    }
  }
delay(1000);  
}