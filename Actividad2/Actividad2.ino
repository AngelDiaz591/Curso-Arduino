int i=0;
int vel=1;
int mil,cien,dec,uni;
byte segmentos[7] = { 11, 7, 3, 5, 6, 10, 2 };
byte orden_digitos[4] = { 13, 8, 9, 12 };
byte siete_segmentos_digitos[10][7] = { 
  { 1,1,1,1,1,1,0 }, { 0,1,1,0,0,0,0 }, { 1,1,0,1,1,0,1 },
  { 1,1,1,1,0,0,1 }, { 0,1,1,0,0,1,1 }, { 1,0,1,1,0,1,1 },
  { 1,0,1,1,1,1,1 }, { 1,1,1,0,0,0,0 }, { 1,1,1,1,1,1,1 }, { 1,1,1,0,0,1,1 }
};
void EscribeDigito(byte digito, byte numero) {
  ActivarDigito(digito); 
  for (byte i = 0; i < 7; i++){
    digitalWrite(segmentos[i], siete_segmentos_digitos[numero][i]);
  }
}
void ActivarDigito(int x){
  for(i=0; i < 4; i++)
    digitalWrite(orden_digitos[i], HIGH);
  switch(x){
    case 1: 
      digitalWrite(orden_digitos[0], LOW); 
      break;
    case 2: 
      digitalWrite(orden_digitos[1], LOW); 
      break;
    case 3: 
      digitalWrite(orden_digitos[2], LOW); 
      break;
    default: 
      digitalWrite(orden_digitos[3], LOW); 
      break;
  }
}
void setup(){
  for(i=2; i <= 13;i++)     
    pinMode(i,OUTPUT);
} 
void loop(){
  for(mil=0;mil<=9;mil++){
    for(cien=0;cien<=9;cien++){
      for(dec=0;dec<=9;dec++){
        for(uni=0;uni<=9;uni++){
          EscribeDigito(1, uni);
          delay(vel);
          EscribeDigito(2, dec);
          delay(vel);
          EscribeDigito(3, cien);
          delay(vel);
          EscribeDigito(4, mil);
          delay(vel);
        } 
      }    
    }
  }
}