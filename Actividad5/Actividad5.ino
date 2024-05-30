#include <LiquidCrystal.h>

// Definir pines
const int rs = 2, en = 3, d4 = 4, d5 = 5, d6 = 6, d7 = 7;
const int boton1 = 11, boton2 = 12;
const int leds[] = {8, 9, 10};

LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

String mensajes1[] = {"INICIALIZAR", "CALIBRAR", "MEDIR"};
String mensajes2[] = {"INICIALIZANDO...", "CALIBRANDO...", "MIDIENDO..."};

int mensajeActual = 0, opcionSeleccionada = -1;
unsigned long tiempoEncendidoLED = 0;
bool muestraMensajes1 = true;

void setup() {
  lcd.begin(16, 2);
  pinMode(boton1, INPUT);
  pinMode(boton2, INPUT);
  configurarPinesLED();
  lcd.clear();
  lcd.print(mensajes1[mensajeActual]);
}

void loop() {
  manejarBotones();
  actualizarPantalla();
  apagarLEDDespuesDeTiempo();
}

void manejarBotones() {
  int estadoBoton1 = digitalRead(boton1);
  int estadoBoton2 = digitalRead(boton2);

  if (estadoBoton1 == HIGH) {
    mensajeActual = (mensajeActual + 1) % (sizeof(mensajes1) / sizeof(String));
  } else if (estadoBoton2 == HIGH) {
    opcionSeleccionada = mensajeActual;
    encenderLED(opcionSeleccionada);
    tiempoEncendidoLED = millis();
    muestraMensajes1 = false;
  }
}

void actualizarPantalla() {
  lcd.clear();
  if (muestraMensajes1) {
    lcd.print(mensajes1[mensajeActual]);
  } else {
    lcd.print(mensajes2[opcionSeleccionada]);
  }
  delay(200);
}

void apagarLEDDespuesDeTiempo() {
  if (millis() - tiempoEncendidoLED >= 5000 && opcionSeleccionada != -1) {
    digitalWrite(leds[opcionSeleccionada], LOW);
    opcionSeleccionada = -1;
    muestraMensajes1 = true;
  }
}

void encenderLED(int opcion) {
  apagarTodosLEDs();
  if (opcion >= 0 && opcion < sizeof(leds) / sizeof(int)) {
    digitalWrite(leds[opcion], HIGH);
  }
}

void apagarTodosLEDs() {
  for (int i = 0; i < sizeof(leds) / sizeof(int); i++) {
    digitalWrite(leds[i], LOW);
  }
}

void configurarPinesLED() {
  for (int i = 0; i < sizeof(leds) / sizeof(int); i++) {
    pinMode(leds[i], OUTPUT);
  }
}