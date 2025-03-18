#include <Arduino.h>

# define LED1 13  // Primer LED
# define LED2 12  // Segundo LED
# define LED3 11  // Tercer LED
# define LED4 10  // Cuarto LED
# define LED5 9   // Quinto LED
# define SW1_pin 15  // Primer switch
# define SW2_pin 16  // Segundo switch
# define SW3_pin 17  // Tercer switch
# define SW4_pin 18  // Cuarto switch

bool ledStates[5]={false, false, false, false, false}; //Vector para estados de LEDs
bool switchSate= false; //

void setup() {
  // LEDs como salida
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);

  // Switches como entrada
  pinMode(SW1_pin, INPUT);
  pinMode(SW2_pin, INPUT);
  pinMode(SW3_pin, INPUT);
  pinMode(SW4_pin, INPUT);
}

void loop() {
  int sw1= digitalRead(SW1_pin);
  int sw2= digitalRead(SW2_pin);
  int sw3= digitalRead(SW3_pin);
  int sw4= digitalRead(SW4_pin);

  //Switch 1 activado: Encendido progresivo de TODOS los LEDs
  if(sw1==HIGH && !switchSate){
    switchSate=true;
    for(int luz = 0; luz <= 255; luz++){
      analogWrite(LED1,luz);
      analogWrite(LED2,luz);
      analogWrite(LED3,luz);
      analogWrite(LED4,luz);
      analogWrite(LED5,luz);
      delay(10);
    }

    for(int i = 0; i<5;i++) ledStates[i]=true;
  }

  if(sw1==LOW && switchSate){
    switchSate=false;
    for(int luz = 255; luz >= 0; luz--){
      analogWrite(LED1,luz);
      analogWrite(LED2,luz);
      analogWrite(LED3,luz);
      analogWrite(LED4,luz);
      analogWrite(LED5,luz);
      delay(10);
    }

    for(int i = 0; i<5;i++) ledStates[i]=true;
  }

  int opcion= 0;
  if(sw2==HIGH)opcion=1;
  else if(sw3==HIGH)opcion=2; 
  else if(sw4==HIGH)opcion=3;
  
  switch (opcion)
  {
  case 1:
    analogWrite(LED1,255);
    ledStates[0]=true;
    break;

  case 2:
    analogWrite(LED2,255);
    analogWrite(LED3,255);
    ledStates[1]=true;
    ledStates[2]=true;
    break;
  case 3:
    analogWrite(LED4,255);
    analogWrite(LED5,255);
    ledStates[3]=true;
    ledStates[4]=true;
    break;
  default:
    if(sw2==LOW){
      analogWrite(LED1,0);
      ledStates[0]=false;
    }
    if(sw3==LOW){
      analogWrite(LED2,0);
      analogWrite(LED3,0);
      ledStates[1]=false;
      ledStates[2]=false;
    }
    if(sw4==LOW){
      analogWrite(LED4,0);
      analogWrite(LED5,0);
      ledStates[3]=false;
      ledStates[4]=false;
    }
    break;
  }
  delay(2000);
}
