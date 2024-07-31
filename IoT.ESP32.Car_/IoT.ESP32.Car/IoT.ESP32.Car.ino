#include <Servo.h>

// Definir los pines de control de los motores
#define MOTOR1_POSITIVE 2
#define MOTOR1_NEGATIVE 3
#define MOTOR2_POSITIVE 4
#define MOTOR2_NEGATIVE 5

void setup() {
  // Inicializar los pines de control de los motores como salidas
  pinMode(MOTOR1_POSITIVE, OUTPUT);
  pinMode(MOTOR1_NEGATIVE, OUTPUT);
  pinMode(MOTOR2_POSITIVE, OUTPUT);
  pinMode(MOTOR2_NEGATIVE, OUTPUT);
}

void loop() {
  // Girar los motores de la derecha hacia adelante
  digitalWrite(MOTOR1_POSITIVE, HIGH);
  digitalWrite(MOTOR1_NEGATIVE, LOW);
  digitalWrite(MOTOR2_POSITIVE, HIGH);
  digitalWrite(MOTOR2_NEGATIVE, LOW);

  // Retardo para mantener los motores en movimiento
  delay(1000); // Tiempo en milisegundos
}
