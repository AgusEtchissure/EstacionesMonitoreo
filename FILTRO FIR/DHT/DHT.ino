// #include <DHT.h>

// #define DHTPIN 34       // Pin del sensor DHT11 conectado al Arduino
// #define DHTTYPE DHT11  // Tipo de sensor DHT

// DHT dht(DHTPIN, DHTTYPE);

#define orden 17

const float coeficientes_b[orden] = {
-0.000000000000000,
-0.003682338570154,
-0.011342896221525,
0.016313834993615,
0.000000000000000, 
0.053490705483130,
0.137120349823866,
0.216264921426391,
0.248926186103814,
0.216264921426391,
0.137120349823866,
0.053490705483130,
0.000000000000000,
-0.016313834993615,
-0.011342896221525,
-0.003682338570154,
-0.000000000000000
};

float memoria[orden];

unsigned int y_n;



void setup(){
  Serial.begin(9600);
  pinMode(13, OUTPUT);
  DDRD = 0b11111111;

  ADCSRA = 0;
  ADCSRB = 0;
  ADMUX |= (0 & 34);
  ADMUX |= (1 << REFSO);
  ADMUX |= (1 << ADLAR);

  ADCSRA | = (1 << ADPS1);

  ADCSRA | = (1 << ADATE);
  ADCSRA | = (1 << ADEN);
  ADCSRA | = (1 << ADSC);
}

void loop() {
  for (n = 1; n <= orden; n++){
    memoria[n] = 0;
  }

  while(1){
    digitalWrite(13,HIGH);
    for(int k = orden; k!=1; k--){
      memoria[k] = memoria[k-1];
    }

    memoria[1] = ADCH;
    y_n = 0;
    for(int n = 1; n <= orden; n++){
      y_n += coeficientes_b[n]*memoria[n];}

      PORTD - y_n&255;
      digitalWrite(13, LOW);
      delayMicroseconds(326);
    }
  }
