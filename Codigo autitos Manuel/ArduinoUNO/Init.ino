/*
  Nombre del Proyecto: Autito Arduino
  Descripción: Kit de auto 4WD programable con Arduino
  Autor: Manuel Bustamante
  Copyright (c) 2024 Manuel Bustamante
*/

void Delayed()   // Retraso de 50 segundos y espera que el módulo WiFi se inicie
{
    int i;
    for(i = 0; i < 20; i++)
    {
        digitalWrite(ledpin, LOW);
        delay(1000);
        digitalWrite(ledpin, HIGH);
        delay(1000);
    }
    for(i = 0; i < 10; i++)
    {
        digitalWrite(ledpin, LOW);
        delay(500);
        digitalWrite(ledpin, HIGH);
        delay(500);
    }
    digitalWrite(ledpin, LOW);
    MOTOR_GO_STOP;
}


void Init_Steer()  // Inicialización del servo (el ángulo es el último valor guardado)
{
    angle1 = EEPROM.read(0x01);  // Leer el valor en el registro 0x01
    angle2 = EEPROM.read(0x02);  // Leer el valor en el registro 0x02
    angle3 = EEPROM.read(0x03);  // Leer el valor en el registro 0x03
    angle4 = EEPROM.read(0x04);  // Leer el valor en el registro 0x04
    //angle5 = EEPROM.read(0x05);  // Leer el valor en el registro 0x05
    //angle6 = EEPROM.read(0x06);  // Leer el valor en el registro 0x06
    angle7 = EEPROM.read(0x07);  // Leer el valor en el registro 0x07
    angle8 = EEPROM.read(0x08);  // Leer el valor en el registro 0x08
    
    if(angle7 == 255 && angle8 == 255)
    {
        EEPROM.write(0x01, 60);  // Guardar el ángulo inicial en la dirección 0x01
        EEPROM.write(0x02, 60);  // Guardar el ángulo inicial en la dirección 0x02
        EEPROM.write(0x03, 60);  // Guardar el ángulo inicial en la dirección 0x03
        EEPROM.write(0x04, 60);  // Guardar el ángulo inicial en la dirección 0x04
        //EEPROM.write(0x05, 60);  // Guardar el ángulo inicial en la dirección 0x05
        //EEPROM.write(0x06, 120); // Guardar el ángulo inicial en la dirección 0x06
        EEPROM.write(0x07, 60);  // Guardar el ángulo inicial en la dirección 0x07
        EEPROM.write(0x08, 60);  // Guardar el ángulo inicial en la dirección 0x08
        return;
    }
 
    servo1.write(angle1);  // Asignar el ángulo guardado al servo 1
    servo2.write(angle2);  // Asignar el ángulo guardado al servo 2
    servo3.write(angle3);  // Asignar el ángulo guardado al servo 3
    servo4.write(angle4);  // Asignar el ángulo guardado al servo 4
    //servo5.write(angle5);  // Asignar el ángulo guardado al servo 5
    //servo6.write(angle6);  // Asignar el ángulo guardado al servo 6
    servo7.write(angle7);  // Asignar el ángulo guardado al servo 7
    servo8.write(angle8);  // Asignar el ángulo guardado al servo 8
    adjust = EEPROM.read(0x10);  // Asignar el ángulo guardado al servo 10
    if(adjust == 0xff) EEPROM.write(0x10, 1);
    
    Left_Speed_Hold = EEPROM.read(0x09);  // Leer el valor en el registro 0x09
    Right_Speed_Hold = EEPROM.read(0x0A);  // Leer el valor en el registro 0x0A
    if(Left_Speed_Hold < 55 | Right_Speed_Hold < 55)
    {
      Left_Speed_Hold = 255;
      Right_Speed_Hold = 255;
    }
    analogWrite(ENB, Left_Speed_Hold);  // Asignar valor a L298 habilitar B
    analogWrite(ENA, Right_Speed_Hold);  // Asignar valor a L298 habilitar A
    MOTOR_GO_STOP;
}
