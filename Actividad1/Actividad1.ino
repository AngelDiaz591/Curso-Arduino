int estadoBoton=0; //Guardará el estado del botón (HIGH 6 LOW)

void setup()
{
	pinMode(4,OUTPUT); //Donde está conectado el LED
	pinMode(8,INPUT); //Donde está conectado el pulsador
}
void loop()
{
	estadoBoton=digitalRead(8);
	//Si se detecta que el botón está pulsado, se enciende el LED
	if (estadoBoton == HIGH){
		digitalWrite (4, HIGH);
	//Si no, no
	}else {
		digitalWrite (4,LOW);
    }
}


