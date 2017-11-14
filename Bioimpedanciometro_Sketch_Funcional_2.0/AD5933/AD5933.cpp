/**
 * @file AD5933.cpp
 * @brief Library code for AD5933
 *
 * Library code for AD5933. Referenced the datasheet and code found at
 * https://github.com/WuMRC/drive
 *
 * @author Michael Meli
 */

#include <Math.h>
#include <Wire.h>
#include "AD5933.h"


AD5933::AD5933(int St)
{
	Serial.begin(St);
	Wire.begin();
}
long AD5933::impedance ()
{ 
   conf=0;
   Imp_Teor();
   BioImpedance = Bio_impedance;
   return BioImpedance;

}


long AD5933::Imp_Teor ()
{

  Value_Imp();
  B_Prev = (Valf_Mag * EC3)+ 11637;
  if(B_Prev < 293)
  {
    Bio_impedance = (Valf_Mag * EC1)+ 5485.9;
  }
  else if((B_Prev < 561)&&(B_Prev > 293))
  {
    Bio_impedance = (Valf_Mag * EC2)+ 16544;    
  }
  else if(B_Prev > 790)
  {
    Bio_impedance = (Valf_Mag * EC4)+ 7538.4;    
  }
  else
  {
    Bio_impedance = B_Prev;
  }
  return Bio_impedance;

}



void AD5933::Value_Imp()
{

  while(conf<10)
  {
    if((conf==0)||(conf==2)||(conf==4)||(conf==6)||(conf==8))
    {
      Init_Command();
      delay(14);
    }
    
    if(conf == 1)
    { 
      digitalWrite(2,HIGH);
      Taking_Data();
      Set_CR1(Rst);
    } 
    
    if((conf == 3)||(conf==5)||(conf==7))
    { 
      digitalWrite(2,LOW);      
      Taking_Data();
      
    } 
    if(conf == 9)
    { 
      digitalWrite(2,LOW);      
      Taking_Data();
    }     
    conf++; 
  } 

}

void Program_Port()
{
  pinMode(2, OUTPUT);
}

void AD5933::Taking_Data ()
{
  AddressPointer(Status);
  ReadCommand_Status();
  Conf_State = (State & 0x02);
  while(Conf_State == VRI_Data)
  {
    for(i=0; i<4; i++)
    {
      AddressPointer(Reg_Data[i]);                             // Selecciono uno a uno los registros donde estan los datos reales e imaginarios
      delay(10);
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
            Mag[h] = (Magnitude[0]+ Magnitude[1]+Magnitude[2])/3;
            Conf_State =  0x00;
            j=0;
            if(h==4)
            {
              Valf_Mag = (Mag[1]+ Mag[2]+ Mag[3]+ Mag[4])/4;
              h=0;
            }
            h++;
          }
        }  
      }      
    }
  }
}

void AD5933::Init_Command ()
{

  WriteCommand_Freq();
  Increment_Freq();
  NumberIncrement();
  CycleNumber();
  Set_CR(Standby_Mode);
  Set_CR(Gain_X1);   
  Set_CR(Start_Process);
  Set_CR(Sweep_Active);
  delay(10);
  
}


void AD5933::WriteCommand_Freq()                  // Funcion para escribir la frecuencia requerida
{
  Frec_lower = 0x7B;
  Frec_med = 0x14;
  Frec_upper = 0x2E;  
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

void AD5933::Increment_Freq()                  // Funcion para la resolucion de los incrementos de la frecuencia
{
  IF_lower = 0x5C;
  IF_med = 0x8F;
  IF_upper = 0x02;  

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

void AD5933::NumberIncrement()                 // Funcion para establecer el numero de incrementos
{
  NI_lower = 0x02;
  NI_upper = 0x00;  
    
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Num_incre1);
  Wire.write(NI_upper);
  Wire.endTransmission(); 

  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Num_incre2);
  Wire.write(NI_lower);
  Wire.endTransmission(); 
}

void AD5933::CycleNumber()
{
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Set_cycles2);
  Wire.write(Cycles);
  Wire.endTransmission();
}   

void AD5933::AddressPointer (byte Reg_Address)                  // Funcion que ubica en el registro la direccion posterior a leer
{
  
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Set_AddPointer);
  Wire.write(Reg_Address);
  Wire.endTransmission(); 
}

void AD5933::ReadCommand ()                // Funcion para leer los registros de los datos reales e imaginarios sensados
{
  Wire.requestFrom(Slave_Add,1);    // request 6 bytes from slave device #2
  while(Wire.available())    // slave may send less than requested
  { 
    Dato[i] = Wire.read();    // receive a byte as character
  }   
}

void AD5933::Set_CR (byte Control)                     // Coloca en el registro de Control datos para avanzar en el proceso
{
  
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Byte_Control1);
  Wire.write(Control);
  Wire.endTransmission();   
 
}

void AD5933::Set_CR1 (byte Control)                     // Coloca en el registro de Control datos para avanzar en el proceso
{
  
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Wire.write(Byte_Control2);
  Wire.write(Control);
  Wire.endTransmission();   
 
}

void AD5933::ReadCommand_Status ()          // Lee unicamente el registro de Estado para obtener datos de confirmacion
{
  Wire.requestFrom(Slave_Add,1);    // request 6 bytes from slave device #2
  while(Wire.available())    // slave may send less than requested
  { 
    State = Wire.read();    // receive a byte as character
  }  
  delay(10);
}

void AD5933::Mag_Data ()
{
  Dato_R = (Dato[0]<<8)|(Dato[1]);
  Dato_I = (Dato[2]<<8)|(Dato[3]);
  Mag_Prev = (pow(Dato_R,2))+(pow(Dato_I,2));
  Magnitude[j] = sqrt(Mag_Prev);
}