int potPin = A0; // Variable para almacenar el valor del potenciómetro
int ledPin = 2; // Pin para el LED
int rango = 1023;

void setup() {
  pinMode(ledPin, OUTPUT); // Configurar el pin del LED como salida
  Serial.begin(9600); // Iniciar la comunicación serial
}

void loop() {
  int potValue = analogRead(potPin); // Leer el valor del potenciómetro (pin A0)

  if (potValue == 0) {
    digitalWrite(ledPin, LOW); // Apagar el LED
    delayMicroseconds(rango); 
  } else if (potValue == 1023) {
    digitalWrite(ledPin, HIGH);
    delayMicroseconds(rango);
  } else {
    digitalWrite(ledPin, HIGH);
    delayMicroseconds(potValue);
    digitalWrite(ledPin, LOW);
    delayMicroseconds(rango - potValue);
  }

  Serial.print("Valor del potenciómetro: ");
  Serial.println(potValue);
}