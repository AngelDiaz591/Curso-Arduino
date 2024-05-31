#include <LiquidCrystal.h>
#include <DHT.h>

// Definir pines
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
const int boton = 8; // Usar solo un botón
const int ledPin = 10; // Pin para el LED
const int sensorPin = A0;
#define DHTTYPE DHT11 // Tipo de sensor DHT11

DHT dht(sensorPin, DHTTYPE);
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String mensajes1[] = {"TEMPERATURA", "HUMEDAD"};
int mensajeActual = 0, opcionSeleccionada = 0; // Iniciar mostrando la temperatura
bool mostrarTemperatura = true; // Variable para indicar si se muestra temperatura o humedad
int valorTemperatura = 200; // Valor máximo de temperatura

void setup() {
    Serial.begin(9600);
    lcd.begin(16, 2);
    dht.begin();
    pinMode(boton, INPUT);
    pinMode(ledPin, OUTPUT); // Configurar pin del LED como salida
    lcd.clear();
    actualizarPantalla(); // Mostrar temperatura inicial
}

void loop() {
    manejarBotones();
    actualizarPantalla();
}

void manejarBotones() {
    int estadoBoton = digitalRead(boton);

    if (estadoBoton == HIGH) {
        mensajeActual = (mensajeActual + 1) % (sizeof(mensajes1) / sizeof(String));
        mostrarTemperatura = !mostrarTemperatura; // Alternar entre temperatura y humedad
        opcionSeleccionada = mensajeActual;
    }
}

void actualizarPantalla() {
    lcd.clear();
    lcd.print(mensajes1[mensajeActual]);

    if (mostrarTemperatura) {
        float temperatura = dht.readTemperature();
        int temperaturaInt = temperatura;
        int intensidad = map(temperaturaInt, 0, valorTemperatura, 0, 255); // Mapear temperatura a rango de 0-255
        analogWrite(ledPin, intensidad); // Ajustar intensidad del LED

        lcd.setCursor(0, 1);
        lcd.print("Temp: ");
        lcd.print(temperaturaInt);
        lcd.print(" C");

        Serial.print("T,"); // Enviar indicador de temperatura
        Serial.print(temperaturaInt);

        if (temperaturaInt > valorTemperatura) {
            lcd.print(" Limite Superado");
            Serial.print(",1"); // Enviar indicador de límite superado
        } else {
            Serial.print(",0"); // Enviar indicador de límite no superado
        }

        Serial.println(); // Enviar nueva línea
    } else {
        analogWrite(ledPin, 0); // Apagar el LED para la humedad
        float humedad = dht.readHumidity();
        int humedadInt = humedad;
        lcd.setCursor(0, 1);
        lcd.print("Humedad: ");
        lcd.print(humedadInt);
        lcd.print("%");

        Serial.print("H,"); // Enviar indicador de humedad
        Serial.println(humedadInt); // Enviar valor de humedad
    }

    delay(200);
}