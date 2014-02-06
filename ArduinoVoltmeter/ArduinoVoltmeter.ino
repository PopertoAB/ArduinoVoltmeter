#include <D7S.h>
#include <EEPROM.h>
#include <Math.h>
#include "EEPROMAnything.h"

int tiempoEspera=4;

D7S dig1(5,tiempoEspera);
D7S dig2(4,tiempoEspera);
D7S dig3(1,tiempoEspera);
D7S dig4(0,tiempoEspera);

int entrada=2;

int voltaje;
long voltajeMap;

int lastButtonState=HIGH;
int buttonState;
long lastDebounceTime=0;
long debounceDelay=50;
long inicioPresion=0;
long finPresion=0;
int calibrar=0;
int canal=-1;
float calibracion[6];
float voltajeCalibrado;

void setup(){
  pinMode(entrada, INPUT_PULLUP);

  EEPROM_readAnything(0, calibracion);
  for(int i=0; i < (sizeof(calibracion)/sizeof(float)) ; i++){
    if(isnan(calibracion[i]) || isinf(calibracion[i])){
      calibracion[i]=1.0;
    }
  }  

  inicioPresion=millis();
}

void loop(){
  leerBoton();
  checarCanal();
  leerVoltaje();
  mostrarVoltaje();
}

void leerBoton(){
  int reading = digitalRead(entrada);

  if(reading!=lastButtonState){
    lastDebounceTime=millis();
  }

  if((millis()-lastDebounceTime)>debounceDelay){
    if(reading!=buttonState){
      buttonState=reading;

      if(buttonState==HIGH){
        finPresion=millis();
        float presion=(float)(finPresion-inicioPresion)/1000.0;
        if(presion>3.0){
          calibrar=!calibrar;
        }
        else{
          if(canal++>=5){
            canal=0;
          }
        }
      }
      else if(buttonState==LOW){
        inicioPresion=millis();     
      }
    }
  }

  lastButtonState=reading;
}

void checarCanal(){
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

void leerVoltaje(){
  voltaje=analogRead(canal);
  voltajeMap=map(voltaje,0,1023,0,200);
  voltajeCalibrado=(float)voltajeMap*calibracion[canal];
  if(!calibrar){
  }
  else{
    float calibracionPrevia=calibracion[canal];
    calibracion[canal]=(200.0*90.0)/((float)voltajeMap*200.0);
    if(calibracionPrevia!=calibracion[canal]){
      EEPROM_updateAnything(0, calibracion);
    }
  }
}

void mostrarVoltaje(){
  if(!calibrar){
    dig1.display(ponerNumero(obtenerDecenas()));
    dig2.display(ponerNumero(obtenerUnidades())|0x80);
    dig3.display(ponerNumero(obtenerDecimales()));
  }
  else{
    dig1.display(ponerNumero(obtenerUnidadCal())|0x80);
    dig2.display(ponerNumero(obtenerDecimasCal()));
    dig3.display(ponerNumero(obtenerCentesimasCal())|0x80);
  }
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
  int temp = ((int)voltajeCalibrado/100)%100;
  return temp;
}

int obtenerUnidades(){
  int temp = (((int)voltajeCalibrado%100)/10)%10;
  return temp;
}

int obtenerDecimales(){
  int temp = (int)voltajeCalibrado%10;
  return temp;
}

int obtenerUnidadCal(){
  return (int)calibracion[canal]%10;
}

int obtenerDecimasCal(){
  return (int)(calibracion[canal]*10.0)%10;
}

int obtenerCentesimasCal(){
  return (int)(calibracion[canal]*100.0)%10;
}
