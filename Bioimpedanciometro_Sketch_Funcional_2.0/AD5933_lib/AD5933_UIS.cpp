/**
 * @file AD5933.cpp
 * @brief Library code for AD5933
 *
 * Library code for AD5933. Referenced the datasheet and code found at
 * https://github.com/WuMRC/drive
 *
 * @author Michael Meli
 */

#include "AD5933.h"
#include <Math.h>


void AD5933_UIS::WriteCommand_Freq()                  // Funcion para escribir la frecuencia requerida
{
  byte Frec_lower = 0x7B;
  byte Frec_med = 0x14;
  byte Frec_upper = 0x2E;  
  
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Start_Freq1);
  Wire.write(Frec_upper);
  Wire.endTransmission();
    
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Start_Freq2);
  Wire.write(Frec_med);
  Wire.endTransmission(); 

  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Start_Freq3);
  Wire.write(Frec_lower);
  Wire.endTransmission(); 
}

void AD5933_UIS::Increment_Freq()                  // Funcion para la resolucion de los incrementos de la frecuencia
{
  byte IF_lower = 0x5C;
  byte IF_med = 0x8F;
  byte IF_upper = 0x02;  

  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Freq_incre1);
  Wire.write(IF_upper);
  Wire.endTransmission(); 
    
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Freq_incre2);
  Wire.write(IF_med);
  Wire.endTransmission(); 

  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Freq_incre3);
  Wire.write(IF_lower);
  Wire.endTransmission(); 
}

void AD5933_UIS::NumberIncrement()                 // Funcion para establecer el numero de incrementos
{
  byte NI_lower = 0x02;
  byte NI_upper = 0x00;  
    
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Num_incre1);
  Wire.write(NI_upper);
  Wire.endTransmission(); 

  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Num_incre2);
  Wire.write(NI_lower);
  Wire.endTransmission(); 
}

void AD5933_UIS::CycleNumber()
{
  byte Cycles = 0x14;  
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Set_cycles2);
  Wire.write(Cycles);
  Wire.endTransmission();
}   

void AD5933_UIS::AddressPointer (byte Reg_Address)                  // Funcion que ubica en el registro la direccion posterior a leer
{
  
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Set_AddPointer);
  Wire.write(Reg_Address);
  Wire.endTransmission(); 
}  

void AD5933_UIS::ReadCommand ()                // Funcion para leer los registros de los datos reales e imaginarios sensados
{
  Wire.requestFrom(Slave_Add,1);    // request 6 bytes from slave device #2
  while(Wire.available())    // slave may send less than requested
  { 
    Dato[i] = Wire.read();    // receive a byte as character
  }  
}

void AD5933_UIS::Set_CR (byte Control)                     // Coloca en el registro de Control datos para avanzar en el proceso
{
  
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Byte_Control1);
  Wire.write(Control);
  Wire.endTransmission();   
 
}

void AD5933_UIS::Set_CR1 (byte Control)                     // Coloca en el registro de Control datos para avanzar en el proceso
{
  
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Byte_Control2);
  Wire.write(Control);
  Wire.endTransmission();   
 
}

void AD5933_UIS::ReadCommand_Status ()          // Lee unicamente el registro de Estado para obtener datos de confirmacion
{
  Wire.requestFrom(Slave_Add,1);    // request 6 bytes from slave device #2
  while(Wire.available())    // slave may send less than requested
  { 
    State = Wire.read();    // receive a byte as character
  }  
  delay(10);
}

void AD5933_UIS::Mag_Data ()
{
  Dato_R = (Dato[0]<<8)|(Dato[1]);
  Dato_I = (Dato[2]<<8)|(Dato[3]);
  word Mag_Prev = (pow(Dato_R,2))+(pow(Dato_I,2));
  word Magnitude = sqrt(Mag_Prev);
}

void AD5933_UIS::Taking_Data ()
{
  int j=0;	
  AddressPointer(Status);
  ReadCommand_Status();
  Conf_State = (State & 0x02);
  while(Conf_State == VRI_Data)
  {
    for(i=0; i<4; i++)
    {
      AddressPointer(Reg_Data[i]);                             // Selecciono uno a uno los registros donde estan los datos reales e imaginarios
      ReadCommand();                                // Leo uno a uno los datos almacenados y los coloco en una matriz
      if(i==3)
      {
        Set_CR(Incr_Frequency);
        Mag_Data();
        j++;
        if(j==3)
        {
          AddressPointer(Status);
          ReadCommand_Status();
          Valid_Sweep = (State & 0x04);   
          if(Valid_Sweep == FS_Valid)
          {
            Conf_State =  0x00;
            j=0;
          }
        }  
      }      
    }
  }
}

void AD5933_UIS::Impedance()
{

  while(conf<4)
  {
    if((conf==0)||(conf==2))
    {
      Init_Command();
      delay(14);
    }
    
    if(conf == 1)
    { 
      PORTD = Switch_Ch2;
      Taking_Data();
      Set_CR1(Reset);
    } 
    
    if(conf == 3)
    { 
      PORTD = Switch_Ch1;      
      Taking_Data();
    } 
    conf++; 
  } 

}
