#include <D7S.h>

int tiempoVista = 4;
int canal=0;
int lectura;
float lecturaFlotante;

D7S dig1(5, tiempoVista);
D7S dig2(4, tiempoVista);
D7S dig3(1, tiempoVista);
D7S dig4(0, tiempoVista);

void setup(){
  pinMode(2, INPUT);
}

void loop(){
  manejarInterrupcion();
  checarCanal();
  mostrarDatos();
}

void checarCanal(){
  lectura=analogRead(canal);
  lecturaFlotante=(float)(((float)lectura/1023.0)*20.0);
  switch(canal){
  case 0:
    dig4.display(0x77);
    break;
  case 1:
    dig4.display(0x7c);
    break;
  case 2:
    dig4.display(0x39);
    break;
  case 3:
    dig4.display(0x5e);
    break;
  case 4:
    dig4.display(0x79);
    break;
  case 5:
    dig4.display(0x71);
    break;
  }
}

void mostrarDatos(){
  dig1.display(ponerNumero(obtenerDecenas()));
  dig2.display(ponerNumero(obtenerUnidades())|0x80);
  dig3.display(ponerNumero(obtenerDecimales()));
}

byte ponerNumero(int numero){
  switch(numero){
  case 0:
    return 0x3f;
  case 1:
    return 0x06;
  case 2:
    return 0x5b;
  case 3:
    return 0x4f;
  case 4:
    return 0x66;
  case 5:
    return 0x6d;
  case 6:
    return 0x7d;
  case 7:
    return 0x07;
  case 8:
    return 0x7f;
  case 9:
    return 0x67;
  default:
    return 0x79;
  }
}

int obtenerDecenas(){
  int temp = ((int)lecturaFlotante/10)%10;
  return temp;
}

int obtenerUnidades(){
  int temp = (int)lecturaFlotante%10;
  return temp;
}

int obtenerDecimales(){
  int temp = (int)(lecturaFlotante*10.0)%10;
  return temp;
}

void manejarInterrupcion(){
  if(digitalRead(2)==0){
    delay(500);
    canal++;
    if(canal>5){
      canal=0;
    }
  }
}


