#include <DHT.h>
const int botonPin = A0; // Pin del botón
const int sensorPin = A1; // Pin de datos del sensor DHT11
int estadoBoton = 0; // Variable para almacenar el estado del botón
int operacion = 1;
unsigned long tiempoComienzo = 0; // Variable para almacenar el tiempo de inicio de la pulsación
#define DHTTYPE DHT11 // Tipo de sensor DHT11
DHT dht(sensorPin, DHTTYPE);
int pinesSegmentos[] = {2, 3, 4, 5, 6, 7, 8};
int pinesDigitos[] = {10, 11, 12, 13};
byte siete_segmentos_Digitos_Numeros_Simbolos[14][7] = {
{1, 1, 1, 1, 1, 1, 0}, // = 0
{0, 1, 1, 0, 0, 0, 0}, // = 1
{1, 1, 0, 1, 1, 0, 1}, // = 2
{1, 1, 1, 1, 0, 0, 1}, // = 3
{0, 1, 1, 0, 0, 1, 1}, // = 4
{1, 0, 1, 1, 0, 1, 1}, // = 5
{1, 0, 1, 1, 1, 1, 1}, // = 6
{1, 1, 1, 0, 0, 0, 0}, // = 7
{1, 1, 1, 1, 1, 1, 1}, // = 8
{1, 1, 1, 0, 0, 1, 1}, // = 9
{1, 0, 0, 1, 1, 1, 0}, // Letra C
{0, 1, 1, 0, 1, 1, 1}, // Letra H
{0, 0, 0, 0, 1, 0, 1}, // Letra r
{1, 1, 0, 0, 0, 1, 1} // Simbolo °
};
void setup() {
  pinMode(botonPin, INPUT_PULLUP);
  Serial.begin(9600);
  dht.begin();
  for (int i = 2; i <= 13; i++) {
    pinMode(i, OUTPUT);
  }
}
void loop() {
  estadoBoton = digitalRead(botonPin); // Leer el estado del botón
  if (estadoBoton == LOW) { // Si el botón está presionado
    unsigned long tiempoPresionado = millis() - tiempoComienzo;
    if (tiempoPresionado >= 3000){ // Si el botón se presiona durante 3 segundos
      operacion = operacion == 1 ? 2 : 1; // Cambiar entre 1 y 2
      tiempoComienzo = millis(); // Restablecer el tiempo de inicio de la pulsación
    }

  } else { // Si el botón se suelta
    tiempoComienzo = millis(); // Restablecer el tiempo de inicio de la pulsación
  }
  if (operacion == 1){
    funcionTemperatura();
  }
  if (operacion == 2){
    funcionHumedad();
  }
}
void ActivarDigito(int x) {
  for (int i = 0; i < 4; i++) {
  digitalWrite(pinesDigitos[i], (i + 1 == x) ? LOW : HIGH);
  }
}
void EscribeDigito(byte digito, byte numero) {
  ActivarDigito(digito);
  for (byte i = 0; i < 7; i++) {
    digitalWrite(pinesSegmentos[i], siete_segmentos_Digitos_Numeros_Simbolos[numero][i]);
  }
}
void funcionTemperatura() {
  // Botón pulsado una vez
  float temperatura = dht.readTemperature();
  // Convertir temperatura a dos dígitos
  int temperaturaInt = temperatura;
  byte digito1 = (temperaturaInt / 10) % 10;
  byte digito2 = temperaturaInt % 10;
  // Mostrar temperatura en el display
  EscribeDigito(1, digito1); // Decenas
  delay(5);
  EscribeDigito(2, digito2); // Unidades
  delay(5);
  EscribeDigito(3, 13); // Simbolo °
  delay(5); // Pequeño retraso para permitir la actualización del display
  EscribeDigito(4, 10); // Letra C
  delay(5);
}
void funcionHumedad(){
  // Botón mantenido presionado
  float humedad = dht.readHumidity();
  // Convertir humedad a dos dígitos
  int humedadInt = int(humedad);
  byte digito1 = (humedadInt / 10) % 10;
  byte digito2 = humedadInt % 10;
  // Mostrar humedad en el display
  EscribeDigito(1, digito1); // Decenas
  delay(5); // Pequeño retraso para permitir la actualización del display
  EscribeDigito(2, digito2); // Unidades
  delay(5);
  EscribeDigito(3, 11); // Letra H
  delay(5); // Pequeño retraso para permitir la actualización del display
  EscribeDigito(4, 12); // Letra r
  delay(5);
}