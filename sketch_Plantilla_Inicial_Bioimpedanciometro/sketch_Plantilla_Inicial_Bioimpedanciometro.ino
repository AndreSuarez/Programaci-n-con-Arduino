#include <Wire.h>
                      /* Direccion del AD5933 */
                      
#define Slave_Add   B0001101    // La dirección asignada por defecto al AD5933 que es nuestro dispositivo esclavo 

                      /* Comandos Principales */
                      
#define Block_W        0xA0        // Comando para escribir en el AD5933 
#define Block_R        0xA1        // Comando para leer del AD5933 
#define Set_AddPointer 0xB0        // Comando para seleccionar el registro a leer 
#define VRI_Data       0x02        // Comando de confimacion de Dato valido en el registro de Estado
#define FS_Valid       0x04        // Finalizacion del Barrido en Frecuencia en el registro de Estado
#define Start_Process  0x1000      // Dato en el registro de control para comenzar proceso
#define Sweep_Active   0x2000      // Activar Barrido de Frecuencia
#define Incr_Frequency 0x3000      // Pasar de un valor de frecuencia al siguiente

                        /* Bytes de Registros */
                        
byte Byte_Control1 = 0x80;        // Byte de Control 1  
byte Byte_Control2 = 0x81;        // Byte de Control 2  
byte Start_Freq1   = 0x82;        // Byte 1 frecuencia de inicio de Sensado  
byte Start_Freq2   = 0x83;        // Byte 2 frecuencia de inicio de Sensado 
byte Start_Freq3   = 0x84;        // Byte 3 frecuencia de inicio de Sensado  
byte Freq_incre1   = 0x85;        // Byte 1 para los increments de frecuencia  
byte Freq_incre2   = 0x86;        // Byte 2 para los increments de frecuencia
byte Freq_incre3   = 0x87;        // Byte 3 para los increments de frecuencia
byte Num_incre1    = 0x88;        // Byte 1 para el # increments de frecuencia
byte Num_incre2    = 0x89;        // Byte 2 para el # increments de frecuencia
byte Set_cycles1   = 0x8A;        // Byte 1 para el # de ciclos para cada Sensado
byte Set_cycles2   = 0x8B;        // Byte 2 para el # de ciclos para cada Sensado
byte Status        = 0x8F;        // Byte que confirma medida Sensada
byte Temp_Data1    = 0x92;        // Byte 1 para el dato de temperatura
byte Temp_Data2    = 0x93;        // Byte 2 para el dato de temperatura
byte Real_Data1    = 0x94;        // Byte 1 dato Real 
byte Real_Data2    = 0x95;        // Byte 2 dato Real 
byte Imag_Data1    = 0x96;        // Byte 1 dato Imaginario 
byte Imag_Data2    = 0x97;        // Byte 2 dato Imaginario 

                      /* Datos a Ingresar */
                      
int Frec_I = 0x015F90;             // Frecuencia inicial de 90KHz
int Frec_F = 0x1388;               // Frecuencia de resolucion de 5KHz   
int Dato; 
byte Frec_lower;                   // Byte menor del dato de frecuencia a ingresar  
byte Frec_med;                     // Byte medio del dato de frecuencia a ingresar                      
byte Frec_upper;                   // Byte mayor del dato de frecuencia a ingresar  


void setup() 
{
  Serial.begin(9600);   // 
  Wire.begin(); 
  
}
void loop() 
{

  WriteCommand_Freq();
  Serial.println("Escritura de Frecuencia Lista");
  AddressPointer();                             // Selecciono uno a uno los registros donde estan los datos reales e imaginarios
  Serial.println("Registro Seleccionado");        
  ReadCommand();                                // Leo uno a uno los datos almacenados y los coloco en una matriz
  Serial.println(Dato); 
}

void WriteCommand_Freq()                  // Funcion para escribir la frecuencia requerida
{
  Frec_lower = 0x90;
  Frec_med = 0x5F;
  Frec_upper = 0x01;  
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Serial.println("Estoy escribiendo el byte 1");
  Wire.write(Start_Freq1);
  Wire.write(Frec_lower);
  Wire.endTransmission();
  Serial.println("Byte 1 correcto");  
    
}

void AddressPointer ()                  // Funcion que ubica en el registro la direccion posterior a leer
{
  
  Wire.beginTransmission(Slave_Add);  // Envia los 7 bits de direccion del AD5933 + el bit de escritura (0), para comenzar
  Serial.println("Registro de Lectura Seleccionado ...");
  Wire.write(Set_AddPointer);
  Wire.write(Start_Freq1); //
  Wire.endTransmission(); 
}

void ReadCommand ()                // Funcion para leer los registros de los datos reales e imaginarios sensados
{
  Wire.requestFrom(Slave_Add,1);    // request 6 bytes from slave device #2
  while(Wire.available())    // slave may send less than requested
  { 
    Dato = Wire.read();    // receive a byte as character
  }  
  delay(1000);
  Serial.println("Dato almacenado ...");  
}



