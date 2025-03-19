#include <Arduino.h>

// Definición de los pines para los LEDs
# define LED1 13  // Primer LED
# define LED2 12  // Segundo LED
# define LED3 11  // Tercer LED
# define LED4 10  // Cuarto LED
# define LED5 9   // Quinto LED

// Definición de los pines para los switches
# define SW1_pin 15  // Primer switch
# define SW2_pin 16  // Segundo switch
# define SW3_pin 17  // Tercer switch
# define SW4_pin 18  // Cuarto switch

// Arreglo para almacenar el estado (encendido/apagado) de cada LED
bool estadoLEDs[5]={false, false, false, false, false};

// Variables para controlar el tiempo en milisegundos
unsigned long milliActual=0;    // Tiempo actual
unsigned long milliPrevio5=0;   // Último tiempo registrado para la opción 5
unsigned long milliPrevio8=0;   // Último tiempo registrado para la opción 8

// Variables para controlar ciertos estados lógicos
bool ledsEncendidosPorSW1= false;      // Indica si los LEDs fueron encendidos con SW1
bool apagadoProgresivoHecho = false;   // Evita apagar progresivamente más de una vez

// Declaración de funciones
void encenderProgresivoTodos();
void apagarProgresivoTodos();
void actualizarEstadoLEDs(bool estados[]);
void setEstados(bool e0, bool e1, bool e2, bool e3, bool e4);
void mostrarEstadoEnSerial();

void setup() {
  Serial.begin(9600); // Inicializa la comunicación serial

  // Configuración de los pines de LEDs como salidas
  pinMode(LED1, OUTPUT);
  pinMode(LED2, OUTPUT);
  pinMode(LED3, OUTPUT);
  pinMode(LED4, OUTPUT);
  pinMode(LED5, OUTPUT);

  // Configuración de los pines de switches como entradas
  pinMode(SW1_pin, INPUT);
  pinMode(SW2_pin, INPUT);
  pinMode(SW3_pin, INPUT);
  pinMode(SW4_pin, INPUT);

  // Apaga todos los LEDs al inicio
  setEstados(false, false, false, false, false);
  actualizarEstadoLEDs(estadoLEDs);

  Serial.println("Sistema de control de LEDs iniciado");
}

void loop() {
  // Lectura del estado de los switches
  bool sw1= digitalRead(SW1_pin);
  bool sw2= digitalRead(SW2_pin);
  bool sw3= digitalRead(SW3_pin);
  bool sw4= digitalRead(SW4_pin);

  // Combinaciones de switches activados
  bool sw5= sw2 && sw3 && sw4;
  bool sw6= sw3 && sw4;
  bool sw7= sw2 && sw3;
  bool sw8= sw2 && sw4;

  milliActual=millis(); // Actualiza el tiempo actual
  int opcion= 0;        // Variable que define la acción a realizar
  
  // Determina la opción según las combinaciones de switches presionados
  if(sw1) opcion=1;
  else if(sw5) opcion=5; 
  else if(sw6) opcion=6;
  else if(sw7) opcion=7; 
  else if(sw8) opcion=8;
  else if(sw4 && !sw1 && !sw2 && !sw3) opcion=4;
  else if(sw3 && !sw1 && !sw2 && !sw4) opcion=3; 
  else if(sw2 && !sw1 && !sw3 && !sw4) opcion=2;
  else opcion=0;  // Si no hay ninguna combinación válida, opción 0 (apagar todo)

  // Si SW1 se suelta después de estar activo, realiza el apagado progresivo
  if(!sw1 && ledsEncendidosPorSW1 && !apagadoProgresivoHecho){
    Serial.println("SW1 desactivado -> Apagado progresivo de TODOS los LEDs");
    apagarProgresivoTodos();
    ledsEncendidosPorSW1=false;
    apagadoProgresivoHecho = true;
    delay(500); // Espera antes de continuar
    return;
  }

  // Si se presiona de nuevo SW1, permite apagar otra vez después
  if(sw1) apagadoProgresivoHecho = false;

  // Ejecuta la acción según la opción determinada
  switch (opcion)
  {
    case 1:
      // SW1: Encendido progresivo de todos los LEDs
      Serial.println("Switch 1 activado: Encendido progresivo de TODOS los LEDs");
      encenderProgresivoTodos();
      ledsEncendidosPorSW1 = true;
      break;

    case 2:
      // SW2: Enciende solo el LED1
      Serial.println("Switch 2 activado: Encender LED1");
      setEstados(true, false, false, false, false);
      actualizarEstadoLEDs(estadoLEDs);
      break;

    case 3:
      // SW3: Enciende LED2 y LED3
      Serial.println("Switch 3 activado: Encender LED2 y LED3");
      setEstados(false, true, true, false, false);
      actualizarEstadoLEDs(estadoLEDs);
      break;

    case 4:
      // SW4: Enciende LED4 y LED5
      Serial.println("Switch 4 activado: Encender LED4 y LED5");
      setEstados(false, false, false, true, true);
      actualizarEstadoLEDs(estadoLEDs);
      break;

    case 5:
      // SW2 + SW3 + SW4: Enciende LED2, LED3 y LED4, y hace un parpadeo cada segundo
      Serial.println("Switches 2, 3 y 4 activados: LED2, LED3 y LED4 encendidos");
      setEstados(false, true, true, true, false);
      actualizarEstadoLEDs(estadoLEDs);

      // Parpadeo cada 1 segundo
      if (milliActual - milliPrevio5 >= 1000 && !estadoLEDs[1] && !estadoLEDs[2] && !estadoLEDs[3]){
        setEstados(false, true, true, true, false);
        actualizarEstadoLEDs(estadoLEDs);
        milliPrevio5 = milliActual;
      } else if (milliActual - milliPrevio5 >= 1000 && estadoLEDs[1] && estadoLEDs[2] && estadoLEDs[3]){
        setEstados(false, false, false, false, false);
        actualizarEstadoLEDs(estadoLEDs);
        milliPrevio5 = milliActual;
      }
      break;

    case 6:
      // SW3 + SW4: Enciende LED3, LED4 y LED5
      Serial.println("Switches 3 y 4 activados: LED3, LED4 y LED5 encendidos");
      setEstados(false, false, true, true, true);
      actualizarEstadoLEDs(estadoLEDs);
      break;

    case 7:
      // SW2 + SW3: Enciende LED1, LED2 y LED5
      Serial.println("Switches 2 y 3 activados: LED1, LED2 y LED5 encendidos");
      setEstados(true, true, false, false, true);
      actualizarEstadoLEDs(estadoLEDs);
      break;

    case 8:
      // SW2 + SW4: Enciende LED1 y LED5, y hace un parpadeo cada segundo
      Serial.println("Switches 2 y 4 activados: LED1 y LED5 encendidos");
      setEstados(true, false, false, false, true);
      actualizarEstadoLEDs(estadoLEDs);

      // Parpadeo cada 1 segundo
      if (milliActual - milliPrevio8 >= 1000 && !estadoLEDs[0] && !estadoLEDs[4]){
        setEstados(true, false, false, false, true);
        actualizarEstadoLEDs(estadoLEDs);
        milliPrevio8 = milliActual;
      } else if (milliActual - milliPrevio8 >= 1000 && estadoLEDs[0] && estadoLEDs[4]){
        setEstados(false, false, false, false, false);
        actualizarEstadoLEDs(estadoLEDs);
        milliPrevio8 = milliActual;
      }
      break;

    default:
      // Opción 0: Apaga todos los LEDs
      setEstados(false, false, false, false, false);
      actualizarEstadoLEDs(estadoLEDs);
      break;
  }

  // Muestra en el serial el estado actual de los LEDs
  mostrarEstadoEnSerial();

  delay(500); // Retardo para evitar lecturas excesivamente rápidas
}

