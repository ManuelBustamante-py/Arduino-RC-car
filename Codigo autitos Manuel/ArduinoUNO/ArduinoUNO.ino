/*
  Nombre del Proyecto: Autito Arduino
  Descripción: Kit de auto 4WD programable con Arduino
  Autor: Manuel Bustamante
  Copyright (c) 2024 Manuel Bustamante
*/

#include <Servo.h>
#include <EEPROM.h>

int ledpin = A0;                    // Define el pin para el indicador de inicio del sistema
int ENA = 5;                        // Pin de habilitación para el controlador del Motor A
int ENB = 6;                        // Pin de habilitación para el controlador del Motor B
int INPUT2 = 7;                     // Interfaz del Motor 1
int INPUT1 = 8;                     // Interfaz del Motor 2
int INPUT3 = 12;                    // Interfaz del Motor 3
int INPUT4 = 13;                    // Interfaz del Motor 4
int adjust = 1;                     // Bandera del motor para ajuste
int Echo = A5;                      // Pin para recibir señales ultrasónicas
int Trig = A4;                      // Pin para transmitir señales ultrasónicas
int Input_Detect_LEFT = A3;         // Pin para detección de infrarrojos del lado izquierdo
int Input_Detect_RIGHT = A2;        // Pin para detección de infrarrojos del lado derecho
int Input_Detect = A1;              // Pin para detección de infrarrojos frontales
int Carled = A0;                    // Pin para los faros del auto
int Cruising_Flag = 0;              // Bandera para el cambio de modo
int Pre_Cruising_Flag = 0;          // Registro del último modo
int Left_Speed_Hold = 255;          // Variable para mantener la velocidad izquierda
int Right_Speed_Hold = 255;         // Variable para mantener la velocidad derecha

Servo servo1;                       // Servomotor 1
Servo servo2;                       // Servomotor 2
Servo servo3;                       // Servomotor 3
Servo servo4;                       // Servomotor 4
Servo servo7;                       // Servomotor 7
Servo servo8;                       // Servomotor 8

byte angle1 = 70;                   // Valor inicial del ángulo para el servomotor 1
byte angle2 = 60;                   // Valor inicial del ángulo para el servomotor 2
byte angle3 = 60;                   // Valor inicial del ángulo para el servomotor 3
byte angle4 = 60;                   // Valor inicial del ángulo para el servomotor 4
byte angle7 = 60;                   // Valor inicial del ángulo para el servomotor 7
byte angle8 = 60;                   // Valor inicial del ángulo para el servomotor 8

int buffer[3];                      // Búfer para la comunicación USART
int rec_flag;                       // Bandera para la comunicación USART
int serial_data;                    // Almacenamiento temporal para datos seriales
unsigned long Costtime;             // Contador para el tiempo de espera de serial
int IR_R;                           // Indicador de estado infrarrojo para el lado derecho
int IR_L;                           // Indicador de estado infrarrojo para el lado izquierdo
int IR;                             // Indicador de estado infrarrojo para el medio

// Define macros para movimientos del motor
#define MOTOR_GO_FORWARD  {digitalWrite(INPUT1,LOW);digitalWrite(INPUT2,HIGH);digitalWrite(INPUT3,LOW);digitalWrite(INPUT4,HIGH);}    // Avanzar
#define MOTOR_GO_BACK     {digitalWrite(INPUT1,HIGH);digitalWrite(INPUT2,LOW);digitalWrite(INPUT3,HIGH);digitalWrite(INPUT4,LOW);}    // Retroceder
#define MOTOR_GO_RIGHT    {digitalWrite(INPUT1,HIGH);digitalWrite(INPUT2,LOW);digitalWrite(INPUT3,LOW);digitalWrite(INPUT4,HIGH);}    // Girar a la derecha
#define MOTOR_GO_LEFT     {digitalWrite(INPUT1,LOW);digitalWrite(INPUT2,HIGH);digitalWrite(INPUT3,HIGH);digitalWrite(INPUT4,LOW);}    // Girar a la izquierda
#define MOTOR_GO_STOP     {digitalWrite(INPUT1,LOW);digitalWrite(INPUT2,LOW);digitalWrite(INPUT3,LOW);digitalWrite(INPUT4,LOW);}      // Detener

int RevStatus = 0;                  // Estado de reversa
int TurnAngle = 0;                  // Ángulo de giro
int Golength = 0;                   // Longitud de movimiento

// Función para el cambio de modo
void Cruising_Mod()
{
  if (Pre_Cruising_Flag != Cruising_Flag)
  {
    if (Pre_Cruising_Flag != 0)
    {
      MOTOR_GO_STOP;
    }
    Pre_Cruising_Flag = Cruising_Flag;
  }
  switch (Cruising_Flag)
  {
    case 2: TrackLine(); return;           // Entrar en el Modo de Seguir Línea
    case 3: Avoiding(); return;            // Entrar en el Modo de Evitar Obstáculos por IR
    case 4: AvoidByRadar(15); return;      // Entrar en el Modo de Evitar Obstáculos por Ultrasonido
    case 5: Send_Distance(); return;       // Entrar en el Modo de Mostrar Distancia por Ultrasonido
    case 7: Route(); return;               // Entrar en el Modo de Planificación de Ruta
    default: return;
  }
}

void setup()
{
  pinMode(ledpin, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(INPUT1, OUTPUT);
  pinMode(INPUT2, OUTPUT);
  pinMode(INPUT3, OUTPUT);
  pinMode(INPUT4, OUTPUT);
  pinMode(Input_Detect_LEFT, INPUT);
  pinMode(Input_Detect_RIGHT, INPUT);
  pinMode(Carled, OUTPUT);
  pinMode(Input_Detect, INPUT);
  pinMode(Echo, INPUT);
  pinMode(Trig, OUTPUT);
  Delayed();                              // Espera de 50 segundos para que el módulo WiFi se inicie
  servo1.attach(3);                       // Conectar servo1 al pin 3
  servo2.attach(4);                       // Conectar servo2 al pin 4
  servo3.attach(2);                       // Conectar servo3 al pin 2
  servo4.attach(11);                      // Conectar servo4 al pin 11
  servo7.attach(9);                       // Conectar servo7 al pin 9
  servo8.attach(10);                      // Conectar servo8 al pin 10
  USART_init();                           // Inicializar puerto serie con baud rate de 9600
  Init_Steer();                           // Inicializar ángulo del servo
}

void loop()
{
  while (1)
  {
    UartTimeoutCheck();                  // Verificar el tiempo de espera del puerto serie
    Cruising_Mod();                      // Realizar cambio de modo
  }
}