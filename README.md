## Descripción
Este proyecto implementa un sistema que controla cinco LEDs utilizando cuatro switches en una placa Arduino. 
Dependiendo de la combinación de switches activados, los LEDs se encienden y apagan de diferentes maneras, incluyendo encendido/apagado progresivo.
## Funcionamiento 👋
El programa lee constantemente el estado de los switches y ejecuta diferentes acciones en los LEDs:
1. SW1 activado : Enciende todos los LEDs progresivamente.
2. SW1 desactivado después de encender : Apaga todos los LEDs progresivamente.
3. SW2 activado : Enciende solo el LED1.
4. SW3 activado : Enciende LED2 y LED3.
5. SW4 activado : Enciende LED4 y LED5.
6. SW2, SW3 y SW4 activados simultáneamente : Enciende LED2, LED3 y LED4 (titilan).
7. SW3 y SW4 activados simultáneamente : Enciende LED3, LED4 y LED5.
8. SW2 y SW3 activados simultáneamente : Enciende LED1, LED2 y LED5.
9. SW2 y SW4 activados simultáneamente : Enciende LED1 y LED5 (titilan).
10. Ningún switch activado : Apaga todos los LEDs.
