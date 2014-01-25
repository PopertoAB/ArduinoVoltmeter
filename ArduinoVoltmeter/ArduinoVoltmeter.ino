int entrada=2;
int salida=13;

int lastButtonState=HIGH;
int buttonState;
long lastDebounceTime=0;
long debounceDelay=50;
long inicioPresion=0;
long finPresion=0;
int calibrar=0;
int canal=0;
float calibracion=1.0;

void setup(){
  Serial.begin(9600);
  while(!Serial);

  pinMode(entrada, INPUT_PULLUP);
  pinMode(salida, OUTPUT);

  inicioPresion=millis();
}

void loop(){
  leerBoton();
  leerVoltaje();
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
        //Serial.println("ALTO");
        finPresion=millis();
        float presion=(float)(finPresion-inicioPresion)/1000.0;
        if(presion>3.0){
          calibrar=!calibrar;
          if(calibrar==0){
            Serial.println("No calibrando");
          }
          else if(calibrar==1){
            Serial.println("Calibrando");
          }
        }
        else{
          Serial.print("Canal ");
          Serial.println(canal);
          if(canal++>=5){
            canal=0;
          }
        }
      }
      else if(buttonState==LOW){
        //Serial.println("BAJO"); 
        inicioPresion=millis();     
      }
    }
  }

  lastButtonState=reading;
}

void leerVoltaje(){
  if(!calibrar){
    int voltaje=analogRead(canal);
    long voltajeMap=map(voltaje,0,1023,0,200);
    Serial.println(voltajeMap*calibracion);
  }
  else{
    calibracion=(1023.0*5.0)/((float)analogRead(canal)*200.0);
    Serial.println(calibracion);
  }
}
