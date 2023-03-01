#include "GravityRtc.h"               // Librería para RTC
#include "Wire.h"                     // Librería para RTC
#include <EEPROM.h>                   // Librería necesaria para guardar datos en la EEPROM
#include <LiquidCrystal.h>            // Librería para LCD
LiquidCrystal lcd(4, 5, 6, 7, 8, 9);  // Definición de pines de lectura del LCD en función del nombramiento del sensor a "lcd"

GravityRtc rtc;  // Se define el nombre del sensor RTC a "rtc"

int seg = 0;    // Variable que guarda los segundos
int min = 0;    // Variable que guarda los minutos
int hor = 0;    // Variable que guarda las horas
int dia = 1;    // Variable que guarda los días
int mes = 1;    // Variable que guarda los meses
int anyo = 23;  // Variable que guarda los años

bool aum_hor = 0;  // Variable que condiciona el aumento de las horas
bool aum_min = 0;  // Variable que condiciona el aumento de los minutos

bool aum_dia = 0;   // Variable que condiciona el aumento de los días
bool aum_mes = 0;   // Variable que condiciona el aumento de los meses
bool aum_anyo = 0;  // Variable que condiciona el aumento de los años

bool cambio_fecha = 0;  // Variable que permite editar la fecha en función de su estado
bool cambio_hora = 0;   // Variable que permite editar la hora en función de su estado
bool cambio = false;    // Variable que permite editar la fecha y hora en funcion de su estado

int dir_dia = 0;   // Dirección en la eeprom para guardar la variable día
int dir_mes = 2;   // Dirección en la eeprom para guardar la variable mes
int dir_anyo = 4;  // Dirección en la eeprom para guardar la variable año
int dir_hora = 8;  // Dirección en la eeprom para guardar la variable hora
int dir_min = 10;  // Dirección en la eeprom para guardar la variable minuto

int aceptar1 = 0;  // Variable que permite guardar la fecha y hora en el RTC

///////////////////////////////////////////////////
// Función que deshabilita la edición de la hora //
///////////////////////////////////////////////////

void cambiar_fecha() {
  cambio = false;
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Presione el");
  lcd.setCursor(0, 1);
  lcd.print("boton naranja");
}

///////////////////////////////////////////////////

void setup() {

  Serial.begin(9600);  // Inicializando el puerto serial

  rtc.setup(); // Se inicializa el RTC

  lcd.begin(16, 2);  // Estableciendo el tamaño del LCD

  lcd.setCursor(0, 0);         // Definiendo la ubicación en el LCD para redactar la siguiente frase
  lcd.print("Bienvenido al");  // Impresión en el LCD de la frase
  lcd.setCursor(0, 1);         // Definiendo la ubicación en el LCD para redactar la siguiente frase
  lcd.print("reloj digital");  // Impresión en el LCD de la frase

  attachInterrupt(digitalPinToInterrupt(2), cambiar_fecha, LOW);  // Estableciendo una interrupción externa con la función "cambiar_fecha" al inicializar el pin digital 2 cuando pase a estado "LOW"


  EEPROM.get(dir_hora, hor);   // Leer la variable hora en la dirección preestablecida
  EEPROM.get(dir_min, min);    // Leer la variable minuto en la dirección preestablecida
  EEPROM.get(dir_dia, dia);    // Leer la variable dia en la dirección preestablecida
  EEPROM.get(dir_mes, mes);    // Leer la variable mes en la dirección preestablecida
  EEPROM.get(dir_anyo, anyo);  // Leer la variable año en la dirección preestablecida

  delay(5000);  // Espera 5 segundos luego de imprimir el mensaje
  lcd.clear();  // Limpia el LCD
}