// Función para actualizar el arreglo de estados de los LEDs
void setEstados(bool e0, bool e1, bool e2, bool e3, bool e4) {
  estadoLEDs[0] = e0;
  estadoLEDs[1] = e1;
  estadoLEDs[2] = e2;
  estadoLEDs[3] = e3;
  estadoLEDs[4] = e4;
}

// Actualiza los LEDs según el arreglo estadoLEDs
void actualizarEstadoLEDs(bool estados[]) {
  int pines[5] = {LED1, LED2, LED3, LED4, LED5};

  for (int i = 0; i < 5; i++) {
    if (estados[i]) {
      analogWrite(pines[i], 255);  // LED encendido (PWM a 255)
    } else {
      analogWrite(pines[i], 0);    // LED apagado (PWM a 0)
    }
  }
}

// ==============================
// Función para encender progresivamente todos los LEDs
void encenderProgresivoTodos() {
  int pines[5] = {LED1, LED2, LED3, LED4, LED5};

  // Si todos ya están encendidos, no hace nada
  if (estadoLEDs[0] && estadoLEDs[1] && estadoLEDs[2] && estadoLEDs[3] && estadoLEDs[4]) {
    Serial.println("Todos los LEDs ya están encendidos");
    return;
  }

  int pwm = 0;
  // Incrementa el brillo de 0 a 255 en pasos de 5
  while (pwm <= 255) {
    for (int i = 0; i < 5; i++) {
      analogWrite(pines[i], pwm);
    }
    pwm += 5;
    delay(30);
  }

  // Una vez encendidos al máximo, actualiza el estado 
  setEstados(true, true, true, true, true);
}

// Función para apagar progresivamente todos los LEDs
void apagarProgresivoTodos() {
  int pines[5] = {LED1, LED2, LED3, LED4, LED5};

  // Si todos ya están apagados, no hace nada
  if (!estadoLEDs[0] && !estadoLEDs[1] && !estadoLEDs[2] && !estadoLEDs[3] && !estadoLEDs[4]) {
    Serial.println("Todos los LEDs ya están apagados");
    return;
  }

  int pwm = 255;
  // Disminuye el brillo de 255 a 0 en pasos de 5
  while (pwm >= 0) {
    for (int i = 0; i < 5; i++) {
      analogWrite(pines[i], pwm);
    }
    pwm -= 5;
    delay(10);
  }

  // Una vez apagados, actualiza el estado
  setEstados(false, false, false, false, false);
}

// Muestra en el monitor serial el estado de cada LED
void mostrarEstadoEnSerial(){
  Serial.println("Estado actual de los LEDs");
  for(int i = 0; i < 5; i++){
    Serial.print("LED");
    Serial.print(i + 1);
    Serial.print(": ");
    Serial.println(estadoLEDs[i] ? "Encendido" : "Apagado");
  }
  Serial.println("-----------------------------------");
}