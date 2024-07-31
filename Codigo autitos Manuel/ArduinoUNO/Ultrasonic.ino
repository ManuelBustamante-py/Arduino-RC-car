/*
  Nombre del Proyecto: Autito Arduino
  Descripción: Kit de auto 4WD programable con Arduino
  Autor: Manuel Bustamante
  Copyright (c) 2024 Manuel Bustamante
*/

#define obstacle_threshold 20 // Umbral de distancia para detectar un obstáculo (en cm)

// Función para evitar obstáculos
void Avoid_Obstacle() {
  // Gira hacia la izquierda para esquivar el obstáculo
  // Aquí deberías llamar a las funciones específicas para girar el vehículo hacia la izquierda
  // Por ejemplo, si tienes una función llamada turn_left(), llámala aquí
}

// Función para medir la distancia con el sensor ultrasónico
char Get_Distance() {
  digitalWrite(Trig, LOW);   // Emite un pulso LOW al sensor ultrasónico
  delayMicroseconds(2);
  digitalWrite(Trig, HIGH);  // Emite un pulso HIGH al sensor ultrasónico
  delayMicroseconds(10);
  digitalWrite(Trig, LOW);    // Mantiene el pulso LOW del sensor ultrasónico
  float distance = pulseIn(Echo, HIGH, 5000);  // Lee la diferencia de tiempo
  distance = distance / 5.8 / 10; // Convierte el tiempo a distancia en centímetros
  return distance;
}

// Función para enviar la distancia a través del puerto serial (sin modificaciones)
void Send_Distance() {
  int dis = Get_Distance();
  Sendbyte(0xff);
  Sendbyte(0x03);
  Sendbyte(0x00);
  Sendbyte(dis);
  Sendbyte(0xff);
  delay(1000);
}

// Función principal de ejecución (sin usar la función loop)
void execute() {
  // Medir la distancia utilizando el sensor ultrasónico
  int distance = Get_Distance();

  // Si la distancia es menor que el umbral, hay un obstáculo
  if (distance < obstacle_threshold) {
    // Evasión de obstáculos
    Avoid_Obstacle();
  } else {
    // Movimiento normal del vehículo
    // Aquí deberías llamar a las funciones específicas para el movimiento normal del vehículo
    // Por ejemplo, si tienes una función llamada move_forward(), llámala aquí
  }
}
