// Definir el pin del LED
const int ledPin = 2; // Cambiar al pin donde esté conectado el LED

// Definir el pin de entrada analógica
const int analogInPin = A0;

int sensorValue = 0; // Valor leído del potenciómetro

float voltageValue = 0; // Valor de voltaje calculado

unsigned long lastTime = 0, sampleTime = 100;

// Parámetros del filtro de media móvil
const int numReadings = 10; // Número de lecturas para el filtro
int readings[numReadings]; // Arreglo para almacenar las lecturas
int index = 0; // Índice actual en el arreglo
int total = 0; // Suma total de las lecturas

void setup() {
Serial.begin(9600);
pinMode(ledPin, OUTPUT); // Configurar el pin del LED como salida
}

void loop() {
  // Si ha pasado suficiente tiempo desde la última muestra
  if (millis() - lastTime > sampleTime) {
    lastTime = millis();
    // Leer el valor analógico
    int rawValue = analogRead(analogInPin);
    // Aplicar el filtro de media móvil
    total = total - readings[index];
    readings[index] = rawValue;
    total = total + readings[index];
    index = (index + 1) % numReadings;
    int smoothedValue = total / numReadings;
    // Calcular el voltaje interpolando linealmente entre las lecturas
    voltageValue = mapFloat(smoothedValue, 0, 1023, 0, 5000) / 1000.0; // Convertira voltaje en volts
    // Enviar el valor a través del puerto serie
    Serial.println(voltageValue, 3); // Imprimir con 3 decimales de precisión

    // Si hay datos disponibles para leer del puerto serie
    if (Serial.available() > 0) {
      char command = Serial.read(); // Leer el comando
      // Si el comando es '1', encender el LED
      if (command == '1') {
        digitalWrite(ledPin, HIGH);
        Serial.println("LED encendido");
      }
      // Si el comando es '0', apagar el LED
      else if (command == '0') {
        digitalWrite(ledPin, LOW);
        Serial.println("LED apagado");
      }
    }
  }
}

// Función para escalar un valor de entrada a un rango específico de salida (float)
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}