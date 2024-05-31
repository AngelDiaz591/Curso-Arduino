#include <DHT.h>

#define DHTPIN A0
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

//        Btn = Pin
const int boton1 = A1;
const int boton2 = A2;
const int boton3 = A3;
const int boton4 = A4;

bool estado1 = true;
bool estado2 = true;
bool estado3 = true;
bool estado4 = true;

int ledRojo = A5;
int ledVerde = 10;

char rightCode[] = "4444";
char code[5] = "";
int cont = 0;

const byte pinesSegmentos[7] = {3, 7, 11, 9, 8, 4, 12};

//                            1   2   3   4
const byte pinesDigitos[4] = {2, 5, 6, 13};

const byte seg[15][7] = {
  {1, 1, 1, 1, 1, 1, 0}, // 0
  {0, 1, 1, 0, 0, 0, 0}, // 1
  {1, 1, 0, 1, 1, 0, 1}, // 2
  {1, 1, 1, 1, 0, 0, 1}, // 3
  {0, 1, 1, 0, 0, 1, 1}, // 4
  {1, 0, 1, 1, 0, 1, 1}, // 5
  {1, 0, 1, 1, 1, 1, 1}, // 6
  {1, 1, 1, 0, 0, 0, 0}, // 7
  {1, 1, 1, 1, 1, 1, 1}, // 8
  {1, 1, 1, 0, 0, 1, 1}, // 9
  {1, 1, 1, 0, 1, 1, 1}, // A  10
  {1, 1, 1, 1, 1, 1, 1}, // B  11
  {1, 1, 0, 0, 0, 1, 1}, // °  12
  {1, 0, 0, 1, 1, 1, 0}, // C  13
  {0, 0, 0, 0, 0, 0, 1} // -  14
};

void setup(){
  for (int i = 0; i < sizeof(pinesSegmentos) / sizeof(pinesSegmentos[0]); i++){
    pinMode(pinesSegmentos[i], OUTPUT);
  }

  for (int i = 0; i < sizeof(pinesDigitos) / sizeof(pinesDigitos[0]); i++){
    pinMode(pinesDigitos[i], OUTPUT);
  }

  pinMode(boton1, INPUT);
  pinMode(boton2, INPUT);
  pinMode(boton3, INPUT);
  pinMode(boton4, INPUT);
  
  pinMode(ledRojo, OUTPUT);
  pinMode(ledVerde, OUTPUT);

  dht.begin();
}

void loop(){

  dht.read();

  int temperatura = dht.readTemperature();

  int B1_state = digitalRead(boton1);
  int B2_state = digitalRead(boton2);
  int B3_state = digitalRead(boton3);
  int B4_state = digitalRead(boton4);
  
  
  if (B1_state == HIGH) {
    Temperatura(temperatura);
    if (B1_state == HIGH) {
      code[cont] = '1';
      cont++;
    }
  }
  if (B2_state == HIGH) {
    Temperatura(temperatura);
    if (B2_state == HIGH) {
      code[cont] = '2';
      cont++;
    }
  }
  if (B3_state == HIGH) {
    Temperatura(temperatura);
    if (B3_state == HIGH) {
      code[cont] = '3';
      cont++;
    }
  }
  if (B4_state == HIGH) {
    Temperatura(temperatura);
    if (B4_state == HIGH) {
      code[cont] = '4';
      cont++;
    }
  }

  if (cont == 4) {
    for (int i = 0; i < 4; i++)
    {
      inicio(temperatura);
    }
    if (strcmp(code, rightCode) == 0 && temperatura > 30) {
      Serial.println(temperatura);
      digitalWrite(ledVerde, HIGH);
      Temp(temperatura);
    } else {
      Serial.println(temperatura);
      digitalWrite(ledRojo, HIGH);
      Temp(temperatura);
    }
    cont = 0;
    memset(code, 0, sizeof(code));
    inicio(temperatura);
  }

  inicio(temperatura);
  digitalWrite(ledVerde, LOW);
  digitalWrite(ledRojo, LOW);
 
}

void inicio(int temperatura)
{
  byte digito1 = (temperatura / 10) % 10;
  byte digito2 = temperatura % 10;
  EscribeDigito(1, digito1);
  delay(5);
  EscribeDigito(2, digito2);
  delay(5);
  EscribeDigito(3, 12);
  delay(5);
  EscribeDigito(4, 13);
  delay(5);
}

void Temperatura(int temperatura)
{
  byte digito1 = (temperatura / 10) % 10;
  byte digito2 = temperatura % 10;
  
  unsigned long startTime = millis();
  while (millis() - startTime < 500)
  {
    EscribeDigito(1, digito1);
    delay(5);
    EscribeDigito(2, digito2);
    delay(5);
    EscribeDigito(3, 12);  // °
    delay(5);
    EscribeDigito(4, 13);  // C
    delay(5);
  }
}

void Temp(int temperatura)
{
  byte digito1 = (temperatura / 10) % 10;
  byte digito2 = temperatura % 10;
  
  unsigned long startTime = millis();
  while (millis() - startTime < 3000)
  {
    EscribeDigito(1, digito1);
    delay(5);
    EscribeDigito(2, digito2);
    delay(5);
    EscribeDigito(3, 12);  // °
    delay(5);
    EscribeDigito(4, 13);  // C
    delay(5);
  }
}

void ActivarDigito(int x)
{
  for (int i = 0; i < 4; i++)
  {
    digitalWrite(pinesDigitos[i], (i + 1 == x) ? LOW : HIGH);
  }
}

void EscribeDigito(byte digito, byte numero)
{
  ActivarDigito(digito);
  for (byte i = 0; i < 7; i++)
  {
    digitalWrite(pinesSegmentos[i], seg[numero][i]);
  }
}