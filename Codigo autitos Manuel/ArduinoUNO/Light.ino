/*
  Nombre del Proyecto: Autito Arduino
  Descripción: Kit de auto 4WD programable con Arduino
  Autor: Manuel Bustamante
  Copyright (c) 2024 Manuel Bustamante
*/

void Open_Light()  // Encender las luces
{
  digitalWrite(Carled, HIGH);  // Establecer nivel alto, conectar el polo positivo a la fuente de alimentación y conectar el polo negativo al puerto IO
  delay(1000);
}

void Close_Light()  // Apagar las luces
{
  digitalWrite(Carled, LOW);   // Establecer nivel bajo, conectar el polo positivo a la fuente de alimentación y conectar el polo negativo al puerto IO
  delay(1000);
}
