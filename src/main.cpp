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

bool estadoLEDs[5]={false, false, false, false, false}; //Vector para estados de LEDs
bool ledsEncendidosPorSW1= false; 
bool apagadoProgresivoHecho = false;

void encenderProgresivoTodos();
void apagarProgresivoTodos();
void actualizarEstadoLEDs(bool estados[]);
void setEstados(bool e0, bool e1, bool e2, bool e3, bool e4);


void setup() {
  Serial.begin(9600);
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

  setEstados(false, false, false, false, false);
  actualizarEstadoLEDs(estadoLEDs);
}


void loop() {
  bool sw1= digitalRead(SW1_pin);
  bool sw2= digitalRead(SW2_pin);
  bool sw3= digitalRead(SW3_pin);
  bool sw4= digitalRead(SW4_pin);

  bool sw5= sw2 && sw3 && sw4;
  bool sw6= sw3 && sw4;
  bool sw7= sw2 && sw3;
  bool sw8= sw2 && sw4;
    

  int opcion= 0;
  if(sw1)opcion=1;
  else if(sw5)opcion=5; 
  else if(sw6)opcion=6;
  else if(sw5)opcion=7; 
  else if(sw6)opcion=8;
  else if(sw4 && !sw1 && !sw2 && !sw3)opcion=4;
  else if(sw3 && !sw1 && !sw2 && !sw4)opcion=3; 
  else if(sw2 && !sw1 && !sw3 && !sw4)opcion=2;
  else opcion=0;
  
  if(!sw1 && ledsEncendidosPorSW1 && !apagadoProgresivoHecho){
    apagarProgresivoTodos();
    ledsEncendidosPorSW1=false;
    apagadoProgresivoHecho = true;
    delay(500);
    return;
  }

  if(sw1) apagadoProgresivoHecho = false;


  switch (opcion)
  {
  case 1:
    Serial.println("Switch 1 activado: Encendido progresivo de TODOS los LEDs");
    encenderProgresivoTodos();
    ledsEncendidosPorSW1 = true;
    break;

  case 2:
    Serial.println("Switch 2 activado: Encender LED1");
    setEstados(true, false, false, false, false);
    actualizarEstadoLEDs(estadoLEDs);
    break;

  case 3:
    Serial.println("Switch 3 activado: Encender LED2 y LED3");
    setEstados(false, true, true, false, false);
    actualizarEstadoLEDs(estadoLEDs);
  break;

  case 4:
    Serial.println("Switch 4 activado: Encender LED4 y LED5");
    setEstados(false, false, false, true, true);
    actualizarEstadoLEDs(estadoLEDs);
  break;
    
  default:
    setEstados(false, false, false, false, false);
    actualizarEstadoLEDs(estadoLEDs);
  break;
  }
  delay(500);
}

void setEstados(bool e0, bool e1, bool e2, bool e3, bool e4) {
  estadoLEDs[0] = e0;
  estadoLEDs[1] = e1;
  estadoLEDs[2] = e2;
  estadoLEDs[3] = e3;
  estadoLEDs[4] = e4;
}

// Actualiza físicamente los LEDs según el array estadoLEDs
void actualizarEstadoLEDs(bool estados[]) {
  int pines[5] = {LED1, LED2, LED3, LED4, LED5};

  for (int i = 0; i < 5; i++) {
    if (estados[i]) {
      analogWrite(pines[i], 255);  // Encendido total
    } else {
      analogWrite(pines[i], 0);    // Apagado
    }
  }
}

// ==============================
// Encendido progresivo
void encenderProgresivoTodos() {
  int pines[5] = {LED1, LED2, LED3, LED4, LED5};

  if (estadoLEDs[0] && estadoLEDs[1] && estadoLEDs[2] && estadoLEDs[3] && estadoLEDs[4]) {
    Serial.println("Todos los LEDs ya están encendidos");
    return;
  }

  int pwm = 0;
  while (pwm <= 255) {
    for (int i = 0; i < 5; i++) {
      analogWrite(pines[i], pwm);
    }
    pwm += 5;
    delay(30);
  }

  setEstados(true, true, true, true, true);
}

// Apagado progresivo
void apagarProgresivoTodos() {
  int pines[5] = {LED1, LED2, LED3, LED4, LED5};

  if (!estadoLEDs[0] && !estadoLEDs[1] && !estadoLEDs[2] && !estadoLEDs[3] && !estadoLEDs[4]) {
    Serial.println("Todos los LEDs ya están apagados");
    return;
  }

  int pwm = 255;
  while (pwm >= 0) {
    for (int i = 0; i < 5; i++) {
      analogWrite(pines[i], pwm);
    }
    pwm -= 5;
    delay(10);
  }

  setEstados(false, false, false, false, false);
}
