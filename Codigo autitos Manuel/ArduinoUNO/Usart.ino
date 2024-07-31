/*
  Nombre del Proyecto: Autito Arduino
  Descripción: Kit de auto 4WD programable con Arduino
  Autor: Manuel Bustamante
  Copyright (c) 2024 Manuel Bustamante
*/

#define BAUD 9600

void USART_init()
{
  SREG = 0x80;                              // Habilitar interrupción total
  // bitSet(UCSR0A,U2X0);
  bitSet(UCSR0B, RXCIE0);                   // Permitir interrupción de recepción completa
  bitSet(UCSR0B, RXEN0);                    // Encender función de recepción
  bitSet(UCSR0B, TXEN0);                    // Habilitar función de envío
  bitSet(UCSR0C, UCSZ01);
  bitSet(UCSR0C, UCSZ00);                   // Establecer comunicación asíncrona, sin paridad, 1 bit de terminación, datos de 8 bits
  UBRR0 = (F_CPU / 16 / BAUD - 1);          // Tasa de baudios 9600
}

void Sendbyte(char c)
{
  loop_until_bit_is_set(UCSR0A, UDRE0);
  UDR0 = c;
}

// ISR(USART0_RX_vect)               // for MEGA 2560
ISR(USART_RX_vect)                      // for UNO
{
  UCSR0B &= ~(1 << RXCIE0);             // Cerrar interrupción del puerto serial
  Get_uartdata(); 
  UCSR0B |= (1 << RXCIE0);              // Abrir interrupción del puerto serial
}

void SendString(char *str)
{
  while (*str)
  {
    Sendbyte(*str);
    str++;
  }
  Sendbyte(0x0d);
  Sendbyte(0x0a);  
}

void Get_uartdata(void)
{
  static int i;
  serial_data = UDR0;                    // Leer datos seriales
  if (rec_flag == 0)
  {
    if (serial_data == 0xff)             // Obtener 0xff por primera vez (es decir, encabezado del paquete)
    {
      rec_flag = 1;
      i = 0;
      Costtime = 0;
    }
  }
  else
  {
    if (serial_data == 0xff)             // Obtener 0xff por segunda vez (es decir, cola del paquete)
    {
      rec_flag = 0;
      if (i == 3)                         // Los datos intermedios obtenidos son 3 bytes, lo que indica que el formato del comando es correcto
      {
        Communication_Decode();           // Ejecutar la función de análisis del comando
      }
      i = 0;
    }
    else
    {
      buffer[i] = serial_data;            // Almacenar datos en búfer
      i++;
    }
  }
}

void UartTimeoutCheck(void)
{
  if (rec_flag == 1)
  {
    Costtime++;
    if (Costtime == 100000)
    {
      rec_flag = 0;
    }
  }
}

void Communication_Decode()
{   
  if (buffer[0] == 0x00)
  {
    switch (buffer[1])   // Para motor
    {
      case 0x01: MOTOR_GO_FORWARD; return;
      case 0x02: MOTOR_GO_BACK;    return;
      case 0x03: MOTOR_GO_LEFT;    return;
      case 0x04: MOTOR_GO_RIGHT;   return;
      case 0x00: MOTOR_GO_STOP;    return;
      default: return;
    } 
  }
  
  else if (buffer[0] == 0x01)  // Para servo
  {
    // if (buffer[2] > 170) return;
    switch (buffer[1])
    {
      case 0x01: angle1 = buffer[2]; servo1.write(angle1); return;
      case 0x02: angle2 = buffer[2]; servo2.write(angle2); return;
      case 0x03: angle3 = buffer[2]; servo3.write(angle3); return;
      case 0x04: angle4 = buffer[2]; servo4.write(angle4); return;
      // case 0x05: angle5 = buffer[2]; servo5.write(angle5); return;
      // case 0x06: angle6 = buffer[2]; servo6.write(angle6); return;
      case 0x07: angle7 = buffer[2]; servo7.write(angle7); return;
      case 0x08: angle8 = buffer[2]; servo8.write(angle8); return;
      default: return;
    }
  }
  
  else if (buffer[0] == 0x02)  // Ajustar velocidad
  {
    if (buffer[2] > 100) return;      
    if (buffer[1] == 0x01)  // Regulación de velocidad del lado izquierdo
    {
      Left_Speed_Hold = buffer[2] * 2 + 55;  // Cuando el rango de velocidad es de 0 ~ 100, se puede convertir en PWM. Cuando el PWM de velocidad es inferior a 55, el motor no girará
      analogWrite(ENB, Left_Speed_Hold);
      EEPROM.write(0x09, Left_Speed_Hold);  // Guardar velocidad
    }
    if (buffer[1] == 0x02)  // Regulación de velocidad del lado derecho
    {
      Right_Speed_Hold = buffer[2] * 2 + 55; // Cuando el rango de velocidad es de 0 ~ 100, se puede convertir en PWM. Cuando el PWM de velocidad es inferior a 55, el motor no girará
      analogWrite(ENA, Right_Speed_Hold);
      EEPROM.write(0x0A, Right_Speed_Hold); // Guardar velocidad
    }
    else return;
  }
  
  else if (buffer[0] == 0x33)  // Leer y asignar ángulo de servo
  {
    Init_Steer(); return;
  }
  
  else if (buffer[0] == 0x32)  // Guardar ángulo
  { 
    EEPROM.write(0x01, angle1);
    EEPROM.write(0x02, angle2);
    EEPROM.write(0x03, angle3);
    EEPROM.write(0x04, angle4);
    // EEPROM.write(0x05, angle5);
    // EEPROM.write(0x06, angle6);
    EEPROM.write(0x07, angle7);
    EEPROM.write(0x08, angle8);
    return;
  }
  
  else if (buffer[0] == 0x13)  // Cambiar de modo
  {
    switch (buffer[1])
    {           
      case 0x02: Cruising_Flag = 2; return;   // Seguir línea
      case 0x03: Cruising_Flag = 3; return;   // Evitar por IR
      case 0x04: Cruising_Flag = 4; return;   // Evitar por ultrasonido
      case 0x05: Cruising_Flag = 5; return;   // Mostrar la distancia por ultrasonido
      case 0x07: Cruising_Flag = 7;            // Planificación de ruta  
                 analogWrite(ENA, 115);
                 analogWrite(ENB, 115);
                 return;                           
      case 0x00: Cruising_Flag = 0; RevStatus = 0;   // Modo manual
                 analogWrite(ENA, Left_Speed_Hold);
                 analogWrite(ENB, Right_Speed_Hold);
                 return;       
      default: Cruising_Flag = 0; return;      // Modo manual
    }
  }
  
  else if (buffer[0] == 0x04)  // Comando de encender luz: ff040000ff, comando de apagar luz: ff040100ff
  {
    switch (buffer[1])  
    {
      case 0x00: Open_Light(); return;   // Encender luz
      case 0x01: Close_Light(); return;  // Apagar luz
      default: return;
    } 
  }
  
  else if (buffer[0] == 0x40)
  {
    adjust = buffer[1];
    EEPROM.write(0x10, adjust);
  }
  
  else if (buffer[0] == 0xA0)
  {
    RevStatus = 2;
    TurnAngle = buffer[1];
    Golength = buffer[2];
  }
  
  else if (buffer[0] == 0xA1)
  {
    RevStatus = 1;
    TurnAngle = buffer[1];
    Golength = buffer[2];
  }   
}
