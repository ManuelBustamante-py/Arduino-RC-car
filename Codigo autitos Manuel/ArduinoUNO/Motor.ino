/*
  Nombre del Proyecto: Autito Arduino
  Descripción: Kit de auto 4WD programable con Arduino
  Autor: Manuel Bustamante
  Copyright (c) 2024 Manuel Bustamante
*/

void forward(int adjust)
{
  switch (adjust)
  {
    case 1: MOTOR_GO_FORWARD; return;  // Avanzar
    case 2: MOTOR_GO_FORWARD; return;  // Avanzar
    case 3: MOTOR_GO_BACK; return;     // Retroceder
    case 4: MOTOR_GO_BACK; return;     // Retroceder
    case 5: MOTOR_GO_LEFT; return;     // Girar a la izquierda
    case 6: MOTOR_GO_LEFT; return;     // Girar a la izquierda
    case 7: MOTOR_GO_RIGHT; return;    // Girar a la derecha
    case 8: MOTOR_GO_RIGHT; return;    // Girar a la derecha
    default: return;
  }
}
 
void back(int adjust)
{
  switch (adjust)
  {
    case 1: MOTOR_GO_BACK; return;     // Retroceder
    case 2: MOTOR_GO_BACK; return;     // Retroceder
    case 3: MOTOR_GO_FORWARD; return;  // Avanzar
    case 4: MOTOR_GO_FORWARD; return;  // Avanzar
    case 5: MOTOR_GO_RIGHT; return;    // Girar a la derecha
    case 6: MOTOR_GO_RIGHT; return;    // Girar a la derecha
    case 7: MOTOR_GO_LEFT; return;     // Girar a la izquierda
    case 8: MOTOR_GO_LEFT; return;     // Girar a la izquierda
    default: return;
  }
}

void left(int adjust)
{
  switch (adjust)
  {
    case 1: MOTOR_GO_LEFT; return;     // Girar a la izquierda
    case 2: MOTOR_GO_RIGHT; return;    // Girar a la derecha
    case 3: MOTOR_GO_LEFT; return;     // Girar a la izquierda
    case 4: MOTOR_GO_RIGHT; return;    // Girar a la derecha
    case 5: MOTOR_GO_FORWARD; return;  // Avanzar
    case 6: MOTOR_GO_BACK; return;     // Retroceder
    case 7: MOTOR_GO_FORWARD; return;  // Avanzar
    case 8: MOTOR_GO_BACK; return;     // Retroceder
    default: return;
  }
}

void right(int adjust)
{
  switch (adjust)
  {
    case 1: MOTOR_GO_RIGHT; return;    // Girar a la derecha
    case 2: MOTOR_GO_LEFT; return;     // Girar a la izquierda
    case 3: MOTOR_GO_RIGHT; return;    // Girar a la derecha
    case 4: MOTOR_GO_LEFT; return;     // Girar a la izquierda
    case 5: MOTOR_GO_BACK; return;     // Retroceder
    case 6: MOTOR_GO_FORWARD; return;  // Avanzar
    case 7: MOTOR_GO_BACK; return;     // Retroceder
    case 8: MOTOR_GO_FORWARD; return;  // Avanzar
    default: return;
  }
}

void  Avoiding()  // Modo de Evitar Obstáculos por IR
{
  IR = digitalRead(Input_Detect);
  if ((IR == LOW))
  {
    MOTOR_GO_STOP;  // DETENER
  }
}

void TrackLine()   // Modo de Seguir Línea
{
  IR_L = digitalRead(Input_Detect_LEFT);   // Leer el valor del sensor izquierdo
  IR_R = digitalRead(Input_Detect_RIGHT);  // Leer el valor del sensor derecho

  if ((IR_L == LOW) && (IR_R == LOW)) // Obstáculos detectados en ambos lados al mismo tiempo
  {
    forward(adjust);  // Avanzar
    return;
  }
  
  if ((IR_L == LOW) && (IR_R == HIGH)) // Obstáculo en el lado derecho
  {
    left(adjust);  // Girar a la izquierda
    return;
  }
  
  if ((IR_L == HIGH) && ( IR_R == LOW)) // Obstáculo en el lado izquierdo
  {
    right(adjust);  // Girar a la derecha
    return;
  }
  
  if ((IR_L == HIGH) && (IR_R == HIGH)) // Detectado en el lado izquierdo y derecho
  {
    MOTOR_GO_STOP;  // DETENER
    return;
  }
}

void AvoidByRadar(int distance)  // Evitar obstáculos por ultrasonido
{
  int leng = Get_Distance();
  if(distance < 10) distance = 10;    // Limitar la distancia mínima de evitación de obstáculos a 10 cm
  if((leng > 1) && (leng < distance)) // Distancia de evitación de obstáculos (unidad: cm), mayor que 1 es para evitar la zona ciega del ultrasonido
  {
    while((Get_Distance() > 1) && (Get_Distance() < distance))
    {
      back(adjust);
    }
    MOTOR_GO_STOP;
  }
}

void Route()
{
  if (RevStatus == 0)
  {
    Sendbyte(0xff);
    Sendbyte(0xA8);
    Sendbyte(0x00);
    Sendbyte(0x00);
    Sendbyte(0xff);
    delay(500);
  }

  while (RevStatus)
  {
    if (RevStatus == 1)
    {
      RevStatus = 0;
      left(adjust);
      delay(TurnAngle * 6);
      MOTOR_GO_STOP;
      forward(adjust);
      delay(Golength * 10);
      MOTOR_GO_STOP;
      break;
    }
    if (RevStatus == 2)
    {
      RevStatus = 0;
      right(adjust);
      delay(TurnAngle * 6);
      MOTOR_GO_STOP;
      forward(adjust);
      delay(Golength * 10);
      MOTOR_GO_STOP;
      break;
    }
  }
}