void loop() {

  rtc.read();  // Permite leer el RTC

  cambio_hora = digitalRead(10);  // Se define la variable adjudicada al pin digital 10
  if (cambio_hora == 1) {         // Si el pin digital 10 no es activado por el pulsador ejecuta las siguientes líneas
    lcd.setCursor(0, 0);          // Definiendo la ubicación en el LCD para redactar la siguiente frase
    lcd.print("Hora: ");          // Impresión en el LCD de la frase
    Serial.print("  Hora: ");     // Impresión en el monitor serial de la frase
    if (hor < 10) {               // Si la variable hora tiene un valor menor a 10
      lcd.print('0');             // Se imprime un cero por delante del valor en el LCD
      Serial.print('0');          // Se imprime un cero por delante del valor en el monitor serial
    }
    lcd.print(hor);     // Se imprime el valor de la variable hora en el LCD
    lcd.print(':');     // Se imprimen : en el LCD para facilitar una visualización "tipo reloj"
    Serial.print(hor);  // Se imprime el valor de la variable hora en el monitor serial
    Serial.print(":");  // Se imprimen : en el monitor serial para facilitar una visualización "tipo reloj"

    if (min < 10) {       // Si la variable minutos tiene un valor menor a 10
      lcd.print('0');     // Se imprime un cero por delante del valor en el LCD
      Serial.print('0');  // Se imprime un cero por delante del valor en el monitor serial
    }
    lcd.print(min);     // Se imprime el valor de la variable minuto en el LCD
    lcd.print(':');     // Se imprimen : en el LCD para facilitar una visualización "tipo reloj"
    Serial.print(min);  // Se imprime el valor de la variable minuto en el monitor serial
    Serial.print(":");  // Se imprimen : en el monitor serial para facilitar una visualización "tipo reloj"

    if (seg < 10) {       // Si la variable minutos tiene un valor menor a 10
      lcd.print('0');     // Se imprime un cero por delante del valor en el LCD
      Serial.print('0');  // Se imprime un cero por delante del valor en el monitor serial
    }
    lcd.print(seg);     // Se imprime el valor de la variable segundos en el LCD
    Serial.print(seg);  // Se imprime el valor de la variable segundos en el monitor serial
    delay(500);         // Se espera 500 milisegundos (el lcd presenta un retardo de 500 milisegundos, por ello se selecciona esta espera)
    seg++;              // Se suma uno a uno los segundos luego del transcurso de cada segundo

    if (seg > 59) { seg = 0, min++; }  // Si el valor de la variable segundos supera el valor de 59, la variable toma el valor cero y suma un minuto
    if (min > 59) { min = 0, hor++; }  // Si el valor de la variable minutos supera el valor de 59, la variable toma el valor cero y suma una hora
    if (hor > 23) { hor = 0; }         // // Si el valor de la variable hora supera el valor de 23, la variable toma el valor cero

    lcd.setCursor(0, 1);        // Definiendo la ubicación en el LCD para redactar la siguiente frase
    lcd.print("Fecha:");        // Impresión en el LCD de la frase
    Serial.print("  Fecha: ");  // Impresión en el monitor serial de la frase
    if (dia < 10) {             // Si la variable día tiene un valor menor a 10
      lcd.print('0');           // Se imprime un cero por delante del valor en el LCD
      Serial.print('0');        // Se imprime un cero por delante del valor en el monitor serial
    }
    lcd.print(dia);       // Se imprime el valor de la variable día en el LCD
    lcd.print('/');       // Se imprime / en el LCD para facilitar una visualización "tipo fecha"
    Serial.print(dia);    // Se imprime el valor de la variable día en el monitor serial
    Serial.print("/");    // Se imprime / en el monitor serial para facilitar una visualización "tipo fecha"
    if (mes < 10) {       // Si la variable mes tiene un valor menor a 10
      lcd.print('0');     // Se imprime un cero por delante del valor en el LCD
      Serial.print('0');  // Se imprime un cero por delante del valor en el monitor serial
    }
    lcd.print(mes);        // Se imprime el valor de la variable mes en el LCD
    lcd.print('/');        // Se imprime / en el LCD para facilitar una visualización "tipo fecha"
    lcd.print(anyo);       // Se imprime el valor de la variable año en el LCD
    Serial.print(mes);     // Se imprime el valor de la variable año en el monitor serial
    Serial.print('/');     // Se imprime / en el monitor serial para facilitar una visualización "tipo fecha"
    Serial.println(anyo);  // Se imprime el valor de la variable año en el monitor serial

    if (dia > 30) {dia = 1; mes++;}  // Si el valor de la variable día supera el valor de 30, la variable toma el valor uno y suma un mes
    if (mes > 11) {mes = 1; anyo++;} // Si el valor de la variable mes supera el valor de 11, la variable toma el valor uno y suma un año
    if (anyo > 25) { anyo = 23;}     // Si el valor de la variable año supera el valor de 25, la variable toma el valor 23 ya que se define como límite de años
  }


  ////////////////////////////////////////////////////////
  ///////////////// EDICIÓN DE LA HORA //////////////////
  ///////////////////////////////////////////////////////

  cambio_hora = digitalRead(10);               // Se define la variable adjudicada al pin digital 10
  if (cambio_hora == 0) {                      // Si el pin digital 10 es activado por el pulsador ejecuta las siguientes líneas
    cambio = true;                             // La variable cambio pasa a estar en estado "true"
    delay(2000);                               // Se espera dos segundos luego de ser activado el pin digital 10 mediante el pulsador
    cambio_hora = digitalRead(10);             // Se actualiza el estado del pin digital 10 activado mediante el pulsador
    if (cambio_hora == 0 && cambio == true) {  // Si el pin digital 10 sigue activado luego de dos segundos y la variable cambio se encuentra en estado "true", se habilita la edición de la hora
      lcd.clear();                             // Se limpia el LCD
      lcd.setCursor(0, 0);                     // Definiendo la ubicación en el LCD para redactar la siguiente frase
      lcd.print("Cambie la hora");             // Impresión en el LCD de la frase
      Serial.print("Cambie la hora");          // Impresión en el monitor serial de la frase

      while (cambio == true) {          // Mientras que la variable cambio continúe en estado "true"
        aum_hor = digitalRead(12);      // Se define la variable adjudicada al pin digital 12
        if (aum_hor == 0) {             // Si el pin digital 12 es activado por el pulsador ejecuta las siguientes líneas
          delay(150);                   // Se espera 150 milisegundos luego de ser activado el pin digital 12 mediante el pulsador
          aum_hor = digitalRead(12);    // Se actualiza el estado del pin digital 12 activado mediante el pulsador
          if (aum_hor == 0) {           // Si el pin digital 12 sigue activado luego de 150 milisegundos
            hor++;                      // Se habilita a sumar en la variable hora
            EEPROM.put(dir_hora, hor);  // Se guarda la variable en su dirección preestablecida en la EEPROM
          }
        }

        aum_min = digitalRead(11);     // Se define la variable adjudicada al pin digital 11
        if (aum_min == 0) {            // Si el pin digital 11 es activado por el pulsador ejecuta las siguientes líneas
          delay(150);                  // Se espera 150 milisegundos luego de ser activado el pin digital 11 mediante el pulsador
          aum_min = digitalRead(11);   // Se actualiza el estado del pin digital 11 activado mediante el pulsador
          if (aum_min == 0) {          // Si el pin digital 11 sigue activado luego de 150 milisegundos
            min++;                     // Se habilita a sumar en la variable minuto
            EEPROM.put(dir_min, min);  // Se guarda la variable en su dirección preestablecida en la EEPROM
          }
        }

        lcd.setCursor(1, 1);  // Definiendo la ubicación en el LCD para redactar la siguiente frase
        if (hor < 10) {       // Si la variable hora tiene un valor menor a 10
          lcd.print('0');     // Se imprime un cero por delante del valor en el LCD
          Serial.print('0');  // Se imprime un cero por delante del valor en el monitor serial
        }
        lcd.print(hor);     // Se imprime el valor de la variable hora en el LCD
        lcd.print(':');     // Se imprimen : en el LCD para facilitar una visualización "tipo reloj"
        Serial.print(hor);  // Se imprime el valor de la variable hora en el monitor serial
        Serial.print(':');  // Se imprimen : en el monitor serial para facilitar una visualización "tipo reloj"

        if (min < 10) {       // Si la variable minuto tiene un valor menor a 10
          lcd.print('0');     // Se imprime un cero por delante del valor en el LCD
          Serial.print('0');  // Se imprime un cero por delante del valor en el monitor serial
        }
        lcd.print(min);     // Se imprime el valor de la variable minuto en el LCD
        lcd.print(':');     // Se imprimen : en el LCD para facilitar una visualización "tipo reloj"
        Serial.print(min);  // Se imprime el valor de la variable minuto en el monitor serial
        Serial.print(':');  // Se imprimen : en el monitor serial para facilitar una visualización "tipo reloj"

        if (seg < 10) {       // Si la variable segundos tiene un valor menor a 10
          lcd.print('0');     // Se imprime un cero por delante del valor en el LCD
          Serial.print('0');  // Se imprime un cero por delante del valor en el monitor serial
        }
        lcd.print(seg);        // Se imprime el valor de la variable segundos en el LCD
        Serial.print(seg);     // Se imprime el valor de la variable segundos en el monitor serial
        Serial.println("  ");  // Se imprime un espacio luego del valor de la variable segundos en el monitor serial para facilitar la visualización
        delay(1000);           // Se espera un segundo
        seg++;                 // Luego de esperar cada segundo, se va sumando en la variable

        if (seg > 59) { seg = 0, min++; }  // Si el valor de la variable segundos supera el valor de 59, la variable toma el valor cero y suma un minuto
        if (min > 59) { min = 0, hor++; }  // Si el valor de la variable minutos supera el valor de 59, la variable toma el valor cero y suma una hora
        if (hor > 23) { hor = 0; }         // // Si el valor de la variable hora supera el valor de 23, la variable toma el valor cero
      }
    }
  }

  /////////////////////////////////////////////////////////
  ///////////////// EDICIÓN DE LA FECHA ///////////////////
  ////////////////////////////////////////////////////////

  cambio_fecha = digitalRead(3);                // Se define la variable adjudicada al pin digital 3
  if (cambio_fecha == 0) {                      // Si el pin digital 3 es activado por el pulsador ejecuta las siguientes líneas
    cambio = true;                              // La variable cambio pasa a estar en estado "true"
    delay(3000);                                // Se espera tres segundos luego de ser activado el pin digital 3 mediante el pulsador
    lcd.clear();                                // Se limpia el LCD
    cambio_fecha = digitalRead(3);              // Se actualiza el estado del pin digital 3 activado mediante el pulsador
    if (cambio_fecha == 0 && cambio == true) {  // Si el pin digital 3 sigue activado luego de tres segundos y la variable cambio se encuentra en estado "true", se habilita la edición de la fecha

      while (cambio == true) {         // Mientras que la variable cambio continúe en estado "true"
        lcd.setCursor(0, 0);           // Definiendo la ubicación en el LCD para redactar la siguiente frase
        lcd.print("Cambie la Fecha");  // Impresión en el LCD de la frase
        aum_dia = digitalRead(12);     // Se define la variable adjudicada al pin digital 12
        if (aum_dia == 0) {            // Si el pin digital 12 es activado por el pulsador ejecuta las siguientes líneas
          delay(400);                  // Se espera 400 milisegundos luego de ser activado el pin digital 12 mediante el pulsador
          aum_dia = digitalRead(12);   // Se actualiza el estado del pin digital 12 activado mediante el pulsador
          if (aum_dia == 0) {          // Si el pin digital 12 sigue activado luego de 400 milisegundos
            dia++;                     // Se habilita a sumar en la variable día
            EEPROM.put(dir_dia, dia);  // Se guarda la variable en su dirección preestablecida en la EEPROM
          }
        }

        aum_mes = digitalRead(11);    // Se define la variable adjudicada al pin digital 11
        if (aum_mes == 0) {           // Si el pin digital 11 es activado por el pulsador ejecuta las siguientes líneas
          delay(400);                 // Se espera 400 milisegundos luego de ser activado el pin digital 11 mediante el pulsador
          aum_mes = digitalRead(11);  // Se actualiza el estado del pin digital 11 activado mediante el pulsador
          if (aum_mes == 0)           // Si el pin digital 11 sigue activado luego de 400 milisegundos
            mes++;                    // Se habilita a sumar en la variable mes
          EEPROM.put(dir_mes, mes);   // Se guarda la variable en su dirección preestablecida en la EEPROM
        }


        aum_anyo = digitalRead(10);      // Se define la variable adjudicada al pin digital 10
        if (aum_anyo == 0) {             // Si el pin digital 10 es activado por el pulsador ejecuta las siguientes líneas
          delay(400);                    // Se espera 400 milisegundos luego de ser activado el pin digital 11 mediante el pulsador
          aum_anyo = digitalRead(10);    // Se actualiza el estado del pin digital 10 activado mediante el pulsador
          if (aum_anyo == 0) {           // Si el pin digital 10 sigue activado luego de 400 milisegundos
            anyo++;                      // Se habilita a sumar en la variable año
            EEPROM.put(dir_anyo, anyo);  // Se guarda la variable en su dirección preestablecida en la EEPROM
          }
        }

        lcd.setCursor(1, 1);  // Definiendo la ubicación en el LCD para redactar la siguiente frase
        if (dia < 10) {       // Si la variable día tiene un valor menor a 10
          lcd.print('0');     // Se imprime un cero por delante del valor en el LCD
          Serial.print('0');  // Se imprime un cero por delante del valor en el monitor serial
        }
        lcd.print(dia);       // Se imprime el valor de la variable día en el LCD
        lcd.print('/');       // Se imprime / en el LCD para facilitar una visualización "tipo fecha"
        Serial.print(dia);    // Se imprime el valor de la variable día en el monitor serial
        Serial.print('/');    // Se imprime / en el monitor serial para facilitar una visualización "tipo fecha"
        if (mes < 10) {       // Si la variable mes tiene un valor menor a 10
          lcd.print('0');     // Se imprime un cero por delante del valor en el LCD
          Serial.print('0');  // Se imprime un cero por delante del valor en el monitor serial
        }
        lcd.print(mes);        // Se imprime el valor de la variable mes en el LCD
        lcd.print('/');        // Se imprime / en el LCD para facilitar una visualización "tipo fecha"
        lcd.print(anyo);       // Se imprime el valor de la variable año en el LCD
        Serial.print(mes);     // Se imprime el valor de la variable mes en el monitor serial
        Serial.print('/');     // Se imprime / en el monitor serial para facilitar una visualización "tipo fecha"
        Serial.print(anyo);    // Se imprime el valor de la variable año en el monitor serial
        Serial.println("  ");  // Se imprime un espacio luego del valor de la variable segundos en el monitor serial para facilitar la visualización

        if (dia > 30) {dia = 1; mes++;}  // Si el valor de la variable día supera el valor de 30, la variable toma el valor uno y suma un mes
        if (mes > 11) {mes = 1; anyo++;} // Si el valor de la variable mes supera el valor de 11, la variable toma el valor uno y suma un año
        if (anyo > 25) { anyo = 23;}     // Si el valor de la variable año supera el valor de 25, la variable toma el valor 23 ya que se define como límite de años
      }
    }
  }

  aceptar1 = digitalRead(12);                           // Se define la variable adjudicada al pin digital 12
  if (aceptar1 == 0) {                                  // Si el pin digital 12 es activado por el pulsador ejecuta las siguientes líneas
    delay(3000);                                        // Se espera tres segundos luego de ser activado el pin digital 3 mediante el pulsador
    aceptar1 = digitalRead(12);                         // Se actualiza el estado del pin digital 12 activado mediante el pulsador
    if (aceptar1 == 0) {                                // Si el pin digital 12 sigue activado luego de tres segundos
      rtc.adjustRtc(anyo, mes, dia, 3, hor, min, seg);  // Se guardan las variables anteriormente editadas en el RTC para su posterior conteo en tiempo real
      lcd.clear();                                      // Se limpia el LCD
      lcd.setCursor(0, 0);                              // Definiendo la ubicación en el LCD para redactar la siguiente frase
      lcd.print("Fecha y Hora");                        // Impresión en el LCD de la frase
      lcd.setCursor(0, 1);                              // Definiendo la ubicación en el LCD para redactar la siguiente frase
      lcd.print("Establecidas");                        // Impresión en el LCD de la frase
      Serial.println("Fecha y Hora Establecidas");      // Impresión en el monitor serial de la frase
      delay(2000);                                      // Se esperan dos segundos para facilitar la lectura del mensaje
    }
  }

  delay(500);  // Se esperan 500 milisegundos para que se imprima el estado inicial del reloj con fecha y hora
}
